/* On-demand 3-thread synchronization */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> // Compile with "gcc cv-sync-2.c -o cv-sync-2 -lpthread"

pthread_cond_t cv1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cv2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cv3 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
int total_runs, t1_runs, t2_runs, t3_runs;
bool t1 = false, t2 = true, t3 = true;
int t1_c = 0, t2_c = 0, t3_c = 0, x = 0;

void * thread1_print()
{
  while(total_runs > 0)
  {
   pthread_mutex_lock(&m);
   if(t1 == true)
   {
    pthread_cond_wait(&cv1, &m);
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
   pthread_cond_signal(&cv2);
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
    pthread_cond_wait(&cv2, &m);
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
   t3 = false;
   pthread_cond_signal(&cv3);
   pthread_mutex_unlock(&m);
  }
}

void * thread3_print()
{
  while(total_runs > 0)
  {
   pthread_mutex_lock(&m);
   if(t3 == true)
   {
    pthread_cond_wait(&cv3, &m);
   }
   for(int k=0; k<t3_runs; k++)
   {
    if(total_runs == 0)
    {
	break;
    }
    x++;
    if(x < 10){
	printf("0%d: IN T3, TID: %u\n", x, (unsigned int)pthread_self());
    }
    else{
    printf("%d: IN T3, TID: %u\n", x, (unsigned int)pthread_self());}
    total_runs--;
    t3_c++;
   }
   t3 = true;
   t1 = false;
   pthread_cond_signal(&cv1);
   pthread_mutex_unlock(&m);
  }
}

void usage(char * bin_str) {
    printf("Usage: %s total_runs t1_runs t2_runs t3_runs\n", bin_str);
}

int main(int argc, char * argv[]) {

    if (argc != 5) {
        usage(argv[0]);
        return 0;
    }

    total_runs = atoi(argv[1]);
    t1_runs = atoi(argv[2]);
    t2_runs = atoi(argv[3]);
    t3_runs = atoi(argv[4]);

    pthread_t thread1, thread2, thread3;

    pthread_create(&thread1, NULL, &thread1_print, NULL);
    pthread_create(&thread2, NULL, &thread2_print, NULL);
    pthread_create(&thread3, NULL, &thread3_print, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    printf("In main thread: T1 printed %d times. T2 printed %d times. T3 printed %d times.\n", t1_c, t2_c, t3_c);

    return 0;
}
