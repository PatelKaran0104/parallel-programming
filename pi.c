#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
    clock_t start = clock();
    int iterations = 0;
    if (argc < 2)
    {
        printf("The number of iterations was not provided.");
        return 1;
    }
    else
    {
        iterations = atoi(argv[1]);
        printf("hey the val is : %d\n", iterations);
    }
    double pi = 0.0;

    for (int i = 0; i < iterations; i++)
    {
        double term = 1.0 / (2 * i + 1);
        if (i % 2 == 0)
        {
            pi += term;
        }
        else
        {
            pi -= term;
        }
    }

    pi *= 4.0;
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Leibniz formula approximation of pi after %d iterations:\n", iterations);
    printf("π = %.15f\n", pi);
    printf("Time taken: %.6f seconds\n", time_spent);

    return 0;
}
