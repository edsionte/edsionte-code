#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv)
{
	int mode;
	int mode_u;
	int mode_g;
	int mode_o;
	char *path = NULL;

	if (argc < 3) {
		printf("%s <mode> <path>\n", argv[0]);
		exit(1);
	}

	mode = atoi(argv[1]);
	if (mode > 777 || mode < 0) {
		printf("mode num error!\n");
		exit(1);
	}

	mode_u = mode / 100;
	mode_g = (mode - (mode_u * 100)) / 10;
	mode_o = mode - mode_u * 100 - mode_g * 10;
	mode = mode_u * 8 * 8 + mode_g * 8 + mode_o;

	path = argv[2];

	if (chmod(path, mode) == -1) {
		printf("chmod error: %s\n", strerror(errno));
		exit(1);
	}

	return 0;
}
