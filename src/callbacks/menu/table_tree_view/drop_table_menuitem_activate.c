#include "../../../chunk.h"

G_MODULE_EXPORT void drop_table_menuitem_activate_cb(GtkImageMenuItem* item, gpointer user_data)
{
	SSE_APP* app			= (SSE_APP*)user_data;
	GtkTreeView* tree_view	= app->table_tree_view;
	if(	tree_view != NULL && app->db != NULL && show_confirm_message(app, "Drop selected tables?") == GTK_RESPONSE_YES)
	{
		GtkTreeModel* model					= gtk_tree_view_get_model(tree_view);
		GtkTreeSelection* selection			= gtk_tree_view_get_selection(tree_view);
		uint32_t row_count					= gtk_tree_selection_count_selected_rows(selection);
		GList* list_path					= gtk_tree_selection_get_selected_rows(selection, NULL);
		GtkTreeRowReference** row_reference	= g_malloc(row_count * sizeof(GtkTreeRowReference*));
		size_t index						= 0;
		for(GList* item=list_path; item!=NULL; item=item->next, index++)
			row_reference[index] = gtk_tree_row_reference_new(model, item->data);

		GtkTreeIter iter;
		gboolean error = FALSE;
		for(size_t i=0; i<row_count && error != TRUE; i++)
		{
			GtkTreePath* path	= gtk_tree_row_reference_get_path(row_reference[i]);
			if(gtk_tree_model_get_iter(model, &iter, path) == TRUE)
			{
				char* table_name	= NULL;

				gtk_tree_model_get(model, &iter, 0, &table_name, -1);

				g_string_erase(app->query_buff, 0, -1);
				g_string_printf(app->query_buff, "DROP TABLE %s;", table_name);

				if(sqlite3_exec(app->db, app->query_buff->str, NULL, NULL, NULL) != SQLITE_ERROR)
					gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
				else
					error = TRUE;

				g_free(table_name);
			}

			gtk_tree_path_free(path);
		}

		if(error == TRUE)
		{
			const char* error_msg = sqlite3_errmsg(app->db);
			show_error_message(app, error_msg);
		}


		g_list_free_full(list_path, (GDestroyNotify)gtk_tree_path_free);

		for(size_t i=0; i<row_count; i++)
			gtk_tree_row_reference_free(row_reference[i]);

		g_free(row_reference);
	}

}
