#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 8
#define MATRIX_SIZE 256

int matrix1[MATRIX_SIZE][MATRIX_SIZE], matrix2[MATRIX_SIZE][MATRIX_SIZE], result[MATRIX_SIZE][MATRIX_SIZE];
pthread_mutex_t mutex; // Mutex to protect shared variables
pthread_cond_t cond;   // Conditional variable for thread synchronization
int threads_completed = 0; // Variable to track the number of completed threads
int current_thread = NUM_THREADS - 1; // Variable to track the current thread

// Write the result matrix to result.txt
void matrixOutput(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    FILE *fp = fopen("result.txt", "w");
    if (fp == NULL) {
        printf("Could not open file!\n");
        exit(-1);
    }

    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            fprintf(fp, "%d ", matrix[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

// Function to print thread completion messages
void printThreadMessage(int num) {
    pthread_mutex_lock(&mutex);
    
    // Wait for the current thread to print
    while (current_thread != num) {
        pthread_cond_wait(&cond, &mutex);
    }

    printf("Thread %d: Done\n", num);
    fflush(stdout); // Flush the output so messages print in order

    // Signal the next thread to print
    current_thread--;

    threads_completed++;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

// Matrix addition function for each thread to complete
void *matrixAdd(void *arg) {
    int num = *((int *)arg);
    free(arg);

    // Create a block of rows
    int startRow = num * (MATRIX_SIZE / NUM_THREADS);
    int endRow = startRow + (MATRIX_SIZE / NUM_THREADS) - 1;

    // Perform matrix addition for the block
    for (int row = startRow; row <= endRow; row++) {
        for (int col = 0; col < MATRIX_SIZE; col++) {
            result[row][col] = matrix1[row][col] + matrix2[row][col];
        }
    }

    // Print the thread complete message
    printThreadMessage(num);

    pthread_exit(NULL);
}

int main() {
    // Initialize the mutex and conditional variable
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // Initialize the two matrices
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrix1[i][j] = j + 1;
            matrix2[i][j] = j + 1;
            result[i][j] = 0;
        }
    }

    // Create threads and assign jobs
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        int *thread_id = (int *)malloc(sizeof(int));
        *thread_id = i;
        pthread_create(&threads[i], NULL, matrixAdd, (void *)thread_id);
    }

    // Wait for all the threads to complete
    while (threads_completed < NUM_THREADS) {
        usleep(500000); // Sleep for 0.5 seconds
    }

    // Write the result
    matrixOutput(result);

    // Destroy the mutex and conditional variable
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
































