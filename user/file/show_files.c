#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <dirent.h>

int my_readdir(const char* path)
{
	DIR *dir;
	struct dirent *ptr;
	int i=0;

	if((dir=opendir(path))==NULL)
	{
		perror("opendir");
		return -1;
	}
	while((ptr=readdir(dir))!=NULL)
	{
		if(i%6==0&&i!=0)
		{
			printf("\n");
		}
		printf("%s ",ptr->d_name);
		i++;
	}
}
int main(int argc,char** argv)
{
	if(argc<2)
	{
		printf("error:show_files <target path>\n");
		exit(1);
	}
	if(my_readdir(argv[1])<0)
	{
		exit(1);
	}
        printf("\n");
	return 0;
}
