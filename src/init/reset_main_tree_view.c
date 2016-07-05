#include "../chunk.h"

int reset_main_tree_view(SSE_APP* app)
{
	int columns_count = (signed)gtk_tree_view_get_n_columns(app->main_tree_view);
	for(int i=0; i<columns_count; i++)
	{
		GtkTreeViewColumn* column = gtk_tree_view_get_column(app->main_tree_view, columns_count - i - 1);
		gtk_tree_view_remove_column(app->main_tree_view, column);
	}

	return EXIT_SUCCESS;
}
