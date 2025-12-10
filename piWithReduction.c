#include <stdio.h>
#include <omp.h>

static long num_steps = 1500000000;
double step;

int main()
{
    double sum = 0.0;
    step = 1.0 / (double)num_steps;

    double start_time = omp_get_wtime();

#pragma omp parallel for reduction(+ : sum)
    for (long i = 0; i < num_steps; i++)
    {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }

    double pi = step * sum;
    double time_taken = omp_get_wtime() - start_time;

    printf("%f\n", time_taken);

    return 0;
}
