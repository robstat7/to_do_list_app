/*
 * task related functions
 */
#include "task.h"
#include "checkbox.h"
#include "include/db/task.h"

short int last_allocated_task_id;

short int get_last_allocated_task_id(void)
{
	return last_allocated_task_id;
}

void set_last_allocated_task_id(short int id)
{
	last_allocated_task_id = id;
}

/* show tasks from the database file in the tasks box */
void show_tasks(GtkWidget *tasks_box, int db_fd)
{
	int i;
	short int num_total_tasks;
	struct task to_do_task;

	num_total_tasks = db_find_num_total_tasks();
	
	lseek(db_fd, sizeof(last_allocated_task_id), SEEK_SET);

	for(i = 0; i < num_total_tasks; i++) {
		read(db_fd, &to_do_task, sizeof(struct task));

		/* do not show the deleted tasks */
		if(to_do_task.id == -1) {
			continue;
		}

		printf("@to_do_task.id = %d\n", to_do_task.id);
		printf("@to_do_task.completed = %d\n", to_do_task.completed);
		printf("@to_do_task.task_string= %s\n", to_do_task.task_string);
		
		add_task_to_tasks_box(tasks_box, to_do_task.id, to_do_task.completed, to_do_task.task_string);
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

void add_task_to_tasks_box(GtkWidget *tasks_box,
			   short int task_id,
			   short int task_completed,
			   char* task_string)
{
	GtkWidget *row, *hbox, *label, *check_button;

	row = gtk_list_box_row_new();
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	label = gtk_label_new(task_string);
	check_button = gtk_check_button_new();
	
	gtk_container_add(GTK_CONTAINER(row), hbox);

	/* store the task id in the row's GObject data */
	g_object_set_data(G_OBJECT(hbox), "task_id", g_new(gint, 1)); /* allocating space for int */
	*(gint *)g_object_get_data(G_OBJECT(hbox), "task_id") = task_id;  /* setting the task id value */

	printf("@task_id = %d\n", *(gint *)g_object_get_data(G_OBJECT(hbox), "task_id"));

	/* check/uncheck check button based on passed task_completed value */
	if(task_completed == 0) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_button), FALSE);
	} else if(task_completed == 1) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_button), TRUE);
	}
		
	/* connect the 'toggled' signal for the checkbox */
	g_signal_connect(check_button, "toggled", G_CALLBACK(on_row_checkbox_toggled), hbox);

	gtk_box_pack_start(GTK_BOX(hbox), check_button, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
	
	gtk_list_box_insert(GTK_LIST_BOX(tasks_box), row, -1);
}
