#include "chunk.h"

int open_db(SSE_APP* app, char* path)
{
	int result = EXIT_SUCCESS;
	if(path != NULL)
	{
		if(app->db != NULL)
		{
			sqlite3_close(app->db);
			app->db = NULL;
		}

		if(sqlite3_open(path, &(app->db)) == SQLITE_OK)
		{
			reset_main_tree_view(app);
			init_table_list_store(app);
		}
		else
		{
			const char* error_msg = sqlite3_errmsg(app->db);
			show_error_message(app, error_msg);

			sqlite3_close(app->db);
			app->db = NULL;
			result = EXIT_FAILURE;
		}
	}
	else
		result = EXIT_FAILURE;

	return result;
}
