#include "../../chunk.h"

G_MODULE_EXPORT void cell_edited(GtkCellRendererText* cell, const gchar* path_string, const gchar* new_text, gpointer user_data)
{
	SSE_APP* app				= (SSE_APP*)user_data;
	GtkTreeModel *model			= GTK_TREE_MODEL(app->main_list_store);
	sqlite3_stmt* query			= NULL;
	char* table_name			= NULL;
	GtkTreeIter iter;

	if(	gtk_tree_model_get_iter_from_string(model, &iter, path_string) == TRUE
		&&
		get_selected_table_name(app, &table_name) == TRUE)
	{
		int column_index		= GPOINTER_TO_INT(g_object_get_data(G_OBJECT(cell), "column_index"));
		const char* column_type = g_object_get_data(G_OBJECT(cell), "column_type");
		const char* column_name = g_object_get_data(G_OBJECT(cell), "column_name");
		char* column_type_up	= (column_type == NULL)? NULL: g_utf8_strup(column_type, -1);

		g_string_erase(app->query_buff, 0, -1);
		g_string_printf(app->query_buff, "UPDATE %s SET %s=? WHERE rowid == ?;",  table_name, column_name);

		if(sqlite3_prepare(app->db, app->query_buff->str, -1, &query, NULL) == SQLITE_OK)
		{
			GType g_column_type = gtk_tree_model_get_column_type(model, column_index);

			gint64 rowid = 0;
			gtk_tree_model_get(GTK_TREE_MODEL(app->main_list_store), &iter, 0, &rowid, -1);
			sqlite3_bind_int64(query, 2, rowid);

			if(g_column_type == G_TYPE_STRING)
			{
				if(*new_text == '\0')
				{
					sqlite3_bind_null(query, 1);
					if(sqlite3_step(query) != SQLITE_ERROR)
						gtk_list_store_set(app->main_list_store, &iter, column_index, NULL, -1);
				}
				else if(column_type_up != NULL && strcmp(column_type_up, "DATE") == 0)
				{
					struct tm ts;
					memset(&ts, 0, sizeof(ts));

					if(sscanf(new_text, "%d:%d:%d - %d.%d.%d",	&(ts.tm_hour), &(ts.tm_min), &(ts.tm_sec),
																&(ts.tm_mday), &(ts.tm_mon), &(ts.tm_year)) == 6)
					{
						ts.tm_year	-= 1900;
						ts.tm_mon	-=1;

						long long int t1 = mktime(&ts);

						t1 = (t1 != -1)? t1: 0;

						sqlite3_bind_int64(query, 1, t1);
						if(sqlite3_step(query) != SQLITE_ERROR)
						{
							if(t1 == 0)
								gtk_list_store_set(app->main_list_store, &iter, column_index, "00:00:00 - 01.01.1970", -1);
							else
								gtk_list_store_set(app->main_list_store, &iter, column_index, new_text, -1);
						}

					}
				}
				else
				{
					sqlite3_bind_text(query, 1, new_text, -1, SQLITE_STATIC);
					if(sqlite3_step(query) != SQLITE_ERROR)
						gtk_list_store_set(app->main_list_store, &iter, column_index, new_text, -1);
				}
			}
			else if(g_column_type == G_TYPE_INT64)
			{
				long long int num = strtoll(new_text, NULL, 10);
				sqlite3_bind_int64(query, 1, num);
				if(sqlite3_step(query) != SQLITE_ERROR)
					gtk_list_store_set(app->main_list_store, &iter, column_index, num, -1);
			}
			else if(g_column_type == G_TYPE_DOUBLE)
			{
				double num = strtod(new_text, NULL);
				sqlite3_bind_double(query, 1, num);
				if(sqlite3_step(query) != SQLITE_ERROR)
					gtk_list_store_set(app->main_list_store, &iter, column_index, num, -1);
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

		g_free(column_type_up);
	}

	g_free(table_name);
}
