#include "../chunk.h"

G_MODULE_EXPORT gboolean main_wnd_delete_event_cb(GtkWidget* widget, GdkEvent* event, gpointer user_data)
{
	SSE_APP* app		= (SSE_APP*)user_data;
	char buff[1024];
	strncpy(buff, app->tmp_dir_name, sizeof(buff));

	if(rrmdirs(buff, sizeof(buff)) == EXIT_SUCCESS)
	{
		g_free(app->tmp_dir_name);
		app->tmp_dir_name = NULL;

		sqlite3_close(app->db);
		gtk_main_quit();

		return FALSE;
	}
	else
	{
		g_object_set(G_OBJECT(app->error_messagedialog), "secondary-text", "Невозможно удалить временный файл", NULL);
		gtk_dialog_run(GTK_DIALOG(app->error_messagedialog));
		gtk_widget_hide(GTK_WIDGET(app->error_messagedialog));

		return TRUE;
	}
}



