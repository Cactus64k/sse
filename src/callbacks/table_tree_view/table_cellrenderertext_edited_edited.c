#include "../../chunk.h"

G_MODULE_EXPORT void table_cellrenderertext_edited_edited_cb(GtkCellRendererText* cell_renderer, const gchar* path_string, const gchar* new_text, gpointer user_data)
{
	SSE_APP* app		= (SSE_APP*)user_data;
	GtkTreeModel* model	= gtk_tree_view_get_model(app->table_tree_view);
	GtkTreePath* path	= gtk_tree_path_new_from_string(path_string);
	GtkTreeIter iter;

	if(gtk_tree_model_get_iter(model, &iter, path) == TRUE)
	{
		char* table_name = NULL;
		gtk_tree_model_get(GTK_TREE_MODEL(app->table_list_store), &iter, 0, &table_name, -1);

		if(strcmp(table_name, new_text) != 0)
		{
			g_string_erase(app->query_buff, 0, -1);
			g_string_printf(app->query_buff, "ALTER TABLE %s RENAME TO %s", table_name, new_text);

			if(sqlite3_exec(app->db, app->query_buff->str, NULL, NULL, NULL) == SQLITE_OK)
			{
				gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, new_text, -1);
			}
			else
			{
				const char* error_msg = sqlite3_errmsg(app->db);
				show_error_message(app, error_msg);
			}
		}

		g_free(table_name);
	}

	gtk_tree_path_free(path);
}
