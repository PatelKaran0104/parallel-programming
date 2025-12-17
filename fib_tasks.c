#include <stdio.h>
#include <omp.h>

int cutoff = 20;

long fib_serial(long n)
{
    if (n < 2)
        return n;
    return fib_serial(n - 1) + fib_serial(n - 2);
}

long fib_parallel(long n)
{
    long x, y;

    if (n < cutoff)
    {
        return fib_serial(n);
    }

#pragma omp task shared(x) firstprivate(n)
    {
        x = fib_parallel(n - 1);
    }

#pragma omp task shared(y) firstprivate(n)
    {
        y = fib_parallel(n - 2);
    }

#pragma omp taskwait
    return x + y;
}

int main()
{
    long n = 40;

    double serial_start = omp_get_wtime();
    long serial_result = fib_serial(n);
    double serial_time = omp_get_wtime() - serial_start;

    fprintf(stderr, "Serial result for n=%ld: %ld\n", n, serial_result);
    fprintf(stderr, "Serial time: %f seconds\n", serial_time);

    long parallel_result;
    double parallel_start, parallel_time;

    parallel_start = omp_get_wtime(

        
    );

#pragma omp parallel
    {
#pragma omp single
        {
            parallel_result = fib_parallel(n);
        }
    }

    parallel_time = omp_get_wtime() - parallel_start;

    fprintf(stderr, "Parallel result for n=%ld: %ld\n", n, parallel_result);
    fprintf(stderr, "Parallel time: %f seconds\n", parallel_time);

    if (serial_result == parallel_result)
    {
        fprintf(stderr, "Results match ✔️\n");
    }
    else
    {
        fprintf(stderr, "Results DO NOT MATCH ❌\n");
    }

    printf("%f\n", parallel_time);

    return 0;
}
