#include "part1.h"
#include <pthread.h>
#include "main.h"
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
int Threads;
int MaxFloors;
int i;
int l = 20;
int current = 0;
int goingfrom, goingto, user_id;
pthread_mutex_t mutex_dest = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_source = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex4 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
//sem_t semaf[20];
//sem_t semat[20];
sem_t *semat;
sem_t *semaf;
int *semaf_no;
int *semat_no;


/**
 * Do any initial setup work in this function.
 * numFloors: Total number of floors elevator can go to
 * maxNumPeople: The maximum capacity of the elevator
 */
void *elevator()
{
	//int current = 0;
	//printf("LIFT IS AT LEVEL %d\n", current );
	//sleep(1);
				
	while(l)
	{
		pthread_mutex_lock(& mutex_dest);
		pthread_mutex_lock(& mutex_source);
		//pthread_mutex_lock(& mutex4);
		// printf("The lift is at floor \t %d\n", current);
		while(semaf_no[current] >= 1){
			//if(semaf_no[current] > 0){
		//printf("Picking user at floor \t %d\n", current);
				//pthread_mutex_lock(& mutex2);
				semaf_no[current]--;
				//pthread_mutex_unlock(& mutex2);

				sem_post(&semaf[current]);
			}
		//}
		while(semat_no[current] >=1){
			//if(semat_no[current] > 0){
		//printf("Dropping user at floor \t %d\n", current);
				//pthread_mutex_lock(& mutex2);
				semat_no[current]--;
				//pthread_mutex_unlock(& mutex2);
				sem_post(&semat[current]);
			}
		//}
		pthread_mutex_unlock(& mutex_source);
		pthread_mutex_unlock(& mutex_dest);

		//break and check on the floor for the passengers
		//pthread_cond_wait(&cond,&mutex);
		if(current < MaxFloors)
		{
			//pthread_mutex_lock(&mutex);
			// printf("Diretion ^\n");
   //  		printf("Moving UP \n");
			current++;
			//pthread_mutex_unlock(&mutex);
		}

		// else if (current == MaxFloors)
		// {
		// 	//pthread_mutex_lock(&mutex);
		// 	printf("Diretion |\n");
  //   		printf("Moving DOWN \n");
		// 	d = 0;
		// 	//pthread_mutex_unlock(&mutex);
		// }

		else if (current == MaxFloors)
		{
			// printf("Floor Exceeded \n");
			current--;
		}
		else 
		{
			current = 1;
		}
		sleep(1);
		 l--;
		//pthread_mutex_unlock(& mutex4);
	}
		

}
void initializeP1(int numFloors, int maxNumPeople) 
{

	Threads = maxNumPeople;
	MaxFloors = numFloors;
	{
	semaf = malloc(sizeof(sem_t)* numFloors);
	semat = malloc(sizeof(sem_t)* numFloors);

	semaf_no = malloc(sizeof(int) * numFloors);
	semat_no = malloc(sizeof(int) * numFloors);
	
	}
	for(i=0;i<MaxFloors;i++)
	{
		sem_init(&semaf[i],0,0);
		semaf_no[i]= 0;
		sem_init(&semat[i],0,0);
		semat_no[i] = 0;
	}
	pthread_t lift;
	int check;
	check = pthread_create(&lift,NULL,elevator,NULL);
	//if(check)
	//{
	//	printf("Failed to initialize the lift");
//		exit(-1);
//	}
	// if(pthread_join(lift,NULL))
	// {
	// 	printf("Error");
	// 	exit(-1);
	// }
	return;
}




/**
 * This program should print data in the specific format
 * described below:
 * If there is a passenger (id 5) going from floor 2 to 4, followed
 * by a passenger (id 6) from 2 to 5, the output should be:
 * 5 2 4
 * 6 2 5
 * i.e. there should be a single space, and each journey should be 
 * on a new line. Incorrectly formatted output will get 0 marks
 */

 /*
		user wait on semaphore of source floor
		signal by elevstor => user in elevator

		destination sema of dest floor (wait)
		elevator signal=> user dropped
 */
void* goingFromToP1(void *arg) {
    struct argument *temp = (struct argument *) arg;
    //goingfrom = temp->from;
    //goingto = temp->to;
    //user_id = temp->id;
    pthread_mutex_lock(& mutex_source);
    semaf_no[temp->from]++;
    pthread_mutex_unlock(& mutex_source);
    sem_wait(&semaf[temp->from]);
    //printf(" User is getting in \t%d, %d\n", temp->id, goingfrom);
    pthread_mutex_lock(& mutex_dest);
    semat_no[temp->to]++;
    pthread_mutex_unlock(& mutex_dest);
    sem_wait(&semat[temp->to]);
    printf("%d %d %d\n", temp->id, temp->from, temp->to);
    //printf(" User is getting out \t%d, %d\n", temp->id, goingto);
    //pthread_mutex_lock(&mutex);
    
    // if(goingfrom == current)
    // {
    // 	printf("User-Id Getting inside from floor %d-%d\n", user_id,current);
    // 	sem_post(&semaf[goingfrom]);

    // }
    /*
    if(goingto == current)
    {
    	printf("User-Id Getting stepping out on floor %d-%d\n", user_id,current);
    	sem_post(&sema[current]);

    }*/
    //pthread_mutex_unlock(&mutex);

    //printf("Thread current is %d\n", current);
    return NULL;
}

void startP1()
{
	//initializeP1(5,7);
	//printf("P1 is called here \n");
}

/*int main()
{
	startP1();
	//Threads = 7;
	pthread_t users[Threads];
	for(i=0;i<Threads;i++)
	{
		struct argument *person = (struct argument*)malloc(sizeof(struct argument));
        person->from = i % MaxFloors;
        person->to =  (i+2) % MaxFloors;
        person->id = 1000 + i;
        printf("value of from %d\n", person->from);
        printf("value of to %d\n", person->to);
        pthread_create(&users[i],NULL,goingFromToP1,(void*)person);

    } 

    for (i=0;i<Threads;i++)
    {
    	if(pthread_join(users[i],NULL))
    	{
    		printf("Warr Gaye %d\n", 1000+i);
    		return -1;
    	}
    }
	
	return 0 ;
}*/
