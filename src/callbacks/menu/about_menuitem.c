#include "../../chunk.h"

G_MODULE_EXPORT void about_menuitem_activate_cb(GtkMenuItem* item, gpointer user_data)
{
	SSE_APP* app		= (SSE_APP*)user_data;
	gtk_dialog_run(app->about_dialog);
	gtk_widget_hide(GTK_WIDGET(app->about_dialog));

}
