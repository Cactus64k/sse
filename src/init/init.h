
#ifndef INIT_H_
#define INIT_H_

	#include "../chunk.h"
	int init_main_tree_view(SSE_APP* app, sqlite3_stmt* query, gboolean editable);
	int init_table_list_store(SSE_APP* app);
	GType sql_to_glib_type(const char* type);
	int reset_main_tree_view(SSE_APP* app);
	int init_sql_query_entry(SSE_APP* app, int count);

#endif /* INIT_H_ */
