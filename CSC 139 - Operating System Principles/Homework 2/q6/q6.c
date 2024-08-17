#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>


#define FILE_PATH "sequence_number.txt"
#define NUM_PROCESSES 10

// Function to perform the sequence number operation 
void sequence_number_operation() {
    FILE *file;

    int sequence_number = 0;


    // Open the file for reading and writing
    file = fopen(FILE_PATH, "r+");


    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }



    // Read the sequence number from the file using fscanf()
    fscanf(file, "%d",&sequence_number);


       


    // Print the sequence number and the process PID
    printf("Process PID: %d, Sequence Number: %d\n", getpid(), sequence_number);





    // Increment the sequence number
    sequence_number++;

    // Move the file pointer to the beginning of the file
    rewind(file);


    fclose(file);
    


    file = fopen(FILE_PATH,"wb");



    // Write the incremented sequence number back to the file using fprintf()
    fprintf(file, "%d", sequence_number);



    fflush(file);

    // Close the file
    fclose(file);


}

// Main function to create processes 
int main() {

    // Create 10 child processes and try accessing the sequece number and perform sequence number operation

    pid_t pids[NUM_PROCESSES];
    

    for(int i = 0; i < NUM_PROCESSES;i++ ) {
        pid_t pid = fork();
        pids[i] = pid;
        if (pid == 0) {
            sequence_number_operation(); 
	    exit(0);
        }

    }
    


    // Parent process waits for all child processes to finish
    for (int i = 0; i < NUM_PROCESSES; i++)
         waitpid(pids[i], NULL, 0);
   
    return 0;
   
}
