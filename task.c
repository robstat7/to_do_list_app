/*
 * task related functions
 */
#include "task.h"

/* show tasks from the database file in the tasks box */
void show_tasks(GtkWidget *tasks_box, int db_fd)
{
	int i, num_tasks, file_size;
	struct task to_do_task;
	GtkWidget *row, *hbox, *label, *check_button;

	lseek(db_fd, 0, SEEK_SET);

	file_size = lseek(db_fd, 0, SEEK_END);

	printf("@file_size = %d\n", file_size);

	num_tasks = file_size/sizeof(struct task);
	
	printf("@num_tasks = %d\n", num_tasks);
	
	lseek(db_fd, 0, SEEK_SET);

	for(i = 0; i < num_tasks; i++) {
		read(db_fd, &to_do_task, sizeof(struct task));
		lseek(db_fd, (i + 1) * sizeof(struct task), SEEK_SET);

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
