#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "task.h"

int db_fd;	/* database file descriptor */

static void on_window_destroy(GtkWidget *widget, gpointer data)
{
	close(db_fd);
	printf("@db file closed successfully!\n");
}

/* add a new task to the tasks box */
static void add_task(gpointer *user_data, const gchar *task_text)
{
	GtkWidget *row, *hbox, *label, *check_button, *tasks_box;
	struct task new_task;

	tasks_box = (GtkWidget *) user_data;

	row = gtk_list_box_row_new();
        hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
        label = gtk_label_new(task_text);
        check_button = gtk_check_button_new();

        gtk_container_add(GTK_CONTAINER(row), hbox);
        gtk_box_pack_start(GTK_BOX(hbox), check_button, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);

        gtk_list_box_insert(GTK_LIST_BOX(tasks_box), row, -1);

	/* add task to the database file */
	strncpy(new_task.task_string, task_text, 100);
	new_task.completed = 0;

	lseek(db_fd, 0, SEEK_END);	

	write(db_fd, &new_task, sizeof(struct task));


        gtk_widget_show_all(tasks_box);
}

static void on_delete_task_clicked(GtkWidget *widget, gpointer tasks_box)
{
	GtkWidget *dialog, *selected_row;
	gint result;
	const gchar *task_text;

	/* create a dialog to delete selected task */
	dialog = gtk_dialog_new_with_buttons("Delete selected task?",
					     GTK_WINDOW(gtk_widget_get_toplevel(widget)),
					     GTK_DIALOG_MODAL |
					     GTK_DIALOG_DESTROY_WITH_PARENT,
					     "_OK",
					     GTK_RESPONSE_OK,
					     "_Cancel",
					     GTK_RESPONSE_CANCEL,
					     NULL);

	/* run the dialog and get user input */
	result = gtk_dialog_run(GTK_DIALOG(dialog));

	if(result == GTK_RESPONSE_OK) {
			selected_row = (GtkWidget *) gtk_list_box_get_selected_row(
					GTK_LIST_BOX(tasks_box));
			/* destroy the row */
			gtk_widget_destroy(selected_row);
	}

	gtk_widget_destroy(dialog);
}

static void on_add_task_clicked(GtkWidget *widget, gpointer tasks_box)
{
	GtkWidget *entry, *dialog, *content_area;
	gint result;
	const gchar *task_text;

	/* create a dialog to input a new task string */
	dialog = gtk_dialog_new_with_buttons("Add a new task",
					     GTK_WINDOW(gtk_widget_get_toplevel(widget)),
					     GTK_DIALOG_MODAL |
					     GTK_DIALOG_DESTROY_WITH_PARENT,
					     "_OK",
					     GTK_RESPONSE_OK,
					     "_Cancel",
					     GTK_RESPONSE_CANCEL,
					     NULL);

	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

	/* create an entry widget to accept the search term */
	entry = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(content_area), entry, TRUE, TRUE, 0);
	gtk_widget_show(entry);

	/* run the dialog and get user input */
	result = gtk_dialog_run(GTK_DIALOG(dialog));

	if(result == GTK_RESPONSE_OK) {
		task_text = gtk_entry_get_text(GTK_ENTRY(entry));
		if(task_text && *task_text!= '\0') {
			add_task(tasks_box, task_text);
		}
	}

	gtk_widget_destroy(dialog);
}

static void activate(GtkApplication *app, gpointer user_data)
{
	GtkApplication *application;
	GtkWidget *win, *today_label, *vbox, *toolbar, *delete_task_tool_img,
		  *add_task_tool_img, *tasks_box;
	GtkToolItem *add_task_button, *delete_task_button;

	/* create a new application window */
	application = GTK_APPLICATION (app);
	win = gtk_application_window_new(GTK_APPLICATION (application));
	gtk_window_set_title (GTK_WINDOW (win), "To-Do List App");
	gtk_window_set_default_size (GTK_WINDOW (win), 500, 300);

	/* create a vertical box to pack the toolbar and other widgets */
    	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    	gtk_container_add(GTK_CONTAINER(win), vbox);

	/* create a new toolbar */
	toolbar = gtk_toolbar_new();

	/* set toolbar style (optional: can be icons, text, or both) */
    	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

	/* create "Add Task" button */
	add_task_tool_img = gtk_image_new_from_file("resources/add_task.png");
    	add_task_button = gtk_tool_button_new(add_task_tool_img, "Add Task");
    	gtk_tool_button_set_label(GTK_TOOL_BUTTON(add_task_button), "Add Task");
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), add_task_button, -1);

	/* set tooltip for the add task button */
    	gtk_widget_set_tooltip_text(GTK_WIDGET(add_task_button), "Add Task");

	/* create delete task button */
	delete_task_tool_img = gtk_image_new_from_file("resources/delete_task.png");
    	delete_task_button = gtk_tool_button_new(delete_task_tool_img, "Delete Task");
    	gtk_tool_button_set_label(GTK_TOOL_BUTTON(delete_task_button), "Delete Task");
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), delete_task_button, -1);

	/* set tooltip for the add task button */
    	gtk_widget_set_tooltip_text(GTK_WIDGET(delete_task_button), "Delete Task");

	/* add toolbar to the vertical box */
    	gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);

	/* create a new today label */
	today_label = gtk_label_new("Today");

	/* add the label to the vbox */
	gtk_box_pack_start(GTK_BOX(vbox), today_label, FALSE, FALSE, 0);

	/* create tasks box */
	tasks_box = gtk_list_box_new();

	/* add tasks box to the vobx */
	gtk_box_pack_start(GTK_BOX(vbox), tasks_box, TRUE, TRUE, 0);

	/* open database file */
	db_fd = open("resources/tasks", O_RDWR, 0600);
	if (db_fd == -1) {
		printf("cannot open database file.\n");
		return;
	}

	printf("@db_fd = %d\n", db_fd);

	/* get last allocated task id from the database */
	db_get_last_allocated_task_id(db_fd);

	/* show tasks from the database file in the tasks box */
	show_tasks(tasks_box, db_fd);

	/* connect the destroy signal to the function that closes the database file */
	g_signal_connect(win, "destroy", G_CALLBACK(on_window_destroy), NULL);

	/* connect the add task button to the callback function */
	g_signal_connect(add_task_button, "clicked", G_CALLBACK(on_add_task_clicked), tasks_box);

	/* connect the delete task button to the callback function */
	g_signal_connect(delete_task_button, "clicked", G_CALLBACK(on_delete_task_clicked), tasks_box);

	gtk_window_present(GTK_WINDOW(win));
	gtk_widget_show_all(win);
	gtk_window_maximize(GTK_WINDOW(win));
}

int main(int argc, char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
