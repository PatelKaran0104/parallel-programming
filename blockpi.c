#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

static long num_steps = 1000000;
double step;

int main()
{
    double pi = 0.0;
    step = 1.0 / (double)num_steps;

    double start_time = omp_get_wtime();
    int threads_env = omp_get_max_threads();

    double *partial = malloc(sizeof(double) * threads_env);

    #pragma omp parallel
    {
        int threadId = omp_get_thread_num();
        int nthreads = omp_get_num_threads();

        long chunk = num_steps / nthreads;
        long start = threadId * chunk;
        long end = (threadId == nthreads - 1) ? num_steps : start + chunk;

        double local_sum = 0.0;
        double x;

        for (long i = start; i < end; i++) {
            x = (i + 0.5) * step;
            local_sum += 4.0 / (1.0 + x * x);
        }

        partial[threadId] = local_sum;
    }

    double sum = 0.0;
    for (int i = 0; i < threads_env; i++)
        sum += partial[i];

    pi = step * sum;

    printf("Approximation of Pi: %.10f\n", pi);
    printf("Threads used: %d\n", threads_env);
    printf("Time taken: %f seconds\n", omp_get_wtime() - start_time);

    free(partial);
    return 0;
}
