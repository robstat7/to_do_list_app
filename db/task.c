#include "../include/db/task.h"
#include "../include/db/db.h"
#include "../task.h"

/* find database file size in bytes */
int find_db_file_size(int db_fd)
{
	int file_size;

	lseek(db_fd, 0, SEEK_SET);

	file_size = lseek(db_fd, 0, SEEK_END);

	return file_size;
}

/* find the number of tasks in the db */
short int db_find_num_total_tasks(void)
{
	short int num_total_tasks, last_task_id;
	int records_size, db_fd;

	db_fd = get_db_fd();
	last_task_id = get_last_allocated_task_id();

	records_size = find_db_file_size(db_fd) - sizeof(last_task_id);

	printf("@records_size = %d\n", records_size);

	num_total_tasks = records_size/sizeof(struct task);
	
	printf("@num_total_tasks = %d\n", num_total_tasks);

	return num_total_tasks;
}

/* delete a task from the db */
void db_delete_task(GtkWidget *selected_row)
{
	GtkWidget *hbox;
	GList *children;
	short int task_id, num_total_tasks, i, last_task_id;
	int db_fd;
	struct task to_do_task;

	children = gtk_container_get_children(GTK_CONTAINER(selected_row));
	if(children != NULL) {
        	/* assuming the child is a GtkBox */
        	hbox = GTK_WIDGET(children->data);
        	/* check if it's a GtkBox */
        	if(GTK_IS_BOX(hbox)) {
        		task_id = *(gint *)g_object_get_data(G_OBJECT(hbox), "task_id");

			printf("@task id to delete is %d\n", task_id);

			num_total_tasks = db_find_num_total_tasks();
			
			db_fd = get_db_fd();

			last_task_id = get_last_allocated_task_id();

			lseek(db_fd, sizeof(last_task_id), SEEK_SET);

			for(i = 0; i < num_total_tasks; i++) {
				read(db_fd, &to_do_task, sizeof(struct task));

				if(to_do_task.id == task_id) {
					/* deleted task will have the id = -1 */
					to_do_task.id = -1;
					lseek(db_fd, -sizeof(struct task), SEEK_CUR);
					write(db_fd, &to_do_task, sizeof(struct task));
					break;
				}
			}
		}
	}
}
	

