#include "chunk.h"

int main(int argc,	char *argv[])
{
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	gtk_init(&argc, &argv);

	GtkBuilder* builder = gtk_builder_new();

	GError* error = NULL;

	if(gtk_builder_add_from_file(builder, GUI_CONSTRUCT_PATH, &error) == 0)
	{
		fprintf(stderr, "ERROR: %s\n", error->message);
		g_error_free(error);
		fgetc(stdin);
		exit(EXIT_FAILURE);
	}

	SSE_APP app;

	memset(&app, 0, sizeof(app));
	app.main_wnd				= GTK_WINDOW(			gtk_builder_get_object(builder, "main_wnd"));
	app.main_tree_view			= GTK_TREE_VIEW(		gtk_builder_get_object(builder, "main_tree_view"));
	app.main_tree_view_menu		= GTK_MENU(				gtk_builder_get_object(builder, "main_tree_view_menu"));
	app.table_list_store		= GTK_LIST_STORE(		gtk_builder_get_object(builder, "table_list_store"));
	app.table_tree_view			= GTK_TREE_VIEW(		gtk_builder_get_object(builder, "table_tree_view"));
	app.table_tree_view_menu	= GTK_MENU(				gtk_builder_get_object(builder, "table_tree_view_menu"));
	app.db_open_dialog			= GTK_DIALOG(			gtk_builder_get_object(builder, "db_chooser_dialog"));
	app.about_dialog			= GTK_DIALOG(			gtk_builder_get_object(builder, "about_dialog"));
	app.sql_help_dialog			= GTK_DIALOG(			gtk_builder_get_object(builder, "sql_help_dialog"));
	app.error_messagedialog		= GTK_MESSAGE_DIALOG(	gtk_builder_get_object(builder, "error_messagedialog"));
	app.confirm_messagedialog	= GTK_MESSAGE_DIALOG(	gtk_builder_get_object(builder, "confirm_messagedialog"));

	app.query_buff				= g_string_new(NULL);
	app.tmp_dir_name			= g_dir_make_tmp(NULL, NULL);


	GtkTreeSelection* selection	= gtk_tree_view_get_selection(app.table_tree_view);
	g_signal_connect(G_OBJECT(selection), "changed", G_CALLBACK(table_tree_selection_row_changed), &app);

	gtk_builder_connect_signals(builder, &app);

	init_sql_query_entry(&app, SQL_QUERY_LINE_BUFF_SIZE);

	if(argc > 1)
		open_db(&app, argv[1]);

	gtk_widget_show_all(GTK_WIDGET(app.main_wnd));
	gtk_main();



	return 0;
}

