#include <stdio.h>
#include <math.h>

void calculate_and_write_to_csv() {
    FILE *fp = fopen("problem1output.csv", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    
    // Write headers to CSV
    fprintf(fp, "n,p,Speedup,Efficiency\n");
    
    int n_values[] = {10, 20, 40, 80, 160, 320};
    int p_values[] = {1, 2, 4, 8, 16, 32, 64, 128};
    
    for (int i = 0; i < 6; i++) {
        int n = n_values[i];
        double T_serial = n * n;
        
        for (int j = 0; j < 8; j++) {
            int p = p_values[j];
            double T_parallel = (n * n) / (double)p + log2(p);
            double speedup = T_serial / T_parallel;
            double efficiency = speedup / p;
            
            // Write the results for each n and p to CSV
            fprintf(fp, "%d,%d,%.4f,%.4f\n", n, p, speedup, efficiency);
        }
    }
    
    fclose(fp);
    printf("Results written to output.csv\n");
}

int main() {
    calculate_and_write_to_csv();
    return 0;
}
