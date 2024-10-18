#include "checkbox.h"
#include "include/db/checkbox.h"

void on_row_checkbox_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
	GtkWidget *hbox;
	short int task_id;

	hbox = GTK_WIDGET(user_data);

	if (gtk_toggle_button_get_active(togglebutton)) {
		task_id = *(gint *)g_object_get_data(G_OBJECT(hbox), "task_id");

		g_print("@checkbox for task id %d is checked\n", task_id);

		/* update the checkbox status in the db */
		db_update_checkbox(task_id, 1);
    	} else {
		g_print("@checkbox for task id %d is unchecked\n", *(gint *)g_object_get_data(G_OBJECT(hbox), "task_id"));
    	}
}
