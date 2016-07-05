#include "../../../chunk.h"

G_MODULE_EXPORT void row_delete_menuitem_activate_cb(GtkMenuItem* menuitem, gpointer user_data)
{
	SSE_APP* app			= (SSE_APP*)user_data;
	GtkTreeView* tree_view	= app->main_tree_view;
	char* table_name		= NULL;
	if(	tree_view != NULL
		&&
		app->db != NULL
		&&
		get_selected_table_name(app, &table_name) == TRUE
		&&
		show_confirm_message(app, "Delete selected rows?") == GTK_RESPONSE_YES)
	{
		GtkTreeModel* model					= gtk_tree_view_get_model(tree_view);
		GtkTreeSelection* selection			= gtk_tree_view_get_selection(tree_view);
		uint32_t row_count					= gtk_tree_selection_count_selected_rows(selection);
		GList* list_path					= gtk_tree_selection_get_selected_rows(selection, NULL);
		sqlite3_stmt* query					= NULL;
		GtkTreeRowReference** row_reference	= g_malloc(row_count * sizeof(GtkTreeRowReference*));
		size_t index						= 0;
		for(GList* item=list_path; item!=NULL; item=item->next, index++)
			row_reference[index] = gtk_tree_row_reference_new(model, item->data);

		g_string_erase(app->query_buff, 0, -1);
		g_string_printf(app->query_buff, "DELETE FROM %s WHERE rowid == ?;", table_name);

		gboolean error = FALSE;

		if(sqlite3_prepare(app->db, app->query_buff->str, -1, &query, NULL) != SQLITE_ERROR)
		{
			GtkTreeIter iter;
			for(size_t i=0; i<row_count && error != TRUE; i++)
			{
				GtkTreePath* path	= gtk_tree_row_reference_get_path(row_reference[i]);
				gint64 row_id		= 0;
				if(gtk_tree_model_get_iter(model, &iter, path) == TRUE)
				{
					gtk_tree_model_get(model, &iter, 0, &row_id, -1);

					sqlite3_bind_int64(query, 1, row_id);
					if(sqlite3_step(query) != SQLITE_ERROR)
						gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
					else
						error = TRUE;
				}
				sqlite3_reset(query);

				gtk_tree_path_free(path);
			}
		}
		else
			error = TRUE;

		if(error == TRUE)
		{
			const char* error_msg = sqlite3_errmsg(app->db);
			show_error_message(app, error_msg);
		}

		g_list_free_full(list_path, (GDestroyNotify)gtk_tree_path_free);

		for(size_t i=0; i<row_count; i++)
			gtk_tree_row_reference_free(row_reference[i]);

		g_free(row_reference);
		sqlite3_finalize(query);
		g_free(table_name);
	}
}
