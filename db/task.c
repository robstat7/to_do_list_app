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
short int db_find_num_tasks(void)
{
	short int num_tasks, last_task_id;
	int records_size, db_fd;

	db_fd = get_db_fd();
	last_task_id = get_last_allocated_task_id();

	records_size = find_db_file_size(db_fd) - sizeof(last_task_id);

	printf("@records_size = %d\n", records_size);

	num_tasks = records_size/sizeof(struct task);
	
	printf("@num_tasks = %d\n", num_tasks);

	return num_tasks;
}

