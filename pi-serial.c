// Instructions on how to use this code:
// Create two versions of the code, one for OpenMP and one for Pthreads.
// For each, provide an implementation of the parallel pi calculation.
// Example file names for these programs are pi-openmp.c and pi-pthreads.c. 


// To compile OpenMP on Lufer with Intel Compiler, use the following: 
//       icc -o pi-openmp -O3 -openmp pi-openmp.c
// To compile for Pthreads, use the following: 
//       icc -o pi-pthreads -O3 pi-pthreads.c -lpthread 
// Run OpenMP version with
//       ./pi-openmp ITERS NumThreads > openmp.out
// And Pthreads version with
//       ./pi-pthreads ITERS NumThreads > pthreads.out 

// Modified by : Didem Unat
// Original Author: Mary Hall

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

double getTime()
{
  const double kMicro = 1.0e-6;
  struct timeval TV;

  const int RC = gettimeofday(&TV, NULL);
  if(RC == -1)
    {
      printf("ERROR: Bad call to gettimeofday\n");
      return(-1);
    }
  return( ((double)TV.tv_sec) + kMicro * ((double)TV.tv_usec) );
}

// TODO: Uncomment one of these include files for each version
#include "pthread.h"
#include "omp.h"
// Function to save speed comparison to CSV
void saveSpeedToCSV(long iterations, int numThreads, double seqTime, double pthreadTime, double ompTime) {
    FILE *fp = fopen("speed_comparison.csv", "a");
    if (fp == NULL) {
        printf("Error opening file for speed comparison.\n");
        return;
    }
    fprintf(fp, "%ld,%d,%f,%f,%f\n", iterations, numThreads, seqTime, pthreadTime, ompTime);
    fclose(fp);
}

// Function to save performance comparison to CSV
void savePerformanceToCSV(long iterations, int numThreads, double seqTime, double pthreadSpeedup, double ompSpeedup) {
    FILE *fp = fopen("performance_comparison.csv", "a");
    if (fp == NULL) {
        printf("Error opening file for performance comparison.\n");
        return;
    }
    fprintf(fp, "%ld,%d,%f,%f,%f\n", iterations, numThreads, seqTime, pthreadSpeedup, ompSpeedup);
    fclose(fp);
}

// TODO: Declare any global variables or types here
double sum = 0.0;
long iterationsPerThread;

// Method declarations
void* calculatePartialSum (void* threadNumber);
double sequentialCompute (long iterations);
double parallelCompute(long iterations, int numberOfThreads);
double parallelComputeOpenMP(long iterations, int numberOfThreads);
double getDifference(double calculatedPi);


// Main method
// int main(int argc, char* argv[])
// {
//   // Variable declarations
//   double sequentialStart, sequentialEnd, sequentialTime;
//   double parallelStart, parallelEnd, parallelTime;

  
//   double parallelStartOpenMP, parallelEndOpenMP, parallelTimeOpenMP;
  
  
//   double sequentialPi, parallelPi, openMPPi;
//   double sequentialDifference, parallelDifference, openMPDifference;
//   long iterations; 
//   int numberOfThreads;



//   // Get number of iterations and number of threads from the command line
//   if(argc > 1)
//     {
//       iterations = strtol(argv[1], NULL, 10);
//       numberOfThreads = strtol(argv[2], NULL, 10);
//     }
//   else
//     {
//       printf("\nWhen running this program, please include number of iterations and number of threads on command line.\n");
//       return 0;
//     }

//   // Time sequential calculation
//   sequentialStart = getTime();
//   sequentialPi = sequentialCompute(iterations);
//   sequentialEnd = getTime();
//   sequentialTime = sequentialEnd - sequentialStart;
    
//   // Time parallel calculation
//   parallelStart = getTime();
//   parallelPi = parallelCompute(iterations, numberOfThreads);
//   parallelEnd = getTime();
//   parallelTime = parallelEnd - parallelStart;
  
//   // Time openMP parralle calculation
//   parallelStartOpenMP = getTime();
//   openMPPi = parallelComputeOpenMP(iterations, numberOfThreads);
//   parallelEndOpenMP = getTime();
//   parallelTimeOpenMP = parallelEndOpenMP - parallelStartOpenMP;

//   // How do results compare with PI?
//   sequentialDifference = getDifference(sequentialPi);
//   parallelDifference = getDifference(parallelPi);
//   openMPDifference = getDifference(openMPPi);
  
  
  
//   // Print results
//   printf("Sequential Calculation: %f\n", sequentialPi);
//   printf("This is %f away from the correct value of PI.\n\n", sequentialDifference);

//   printf("ParallelCalculation: %f\n", parallelPi);
//   printf("This is %f away from the correct value of PI.\n\n", parallelDifference);

//   printf("ParallelCalculationOpenMp: %f\n", openMPPi);
//   printf("This is %f away from the correct value of PI.\n\n", openMPDifference);
  
//   printf("Number of iterations: %ld, Number of Threads: %d\n\n", iterations, numberOfThreads);

//   // Calculate the validity of the parallel computation
//   double difference = parallelDifference - sequentialDifference;

//   if (difference < .01 && difference > -.01)
//     printf("Parallel calculation is VALID!\n");
//   else
//     printf("Parallel calculation is INVALID!\n");


//   double differenceOpenMP = openMPDifference - sequentialDifference;

//   if (differenceOpenMP < .01 && difference > -.01)
//     printf("OpenMp Parallel calculation is VALID!\n");
//   else
//     printf("OpenMp Parallel calculation is INVALID!\n");

//   // Calculate and print speedup results
//   double speedup = ((double)sequentialTime)/((double)parallelTime);
//   printf("Sequential Time: %f, Parallel Time: %f, Speedup: %f\n", sequentialTime, parallelTime, speedup);
  
//   // Calculate and print speedup results
//   double speedupOpenMP = ((double)sequentialTime)/((double)parallelTimeOpenMP);
//   printf("Sequential Time: %f, Parallel Time: %f, Speedup: %f\n", sequentialTime, parallelTimeOpenMP, speedupOpenMP);


//   return 0;
// }
int main(int argc, char* argv[])
{
    // Variables to store times and results
    double sequentialStart, sequentialEnd, sequentialTime;
    double parallelStart, parallelEnd, parallelTime;
    double parallelStartOpenMP, parallelEndOpenMP, parallelTimeOpenMP;
    double sequentialPi, parallelPi, openMPPi;
    double sequentialDifference, parallelDifference, openMPDifference;
  long giterations; 
  int gnumberOfThreads;

  if(argc > 1)
    {
      giterations = strtol(argv[1], NULL, 10);
      gnumberOfThreads = strtol(argv[2], NULL, 10);
    }
  else
    {
      printf("\nWhen running this program, please include number of iterations and number of threads on command line.\n");
      return 0;
    }

    // Loop through different powers of 10 for iterations (from 10^1 to 10^9)
    for (long iterations = 10; iterations <= giterations; iterations *= 10) {
        // Loop through different thread counts (e.g., 2, 4, 8 threads)
        for (int numberOfThreads = 2; numberOfThreads <= gnumberOfThreads; numberOfThreads *= 2) {

            // Time sequential calculation
            sequentialStart = getTime();
            sequentialPi = sequentialCompute(iterations);
            sequentialEnd = getTime();
            sequentialTime = sequentialEnd - sequentialStart;

            // Time parallel Pthreads calculation
            parallelStart = getTime();
            parallelPi = parallelCompute(iterations, numberOfThreads);
            parallelEnd = getTime();
            parallelTime = parallelEnd - parallelStart;

            // Time parallel OpenMP calculation
            parallelStartOpenMP = getTime();
            openMPPi = parallelComputeOpenMP(iterations, numberOfThreads);
            parallelEndOpenMP = getTime();
            parallelTimeOpenMP = parallelEndOpenMP - parallelStartOpenMP;

            // Calculate how far from PI the results are
            sequentialDifference = getDifference(sequentialPi);
            parallelDifference = getDifference(parallelPi);
            openMPDifference = getDifference(openMPPi);

            // Print results
            printf("Iterations: %ld, Threads: %d\n", iterations, numberOfThreads);
            printf("Sequential Time: %f, Parallel Time (Pthreads): %f, Parallel Time (OpenMP): %f\n", 
                    sequentialTime, parallelTime, parallelTimeOpenMP);
            printf("Sequential PI: %f, Parallel PI (Pthreads): %f, Parallel PI (OpenMP): %f\n", 
                    sequentialPi, parallelPi, openMPPi);

            // Calculate and print speedup
            double pthreadSpeedup = sequentialTime / parallelTime;
            double openMPSpeedup = sequentialTime / parallelTimeOpenMP;
            printf("Speedup (Pthreads): %f, Speedup (OpenMP): %f\n\n", pthreadSpeedup, openMPSpeedup);

            // Save results to CSV files
            saveSpeedToCSV(iterations, numberOfThreads, sequentialTime, parallelTime, parallelTimeOpenMP);
            savePerformanceToCSV(iterations, numberOfThreads, sequentialTime, pthreadSpeedup, openMPSpeedup);
        }
    }

    return 0;
}

// Sequential computation of PI
double sequentialCompute (long iterations)
{
  double factor = 1.0;
  double sum = 0;
  double pi_approximation; 

  long k = 0;
  for (k = 0; k < iterations; k++)
    {
      sum += factor/(2*k+1);
      factor = -factor;
    }

  pi_approximation = 4.0 * sum; 
  return pi_approximation;
}

// Find how close the calculation is to the actual value of PI
double getDifference(double calculatedPi)
{
  return calculatedPi - 3.14159265358979323846;
}
/*
// TODO: You need to implement this function.
// Parallel computation of PI
double parallelCompute(long iterations, int numberOfThreads)
{
    // TODO: This is the code you need to write for both OpenMP and Pthreads.
    // TODO: It should initialize the number of threads.  For pthreads, it
    // TODO: needs to create the threads.  Then it performs the pi computation 
    // TODO: in parallel.  Each thread performs the sum computation to arrive
    // TODO: at global sum.  Finally, it returns the result. 

  return 0.0; 
}


*/

// Add these global variables at the top with other globals
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
typedef struct {
    long start;
    long end;
    int thread_id;
    double local_sum;
} ThreadData;

// Add this thread function before parallelCompute
void* calculatePi(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    double factor = (data->start % 2 == 0) ? 1.0 : -1.0;
    data->local_sum = 0.0;
    
    for (long k = data->start; k < data->end; k++) {
        data->local_sum += factor/(2*k+1);
        factor = -factor;
    }
    
    pthread_mutex_lock(&mutex);
    sum += data->local_sum;
    pthread_mutex_unlock(&mutex);
    
    return NULL;
}

double parallelCompute(long iterations, int numberOfThreads) {
    pthread_t* threads = (pthread_t*)malloc(numberOfThreads * sizeof(pthread_t));
    ThreadData* thread_data = (ThreadData*)malloc(numberOfThreads * sizeof(ThreadData));
    
    // Reset global sum
    sum = 0.0;
    
    // Calculate iterations per thread
    long chunk_size = iterations / numberOfThreads;
    long remainder = iterations % numberOfThreads;
    
    // Create threads
    for (int i = 0; i < numberOfThreads; i++) {
        thread_data[i].start = i * chunk_size;
        thread_data[i].end = (i == numberOfThreads - 1) ? 
            (i + 1) * chunk_size + remainder : (i + 1) * chunk_size;
        thread_data[i].thread_id = i;
        thread_data[i].local_sum = 0.0;
        
        if (pthread_create(&threads[i], NULL, calculatePi, (void*)&thread_data[i]) != 0) {
            printf("Error creating thread %d\n", i);
            exit(1);
        }
    }
    
    // Join threads
    for (int i = 0; i < numberOfThreads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            printf("Error joining thread %d\n", i);
            exit(1);
        }
    }
    
    // Clean up
    free(threads);
    free(thread_data);
    
    // Calculate final pi approximation
    return 4.0 * sum;

}



double parallelComputeOpenMP(long iterations, int numberOfThreads) {
    double factor = 1.0;
    double pi_approximation = 0.0;

    // Set the number of threads
    omp_set_num_threads(numberOfThreads);

    // Parallel region
    #pragma omp parallel for reduction(+:pi_approximation) private(factor)
    for (long k = 0; k < iterations; k++) {
        factor = (k % 2 == 0) ? 1.0 : -1.0;
        pi_approximation += factor / (2 * k + 1);
    }

    return 4.0 * pi_approximation;
}