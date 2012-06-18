#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#define opt_num 20
#define opt_len 20

void chmod_single(char* path,int mode)
{
	if(chmod(path,mode)==-1)
	{
		perror("my_cp:chmod error.\n");
		exit(1);
	}
}

void get_mode(int u,int g,int o,int r,int w,int x,int sign,mode_t* mode)
{
//	int mode=00;

	switch(sign)
	{
		case 1:
			{
				if(u)
				{
					if(r)
						*mode|=S_IRUSR;
					if(w)
						*mode|=S_IWUSR;
					if(x)
						*mode|=S_IXUSR;
				}
				if(g)
				{
					if(r)
						*mode|=S_IRGRP;
					if(w)
						*mode|=S_IWGRP;
					if(x)
						*mode|=S_IXGRP;
				}
				if(o)
				{
					if(r)
						*mode|=S_IROTH;
					if(w)
						*mode|=S_IWOTH;
					if(x)
						*mode|=S_IXOTH;
				}
			}
			break;
		case 2:
			{
				if(u)
				{
					if(r&&(*mode&S_IRUSR))
						*mode^=S_IRUSR;
					if(w&&(*mode&S_IWUSR))
						*mode^=S_IWUSR;
					if(x&&(*mode&S_IXUSR))
						*mode^=S_IXUSR;
				}
				if(g)
				{
					if(r&&(*mode&S_IRGRP))
						*mode^=S_IRGRP;
					if(w&&(*mode&S_IWGRP))
						*mode^=S_IWGRP;
					if(x&&(*mode&S_IXGRP))	
						*mode^=S_IXGRP;
				}
				if(x)
				{
					if(r&&(*mode&S_IROTH))
						*mode^=S_IROTH;
					if(w&&(*mode&S_IWOTH))
						*mode^=S_IWOTH;
					if(x&&(*mode&S_IXOTH))
						*mode^=S_IXOTH;
				}

			}
			break;
		case 3:
			{
				if(u)
				{
					*mode&=(S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH);
					if(r)
						*mode|=S_IRUSR;
					if(w)
						*mode|=S_IWUSR;
					if(x)
						*mode|=S_IXUSR;
				}
				if(g)
				{
					*mode&=(S_IRUSR|S_IWUSR|S_IXUSR|S_IROTH|S_IWOTH|S_IXOTH);
					if(r)
						*mode|=S_IRGRP;
					if(w)
						*mode|=S_IWGRP;
					if(x)
						*mode|=S_IXGRP;
				}
				if(o)
				{
					*mode&=(S_IRGRP|S_IWGRP|S_IXGRP|S_IRUSR|S_IWUSR|S_IXUSR);
					if(r)
						*mode|=S_IROTH;
					if(w)
						*mode|=S_IWOTH;
					if(x)
						*mode|=S_IXOTH;
				}
			}
			break;
		default:
			printf("sign error\n");
			exit(1);
	}
}

int check_option(char* original_args,mode_t original_mode)
{
	char args[PATH_MAX+1];
	char option[opt_num][opt_len];
	int i,j,k,num;
	int flag=0;
	int u,g,o,r,w,x,sign;
	int mode,t_mode;

	strncpy(args,original_args,strlen(original_args));

	//extract options
	k=0;
	j=0;
	for(i=0;i<strlen(args);i++)
	{
		if(args[i]==',')
		{
			option[k][j]='\0';
			k++;
			j=0;
			i++;
		}
		option[k][j++]=args[i];
	}

	//get the number of ','
	num=++k;

	flag=0;
	//check the legality of the option
	for(i=0;i<num;i++)
	{
		flag=0;
		for(j=0;j<strlen(option[i]);j++)
		{
			if(option[i][j]=='+'||option[i][j]=='-'||option[i][j]=='=')
			{
				flag=1;
				continue;
			}
			if(flag==0)
			{
				if(option[i][j]=='u'||option[i][j]=='g'||option[i][j]=='o'||option[i][j]=='a')
				{
					continue;
				}
				else
				{
					printf("my_chmod:无效模式:\"%s\"\n",args);
					exit(1);
				}
			}
			else
			{
				if(option[i][j]=='r'||option[i][j]=='w'||option[i][j]=='x')
				{
					continue;
				}
				else
				{
					printf("my_chmod:无效模式：\"%s\"\n",args);
					exit(1);
				}
			}

		}//inner for
	}
        
	//get the mode
	u=g=o=sign=r=w=x=0;
	t_mode=mode=00;
	for(i=0;i<num;i++)
	{
		u=g=o=r=w=x=sign=0;
		for(j=0;j<strlen(option[i]);j++)
		{
			switch(option[i][j])
			{
				case 'u':
					u=1;
					break;
				case 'g':
					g=1;
					break;
				case 'o':
					o=1;
					break;
				case 'a':
					u=g=o=1;
					break;
				case 'r':
					r=1;
					break;
				case 'w':
					w=1;
					break;
				case 'x':
					x=1;
					break;
				default:
					if(option[i][j]=='+')
						sign=1;
					else if(option[i][j]=='-')
						sign=2;
					else
						sign=3;
			}


		}
		get_mode(u,g,o,r,w,x,sign,&original_mode);
	}
	
	mode=original_mode;
	

	return mode;
}


int main(int argc,char** argv)
{

	int temp_mode,mode;
	int mode_u,mode_g,mode_o;
	int i,k,j,num_mode=0;
	struct stat buf;

	//check the legality of the command arguments
	if(argc<3)
	{
		if(argc<2)
		{
			if(!strcmp(argv[0],"./my_chmod"))
			{
				printf("my_chmod: 缺少操作数\n",argv[0]);
				exit(1);
			}
		}
		else
		{
			printf("my_chmod: \"%s\" 后缺少操作数\n",argv[1]);
			exit(1);
		}
	}


        //get the mode 
	temp_mode=atoi(argv[1]);
	if((temp_mode>0&&temp_mode<=777)||!strcmp(argv[1],"000"))
	{
		mode_u=temp_mode/100;
		mode_g=(temp_mode-(mode_u*100))/10;
		mode_o=temp_mode-(mode_u*100)-(mode_g*10);
		mode=mode_u*8*8+mode_g*8+mode_o;
		num_mode=1;
	}
	else if(temp_mode<0||temp_mode>777)
	{
		printf("my_cp:数字权限有误！应该在0～777之间\n");
		exit(1);
	}
	else
	{
		//the right is unnumber
		
		for(j=0;j<strlen(argv[1]);j++)
		{
			if(!(argv[1][j]==','||argv[1][j]=='+'||argv[1][j]=='-'||argv[1][j]=='='||argv[1][j]=='u'||argv[1][j]=='g'
					||argv[1][j]=='o'||argv[1][j]=='r'||argv[1][j]=='w'||argv[1][j]=='x'||argv[1][j]=='a'))
			{
				printf("my_cp:无效模式:\"%s\"\n",argv[1]);
				exit(1);
			}
		}
	}

	//if a file doesn't exist,skip this file and continue changing the rest
	for(i=2;i<argc;i++)
	{
		if(stat(argv[i],&buf)==-1)
		{
			printf("my_cp:无法访问\"%s\":没有那个文件或者目录\n",argv[i]);
			continue;
		}
		else
		{
			if(!num_mode)
			{
				mode=check_option(argv[1],buf.st_mode);
			}
			if(S_ISDIR(buf.st_mode))
			{

			}
			else
			{
				chmod_single(argv[i],mode);
			}	
		}
	}
	return 0;
}
