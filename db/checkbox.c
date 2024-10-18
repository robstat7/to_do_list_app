#include "../include/db/checkbox.h"
#include "../include/db/task.h"
#include "../include/db/db.h"
#include "../task.h"

/* update task completed status for the task id in the db file */
void db_update_checkbox(short int task_id, short int completed)
{
	int i, db_fd;
	short int num_tasks, last_task_id;
	struct task to_do_task;

	db_fd = get_db_fd();

	num_tasks = db_find_num_tasks();

	last_task_id = get_last_allocated_task_id();

	lseek(db_fd, sizeof(last_task_id), SEEK_SET);

	for(i = 0; i < num_tasks; i++) {
		read(db_fd, &to_do_task, sizeof(struct task));
	
		if(to_do_task.id == task_id) {
			to_do_task.completed = completed;
			lseek(db_fd, -sizeof(struct task), SEEK_CUR);
			write(db_fd, &to_do_task, sizeof(struct task));
			break;
		}
	}
		
}

