#include "../include/db/db.h"

int db_fd;	/* database file descriptor */

/* get database file descriptor */
int get_db_fd(void)
{
	return db_fd;
}

/* open database file */
void open_db_file(void)
{
	db_fd = open("resources/tasks", O_RDWR, 0600);
	if (db_fd == -1) {
		printf("cannot open database file.\n");
		return;
	}

	printf("@db_fd = %d\n", db_fd);
}

void on_window_destroy(GtkWidget *widget, gpointer data)
{
	close(db_fd);
	printf("@db file closed successfully!\n");
}
