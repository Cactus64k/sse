#include "init.h"

int init_sql_query_entry(SSE_APP* app, int count)
{
	GList* list = NULL;

	for(int i=0; i<count; i++)
	{
		list = g_list_append(list, NULL);
	}

	GList* first	= g_list_first(list);
	GList* last		= g_list_last(list);

	last->next		= first;
	first->prev		= last;

	app->query_list	= list;
	app->last_query_item = first;

	return EXIT_SUCCESS;
}
