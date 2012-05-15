#include <stdio.h>
#include <string.h>

#define count 5
int main()
{
	int i,k,j;
        char mtime[count][20]={"2010-5-6 11:34:23","2010-7-6 11:34:23","2010-5-6 12:34:23","2011-5-6 11:34:23","2000-5-6 11:34:23"};
        char filename[count][20]={"file0","file1","file2","file3","file4"};
	char temp[20];
	//sorting
	for(i=0;i<count;i++)
	{
		k=i;
		for(j=i;j<count;j++)
		{
			if(strcmp(mtime[j],mtime[k])>0) k=j;
		}
		printf("%d-",k);
		if(k!=i)
		{
			strcpy(temp,mtime[i]);
			strcpy(mtime[i],mtime[k]);
			strcpy(mtime[k],temp);

			strcpy(temp,filename[i]);
			temp[strlen(filename[i])]='\0';
			strcpy(filename[i],filename[k]);
			filename[i][strlen(filename[k])]='\0';
			strcpy(filename[k],temp);
			filename[k][strlen(temp)]='\0';
		}
//		printf("i=%s %s\n",mtime[i],filename[i]);
//		printf("k=%s %s\n",mtime[k],filename[k]);
	}
	printf("------------\n");
	for(i=0;i<count;i++)
	{
		printf("%s %s\n",mtime[i],filename[i]);
	}

	return 0;
}
