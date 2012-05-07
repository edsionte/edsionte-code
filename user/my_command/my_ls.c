#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <linux/limits.h>

#define PARAM_NONE 0//no argument
#define PARAM_A    1//-a:print all files
#define PARAM_L    2//-l:The detail information of a file be printed in a line
#define MAXROWLEN  80//the biggest number of character in a single line

int g_leave_len=MAXROWLEN;
int g_maxlen;

//function declaration
void my_err(const char*,int,int);
void my_err2(const char*,int);
void display(int,char*);
void display_single(char*);
void display_attribute(struct stat,char*);
void display_dir(int,char*);



//the function of catching error
void my_err(const char* err_string,int error_num,int line)
{
	printf("error:  line%d:%s() %s with errno %d\n",line,err_string,strerror(error_num),error_num);
	exit(1);
}

//another function of processing error
void my_err2(const char* err_string,int line)
{
	printf("error:  line%d:%s\n",line,err_string);
	exit(1);
}

void display(int flag,char* pathname)
{
	int i,j;
	struct stat buf;
	char name[NAME_MAX+1];

	//extract the file name from pathname;
	for(i=0,j=0;i<strlen(pathname);i++)
	{
		if(pathname[i]=='/')
		{
			j=0;
			continue;
		}
		name[j++]=pathname[i];
	}
	name[j]='\0';

	if(lstat(pathname,&buf)==-1)
	{
		my_err("lstat",errno,__LINE__);
	}
	switch(flag)
	{
		case PARAM_NONE://no any argument
			if(name[0]!='.')
			{
				display_single(name);
			}
			break;
	        case PARAM_L://-l
			if(name[0]!='.')
			{
				display_attribute(buf,name);
				printf(" %-s\n",name);
			}
			break;
		case PARAM_A://-a
			//the first character of hidden files is '.'(dot)
			display_single(name);
			break;
		case PARAM_A+PARAM_L://-a -l
			display_attribute(buf,name);
			printf(" %-s\n",name);
			break;
		default:
			break;
	}
}

//if the command does not include the argument '-l',the file names is only  printed
void display_single(char *name)
{
	int i,len;

	if(g_leave_len<g_maxlen)
	{
		printf("\n");
		g_leave_len=MAXROWLEN;
	}
	len=strlen(name);
	len=g_maxlen-len;
	printf("%-s",name);

	for(i=0;i<len;i++)
		printf(" ");
	printf("  ");//two blank
	g_leave_len-=(g_maxlen+2);

}

//get attributes of a file and print them
void display_attribute(struct stat buf,char *name)
{
	char buf_time[32];
	struct passwd *psd;//get the name of owner who own this file
	struct group *grp;//get the name of a group where owner is

	//get the type of this file and print it
	if(S_ISLNK(buf.st_mode))
	{
		printf("l");
	}
	else if(S_ISREG(buf.st_mode))
	{
		printf("-");
	}
	else if(S_ISDIR(buf.st_mode))
	{
		printf("d");
	}
	else if(S_ISCHR(buf.st_mode))
	{
		printf("c");
	}
	else if(S_ISBLK(buf.st_mode))
	{
		printf("b");
	}
	else if(S_ISFIFO(buf.st_mode))
	{
		printf("f");
	}
	else if(S_ISSOCK(buf.st_mode))
	{
		printf("s");
	}

	//get and print the right of file owner
	if(buf.st_mode&S_IRUSR)
	{
		printf("r");
	}
	else 
	{
		printf("-");
	}
	if(buf.st_mode&S_IWUSR)
	{
		printf("w");
	}
	else
	{
		printf("-");
	}
	if(buf.st_mode&S_IXUSR)
	{
		printf("x");
	}
	else
	{
		printf("-");
	}

	//get and print the right of the group where the file owner is

	if(buf.st_mode&S_IRGRP)
	{
		printf("r");
	}
	else 
	{
		printf("-");
	}
	if(buf.st_mode&S_IWGRP)
	{
		printf("w");
	}
	else
	{
		printf("-");
	}
	if(buf.st_mode&S_IXGRP)
	{
		printf("x");
	}
	else
	{
		printf("-");
	}
        //get and print the right of the others
	if(buf.st_mode&S_IROTH)
	{
		printf("r");
	}
	else 
	{
		printf("-");
	}
	if(buf.st_mode&S_IWOTH)
	{
		printf("w");
	}
	else
	{
		printf("-");
	}
	if(buf.st_mode&S_IXOTH)
	{
		printf("x");
	}
	else
	{
		printf("-");
	}
    
	printf("  ");//two blank
	//get the username and group name by uid and pid
	psd=getpwuid(buf.st_uid);
	grp=getgrgid(buf.st_gid);
	printf("%4d  ",buf.st_nlink);
	printf("%-10s",psd->pw_name);
	printf("%-10s",grp->gr_name);

	printf("%6d",(int)buf.st_size);
	strcpy(buf_time,ctime(&buf.st_mtime));
	buf_time[strlen(buf_time)-1]='\0';
	printf(" %s",buf_time);
		

}

void display_dir(int flag_param,char *path)
{
	
	DIR *dir;
	struct dirent *ptr;
	int count=0;
	char filename[256][PATH_MAX+1],temp[PATH_MAX+1];

	//get the sum of files and the longest file name in this directory
	if((dir=opendir(path))==NULL)
	{
		my_err("opendir",errno,__LINE__);
	}
	while((ptr=readdir(dir))!=NULL)
	{
		if(g_maxlen<strlen(ptr->d_name))
		{
			g_maxlen=strlen(ptr->d_name);
		}
		count++;
	}
	closedir(dir);

	if(count>256)
	{
		my_err2("to many files under this directory",__LINE__);
	}
	//get all file names under this dierctory
	int i,j,len=strlen(path);
	dir=opendir(path);
	for(i=0;i<count;i++)
	{
		if((ptr=readdir(dir))==NULL)
		{
			my_err("readdir",errno,__LINE__);
		}
		strncpy(filename[i],path,len);
	        filename[i][len]='\0';
		strcat(filename[i],ptr->d_name);
		filename[i][len+strlen(ptr->d_name)]='\0';
	}

	//sort the file using bubble sort
	for(i=0;i<count-1;i++)
		for(j=0;j<count-i-1;j++)
		{
			if(strcmp(filename[j],filename[j+1])>0)
			{
				strcpy(temp,filename[j]);
				temp[strlen(filename[j])]='\0';
				strcpy(filename[j],filename[j+1]);
				filename[j][strlen(filename[j+1])]=='\0';
				strcpy(filename[j+1],temp);
				filename[j+1][strlen(temp)]='\0';
			}
		}
	for(i=0;i<count;i++)
	{
		display(flag_param,filename[i]);
	}

	closedir(dir);
	//if the command has not the argument '-l',print a '\n'
//	printf("\n");
	
	if(flag_param==PARAM_A||flag_param==PARAM_NONE)
	{
		printf("\n");
	}
	
}

int main(int argc,char** argv)
{
	int i,j,k,num;
	char path[PATH_MAX+1];
	char param[32];//save the arguments of command and it doesn't include the target file and directory name
	int flag_param=PARAM_NONE;//the sort of argument
	struct stat buf;//the struct receive all information of a file
	
	j=0;
	num=0;
	for(i=1;i<argc;i++)
	{
		if(argv[i][0]=='-')
		{
			for(k=1;k<strlen(argv[i]);k++)
			{
				param[j++]=argv[i][k];
			}
			num++;//count the number of '-'
		}
	}
        //check the argument because of only supporting -a and -l
	for(i=0;i<j;i++)
	{
		if(param[i]=='a')
		{
			flag_param|=PARAM_A;
			continue;
		}
		else if(param[i]=='l')
		{
			flag_param|=PARAM_L;
			continue;
		}
		else
		{
			printf("error:my_ls invalid option -%c\n",param[i]);
			exit(1);
		}
	}
	param[j]='\0';

	//print the information of  current directory if the command without the name of target file and current directory
        if((num+1)==argc)
	{
		strcpy(path,"./");
		path[2]='\0';
		display_dir(flag_param,path);
		return 0;
	}
        
	//extract the target file name and dirctory name form command
	i=1;
	do
	{
		//the current argument doesn't comprise the target file name and dirctory name
		if(argv[i][0]=='-')
		{
			i++;
			continue;
		}
		else
		{
			strcpy(path,argv[i]);
			//detect if the "path" exsit 
			if(stat(path,&buf)==-1)
			{
				my_err("stat",errno,__LINE__);
			}
			//detect the "path" is a file or a directory
			if(S_ISDIR(buf.st_mode))
			{
				//directory
				//detect if the last character of the "path" is '/' 
				if(path[strlen(path)-1]=='/')
				{
					path[strlen(path)]=='\0';
				}
				else
				{
					path[strlen(path)]='/';
					path[strlen(path)]='\0';
				}
				display_dir(flag_param,path);
				i++;
			}
			else
			{
				//file
				display(flag_param,path);
				i++;

			}
		}

	}while(i<argc);

	return 0;
}
