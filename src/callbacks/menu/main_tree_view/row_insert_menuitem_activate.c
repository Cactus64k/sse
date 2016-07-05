#include "../../../chunk.h"

G_MODULE_EXPORT void row_insert_menuitem_activate_cb(GtkImageMenuItem* item, gpointer user_data)
{
	SSE_APP* app			= (SSE_APP*)user_data;
	char* table_name		= NULL;
	GtkTreeView* tree_view	= app->main_tree_view;

	if(	tree_view != NULL
		&&
		app->db != NULL
		&&
		get_selected_table_name(app, &table_name) == TRUE)
	{
		g_string_erase(app->query_buff, 0, -1);
		g_string_printf(app->query_buff, "INSERT INTO %s DEFAULT VALUES;", table_name);

		if(sqlite3_exec(app->db, app->query_buff->str, NULL, NULL, NULL) == SQLITE_OK)
		{
			sqlite3_int64 last_row_id = sqlite3_last_insert_rowid(app->db);

			GtkTreeIter iter;
			gtk_list_store_append(app->main_list_store, &iter);
			gtk_list_store_set(app->main_list_store, &iter, 0, last_row_id, -1);

			GtkTreePath* path			= gtk_tree_model_get_path(GTK_TREE_MODEL(app->main_list_store), &iter);
			GtkTreeViewColumn* column	= gtk_tree_view_get_column(tree_view, 0);
			gtk_tree_view_scroll_to_cell(tree_view, path, column, FALSE, 0.f, 0.f);
			gtk_tree_selection_select_iter(gtk_tree_view_get_selection(tree_view), &iter);
			gtk_widget_grab_focus(GTK_WIDGET(tree_view));

			gtk_tree_path_free(path);

		}
		else
		{
			const char* error_msg = sqlite3_errmsg(app->db);
			show_error_message(app, error_msg);
		}
	}

	g_free(table_name);
}
