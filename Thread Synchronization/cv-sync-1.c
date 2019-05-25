/* On-demand 2-thread synchronization */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> // Compile with "gcc cv-sync-1.c -o cv-sync-1 -lpthread"

pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
int total_runs, t1_runs, t2_runs;
bool t1 = false, t2 = true;
int t1_c = 0, t2_c = 0, x = 0;


void usage(char * bin_str) {
    printf("Usage: %s total_runs t1_runs t2_runs\n", bin_str);
}

void * thread1_print()
{
   while(total_runs > 0)
   {
	pthread_mutex_lock(&m);
	if(t1 == true)
	{
	  pthread_cond_wait(&cv, &m);
	}
	for(int i=0; i<t1_runs; i++)
	{
	  if(total_runs == 0)
	  {
		break;
	  }
	  x++;
	  if(x < 10){
  		printf("0%d: IN T1, TID: %u\n", x, (unsigned int)pthread_self());
	  }
	  else{
	  printf("%d: IN T1, TID: %u\n", x,  (unsigned int)pthread_self());}
	  total_runs--;
	  t1_c++;
	}
	t1 = true;
	t2 = false;
	pthread_cond_signal(&cv);
	pthread_mutex_unlock(&m);
   }
}

void * thread2_print()
{
   while(total_runs > 0)
   {
 	pthread_mutex_lock(&m);
	if(t2 == true)
	{
	 pthread_cond_wait(&cv, &m);
	}
	for(int j=0; j<t2_runs; j++)
	{
	  if(total_runs == 0)
	  {
		  break;
	  }
	  x++;
	  if(x < 10){
		printf("0%d: IN T2, TID: %u\n", x, (unsigned int)pthread_self());
	  }
	  else{
	  printf("%d: IN T2, TID: %u\n", x, (unsigned int)pthread_self());}
	  total_runs--;
	  t2_c++;
	}
	t2 = true;
	t1 = false;
	pthread_cond_signal(&cv);
	pthread_mutex_unlock(&m);
   }
}


int main(int argc, char * argv[]) {

    if (argc != 4) {
        usage(argv[0]);
        return 0;
    }

    total_runs = atoi(argv[1]);
    t1_runs = atoi(argv[2]);
    t2_runs = atoi(argv[3]);

    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, &thread1_print, NULL);
    pthread_create(&thread2, NULL, &thread2_print, NULL);
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("In main thread: T1 printed %d times. T2 printed %d times.\n", t1_c, t2_c);

    return 0;
}

