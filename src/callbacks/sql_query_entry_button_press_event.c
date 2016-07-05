#include "../chunk.h"

G_MODULE_EXPORT gboolean sql_query_entry_key_press_event_cb(GtkWidget* widget, GdkEvent* event, gpointer user_data)
{
	SSE_APP* app = (SSE_APP*)user_data;

	if(event->type == GDK_KEY_PRESS)
	{
		GdkEventKey* key_event	= &(event->key);
		GList* list				= app->query_list;

		if(key_event->keyval == GDK_KEY_Up)
		{
			list = list->next;

			for(size_t i=0; list->data == NULL && i<SQL_QUERY_LINE_BUFF_SIZE; i++)
				list = list->next;

			if(list->data != NULL)
			{
				app->query_list = list;
				gtk_entry_set_text(GTK_ENTRY(widget), list->data);
			}
			return TRUE;
		}
		else if(key_event->keyval == GDK_KEY_Down)
		{
			list = list->prev;

			for(size_t i=0; list->data == NULL && i<SQL_QUERY_LINE_BUFF_SIZE; i++)
				list = list->prev;

			if(list->data != NULL)
			{
				app->query_list = list;
				gtk_entry_set_text(GTK_ENTRY(widget), list->data);
			}
			return TRUE;
		}
	}

	return FALSE;
}
