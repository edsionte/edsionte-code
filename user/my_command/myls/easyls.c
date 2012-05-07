#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

int get_file_name(char *path)
{
	DIR *mydir;
	struct dirent *myentry;

	if ((mydir = opendir(path)) == NULL) {
		printf("open dir error.\n");
		return -1;
	}

	int i = 0;
	while ((myentry = readdir(mydir)) != NULL) {
		printf("%s    ", myentry->d_name);
	}

	printf("\n");
	closedir(mydir);

	return 0;
}

int main()
{
	get_file_name("./");

	return 0;
}
