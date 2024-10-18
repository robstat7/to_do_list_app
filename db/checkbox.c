#include "../include/db/checkbox.h"
#include "../include/db/db.h"

/* update task completed status for the task id in the db file */
void db_update_checkbox(short int task_id, short int completed)
{
	int db_fd;

	db_fd = get_db_fd();

}

