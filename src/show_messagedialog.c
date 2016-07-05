#include "chunk.h"

int show_error_message(SSE_APP* app, const char* secondary_text)
{
	g_object_set(G_OBJECT(app->error_messagedialog), "text", secondary_text, NULL);
	gint result = gtk_dialog_run(GTK_DIALOG(app->error_messagedialog));
	gtk_widget_hide(GTK_WIDGET(app->error_messagedialog));

	return result;
}

int show_confirm_message(SSE_APP* app, const char* text)
{
	g_object_set(G_OBJECT(app->confirm_messagedialog), "text", text, NULL);
	gint result = gtk_dialog_run(GTK_DIALOG(app->confirm_messagedialog));
	gtk_widget_hide(GTK_WIDGET(app->confirm_messagedialog));

	return result;
}
