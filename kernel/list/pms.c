#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <list.h>

struct postinfo
{
	char id[20];
	char name[20];
	char sex[10];
	char addr[50];
	char email[20];
	struct list_head list;
};

void inputInfo(struct list_head *head)
{
	int i,num,tmp_sex;
	struct postinfo *tmp;
	char tmp_id[20];
	int fd;

	printf("studet ID:\n");
	scanf("%s",tmp_id);
	for(i=0;tmp_id[0]!='@';i++)
	{
		tmp=(struct postinfo*)malloc(sizeof(struct postinfo));
		strcpy(tmp->id,tmp_id);
		printf("name:\n");
		scanf("%s",tmp->name);
		printf("sex: 1.male 2.female\n");
		scanf("%d",&tmp_sex);
		num=3;
		while(num)
		{
			switch(tmp_sex)
			{
				case 1:
					strcpy(tmp->sex,"male");
					break;
				case 2:
					strcpy(tmp->sex,"female");
					break;
				default:
					printf("sorry,try again:");
					scanf("%d",&tmp_sex);
			}
			if(tmp_sex==1||tmp_sex==2)
				break;
			else
				--num;
			
		}
		if(num==0)
		{
			printf("sorry,the system end\n");
			exit(1);
		}
		printf("address:\n");
		scanf("%s",tmp->addr);
		printf("Email:\n");
		scanf("%s",tmp->email);

		list_add_tail(&tmp->list,head);
		printf("student ID:\n");
		scanf("%s",tmp_id);
	}

}

void print_list(struct list_head *head)
{
	struct list_head *pos;
	struct postinfo *pinfo;
	int fd;

	if((fd=open("stuinfo.txt",O_WRONLY|O_CREAT|O_TRUNC,0644))==-1)
	{
		printf("open file error!\n");
		exit(1);
	}
	dup2(fd,1);
	list_for_each(pos,head)
	{
		pinfo=list_entry(pos,struct postinfo,list);
		printf("%s %s %s %s %s\n",pinfo->id,pinfo->name,pinfo->sex,pinfo->addr,pinfo->email);
        }
	close(fd);
}

int main()
{
	struct list_head head;

	INIT_LIST_HEAD(&head);


	inputInfo(&head);
	print_list(&head);
	
	return 0;
}
