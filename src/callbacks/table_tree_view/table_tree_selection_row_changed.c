#include "../../chunk.h"

void table_tree_selection_row_changed(GtkTreeSelection* treeselection, gpointer user_data)
{
	SSE_APP* app		= (SSE_APP*)user_data;
	GtkTreeModel* model	= NULL;
	GtkTreeIter iter;

	if(gtk_tree_selection_get_selected(treeselection, &model, &iter) == TRUE)
	{
		char* table_name = NULL;
		gtk_tree_model_get(GTK_TREE_MODEL(app->table_list_store), &iter, 0, &table_name, -1);

		g_string_erase(app->query_buff, 0, -1);
		g_string_printf(app->query_buff, "SELECT rowid,* FROM %s;", table_name);

		sqlite3_stmt* query = NULL;
		if(sqlite3_prepare(app->db, app->query_buff->str, -1, &query, NULL) == SQLITE_OK)
		{
			reset_main_tree_view(app);
			init_main_tree_view(app, query, TRUE);
			init_main_list_store(app->main_list_store, query);
		}
		else
		{
			const char* error_msg = sqlite3_errmsg(app->db);
			show_error_message(app, error_msg);
		}

		g_free(table_name);
		sqlite3_finalize(query);
	}
}
