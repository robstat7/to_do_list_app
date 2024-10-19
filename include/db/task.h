#include <gtk/gtk.h>

int find_db_file_size(int db_fd);
short int db_find_num_total_tasks(void);
void db_delete_task(GtkWidget *selected_row);
void db_edit_task(short int task_id, const gchar *task_string);
