#include "../chunk.h"

GType sql_to_glib_type(const char* type)
{
	GType cur_type	= G_TYPE_NONE;

	if(type != NULL)
	{
		if(	(strcmp(type,		"TEXT") == 0) ||
			(strncmp(type,		"CHAR", strlen("CHAR")) == 0) ||
			(strcmp(type,		"DATE") == 0) ||
			(strcmp(type,		"DESCRIPTION") == 0))
			cur_type = G_TYPE_STRING;
		else if((strcmp(type, 	"FLOAT") == 0) ||
				(strcmp(type,	"REAL") == 0) ||
				(strcmp(type,	"DOUBLE") == 0))
			cur_type = G_TYPE_DOUBLE;
		else if((strcmp(type, 	"INTEGER") == 0) ||
				(strcmp(type, 	"INT") == 0))
			cur_type = G_TYPE_INT64;
		else if((strcmp(type, 	"BOOLEAN") == 0) ||
				(strcmp(type, 	"BOOL") == 0))
			cur_type = G_TYPE_BOOLEAN;
		else if(strcmp(type, 	"BLOB") == 0)
			cur_type = G_TYPE_BYTE_ARRAY;
		else
		{
			fprintf(stderr, "ERROR: uncnown column type %s\n", type);
			cur_type = G_TYPE_STRING;
		}
	}
	else
		cur_type = G_TYPE_STRING;

	return cur_type;
}
