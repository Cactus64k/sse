#include "../chunk.h"

G_MODULE_EXPORT void sql_query_entry_activate_cb(GtkEntry* entry, gpointer user_data)
{
	SSE_APP* app = (SSE_APP*)user_data;
	if(app->db != NULL)
	{
		sqlite3_stmt* query		= NULL;
		const char* query_str	= gtk_entry_get_text(entry);

		if(sqlite3_prepare(app->db, query_str, -1, &query, NULL) == SQLITE_OK)
		{
			reset_main_tree_view(app);

			if(sqlite3_column_count(query) != 0)
			{
				init_main_tree_view(app, query, FALSE);
				init_main_list_store(app->main_list_store, query);
			}
			else
				sqlite3_step(query);

			init_table_list_store(app);

			GList* last_item = app->last_query_item;
			g_free(last_item->data);
			last_item->data = g_strdup(query_str);
			app->last_query_item = last_item->next;


		}
		else
		{
			const char* error_msg = sqlite3_errmsg(app->db);
			show_error_message(app, error_msg);
		}


		sqlite3_finalize(query);
	}
}
