#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char **argv)
{
	DIR *dir;
	struct dirent *ptr = NULL;

	char path1[20] = "/root";
	char path2[20] = "/usr";
/*
	if ((dir = opendir(argv[1])) == NULL) {
		printf("not cd..\n");
	} else {
		printf("cd..\n");
	}
	closedir(dir);
	*/

	if (chdir(argv[1]) < 0) {
		printf("not cd..\n");
	} else {
		printf("cd..\n");
	}

	return 0;
}
