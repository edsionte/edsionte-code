#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <time.h>


#define BUF_SIZE 256
#define PROJ_ID 32
#define PATH_NAME "."

void getmsgattr(int msgid,struct msqid_ds msq_info);

int main()
{
	struct mymsgbuf
	{
		long msgtype;
		char ctrlstring[BUF_SIZE];
	}msgbuffer;

	int qid;
	int msglen;
	key_t msgkey;
	struct msqid_ds msq_attr;

	if((msgkey=ftok(PATH_NAME,PROJ_ID))==-1)
	{
		perror("ftok error!\n");
		exit(1);
	}

	if((qid=msgget(msgkey,IPC_CREAT|0666))==-1)
	{
		perror("msgget error!\n");
		exit(1);
	}
	getmsgattr(qid,msq_attr);

	msgbuffer.msgtype=2;
	strcpy(msgbuffer.ctrlstring,"Another message");
	msglen=sizeof(struct mymsgbuf)-4;
	if(msgsnd(qid,&msgbuffer,msglen,0)==-1)
	{
		perror("msgget error\n");
		exit(1);
	}

	getmsgattr(qid,msq_attr);

	msq_attr.msg_perm.uid=3;
	msq_attr.msg_perm.gid=2;
	if(msgctl(qid,IPC_SET,&msq_attr)==-1)
	{
		perror("msg set error\n");
		exit(1);
	}

	getmsgattr(qid,msq_attr);
	if(msgctl(qid,IPC_RMID,NULL)==-1)
	{
		perror("delete msg error\n");
		exit(1);
	}

	getmsgattr(qid,msq_attr);
	exit(0);
}



void getmsgattr(int msgid,struct msqid_ds msg_info)
{
	if(msgctl(msgid,IPC_STAT,&msg_info)==-1)
	{
		perror("msgctl error\n");
		return ;
	}

	printf("************information of message queue %d****************\n",msgid);
	printf("last msgsnd to msq time is %s\n",ctime(&(msg_info.msg_stime)));
	printf("last msgrcv time from msg is %s\n",ctime(&(msg_info.msg_rtime)));
	printf("last change msq time is %s\n",ctime(&(msg_info.msg_ctime)));
	printf("current number of butes on queue is %d\n",msg_info.msg_cbytes);
	printf("number of messages in queue is %d\n",msg_info.msg_qnum);
	printf("max number of bytes on queue is %d\n",msg_info.msg_qbytes);
	printf("pid of last msgsnd is %d\n",msg_info.msg_lspid);
	printf("pid of last msgrcv is %d\n",msg_info.msg_lrpid);

	printf("msg uid is %d\n",msg_info.msg_perm.uid);
	printf("msg gid is %d\n",msg_info.msg_perm.gid);

	printf("************information end**************\n");

}
