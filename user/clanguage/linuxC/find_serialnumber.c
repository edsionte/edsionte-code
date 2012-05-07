#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int findnumstring(char* outputstr,char* inputstr)
{
	int i,j,k;
	char* temp;
	int length=strlen(inputstr);

	for(i=0;i<length;i++)
	{
		outputstr[i]='\0';
	}

	temp=(char*)malloc(length+1);
	for(i=0;i<length;)
	{
		if(inputstr[i]>='0'&&inputstr[i]<='9')
		{
			k=0;
		        temp[k++]=inputstr[i];
			for(j=i+1;j<length;j++)
			{
				if(inputstr[j]>='0'&&inputstr[j]<='9')
				{
					temp[k++]=inputstr[j];
				}
				else break;
			}
			temp[k]='\0';
			if(strlen(temp)>strlen(outputstr))
			{
				strcpy(outputstr,temp);
			}
			i=j;

		}
		else 
		{
			i++;
			continue;
		}
	}
	free(temp);

	return strlen(outputstr);
}

int main()
{
	char inputstr[100];
	char* outputstr=NULL;
	int len;

	printf("please input a serial string whose length is less than 100:\n");
	scanf("%s",inputstr);
	outputstr=(char*)malloc(strlen(inputstr)+1);
	len=findnumstring(outputstr,inputstr);
	printf("the longest numstring is %s,whose length is %d\n",outputstr,len);
	free(outputstr);
	return 0;
}
