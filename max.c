/**
 * To compile
 * gcc max.c -lpthread
 */

#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>     
#include <errno.h>       


int max; /* this data is shared by the thread(s) */
int arr [4096];
int arrcount;
sem_t mutex;
sem_t barrier;

int numThreads = 8;
int count = 0;

void *runner(void *param); /* the thread */

int main(int argc, char *argv[])
{
pthread_t tid; /* the thread identifier */
pthread_attr_t attr; /* set of attributes for the thread */
int num;


/********* READ INPUT FILE ***********/
FILE * fileptr;
int i;

if (argc != 2) {
	fprintf(stderr,"Please enter a file name as well \n");
	/*exit(1);*/
	return -1;
}

fileptr = fopen(argv[1], "r");
if(fileptr == NULL){
	fprintf(stderr, "Error opening file\n");
	exit(1);
}
i=0;
while (fscanf(fileptr, "%d", &num) != EOF){
	arr[i] = num;
	i++;
	arrcount = i;
}
   fclose(fileptr);

/*********** THREADS ****************/

/**** initialize semaphores ***/
if (sem_init(&mutex,0,1) == -1) {
	printf("%s\n",strerror(errno));
} 
if (sem_init(&barrier,0,0) == -1) {
	printf("%s\n",strerror(errno));
}

/* get the default attributes */
pthread_attr_init(&attr);
 /*create the thread */
printf("%s\n", "Starting thread. Semaphore is unlocked");
pthread_create(&tid,&attr,runner,&arr);

/* now wait for the thread to exit */
//pthread_join(tid,NULL);


printf("%d\n",max);
}


/**
 * The thread will begin control in this function
 */
void *runner(void *param) {
	printf("%s", "In runner");

	sem_wait(&mutex);
	count = count + 1;
	if (count == numThreads){
		sem_post(&barrier);
	}
	sem_post(&mutex);

	sem_wait(&barrier);

	/******* CRITICAL SECTION *********/

		int i;
		max = 0;
		for (i =0; i<arrcount ; i++){
			if (arr[i] > max){
				max = arr [i];
			}
		}

	/******* reset barrier *********/

	sem_wait(&mutex);
	count = count -1;
	if (count!= 0){
		sem_post(&barrier);
	}
	sem_post(&mutex);
	printf("%s\n", "Semaphore is unlocked");


	pthread_exit(0);
}
