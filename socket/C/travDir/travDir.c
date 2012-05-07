#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include "list.h"

int read_dir(const char *path)
{
	DIR *dir;
	struct dirent *ptr;
	struct stat fstat;

	if ((dir = opendir(path)) == NULL) {
		printf("opendir error\n");
		return -1;
	}

	while ((ptr = readdir(dir)) != NULL) {
		if (strcmp(ptr->d_name, ".") == 0)
			continue;
		if (strcmp(ptr->d_name, "..") == 0)
			continue;

		char bufname[512] = {0};
		strcpy(bufname, path);
		strcat(bufname, "/");
		strcat(bufname, ptr->d_name);
		
		
		if (stat(bufname, &fstat) == -1) {
			printf("stat error\n");
			return -1;
		}

		if (S_ISDIR(fstat.st_mode) == 1) {
			/*
			char dirname[512] = {0};
			strcpy(dirname, path);
			strcat(dirname, "/");
			strcat(dirname, ptr->d_name);
			*/
			read_dir(bufname);
		} else {
			/*
			char filename[512] = {0};
			strcpy(filename, path);
			strcat(filename, "/");
			strcat(filename, ptr->d_name);
			*/
			read_file(bufname);
		}
	}

	closedir(dir);

	return 0;
}

int read_file(const char *path)
{
	printf("%s\n", path);
	return 0;
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("listfile <target path>\n");
		exit(1);
	}

	if (read_dir(argv[1]) < 0) {
		exit(1);
	}

	return 0;
}
