#include "../../chunk.h"

G_MODULE_EXPORT gboolean main_tree_view_button_press_event_cb(GtkWidget* widget, GdkEvent* event, gpointer user_data)
{
	SSE_APP* app = (SSE_APP*)user_data;

	if(event->type == GDK_BUTTON_PRESS && app->db != NULL)
	{
		GdkEventButton* btn_event = &(event->button);
		if(btn_event->button == 3)
		{
			GtkTreeSelection* selection = gtk_tree_view_get_selection(app->main_tree_view);
			if(gtk_tree_selection_count_selected_rows(selection) > 0)
				gtk_menu_popup(app->main_tree_view_menu, NULL, NULL, NULL, user_data, btn_event->button, gdk_event_get_time(event));

			return TRUE;
		}
	}

	return FALSE;
}
