DIR *dir;
struct dirent *ptr;

if((dir=opendir(path))==NULL)
{
	perror("opendir");
	return -1;
}
while((ptr=readdir(dir))!=NULL)
{
	printf("file name:%s\n",ptr->d_name);
}
/*do something*/
closedir(dir);
