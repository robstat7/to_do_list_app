#include <gtk/gtk.h>
#include <stdio.h>

struct task {
	short int id;
	short int completed;
	char task_string[100];
};

void show_tasks(GtkWidget *tasks_box, int db_fd);
void db_get_last_allocated_task_id(int db_fd);
short int get_last_allocated_task_id();
void set_last_allocated_task_id(short int id);
static int find_db_file_size(int db_fd);
