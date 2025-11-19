#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

static long num_steps = 1000000;
double step;

int main()
{
    int i;
    double x, pi, sum = 0.0;
    step = 1.0 / (double)num_steps;
    double start_time = omp_get_wtime();
    #pragma omp parallel for reduction(+:sum) private(x)
    for (i = 0; i < num_steps; i++)
    {
        x = (i + 0.5) * step;
        sum = sum + 4.0 / (1.0 + x * x);
    }
    pi = step * sum;
    printf(" Approximation of Pi : %.10f\n", pi);
    printf(" Time taken: %f seconds\n", omp_get_wtime() - start_time);
    return 0;
}
