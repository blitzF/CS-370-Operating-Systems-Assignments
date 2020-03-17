#include <pthread.h>
#include "main.h"
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#include "part2.h"

const int INTER_ARRIVAL_TIME = 5;
const int NUM_TRAINS = 5;
int j;
int I = 30;
int stations_num;
int MaxPeople;
int current_stat = 0;
pthread_mutex_t mutex_board = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_train = PTHREAD_MUTEX_INITIALIZER;
sem_t *sema_board_area;
sem_t *sema_enter_train;
int  *b_waiters;
int  *t_waiters;

/**
 * Do any initial setup work in this function.
 * numStations: Total number of stations. Will be >= 5. Assume that initially
 * the first train is at station 1, the second at 2 and so on.
 * maxNumPeople: The maximum number of people in a train
 */
void *train_func()
{
	while(I)
	{
		pthread_mutex_lock(& mutex_board);
		pthread_mutex_lock(& mutex_train);
		while(b_waiters[current_stat] >= 1)
		{
			b_waiters[current_stat]--;
			sem_post(&sema_board_area[current_stat]);
		}
		while(t_waiters[current_stat] >= 1)
		{
			t_waiters[current_stat]--;
			sem_post(&sema_enter_train[current_stat]);
		}
		pthread_mutex_unlock(& mutex_board);
		pthread_mutex_unlock(& mutex_train);
		current_stat = (current_stat + 1) % stations_num;
		sleep(5);
		I--;
	}

}


void initializeP2(int numStations, int maxNumPeople) 
{
	//stations_num = malloc(sizeof(int) * numStations);
	stations_num = numStations;	
	MaxPeople = maxNumPeople;

	{
	sema_board_area = malloc(sizeof(sem_t)* numStations);
	sema_enter_train = malloc(sizeof(sem_t)* numStations);

	b_waiters = malloc(sizeof(int) * numStations);
	t_waiters = malloc(sizeof(int) * numStations);
	}
	for(j=0;j<numStations;j++)
	{
		sem_init(&sema_board_area[j],0,0);
		b_waiters[j]= 0;
		sem_init(&sema_enter_train[j],0,0);
		t_waiters[j] = 0;
	}
	pthread_t m_train;
	pthread_create(&m_train,NULL,train_func,NULL);
	return;


}

/**
 * Print in the following format:
 * If a user borads on train 0, from station 0 to station 1, and another boards
 * train 2 from station 2 to station 4, then the output will be
 * 0 0 1
 * 2 2 4
 */
void *goingFromToP2(void *argu) {
    struct argument *arg = (struct argument*)(argu);
    pthread_mutex_lock(& mutex_board);
    b_waiters[arg->from]++;
    pthread_mutex_unlock(& mutex_board);
    sem_wait(&sema_board_area[arg->from]);
    pthread_mutex_lock(& mutex_train);
    t_waiters[arg->to]++;
    pthread_mutex_unlock(& mutex_train);
    sem_wait(&sema_enter_train[arg->to]);
    printf("%d %d %d\n", arg->id, arg->from, arg->to);
    return NULL;
}

void startP2()
{

}
