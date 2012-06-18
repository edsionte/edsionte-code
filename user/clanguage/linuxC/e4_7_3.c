#include <stdio.h>
#include <string.h>

void delete(char* str,int start,int num)
{
	int i,k=0;
	int len=strlen(str);

	for(i=start;(i+num)<len;i++)
	{
		str[i]=str[i+num];
	}
	str[i]='\0';
}

int main()
{
	int start,num;
	char str[100];
	int len;

	printf("please input a string:\n");
//	scanf("%s",str);//the input string can not include bankspace
        gets(str);
	len=strlen(str);
	printf("please input the starting index:");
        scanf("%d",&start);
	while(1)
	{
		if(start>=len||start<0)
		{
			puts("illegal input num!\ntry again:");
			scanf("%d",&start);
		}
		else break;
	}
	printf("please input the the number of character which be deleted:");
	scanf("%d",&num);
	while(1)
	{
		if((start+num)>=len||(start+num)<start)
		{

			puts("illegal input num!\ntry again:");
			scanf("%d",&num);
		}
		else break;
	}
	//check the legality of start and num
	delete(str,start,num);
	puts(str);
	return 0;
}
