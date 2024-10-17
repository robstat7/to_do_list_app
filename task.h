#include <gtk/gtk.h>
#include <stdio.h>

void show_tasks(GtkWidget *tasks_box, int db_fd);
struct task {
	short int completed;
	char task_string[100];
};
