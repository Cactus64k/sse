#include "../../chunk.h"

G_MODULE_EXPORT void cell_toggle(GtkCellRendererToggle* cell_renderer, gchar* path, gpointer user_data)
{
	SSE_APP* app				= (SSE_APP*)user_data;
	GtkListStore* list_store	= app->main_list_store;
	sqlite3_stmt* query			= NULL;
	GString* query_buff			= app->query_buff;
	GtkTreeIter iter;

	if(gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(list_store), &iter, path) == TRUE)
	{
		int column_index		= GPOINTER_TO_INT(g_object_get_data(G_OBJECT(cell_renderer), "column_index"));
		const char* column_name = g_object_get_data(G_OBJECT(cell_renderer), "column_name");

		g_string_erase(query_buff, 0, -1);
		g_string_printf(query_buff, "UPDATE objects SET %s=? WHERE rowid == ?;", column_name);

		if(sqlite3_prepare(app->db, query_buff->str, -1, &query, NULL) == SQLITE_OK)
		{
			gint64 rowid = 0;
			gtk_tree_model_get(GTK_TREE_MODEL(list_store), &iter, 0, &rowid, -1);
			sqlite3_bind_int64(query, 2, rowid);

			GType g_column_type = gtk_tree_model_get_column_type(GTK_TREE_MODEL(list_store), column_index);

			if(g_column_type == G_TYPE_BOOLEAN)
			{
				gboolean flag = FALSE;
				gtk_tree_model_get(GTK_TREE_MODEL(list_store), &iter, column_index, &flag, -1);

				sqlite3_bind_int(query, 1, (int)!flag);
				if(sqlite3_step(query) != SQLITE_ERROR)
					gtk_list_store_set(list_store, &iter, column_index, !flag, -1);
			}
		}
		else
		{
			if(app->main_list_store != NULL)
				gtk_list_store_clear(app->main_list_store);

			const char* error_msg = sqlite3_errmsg(app->db);
			show_error_message(app, error_msg);
		}
		sqlite3_finalize(query);
	}
}
