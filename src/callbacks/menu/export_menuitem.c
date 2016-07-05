#include "../../chunk.h"

G_MODULE_EXPORT void export_menuitem_activate_cb(GtkMenuItem* item, gpointer user_data)
{
	SSE_APP* app		= (SSE_APP*)user_data;
	if(app->db != NULL)
	{
		char buff[L_tmpnam + sizeof("export.xml")];

		strncpy(buff, tmpnam(NULL), sizeof(buff)-1);
		strncat(buff, "export.xml", sizeof(buff)-1);

		char* path			= g_build_path(G_DIR_SEPARATOR_S, app->tmp_dir_name, buff, NULL);

		excel_xml_tree_model_export(path, app->main_tree_view);

		char* command = g_strdup_printf(EXEL_COMMAND_TEMPLATE, path);
		system(command);

		g_free(path);
		g_free(command);
	}
}
