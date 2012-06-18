#include <stdio.h>
#include <string.h>

#define N 3

struct student
{
	int number;
	char name[20];
	char sex;
	int age;
	char addr[30];
};

void output_info(struct student *p,int n)
{
	int i;
	for(i=0;i<n;i++)
	{
		printf("%d. %d %s %c %d %s\n",i+1,p[i].number,p[i].name,p[i].sex,p[i].age,p[i].addr);
	}
}

void sort_byname(struct student *p,int n)
{
	int i,j;
	char temp[20];

	for(i=0;i<n;i++)
	{
		for(j=0;j<n-1-i;j++)
		{
			if(strcmp(p[j].name,p[j+1].name)>0)
			{
				//exchange all information
				strcpy(temp,p[j].name);
				strcpy(p[j].name,p[j+1].name);
				strcpy(p[j+1].name,temp);


			}
		}
	}

	printf("output in sort_byname function..\n");
	output_info(p,n);
}

int main()
{
	struct student s[N];
	struct student *p=NULL;
	int i;

	for(i=0;i<N;i++)
	{
		printf("student %d..\n",i+1);
		printf("number:");
		scanf("%d",&s[i].number);
		getchar();
		printf("name:");
		scanf("%s",s[i].name);
		getchar();
		printf("sex(m or f):");
		scanf("%c",&s[i].sex);
		getchar();
		printf("age:");
		scanf("%d",&s[i].age);
		getchar();
		printf("address:");
		scanf("%s",s[i].addr);
		getchar();
	}

	p=s;

	printf("the information which your inputed..\n");
	output_info(p,sizeof(s)/sizeof(s[0]));
	printf("sorting the student information by name..\n");
	sort_byname(s,sizeof(s)/sizeof(s[0]));
	printf("output in main funtion..\n");
	output_info(p,sizeof(s)/sizeof(s[0]));
	return 0;
}
