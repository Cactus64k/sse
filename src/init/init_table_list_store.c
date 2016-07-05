#include "../chunk.h"


int init_table_list_store(SSE_APP* app)
{
	sqlite3_stmt* query = NULL;

	if(sqlite3_prepare(app->db, "SELECT name FROM sqlite_master;", -1, &query, NULL) == SQLITE_OK)
	{
		GtkTreeIter iter;
		GtkTreeView* table_tree_view	= app->table_tree_view;
		GtkListStore* table_list_store	= app->table_list_store;
		GtkTreeSelection* selection		= gtk_tree_view_get_selection(table_tree_view);
		gtk_tree_selection_unselect_all(selection);
		gtk_list_store_clear(app->table_list_store);

		while(sqlite3_step(query) == SQLITE_ROW)
		{
			const char* table_name = (const char*)sqlite3_column_text(query, 0);

			gtk_list_store_append(table_list_store, &iter);
			gtk_list_store_set(table_list_store, &iter, 0, table_name, -1);
		}

		sqlite3_finalize(query);

		return EXIT_SUCCESS;
	}
	else
	{
		const char* error_msg = sqlite3_errmsg(app->db);
		show_error_message(app, error_msg);
		return EXIT_FAILURE;
	}



}
