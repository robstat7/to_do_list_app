#include <gtk/gtk.h>

static void activate(GtkApplication *app, gpointer user_data)
{
	GtkApplication *application;
	GtkWidget *win, *vbox, *toolbar, *add_task_tool_img;
	GtkToolItem *add_task_button;

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

	/* add toolbar to the vertical box */
    	gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);

	gtk_window_present(GTK_WINDOW (win));
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
