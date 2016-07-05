#include "../../chunk.h"

G_MODULE_EXPORT void open_file_menuitem_activate_cb(GtkMenuItem* item, gpointer user_data)
{
	SSE_APP* app = (SSE_APP*)user_data;
	while(gtk_dialog_run(app->db_open_dialog) == 2)
	{
		char* path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(app->db_open_dialog));

		if(open_db(app, path) == EXIT_SUCCESS)
		{
			gtk_widget_hide(GTK_WIDGET(app->db_open_dialog));
			break;
		}

		g_free(path);
	}

	gtk_widget_hide(GTK_WIDGET(app->db_open_dialog));
}


G_MODULE_EXPORT void db_chooser_dialog_file_activated_cb(GtkFileChooser* chooser, gpointer user_data)
{
	char* path		= gtk_file_chooser_get_filename(chooser);
	SSE_APP* app	= (SSE_APP*)user_data;

	if(open_db(app, path) == EXIT_SUCCESS)
		gtk_widget_hide(GTK_WIDGET(chooser));

	g_free(path);
}

