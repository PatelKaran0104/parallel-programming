#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4

int total_iterations;
double partial_sums[NUM_THREADS];

void *calculate_part(void *arg) {
    int thread_id = *(int *)arg;
    int start = (total_iterations / NUM_THREADS) * thread_id;
    int end = (thread_id == NUM_THREADS - 1) ? total_iterations : start + (total_iterations / NUM_THREADS);
    
    double sum = 0.0;
    for (int i = start; i < end; i++) {
        if (i % 2 == 0)
            sum += 1.0 / (2 * i + 1);
        else
            sum -= 1.0 / (2 * i + 1);
    }
    
    partial_sums[thread_id] = sum;
    return NULL;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <iterations>\n", argv[0]);
        return 1;
    }
    
    total_iterations = atoi(argv[1]);
    printf("Calculating pi with %d iterations using %d threads...\n", total_iterations, NUM_THREADS);
    
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, calculate_part, &thread_ids[i]);
    }
    
    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Add up all partial results
    double pi = 0.0;
    for (int i = 0; i < NUM_THREADS; i++) {
        pi += partial_sums[i];
    }
    pi *= 4.0;
    
    printf("π ≈ %.15f\n", pi);
    return 0;
}
