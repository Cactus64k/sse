#include "../chunk.h"

static gboolean is_date_column(sqlite3_stmt* query, int i);

int init_main_list_store(GtkListStore* list_store,  sqlite3_stmt* query)
{
	if(gtk_tree_model_get_n_columns(GTK_TREE_MODEL(list_store)) != sqlite3_column_count(query))		// последний столбец пустой
	{
		fputs("ERROR: different columns count\n", stderr);
		return EXIT_FAILURE;
	}

	GtkTreeIter iter;
	int column_count = gtk_tree_model_get_n_columns(GTK_TREE_MODEL(list_store));

	while(sqlite3_step(query) == SQLITE_ROW)
	{
		gtk_list_store_append(list_store, &iter);

		for(int i=0; i<column_count; i++)
		{
			GType column_type = gtk_tree_model_get_column_type(GTK_TREE_MODEL(list_store), i);

			if(column_type == G_TYPE_STRING)
			{
				if(is_date_column(query, i) == TRUE)
				{
					time_t bin_time = (time_t)sqlite3_column_int64(query, i);
					if(bin_time != -1)
					{
						char time_str[32];
						struct tm* struct_time = localtime(&bin_time);
						strftime(time_str, sizeof(time_str), "%H:%M:%S - %d.%m.%Y", struct_time);
						gtk_list_store_set(list_store, &iter, i, time_str, -1);
					}
					else
						gtk_list_store_set(list_store, &iter, i, "-1", -1);
				}
				else
				{
					const unsigned char* text	= sqlite3_column_text(query, i);
					gtk_list_store_set(list_store, &iter, i, text, -1);
				}
			}
			else if(column_type == G_TYPE_INT64)
			{
				sqlite_int64 num	= sqlite3_column_int64(query, i);
				gtk_list_store_set(list_store, &iter, i, num, -1);
			}
			else if(column_type == G_TYPE_INT64)
			{
				int num	= sqlite3_column_int(query, i);
				gtk_list_store_set(list_store, &iter, i, num, -1);
			}
			else if(column_type == G_TYPE_DOUBLE)
			{
				double num = sqlite3_column_double(query, i);
				gtk_list_store_set(list_store, &iter, i, num, -1);
			}
			else if(column_type == G_TYPE_BOOLEAN)
			{
				 int flag = sqlite3_column_int(query, i);
				gtk_list_store_set(list_store, &iter, i, flag, -1);
			}
			else if(column_type == G_TYPE_BYTE_ARRAY)
			{
				const void* data = sqlite3_column_blob(query, i);
				gtk_list_store_set(list_store, &iter, i, data, -1);
			}
		}
	}

	return EXIT_SUCCESS;
}

static gboolean is_date_column(sqlite3_stmt* query, int i)
{
	gboolean res;
	const char* column_type		= sqlite3_column_decltype(query, i);
	char* column_type_up		= (column_type == NULL)? NULL: g_utf8_strup(column_type, -1);

	if((column_type_up != NULL) && (strcmp(column_type_up, "DATE") == 0))
		res = TRUE;
	else
		res = FALSE;

	g_free(column_type_up);

	return res;
}


