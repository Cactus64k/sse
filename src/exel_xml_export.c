#include "chunk.h"

int excel_xml_tree_model_export(char* file_path, GtkTreeView* tree_view)
{
	assert(file_path != NULL);
	assert(tree_view != NULL);

	GtkTreeModel* model	= gtk_tree_view_get_model(tree_view);

	if(model != NULL)
	{
		xmlDoc* doc					= xmlNewDoc("1.0");
		xmlNode* node				= xmlNewPI("mso-application", "progid=\"Excel.Sheet\"");
		xmlDocSetRootElement(doc, node);

		xmlNode* Workbook_node		= xmlNewNode(NULL, "Workbook");
		xmlNs* param_ns				= xmlNewNs(Workbook_node, "urn:schemas-microsoft-com:office:spreadsheet", "ss");
		xmlNewNs(Workbook_node, "urn:schemas-microsoft-com:office:spreadsheet", NULL);
		xmlAddChild(doc, Workbook_node);

		xmlNode* Styles_node		= xmlNewNode(NULL, "Styles");
		xmlAddChild(Workbook_node, Styles_node);

		//################################################################
		{
			xmlNode* Style_def_node			= xmlNewNode(NULL, "Style");
			xmlSetNsProp(Style_def_node, param_ns, "ID", "Default");
			xmlAddChild(Styles_node, Style_def_node);

			xmlNode* Alignment_node		= xmlNewNode(NULL, "Alignment");
			xmlSetNsProp(Alignment_node, param_ns, "Horizontal", "Left");
			xmlSetNsProp(Alignment_node, param_ns, "Vertical", "Bottom");
			xmlAddChild(Style_def_node, Alignment_node);

			xmlNode* Font_node			= xmlNewNode(NULL, "Font");
			xmlSetNsProp(Font_node, param_ns, "Bold", "0");
			xmlSetNsProp(Font_node, param_ns, "FontName", "Calibri");
			xmlSetNsProp(Font_node, param_ns, "Size", "10");
			xmlAddChild(Style_def_node, Font_node);
		}

		{
			xmlNode* Style_bold_node	= xmlNewNode(NULL, "Style");
			xmlSetNsProp(Style_bold_node, param_ns, "ID", "Header");
			xmlAddChild(Styles_node, Style_bold_node);

			xmlNode* Font_node			= xmlNewNode(NULL, "Font");
			xmlSetNsProp(Font_node, param_ns, "Bold", "1");
			xmlSetNsProp(Font_node, param_ns, "FontName", "Calibri");
			xmlSetNsProp(Font_node, param_ns, "Size", "12");
			xmlAddChild(Style_bold_node, Font_node);

			xmlNode* Alignment_node				= xmlNewNode(NULL, "Alignment");
			xmlSetNsProp(Alignment_node, param_ns, "Horizontal", "Center");
			xmlSetNsProp(Alignment_node, param_ns, "Vertical", "Bottom");
			xmlAddChild(Style_bold_node, Alignment_node);
		}
		//################################################################

		xmlNode* Worksheet_node		= xmlNewNode(NULL, "Worksheet");
		xmlSetNsProp(Worksheet_node, param_ns, "Name", "Export");
		xmlAddChild(Workbook_node, Worksheet_node);

		xmlNode* Table_node			= xmlNewNode(NULL, "Table");
		xmlAddChild(Worksheet_node, Table_node);

		int column_count	= gtk_tree_model_get_n_columns(model);
		GtkTreeIter iter;

		gtk_tree_model_get_iter_first(model, &iter);

		char buff[256];
		for(int i=0; i<column_count; i++)
		{
			GtkTreeViewColumn* column = gtk_tree_view_get_column(tree_view, i);
			int width = gtk_tree_view_column_get_width(column);

			xmlNode* Column_node = xmlNewNode(NULL, "Column");
			snprintf(buff, sizeof(buff)-1, "%d", width);
			xmlSetNsProp(Column_node, param_ns, "Width", buff);
			xmlAddChild(Table_node, Column_node);
		}

		xmlNode* Row_node = xmlNewNode(NULL, "Row");
		xmlSetNsProp(Row_node, param_ns, "StyleID", "Header");
		xmlAddChild(Table_node, Row_node);

		for(int i=0; i<column_count; i++)
		{
			GtkTreeViewColumn* column	= gtk_tree_view_get_column(tree_view, i);
			GtkWidget* widget			= gtk_tree_view_column_get_widget(column);

			if(GTK_IS_LABEL(widget) == TRUE)
			{
				xmlNode* Cell_node		= xmlNewNode(NULL, "Cell");
				xmlAddChild(Row_node, Cell_node);

				xmlNode* Data_node		= xmlNewNode(NULL, "Data");
				const char* label_text	= gtk_label_get_text(GTK_LABEL(widget));
				char* div				= strchr(label_text, '\n');
				xmlSetNsProp(Data_node, param_ns, "Type", "String");
				xmlNodeSetContentLen(Data_node, label_text, (ptrdiff_t)(div - label_text));
				xmlAddChild(Cell_node, Data_node);
			}
		}

		while(gtk_list_store_iter_is_valid(GTK_LIST_STORE(model), &iter) == TRUE)
		{
			Row_node = xmlNewNode(NULL, "Row");
			xmlAddChild(Table_node, Row_node);
			gboolean empty_cell = FALSE;

			for(int i=0; i<column_count; i++)
			{
				xmlNode* Cell_node = xmlNewNode(NULL, "Cell");
				xmlAddChild(Row_node, Cell_node);
				if(empty_cell == TRUE)
				{
					snprintf(buff, sizeof(buff)-1, "%d", i+1);
					xmlSetNsProp(Cell_node, param_ns, "Index", buff);
				}

				xmlNode* Data_node = xmlNewNode(NULL, "Data");
				xmlAddChild(Cell_node, Data_node);

				GType col_type = gtk_tree_model_get_column_type(model, i);
				if(col_type == G_TYPE_STRING)
				{
					char* data = NULL;
					gtk_tree_model_get(model, &iter, i, &data, -1);

					if(data != NULL)
					{
						empty_cell = FALSE;

						xmlSetNsProp(Data_node, param_ns, "Type", "String");
						xmlNodeSetContent(Data_node, data);
						g_free(data);
					}
					else
					{
						empty_cell = TRUE;

						xmlUnlinkNode(Cell_node);
						xmlFreeNode(Cell_node);
					}
				}
				else if(col_type == G_TYPE_DOUBLE)
				{
					empty_cell = FALSE;

					double data = 0;
					gtk_tree_model_get(model, &iter, i, &data, -1);
					snprintf(buff, sizeof(buff)-1, "%g", data);
					xmlSetNsProp(Data_node, param_ns, "Type", "Number");
					xmlNodeSetContent(Data_node, buff);
				}
				else if(col_type == G_TYPE_INT64)
				{
					empty_cell = FALSE;

					int64_t data = 0;
					gtk_tree_model_get(model, &iter, i, &data, -1);
					snprintf(buff, sizeof(buff)-1, "%lld", data);
					xmlSetNsProp(Data_node, param_ns, "Type", "Number");
					xmlNodeSetContent(Data_node, buff);
				}
				else if(col_type == G_TYPE_BOOLEAN)
				{
					empty_cell = FALSE;

					gboolean data = FALSE;
					gtk_tree_model_get(model, &iter, i, &data, -1);
					snprintf(buff, sizeof(buff)-1, "%s", data? "TRUE": "FALSE");
					xmlSetNsProp(Data_node, param_ns, "Type", "String");
					xmlNodeSetContent(Data_node, buff);
				}
			}

			gtk_tree_model_iter_next(model, &iter);
		}

		xmlSaveFileEnc(file_path, doc, "utf-8");

		xmlFreeDoc(doc);
		xmlCleanupGlobals();

		return EXIT_SUCCESS;
	}
	else
		return EXIT_FAILURE;
}
