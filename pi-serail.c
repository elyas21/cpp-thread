#include <stdio.h>
#include <time.h>

#define NUM_TERMS 1000000  // Number of terms to approximate pi

double calculate_pi(int num_terms) {
    double pi = 0.0;
    
    // Leibniz formula for pi: pi = 4 * (1 - 1/3 + 1/5 - 1/7 + 1/9 - ...)
    for (int i = 0; i < num_terms; i++) {
        // Alternating sign: (-1)^i
        if (i % 2 == 0) {
            pi += 1.0 / (2 * i + 1);
        } else {
            pi -= 1.0 / (2 * i + 1);
        }
    }
    
    return 4 * pi;  // Multiply the sum by 4 to get the approximation of pi
}

int main() {
    // Start the timer
    clock_t start_time = clock();
    
    // Calculate pi using the Leibniz sequence
    double pi = calculate_pi(NUM_TERMS);
    
    // End the timer
    clock_t end_time = clock();
    
    // Calculate the time taken
    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    
    // Output the result
    printf("Approximated Pi: %.15f\n", pi);
    printf("Time taken: %f seconds\n", time_taken);
    
    return 0;
}
