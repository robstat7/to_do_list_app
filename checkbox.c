#include "checkbox.h"

void on_row_checkbox_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
	GtkWidget *row;

	row = GTK_WIDGET(user_data);

	if (gtk_toggle_button_get_active(togglebutton)) {
		g_print("@checkbox in row is checked\n");
    } else {
		g_print("@checkbox in row is unchecked\n");
    }
}
