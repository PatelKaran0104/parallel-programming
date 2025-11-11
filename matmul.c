#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 700
#define THREADS 2
int main(int argc, char **argv)
{
    int i, j, k;
    double sum = 0.0;
    
    // Allocate matrices on heap instead of stack to avoid segmentation fault
    double (*a)[N] = malloc(N * N * sizeof(double));
    double (*b)[N] = malloc(N * N * sizeof(double));
    double (*c)[N] = malloc(N * N * sizeof(double));
    
    if (a == NULL || b == NULL || c == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        return 1;
    }
    /* Init */
    for (i = 0; i < N; ++i)
    {
        for (j = 0; j < N; ++j)
        {
            a[i][j] = 3.0 * i + j;
            b[i][j] = 5.2 * i + 2.3 * j;
            c[i][j] = 0.0;
        }
    }
    omp_set_num_threads(THREADS);
    
    /* Matrixmultiplication with parallal for-loop*/
#pragma omp parallel for shared(a, b, c) private(i, j, k) schedule(static)
    for (i = 0; i < N; ++i)
    {
        for (j = 0; j < N; ++j)
        {
            for (k = 0; k < N; ++k)
            {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    for (i = 0; i < N; ++i)
    {
        for (j = 0; j < N; ++j)
        {
            sum += c[i][j];
        }
    }
    printf("Result: %f\n", sum);
    
    // Free allocated memory
    free(a);
    free(b);
    free(c);
    
    return 0;
}