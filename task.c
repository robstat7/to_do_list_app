/*
 * task related functions
 */
#include "task.h"

short int last_allocated_task_id;

short int get_last_allocated_task_id(void)
{
	return last_allocated_task_id;
}

void set_last_allocated_task_id(short int id)
{
	last_allocated_task_id = id;
}

/* find database file size in bytes */
static int find_db_file_size(int db_fd)
{
	int file_size;

	lseek(db_fd, 0, SEEK_SET);

	file_size = lseek(db_fd, 0, SEEK_END);

	return file_size;
}

/* show tasks from the database file in the tasks box */
void show_tasks(GtkWidget *tasks_box, int db_fd)
{
	int i, num_tasks, records_size;
	struct task to_do_task;
	GtkWidget *row, *hbox, *label, *check_button;

	records_size = find_db_file_size(db_fd) - sizeof(last_allocated_task_id);

	printf("@records_size = %d\n", records_size);

	num_tasks = records_size/sizeof(struct task);
	
	printf("@num_tasks = %d\n", num_tasks);
	
	lseek(db_fd, sizeof(last_allocated_task_id), SEEK_SET);

	for(i = 0; i < num_tasks; i++) {
		read(db_fd, &to_do_task, sizeof(struct task));
		lseek(db_fd,
		      (sizeof(last_allocated_task_id) +
		       ((i + 1) * sizeof(struct task))),
		      SEEK_SET);

		printf("@to_do_task.id = %d\n", to_do_task.id);
		printf("@to_do_task.completed = %d\n", to_do_task.completed);
		printf("@to_do_task.task_string= %s\n", to_do_task.task_string);
		
		row = gtk_list_box_row_new();
		hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
		label = gtk_label_new(to_do_task.task_string);
		check_button = gtk_check_button_new();
		
		gtk_container_add(GTK_CONTAINER(row), hbox);
		gtk_box_pack_start(GTK_BOX(hbox), check_button, FALSE, FALSE, 0);
		gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
		
		gtk_list_box_insert(GTK_LIST_BOX(tasks_box), row, -1);
	}
        
	gtk_widget_show_all(tasks_box);
}

void db_get_last_allocated_task_id(int db_fd)
{
	int file_size;

	file_size = find_db_file_size(db_fd);

	lseek(db_fd, 0, SEEK_SET);

	if(file_size == 0x0) {	/* first usage of the application */
		last_allocated_task_id = 0;
		write(db_fd, &last_allocated_task_id, sizeof(last_allocated_task_id));
		printf("@last_allocated_task_id = %d\n", last_allocated_task_id);
		return;
	} else {
		read(db_fd, &last_allocated_task_id, sizeof(last_allocated_task_id));
		printf("@last_allocated_task_id = %d\n", last_allocated_task_id);
	}
}

	
