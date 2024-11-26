#include <stdio.h>
#include <math.h>

int main() {
    for (int n = 10; n <= 3200; n *= 2) {
        for (int p = 1; p <= 128; p *= 2) {
            double Tserial = n * n;
            double Tparallel = (n * n) / p + log2(p);
            double speedup = Tserial / Tparallel;
            double efficiency = speedup / p;

            printf("n = %d, p = %d, Speedup = %.2f, Efficiency = %.2f\n", n, p, speedup, efficiency);
        }
    }

    return 0;
}
