#include <stdio.h>
#include <malloc.h>

int main(int argc,char* argv[],char** envp)
{
	int i;

	printf("Argument:\n");
	for(i=0;i<argc;i++)
	{
		printf("argv[%d]=%s\n",i,argv[i]);
	}

	printf("Environment:\n");
	for(i=0;envp[i]!=NULL;i++)
	{
		printf("%s\n",envp[i]);
	}

	return 0;
}
