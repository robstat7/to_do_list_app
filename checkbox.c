#include "checkbox.h"

void on_row_checkbox_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
	GtkWidget *hbox;

	hbox = GTK_WIDGET(user_data);

	if (gtk_toggle_button_get_active(togglebutton)) {
		g_print("@checkbox for task id %d is checked\n", *(gint *)g_object_get_data(G_OBJECT(hbox), "task_id"));
    } else {
		g_print("@checkbox for task id %d is unchecked\n", *(gint *)g_object_get_data(G_OBJECT(hbox), "task_id"));
    }
}
