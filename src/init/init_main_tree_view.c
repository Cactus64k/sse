#include "../chunk.h"

int init_main_tree_view(SSE_APP* app, sqlite3_stmt* query, gboolean editable)
{
	GArray* type_array		= g_array_new(TRUE, FALSE, sizeof(GType));
	int column_count		= sqlite3_column_count(query);

	for(int i=0; i<column_count; i++)
	{
		const char* column_name		= (const char*)sqlite3_column_name(query, i);
		const char* column_type		= (const char*)sqlite3_column_decltype(query, i);
		char* column_type_up		= (column_type == NULL)? NULL: g_utf8_strup(column_type, -1);
		GType cur_type				= sql_to_glib_type(column_type_up);

		GtkCellRenderer* renderer	= NULL;
		GtkTreeViewColumn* column	= gtk_tree_view_column_new();
		GtkWidget* label			= gtk_label_new(NULL);
		gtk_tree_view_column_set_resizable(column, TRUE);
		gtk_tree_view_column_set_sort_column_id(column, i);

		char buff[128];
		snprintf(buff, sizeof(buff), "%s\n%s", column_name, column_type);
		gtk_label_set_text(GTK_LABEL(label), buff);
		gtk_widget_show(label);
		g_object_set(column, "widget", label, NULL);

		if(cur_type == G_TYPE_BOOLEAN)
		{
			renderer = gtk_cell_renderer_toggle_new();
			gtk_tree_view_column_pack_start(column, renderer, TRUE);
			gtk_cell_renderer_toggle_set_activatable(GTK_CELL_RENDERER_TOGGLE(renderer), TRUE);
			g_signal_connect(renderer, "toggled", G_CALLBACK(cell_toggle), app);
			gtk_tree_view_column_set_attributes(column, renderer, "active", i, NULL);
		}
		else
		{
			renderer = gtk_cell_renderer_text_new();
			gtk_tree_view_column_pack_start(column, renderer, TRUE);
			g_object_set(G_OBJECT(renderer), "editable", editable, NULL);
			g_signal_connect(renderer, "edited", G_CALLBACK(cell_edited), app);
			gtk_tree_view_column_set_attributes(column, renderer, "text", i, NULL);
		}


		g_object_set_data(G_OBJECT(renderer),		"column_index",		GINT_TO_POINTER(i));
		g_object_set_data(G_OBJECT(column),			"column_g_type",	GINT_TO_POINTER(cur_type));
		g_object_set_data_full(G_OBJECT(renderer),	"column_name",		g_strdup(column_name), g_free);
		g_object_set_data_full(G_OBJECT(renderer),	"column_type",		column_type_up, g_free);

		gtk_tree_view_append_column(app->main_tree_view, column);

		g_array_append_val(type_array, cur_type);
	}

	if(column_count > 0)
	{
		GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
		GtkTreeViewColumn* column = gtk_tree_view_column_new_with_attributes("", renderer, NULL);		// последний пустой столбец для нормальной длины предпоследнего столбца
		gtk_tree_view_append_column(app->main_tree_view, column);

		GtkTreeSelection* selection	= gtk_tree_view_get_selection(app->main_tree_view);
		gtk_tree_selection_set_mode(selection, GTK_SELECTION_MULTIPLE);

		app->main_list_store	= gtk_list_store_newv(column_count, (GType*)(type_array->data));				// при отсутствии столбцов ошибка

		gtk_tree_view_set_reorderable(app->main_tree_view, TRUE);
		gtk_tree_view_set_model(app->main_tree_view, GTK_TREE_MODEL(app->main_list_store));
	}
	g_array_free(type_array, TRUE);

	return EXIT_SUCCESS;
}
