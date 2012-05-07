#include <shm.h>


int main()
{
	int num,in;
	int shmid_goods,shmid_index,semid;
	char* shmaddr=NULL;
	int *indexaddr=NULL;
	int is_noexist=0;
	struct shmid_ds buf;

	//create a shared memory as goods buffer
	if((shmid_goods=openshm(".",'s'))==-1)
	{
		if(shmctl(shmid_goods,IPC_RMID,&buf)==-1)
                {
			perror("remove share memory failed");
			exit(1);
                }
	}

	//create a shared memory as index
	if((shmid_index=openshm(".",'z'))==-1)
	{
		if(shmctl(shmid_index,IPC_RMID,&buf)==-1)
                {
			perror("remove share memory failed");
			exit(1);
                }
	}

	if((semid=opensem(".",'t'))==-1)
	{
		deletesem(semid);
	}
        return 0;
}
