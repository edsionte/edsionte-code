
/*



code by edsionte
www.edsionte.com






*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/limits.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <fcntl.h>
#include <errno.h>

int param_r;

void cp_single(char *original_src_path,char* original_dest_path)
{
	struct stat buf;
	int len;
	char ch[10],filename[PATH_MAX+1],dest_dir[PATH_MAX+1];
	int fdrd,fdwt,i,j,k;
	char src_path[PATH_MAX+1],dest_path[PATH_MAX+1];

	strcpy(src_path,original_src_path);
	strcpy(dest_path,original_dest_path);
	for(k=0;k<strlen(src_path);k++)
	{
		if(src_path[k]=='/')
		break;
	}
	char temp_path[PATH_MAX+1]="./";
	if(k==strlen(src_path))
	{
		strcat(temp_path,src_path);
	        strcpy(src_path,temp_path);
	}

	//extract the file name from src_path
	for(i=strlen(src_path)-1;i>0;i--)
	{
		if(src_path[i]=='/')
			break;
	}
	j=k=0;
	for(j=i;j<strlen(src_path);j++)
	{
		filename[k++]=src_path[j];
	}
	filename[k]='\0';

	//check the if dest path has exsited
	if(stat(dest_path,&buf)==0)
	{
		//the dest_path exsited
		if(S_ISDIR(buf.st_mode))
		{
			if(dest_path[strlen(dest_path)-1]=='/')
				dest_path[strlen(dest_path)-1]='\0';
			strcat(dest_path,filename);
		}
		/*
		   if the path is not a directory and it means that the file has existed.
		   copying the src file to this exist file will cover the current content.
		 */
	}
	else
	{
		/*
		   this code is not exact,
		   it can not check the legality of the directory 
		   such as "~/existdirectory/nothisdirectory/nothisfile"
		 */

	        /*
		if(dest_path[strlen(dest_path)-1]=='/')
		{
			printf("my_cp:can't create the file:\"%s\":it is a directory.\n",dest_path);
			exit(1);
		}
		*/


		//the dest_path didn't exsit
		for(i=strlen(dest_path)-1;i>=0;i--)
		{
			if(dest_path[i]=='/')
				break;
		}
		if(i>=0)
		{
			strncpy(dest_dir,dest_path,i+1);
		        if(stat(dest_dir,&buf)==-1)
	            	 {
		         	printf("my_cp:accessing:\"%s\" :it is't a directory.\n",dest_path);
			        exit(1);
               		}
		}

	}
	//fistly the content which was read from srouce file will be write to dest file
	if((fdrd=open(src_path,O_RDONLY))==-1)
	{
		perror("open");
		exit(1);
	}
	if(lseek(fdrd,0,SEEK_END)==-1)
	{
		perror("lseek");
		exit(1);
	}
	if((len=lseek(fdrd,0,SEEK_CUR))==-1)
	{
		perror("lseek");
		exit(1);
	}
	if(lseek(fdrd,0,SEEK_SET)==-1)
	{
		perror("lseek");
		exit(1);
	}
	//open the dest file
	if((fdwt=open(dest_path,O_CREAT|O_TRUNC|O_RDWR,S_IRWXU))==-1)
	{
		perror("open");
		exit(1);
	}
	close(fdwt);
	if((fdwt=open(dest_path,O_WRONLY|O_APPEND))==-1)
	{
		perror("open");
		exit(1);
	}

	while(len-->0)
	{
		//write all characters to dest file
		if(read(fdrd,ch,1)!=1)
		{
			perror("read");
			exit(1);
		}
		if(write(fdwt,ch,1)!=1)
		{
			perror("write");
			exit(1);
		}

	}

	//get src file's attributes
	if(fstat(fdrd,&buf)==-1)
	{
		perror("fstat");
		exit(1);
	}
	//set the dset file's access right
	if(fchmod(fdwt,buf.st_mode)==-1)
	{
		perror("fchmod");
		exit(1);
	}
	//set file's user id and group id
	if(fchown(fdwt,buf.st_uid,buf.st_gid)==-1)
	{
		perror("fchown");
		exit(1);
	}
	close(fdwt);
	close(fdrd);
}


void cp_directory(char* src_path,char* dest_path)
{
	struct stat buf;
	DIR *dir;
	struct dirent *ptr;
	char path[PATH_MAX+1];
//	char src_path[PATH_MAX+1],dest_path[PATH_MAX+1];

//	strcpy(src_path,original_src_path);
//	strcpy(dest_path,original_dest_path);

	if(src_path[strlen(src_path)-1]!='/')
	{
		strncat(src_path,"/",1);
	}

	if(stat(dest_path,&buf)==-1)
	{
		//create a directory which name is dest_path
		stat(src_path,&buf);
		if(mkdir(dest_path,buf.st_mode)==-1)
		{
			printf("my_cp:create the directory \"%s\" error.\n",dest_path);
			return ;
		}
 	}
	else
	{
		//exist
		if(!S_ISDIR(buf.st_mode))
		{
			printf("my_cp:the directory \"%s\" can't cover the no-directory \"%s\".\n",src_path,dest_path);
			return ;
		}
		else
		{
			if(dest_path[strlen(dest_path)-1]!='/')
			{
				strncat(dest_path,"/",1);
			}
			//extract the lowest directory
			int i,k=0;
			char lowestdir[PATH_MAX+1];
			for(i=strlen(src_path)-1-1;i>0;i--)
			{
				if(src_path[i]=='/')
				{
					i=i+1;
					break;
				}
			}

			for(;i<strlen(src_path);i++)
			{
				lowestdir[k++]=src_path[i];
			}
			strncat(dest_path,lowestdir,strlen(lowestdir));
			struct stat temp_buf;
			char temp_path[PATH_MAX+1]="rm -rf ";
			if(stat(dest_path,&temp_buf)==0)
			{
				strcat(temp_path,dest_path);
				system(temp_path);
			}
              		if(mkdir(dest_path,buf.st_mode)==-1)
	        	{
				printf("my_cp:create the directory \"%s\" error.\n",dest_path);
	 		        return ;
	          	}
		}
	}

	if((dir=opendir(src_path))==NULL)
	{
		printf("my_cp:open the srouce path \"%s\" error.\n",src_path);
		return ;
	}

//	printf("src_path=%s\n",src_path);
//	printf("dest_path=%s\n",dest_path);
	char temp_dest_path[PATH_MAX+1];
	strcpy(temp_dest_path,dest_path);
	while((ptr=readdir(dir))!=NULL)
	{
		if(!strcmp(ptr->d_name,"."))
			continue;
		if(!strcmp(ptr->d_name,".."))
			continue;
		strcpy(path,src_path);
		strcat(path,ptr->d_name);
		if(stat(path,&buf)==-1)
		{
			printf("my_cp:open the file \"%s\" error.\n",path);
			return ;
		}
		strcpy(dest_path,temp_dest_path);
		//get the right dest_path
		if(S_ISDIR(buf.st_mode))
		{
			cp_directory(path,dest_path);
		}
		else
		{
			cp_single(path,dest_path);
		}
	}
}



int main(int argc,char** argv)
{
	char dest_path[PATH_MAX+1];
	char temp_dest_path[PATH_MAX+1],temp_src_path[PATH_MAX+1];
	char src_path[256][PATH_MAX+1];
	int i,k,num,src_num,index_r;
	struct stat buf;

	//check the legality of the options,only -r or -R
	for(i=1;i<argc;i++)
	{
		if(argv[i][0]=='-')
		{
			if((!strcmp(argv[i],"-r")||!strcmp(argv[i],"-R")))
			{
				param_r+=1;
				index_r=i;
			}
			else
			{
				printf("my_cp:invalid options: %s\n",argv[i]);
				exit(1);
			}
		}
	}
	if(param_r>1)
	{
		printf("my_cp:invalid options.\n");
		exit(1);
	}
	if(param_r)
	{
		num=argc-1-1;
		src_num=num-1;
	}
	else
	{
		num=argc-1;
		src_num=num-1;
	}
	if(num<2)
	{
		printf("my_cp: [option] <source> <dest>\n");
		exit(1);
	}
	//extract the dest path
	if(index_r!=argc-1)
	{
		strcpy(dest_path,argv[argc-1]);
	}
	else
	{
		strcpy(dest_path,argv[argc-2]);
	}
	//extract the src path
	k=0;
	for(i=1;i<argc-1;i++)
	{
		if(i==index_r&&param_r)
			continue;
		else 
		{
			strcpy(src_path[k++],argv[i]);
		}
	}

	if(src_num>1)
	{
		if(stat(dest_path,&buf)==-1)
		{
			printf("my_cp: \"%s\" is not a directory.\n",dest_path);
			exit(1);
		}
		//the dest path is valid
		if(S_ISDIR(buf.st_mode))
		{
			strcpy(temp_dest_path,dest_path);
			//the dest path is directory
			for(i=0;i<src_num;i++)
			{
				if(stat(src_path[i],&buf)==-1)
				{
					printf("my_cp: can't get file status of \"%s\" : no this file or directory.\n",src_path[i]);
					continue;
				}
				//the src_path exist
				if(!S_ISDIR(buf.st_mode))
				{
					cp_single(src_path[i],dest_path);
				}
				else if(param_r)
				{
					cp_directory(src_path[i],dest_path);

				}
				else
				{
					printf("my_cp: skip the directory: \"%s\".\n",src_path[i]);
				}
				strcpy(dest_path,temp_dest_path);

			}

		}
		else
		{
			printf("my_cp: \"%s\" is not a directory.\n",dest_path);
			exit(1);
		}
	}
	else
	{
		//only one src path
		if(stat(src_path[0],&buf)==-1)
		{
			printf("my_cp: can't get file status of \"%s\" : no this file or directory.\n",src_path[0]);
			exit(1);
		}
		if(S_ISDIR(buf.st_mode))
		{
			if(param_r)
			{
				cp_directory(src_path[0],dest_path);
				exit(0);
			}
			else
			{
				printf("my_cp: skip the directory: \"%s\".\n",src_path[0]);
				exit(1);
			}
		}
		else
                {
			cp_single(src_path[0],dest_path);
		}
	}





	
	return 0;
}
