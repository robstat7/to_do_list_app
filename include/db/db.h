#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <gtk/gtk.h>

int get_db_fd(void);
void open_db_file(void);
void on_window_destroy(GtkWidget *widget, gpointer data);
