
#ifndef CHUNK_H_
	#define CHUNK_H_

	#include <stdlib.h>
	#include <string.h>
	#include <stdio.h>
	#include <stdint.h>
	#include <locale.h>
	#include <time.h>
	#include <assert.h>
	#include <sqlite3.h>
	#include <gtk/gtk.h>
	#include <glib.h>

	#define LIBXML_TREE_ENABLED

	#include <libxml2/libxml/parser.h>
	#include <libxml2/libxml/xmlwriter.h>
	#include <libxml2/libxml/xpath.h>
	#include <libxml2/libxml/tree.h>

	typedef struct SSE_APP
	{
		sqlite3*			db;
		GtkWindow*			main_wnd;

		GtkTreeView*		main_tree_view;
		GtkListStore*		main_list_store;
		GtkMenu*			main_tree_view_menu;

		GtkListStore*		table_list_store;
		GtkTreeView*		table_tree_view;
		GtkMenu*			table_tree_view_menu;

		GtkDialog*			db_open_dialog;
		GtkDialog*			about_dialog;
		GtkDialog*			sql_help_dialog;

		GtkMessageDialog*	error_messagedialog;
		GtkMessageDialog*	confirm_messagedialog;

		GString*			query_buff;

		char*				tmp_dir_name;

		GList*				query_list;
		GList*				last_query_item;

	} SSE_APP;

	#define GUI_CONSTRUCT_PATH "sse.glade"

	#define EXEL_COMMAND_TEMPLATE "start \"\" \"%s\""

	#define SQL_QUERY_LINE_BUFF_SIZE 10

	#include "init/init.h"

	int show_error_message(SSE_APP* app, const char* secondary_text);
	int show_confirm_message(SSE_APP* app, const char* text);
	int open_db(SSE_APP* app, char* path);
	int init_main_list_store(GtkListStore* list_store,  sqlite3_stmt* query);
	int reset_main_tree_view(SSE_APP* app);
	gboolean get_selected_table_name(SSE_APP* app, char** table_name);
	int excel_xml_tree_model_export(char* file_path, GtkTreeView* tree_view);
	int rrmdirs(char* path_buff, size_t path_buff_max_size);

	G_MODULE_EXPORT void cell_toggle(GtkCellRendererToggle* cell_renderer, gchar* path, gpointer user_data);
	G_MODULE_EXPORT void cell_edited (GtkCellRendererText* cell, const gchar* path_string, const gchar* new_text, gpointer data);
	G_MODULE_EXPORT void table_tree_selection_row_changed(GtkTreeSelection* treeselection, gpointer user_data);

#endif /* CHUNK_H_ */
