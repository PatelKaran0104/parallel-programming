#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

static long num_steps = 1000000;
double step;

int main()
{
    step = 1.0 / (double)num_steps;

    double start_time = omp_get_wtime();

    int max_threads = omp_get_max_threads();
    double *partial = malloc(sizeof(double) * max_threads);

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int nthreads = omp_get_num_threads();

        double local_sum = 0.0;
        double x;

        for (long i = tid; i < num_steps; i += nthreads) {
            x = (i + 0.5) * step;
            local_sum += 4.0 / (1.0 + x * x);
        }

        partial[tid] = local_sum;
    }

    double sum = 0.0;
    for (int i = 0; i < max_threads; i++)
        sum += partial[i];

    double pi = step * sum;

    printf("Approximation of Pi (cyclic): %.10f\n", pi);
    printf("Threads used: %d\n", max_threads);
    printf("Time taken: %f seconds\n", omp_get_wtime() - start_time);

    free(partial);
    return 0;
}
