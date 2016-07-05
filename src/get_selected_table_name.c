#include "chunk.h"

gboolean get_selected_table_name(SSE_APP* app, char** table_name)
{
	GtkTreeSelection* selection	= gtk_tree_view_get_selection(app->table_tree_view);
	GtkTreeModel* table_model	= NULL;
	GtkTreeIter table_iter;
	if(gtk_tree_selection_get_selected(selection, &table_model, &table_iter) == TRUE)
		gtk_tree_model_get(table_model, &table_iter, 0, table_name, -1);
	else
		return FALSE;

	return TRUE;
}
