#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <omp.h>

double getTime() {
    const double kMicro = 1.0e-6;
    struct timeval TV;

    const int RC = gettimeofday(&TV, NULL);
    if (RC == -1) {
        printf("ERROR: Bad call to gettimeofday\n");
        return(-1);
    }
    return(((double)TV.tv_sec) + kMicro * ((double)TV.tv_usec));
}

// Function to save performance comparison to CSV
void savePerformanceToCSV(long iterations, int numThreads, double seqTime, double pthreadSpeedup, double ompSpeedup) {
    FILE *fp = fopen("mppc5.csv", "a");
    if (fp == NULL) {
        printf("Error opening file for performance comparison.\n");
        return;
    }
    fprintf(fp, "%ld,%d,%f,%f,%f\n", iterations, numThreads, seqTime, pthreadSpeedup, ompSpeedup);
    fclose(fp);
}

// Global variables
double sum = 0.0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
typedef struct {
    long start;
    long end;
    int thread_id;
    double local_sum;
} ThreadData;

void* calculatePi(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    double factor = (data->start % 2 == 0) ? 1.0 : -1.0;
    data->local_sum = 0.0;

    for (long k = data->start; k < data->end; k++) {
        data->local_sum += factor / (2 * k + 1);
        factor = -factor;
    }

    pthread_mutex_lock(&mutex);
    sum += data->local_sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

// Sequential computation of PI
double sequentialCompute(long iterations) {
    double factor = 1.0;
    double sum = 0;
    double pi_approximation;

    for (long k = 0; k < iterations; k++) {
        sum += factor / (2 * k + 1);
        factor = -factor;
    }

    pi_approximation = 4.0 * sum;
    return pi_approximation;
}

// Parallel computation with Pthreads
double parallelCompute(long iterations, int numberOfThreads) {
    pthread_t* threads = (pthread_t*)malloc(numberOfThreads * sizeof(pthread_t));
    ThreadData* thread_data = (ThreadData*)malloc(numberOfThreads * sizeof(ThreadData));

    sum = 0.0;
    long chunk_size = iterations / numberOfThreads;
    long remainder = iterations % numberOfThreads;

    for (int i = 0; i < numberOfThreads; i++) {
        thread_data[i].start = i * chunk_size;
        thread_data[i].end = (i == numberOfThreads - 1) ? (i + 1) * chunk_size + remainder : (i + 1) * chunk_size;
        thread_data[i].thread_id = i;
        thread_data[i].local_sum = 0.0;

        if (pthread_create(&threads[i], NULL, calculatePi, (void*)&thread_data[i]) != 0) {
            printf("Error creating thread %d\n", i);
            exit(1);
        }
    }

    for (int i = 0; i < numberOfThreads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            printf("Error joining thread %d\n", i);
            exit(1);
        }
    }

    return 4.0 * sum;
}

// Parallel computation with OpenMP
double parallelComputeOpenMP(long iterations, int numberOfThreads) {
    double sum = 0.0;
    #pragma omp parallel for num_threads(numberOfThreads) reduction(+:sum)
    for (long k = 0; k < iterations; k++) {
        double factor = (k % 2 == 0) ? 1.0 : -1.0;
        sum += factor / (2 * k + 1);
    }
    return 4.0 * sum;
}

double getDifference(double calculatedPi) {
    return calculatedPi - 3.14159265358979323846;
}

int main(int argc, char* argv[]) {
    if (argc <= 2) {
        printf("\nPlease provide number of iterations and number of threads as command line arguments.\n");
        return 0;
    }

    long giterations = strtol(argv[1], NULL, 10);
    int gnumberOfThreads = strtol(argv[2], NULL, 10);

    // Write CSV headers
    FILE *fp = fopen("performance_comparison.csv", "w");
    if (fp != NULL) {
        fprintf(fp, "Iterations,Threads,Sequential Time,Pthread Speedup,OpenMP Speedup\n");
        fclose(fp);
    }

    // Loop through different powers of 10 for iterations
    for (long iterations = 10; iterations <= giterations; iterations *= 10) {
        // Loop through different thread counts
        for (int numberOfThreads = 2; numberOfThreads <= gnumberOfThreads; numberOfThreads *= 2) {
            double sequentialStart = getTime();
            double sequentialPi = sequentialCompute(iterations);
            double sequentialEnd = getTime();
            double sequentialTime = sequentialEnd - sequentialStart;

            double parallelStart = getTime();
            double parallelPi = parallelCompute(iterations, numberOfThreads);
            double parallelEnd = getTime();
            double parallelTime = parallelEnd - parallelStart;

            double parallelStartOpenMP = getTime();
            double openMPPi = parallelComputeOpenMP(iterations, numberOfThreads);
            double parallelEndOpenMP = getTime();
            double parallelTimeOpenMP = parallelEndOpenMP - parallelStartOpenMP;

            // Speedup calculation
            double pthreadSpeedup = sequentialTime / parallelTime;
            double openMPSpeedup = sequentialTime / parallelTimeOpenMP;

            // Print results
            printf("Iterations: %ld, Threads: %d\n", iterations, numberOfThreads);
            printf("Sequential Time: %f, Parallel Time (Pthreads): %f, Parallel Time (OpenMP): %f\n", 
                    sequentialTime, parallelTime, parallelTimeOpenMP);
            printf("Sequential PI: %f, Parallel PI (Pthreads): %f, Parallel PI (OpenMP): %f\n", 
                    sequentialPi, parallelPi, openMPPi);
            printf("Speedup (Pthreads): %f, Speedup (OpenMP): %f\n\n", pthreadSpeedup, openMPSpeedup);

            // Save results to the same CSV file
            savePerformanceToCSV(iterations, numberOfThreads, sequentialTime, pthreadSpeedup, openMPSpeedup);
        }
    }

    return 0;
}
