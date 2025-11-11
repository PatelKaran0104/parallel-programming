/*
 * Count3s - Parallel Programming Performance Analysis
 * 
 * This program implements multiple variants of counting the number of 3s
 * in an array using different parallelization strategies with OpenMP.
 * 
 * Variants implemented:
 * 1. Sequential (baseline)
 * 2. Parallel with race condition (incorrect but demonstrates the problem)
 * 3. Parallel with critical section
 * 4. Parallel with atomic operations
 * 5. Parallel with reduction clause
 * 6. Parallel with private counters (manual reduction)
 * 
 * Compile: gcc -fopenmp -O2 count3s.c -o count3s
 * Run: count3s.exe [array_size] [num_threads]
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

// Default values
#define DEFAULT_ARRAY_SIZE 100000000
#define DEFAULT_NUM_THREADS 4
#define NUM_RUNS 5  // Number of runs for averaging

// Function prototypes
void initialize_array(int *arr, long long size);
long long count3s_sequential(int *arr, long long size);
long long count3s_parallel_race(int *arr, long long size);
long long count3s_parallel_critical(int *arr, long long size);
long long count3s_parallel_atomic(int *arr, long long size);
long long count3s_parallel_reduction(int *arr, long long size);
long long count3s_parallel_private(int *arr, long long size);
void run_benchmark(int *arr, long long size, int num_threads);
double get_average_time(double *times, int num_runs);

int main(int argc, char *argv[]) {
    long long array_size = DEFAULT_ARRAY_SIZE;
    int num_threads = DEFAULT_NUM_THREADS;
    
    // Parse command line arguments
    if (argc > 1) {
        array_size = atoll(argv[1]);
    }
    if (argc > 2) {
        num_threads = atoi(argv[2]);
    }
    
    printf("=======================================================\n");
    printf("Count3s - Parallel Programming Performance Analysis\n");
    printf("=======================================================\n");
    printf("Array size: %lld elements\n", array_size);
    printf("Number of threads: %d\n", num_threads);
    printf("Number of runs per variant: %d\n", NUM_RUNS);
    printf("=======================================================\n\n");
    
    // Allocate and initialize array
    printf("Allocating and initializing array...\n");
    int *arr = (int *)malloc(array_size * sizeof(int));
    if (arr == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for array\n");
        return 1;
    }
    
    initialize_array(arr, array_size);
    printf("Array initialized successfully.\n\n");
    
    // Set number of threads
    omp_set_num_threads(num_threads);
    
    // Run benchmarks
    run_benchmark(arr, array_size, num_threads);
    
    // Cleanup
    free(arr);
    
    printf("\n=======================================================\n");
    printf("Benchmark completed successfully!\n");
    printf("=======================================================\n");
    
    return 0;
}

/**
 * Initialize array with random values (0-9)
 * Expected count of 3s: approximately array_size / 10
 */
void initialize_array(int *arr, long long size) {
    // Use a fixed seed for reproducibility
    srand(42);
    
    #pragma omp parallel
    {
        // Each thread uses a different seed based on thread ID
        int tid = omp_get_thread_num();
        unsigned int seed = 42 + tid * 1000;
        
        #pragma omp for
        for (long long i = 0; i < size; i++) {
            // Use rand() with thread-local manipulation for Windows compatibility
            int local_rand = rand() + tid;
            arr[i] = (local_rand % 10);  // Values from 0 to 9
        }
    }
}

/**
 * Variant 1: Sequential version (baseline)
 */
long long count3s_sequential(int *arr, long long size) {
    long long count = 0;
    
    for (long long i = 0; i < size; i++) {
        if (arr[i] == 3) {
            count++;
        }
    }
    
    return count;
}

/**
 * Variant 2: Parallel with race condition
 * WARNING: This version is INCORRECT and demonstrates the race condition problem
 */
long long count3s_parallel_race(int *arr, long long size) {
    long long count = 0;
    
    #pragma omp parallel for
    for (long long i = 0; i < size; i++) {
        if (arr[i] == 3) {
            count++;  // RACE CONDITION: Multiple threads accessing shared variable
        }
    }
    
    return count;
}

/**
 * Variant 3: Parallel with critical section
 */
long long count3s_parallel_critical(int *arr, long long size) {
    long long count = 0;
    
    #pragma omp parallel for
    for (long long i = 0; i < size; i++) {
        if (arr[i] == 3) {
            #pragma omp critical
            {
                count++;
            }
        }
    }
    
    return count;
}

/**
 * Variant 4: Parallel with atomic operations
 */
long long count3s_parallel_atomic(int *arr, long long size) {
    long long count = 0;
    
    #pragma omp parallel for
    for (long long i = 0; i < size; i++) {
        if (arr[i] == 3) {
            #pragma omp atomic
            count++;
        }
    }
    
    return count;
}

/**
 * Variant 5: Parallel with reduction clause
 * This is typically the BEST approach for this problem
 */
long long count3s_parallel_reduction(int *arr, long long size) {
    long long count = 0;
    
    #pragma omp parallel for reduction(+:count)
    for (long long i = 0; i < size; i++) {
        if (arr[i] == 3) {
            count++;
        }
    }
    
    return count;
}

/**
 * Variant 6: Parallel with private counters (manual reduction)
 */
long long count3s_parallel_private(int *arr, long long size) {
    long long count = 0;
    int num_threads = omp_get_max_threads();
    long long *private_counts = (long long *)calloc(num_threads, sizeof(long long));
    
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        
        #pragma omp for
        for (long long i = 0; i < size; i++) {
            if (arr[i] == 3) {
                private_counts[tid]++;
            }
        }
    }
    
    // Manual reduction
    for (int i = 0; i < num_threads; i++) {
        count += private_counts[i];
    }
    
    free(private_counts);
    return count;
}

/**
 * Calculate average time from multiple runs
 */
double get_average_time(double *times, int num_runs) {
    double sum = 0.0;
    for (int i = 0; i < num_runs; i++) {
        sum += times[i];
    }
    return sum / num_runs;
}

/**
 * Run all variants and measure performance
 */
void run_benchmark(int *arr, long long size, int num_threads) {
    double times[NUM_RUNS];
    double start_time, end_time;
    long long result;
    long long correct_count = 0;
    
    printf("-------------------------------------------------------\n");
    printf("Running benchmarks...\n");
    printf("-------------------------------------------------------\n\n");
    
    // ===== VARIANT 1: Sequential =====
    printf("1. SEQUENTIAL VERSION (Baseline)\n");
    for (int run = 0; run < NUM_RUNS; run++) {
        start_time = omp_get_wtime();
        result = count3s_sequential(arr, size);
        end_time = omp_get_wtime();
        times[run] = end_time - start_time;
        
        if (run == 0) {
            correct_count = result;  // Use as reference
            printf("   Count of 3s: %lld\n", result);
        }
    }
    double seq_time = get_average_time(times, NUM_RUNS);
    printf("   Average time: %.6f seconds\n", seq_time);
    printf("   Speedup: 1.00x (baseline)\n\n");
    
    // ===== VARIANT 2: Parallel with Race Condition =====
    printf("2. PARALLEL WITH RACE CONDITION (INCORRECT!)\n");
    printf("   WARNING: This version demonstrates the race condition problem\n");
    for (int run = 0; run < NUM_RUNS; run++) {
        start_time = omp_get_wtime();
        result = count3s_parallel_race(arr, size);
        end_time = omp_get_wtime();
        times[run] = end_time - start_time;
        
        if (run == 0) {
            printf("   Count of 3s: %lld (Correct: %lld) - ", result, correct_count);
            if (result == correct_count) {
                printf("Correct (by chance)\n");
            } else {
                printf("INCORRECT! (Error: %lld)\n", correct_count - result);
            }
        }
    }
    double race_time = get_average_time(times, NUM_RUNS);
    printf("   Average time: %.6f seconds\n", race_time);
    printf("   Speedup: %.2fx\n", seq_time / race_time);
    printf("   Note: Result is unreliable due to race condition!\n\n");
    
    // ===== VARIANT 3: Parallel with Critical Section =====
    printf("3. PARALLEL WITH CRITICAL SECTION\n");
    for (int run = 0; run < NUM_RUNS; run++) {
        start_time = omp_get_wtime();
        result = count3s_parallel_critical(arr, size);
        end_time = omp_get_wtime();
        times[run] = end_time - start_time;
        
        if (run == 0) {
            printf("   Count of 3s: %lld ", result);
            printf(result == correct_count ? "(Correct)\n" : "(INCORRECT!)\n");
        }
    }
    double critical_time = get_average_time(times, NUM_RUNS);
    printf("   Average time: %.6f seconds\n", critical_time);
    printf("   Speedup: %.2fx\n", seq_time / critical_time);
    printf("   Efficiency: %.2f%%\n", (seq_time / critical_time) / num_threads * 100);
    printf("   Note: Critical section creates bottleneck\n\n");
    
    // ===== VARIANT 4: Parallel with Atomic =====
    printf("4. PARALLEL WITH ATOMIC OPERATIONS\n");
    for (int run = 0; run < NUM_RUNS; run++) {
        start_time = omp_get_wtime();
        result = count3s_parallel_atomic(arr, size);
        end_time = omp_get_wtime();
        times[run] = end_time - start_time;
        
        if (run == 0) {
            printf("   Count of 3s: %lld ", result);
            printf(result == correct_count ? "(Correct)\n" : "(INCORRECT!)\n");
        }
    }
    double atomic_time = get_average_time(times, NUM_RUNS);
    printf("   Average time: %.6f seconds\n", atomic_time);
    printf("   Speedup: %.2fx\n", seq_time / atomic_time);
    printf("   Efficiency: %.2f%%\n", (seq_time / atomic_time) / num_threads * 100);
    printf("   Note: Better than critical, but still has overhead\n\n");
    
    // ===== VARIANT 5: Parallel with Reduction =====
    printf("5. PARALLEL WITH REDUCTION CLAUSE (BEST)\n");
    for (int run = 0; run < NUM_RUNS; run++) {
        start_time = omp_get_wtime();
        result = count3s_parallel_reduction(arr, size);
        end_time = omp_get_wtime();
        times[run] = end_time - start_time;
        
        if (run == 0) {
            printf("   Count of 3s: %lld ", result);
            printf(result == correct_count ? "(Correct)\n" : "(INCORRECT!)\n");
        }
    }
    double reduction_time = get_average_time(times, NUM_RUNS);
    printf("   Average time: %.6f seconds\n", reduction_time);
    printf("   Speedup: %.2fx\n", seq_time / reduction_time);
    printf("   Efficiency: %.2f%%\n", (seq_time / reduction_time) / num_threads * 100);
    printf("   Note: Optimal approach with minimal overhead\n\n");
    
    // ===== VARIANT 6: Parallel with Private Counters =====
    printf("6. PARALLEL WITH PRIVATE COUNTERS (Manual Reduction)\n");
    for (int run = 0; run < NUM_RUNS; run++) {
        start_time = omp_get_wtime();
        result = count3s_parallel_private(arr, size);
        end_time = omp_get_wtime();
        times[run] = end_time - start_time;
        
        if (run == 0) {
            printf("   Count of 3s: %lld ", result);
            printf(result == correct_count ? "(Correct)\n" : "(INCORRECT!)\n");
        }
    }
    double private_time = get_average_time(times, NUM_RUNS);
    printf("   Average time: %.6f seconds\n", private_time);
    printf("   Speedup: %.2fx\n", seq_time / private_time);
    printf("   Efficiency: %.2f%%\n", (seq_time / private_time) / num_threads * 100);
    printf("   Note: Similar to reduction but manual implementation\n\n");
    
    // ===== SUMMARY TABLE =====
    printf("-------------------------------------------------------\n");
    printf("PERFORMANCE SUMMARY\n");
    printf("-------------------------------------------------------\n");
    printf("%-25s %12s %10s %10s\n", "Variant", "Time (s)", "Speedup", "Efficiency");
    printf("-------------------------------------------------------\n");
    printf("%-25s %12.6f %10.2fx %9.1f%%\n", "Sequential", seq_time, 1.0, 100.0);
    printf("%-25s %12.6f %10.2fx %9.1f%%\n", "Race Condition", race_time, 
           seq_time / race_time, (seq_time / race_time) / num_threads * 100);
    printf("%-25s %12.6f %10.2fx %9.1f%%\n", "Critical Section", critical_time, 
           seq_time / critical_time, (seq_time / critical_time) / num_threads * 100);
    printf("%-25s %12.6f %10.2fx %9.1f%%\n", "Atomic", atomic_time, 
           seq_time / atomic_time, (seq_time / atomic_time) / num_threads * 100);
    printf("%-25s %12.6f %10.2fx %9.1f%%\n", "Reduction", reduction_time, 
           seq_time / reduction_time, (seq_time / reduction_time) / num_threads * 100);
    printf("%-25s %12.6f %10.2fx %9.1f%%\n", "Private Counters", private_time, 
           seq_time / private_time, (seq_time / private_time) / num_threads * 100);
    printf("-------------------------------------------------------\n");
    
    // ===== ANALYSIS =====
    printf("\nANALYSIS:\n");
    printf("-------------------------------------------------------\n");
    printf("Best performing variant: ");
    double best_time = reduction_time;
    const char *best_name = "Reduction";
    
    if (private_time < best_time) {
        best_time = private_time;
        best_name = "Private Counters";
    }
    
    printf("%s (%.6f seconds, %.2fx speedup)\n", best_name, best_time, seq_time / best_time);
    
    printf("\nKey Observations:\n");
    printf("1. Race Condition: Produces incorrect results due to data races\n");
    printf("2. Critical Section: Correct but very slow due to serialization\n");
    printf("3. Atomic: Better than critical but still has synchronization overhead\n");
    printf("4. Reduction: Optimal - compiler optimizes the reduction operation\n");
    printf("5. Private Counters: Similar to reduction, good manual alternative\n");
    
    printf("\nFactors affecting performance:\n");
    printf("- Number of threads: More threads = more potential speedup (up to a limit)\n");
    printf("- Array size: Larger arrays benefit more from parallelization\n");
    printf("- Frequency of 3s: More 3s = more synchronization overhead (except reduction)\n");
    printf("- Cache effects: False sharing can reduce performance\n");
    printf("- Thread scheduling: OS scheduling can impact performance\n");
}
