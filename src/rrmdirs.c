
#include "chunk.h"
#include <sys/stat.h>
#include <dirent.h>

int rrmdirs(char* path_buff, size_t path_buff_max_size)
{
	struct stat st;
	struct dirent* cur_dir;

	int res = stat(path_buff, &st);
	if(res != 0)
		return EXIT_FAILURE;

	if(S_ISDIR(st.st_mode))
	{
		size_t path_buff_size = strlen(path_buff);

		DIR* d = opendir(path_buff);
		if(d == NULL)
			return EXIT_FAILURE;

		while((cur_dir = readdir(d)) != NULL)
		{
			if(strcmp(cur_dir->d_name, "..") && strcmp(cur_dir->d_name, "."))
			{
				strncat(path_buff, "/",				path_buff_max_size-1);
				strncat(path_buff, cur_dir->d_name,	path_buff_max_size-1);

				res = rrmdirs(path_buff, path_buff_max_size);

				path_buff[path_buff_size] = 0;
			}
		}
		res = closedir(d);

		res = rmdir(path_buff);
	}
	else
		res = remove(path_buff);

	return res;
}
