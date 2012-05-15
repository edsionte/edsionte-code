#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>

#define MAX 5
#define UNUSED      0
#define USED        1

int GnChopstick[5] = {0} ;  // the number of the chopstick

pthread_mutex_t  mutex[5]; // five mutex
pthread_t        GpThread[5]; // five thread

void *Aristotle ();
void *Plato();
void *Marx();
void *Laozi();
void *Zhuangzi();
int thread_create();     // create the five threads
void thread_wait();     // waiting for the thread end

int main()
{
	
	   int i;
	   
	   for(i = 0;i < MAX;i++)
	   {
		  pthread_mutex_init(&mutex[i],NULL);   // initialize the five mutex
	     	
	}
	
	printf("Now the philosopher begin to dinner!\n" );
	
	for( i = 0; i< MAX ;i ++)
	{
	  thread_create();  // create the threads
	
	  thread_wait();    // waiting for the thread finish
     }  
    return 0;
}

void *Aristotle()
{
	if (GnChopstick[0] == UNUSED && GnChopstick[4] == UNUSED)  // this time Aristotle can eating
	{
	    pthread_mutex_lock(&mutex[0]);
	    pthread_mutex_lock(&mutex[4]);
	    printf("Aristotle is eating!\n");
	    sleep(2);
	    pthread_mutex_unlock(&mutex[0]);
	    pthread_mutex_unlock(&mutex[4]);
	}
	else 
	{
	     printf("Aristotle is thinking!\n");   	 
    }
    
    pthread_exit(NULL);
}

void *Plato()
{
	if (GnChopstick[0] == UNUSED && GnChopstick[1] == UNUSED)  // this time Plato can eating
	{
	    pthread_mutex_lock(&mutex[0]);
	    pthread_mutex_lock(&mutex[1]);
	    printf("Plato is eating!\n");
	    sleep(3);
	    pthread_mutex_unlock(&mutex[0]);
	    pthread_mutex_unlock(&mutex[1]);
	}
	else 
	{
	     printf("Plato is thinking!\n");   	 
    }
    pthread_exit(NULL);
}

void *Marx()
{
	if (GnChopstick[1] == UNUSED && GnChopstick[2] == UNUSED)  // this time Marx can eating
	{
	    pthread_mutex_lock(&mutex[1]);
	    pthread_mutex_lock(&mutex[2]);
	    printf("Marx is eating!\n");
	    sleep(4);
	    pthread_mutex_unlock(&mutex[1]);
	    pthread_mutex_unlock(&mutex[2]);
	}
	else 
	{
	     printf("Marx is thinking!\n");   	 
    }
    pthread_exit(NULL);
}

void *Laozi()
{
	if (GnChopstick[2] == UNUSED && GnChopstick[3] == UNUSED)  // this time Laozi can eating
	{
	    pthread_mutex_lock(&mutex[2]);
	    pthread_mutex_lock(&mutex[3]);
	    printf("Laozi is eating!\n");
	    sleep(5);
	    pthread_mutex_unlock(&mutex[2]);
	    pthread_mutex_unlock(&mutex[3]);
	}
	else 
	{
	     printf("Laozi is thinking!\n");   	 
    }
    pthread_exit(NULL);
}

void *Zhuangzi()
{
	if (GnChopstick[3] == UNUSED && GnChopstick[4] == UNUSED)  // this time Zhuangzi can eating
	{
	    pthread_mutex_lock(&mutex[3]);
	    pthread_mutex_lock(&mutex[4]);
	    printf("Zhuangzi is eating!\n");
	    sleep(6);
	    pthread_mutex_unlock(&mutex[3]);
	    pthread_mutex_unlock(&mutex[4]);
	}
	else 
	{
	     printf("Zhuangzi is thinking!\n");   	 
    }
    pthread_exit(NULL);
}

int thread_create()
{
   	int nNum;
   	
   	if((nNum =pthread_create(&GpThread[0],NULL,Aristotle,NULL)) != 0)
   	{
	     fprintf(stderr,"fail to create the Aristotle thread");	
	     return -1;
    }
    
    if((nNum =pthread_create(&GpThread[1],NULL,Plato,NULL)) != 0)
   	{
	     fprintf(stderr,"fail to create the Plato thread");	
	     return -1;
    }
    
    if((nNum =pthread_create(&GpThread[2],NULL,Marx,NULL)) != 0)
   	{
	     fprintf(stderr,"fail to create the Marx thread");	
	     return -1;
    }
    
    if((nNum =pthread_create(&GpThread[3],NULL,Laozi,NULL)) != 0)
   	{
	     fprintf(stderr,"fail to create the Laozi thread");	
	     return -1;
    }
    
    if((nNum =pthread_create(&GpThread[4],NULL,Zhuangzi,NULL)) != 0)
   	{
	     fprintf(stderr,"fail to create the Aristotle thread");	
	     return -1;
    }
}

void thread_wait()
{
   int i;
   
   for(i =0 ;i < MAX; i ++)	 
   {
	   if(GpThread[i] != 0)
	      pthread_join(GpThread[i],NULL);
   }
}
