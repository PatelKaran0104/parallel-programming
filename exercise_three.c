#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

int main(int argc, char **argv)
{

    // #pragma omp parallel
    // printf(" I am parallel \n ");
    // printf(" I am not parallel \n ");


    printf("Outside parallel region: Number of threads available = %d\n\n", 
           omp_get_max_threads());
    
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();
        
        printf("Thread %d of %d: I am parallel\n", thread_id, total_threads);
        printf("Thread %d of %d: I am ALSO parallel now!\n", thread_id, total_threads);
    }
    
    printf("\nOutside parallel region again: Number of threads available = %d\n", 
           omp_get_max_threads());
    
    return 0;
}