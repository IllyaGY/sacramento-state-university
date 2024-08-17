/*Names: Mansoor Ali, Illya Gordyy
 * Class: 139-02
 * Date: 07/18/2024
 * Description: This program fixes the error from question 6 and does the number addition properly, using 10 different processes
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <semaphore.h>
#include <sys/mman.h>

#define FILE_PATH "sequence_number.txt"
#define NUM_PROCESSES 10

#define SHARED_MEM_NAME "/shared_buffer"
#define SEMAPHORE_NAME  "/semaphore" 

// Function to perform the sequence number operation 
void sequence_number_operation(int *sequence_number, sem_t *sem) {
    
    //Lock the semaphore
    sem_wait(sem);
    
    FILE *file;
    
    // Open the file for reading and writing    
    file = fopen(FILE_PATH, "r+");
    
    if (file == NULL) {
        perror("Error opening file");
        sem_post(sem);
        exit(1);
    }
    

    // Read the sequence number from the file
    if (fscanf(file, "%d", sequence_number) != 1) {
        perror("Error reading sequence number");
        fclose(file);
        exit(1);
    }

    // Print the sequence number and the process PID
    printf("Process PID: %d, Sequence Number: %d\n", getpid(), *sequence_number);

    // Increment the sequence number
    (*sequence_number)++;

    // Move the file pointer to the beginning of the file
    rewind(file);

    // Write the incremented sequence number back to the file
    fprintf(file, "%d", *sequence_number);
    
    
    //Flush the buffer
    fflush(file);

      
     
    // Close the file
    fclose(file);
    
    // Unlock the semaphore
    sem_post(sem);
}

// Main function to create processes 
int main() {


    shm_unlink(SHARED_MEM_NAME);
    sem_unlink(SEMAPHORE_NAME);
    
    
    int shm_fd = shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, 0666); //Creating a shared memory buffer, with read and write permission


    ftruncate(shm_fd, sizeof(int)); 
    
    int *sequence_number; 
    
    //Map the shared memory, with read and write rights, while also sharing the buffer along all of the processes
    sequence_number = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);  
    if (sequence_number == MAP_FAILED) {
        perror("Error mapping shared memory");
        exit(1);
    }
    
    *sequence_number = 0; 
    
    //Creating a semaphore, with reading and writing rights
    sem_t *semaphore = sem_open(SEMAPHORE_NAME, O_CREAT, 0666, 1);
    if (semaphore == SEM_FAILED) {
        perror("Error creating semaphore");
        exit(1);
    }

    pid_t pids[NUM_PROCESSES];

    // Create 10 child processes
    for (int i = 0; i < NUM_PROCESSES; i++) {
        int pid = fork();
        if (pid == 0) {
            // Child process
            sequence_number_operation(sequence_number, semaphore);
            exit(0);
        } else if (pid > 0) {
            // Parent process
            pids[i] = pid;
        } else {
            perror("Error forking");
            exit(1);
        }
    }

    // Wait for all child processes to complete
    for (int i = 0; i < NUM_PROCESSES; i++) {
        waitpid(pids[i], NULL, 0);
    }
    
    sem_close(semaphore);
    sem_unlink(SEMAPHORE_NAME); 
    
    munmap(sequence_number, sizeof(int)); 
    shm_unlink(SHARED_MEM_NAME);

     
    return 0;
}