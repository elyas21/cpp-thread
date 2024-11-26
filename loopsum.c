#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREAD 1
#define ARRAY_SIZE   100000000



int array[ARRAY_SIZE];
long global_sum = 0;
pthread_mutex_t mutex;

void* sum_array(void* arg){
    int thread_part = *(int*)arg;
    long local_sum = 0;

    int start = thread_part * (ARRAY_SIZE / NUM_THREAD);
    int end  = (thread_part + 1 ) * (ARRAY_SIZE / NUM_THREAD);

    for(int i = start; i < end; i++){
        local_sum+= array[i];
    }    

    pthread_mutex_lock(&mutex);
    global_sum+=local_sum;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);

}


int main(){
    for(int i =0 ; i<ARRAY_SIZE; i++){
        array[i]=1;

    }

    pthread_mutex_init(&mutex, NULL);

    pthread_t threads[NUM_THREAD];
    int thread_ids[NUM_THREAD];

    clock_t start_time = clock();

    for (int i = 0; i< NUM_THREAD; i++){
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, sum_array, (void*)&thread_ids[i]);

    }

    for (int i = 0; i < NUM_THREAD; i++)
    {
        pthread_join(threads[i], NULL);
    }

    clock_t end_time= clock();


    double time_taken = (double)(end_time - start_time)/ CLOCKS_PER_SEC;


    printf("Global sum: %ld\n", global_sum);
    printf("Time taken: %f seconds\n", time_taken);

    pthread_mutex_destroy(&mutex);
    return 0;        
    }
