/*Names: Mansoor Ali, Illya Gordyy
 * Class: 139-02
 * Date: 07/18/2024
 * Description: This program fixes the error from question 6 and does the number addition properly, allocating the the processes to different threads
 * */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#define FILE_PATH "sequence_number.txt"
#define NUM_PROCESSES 10

//Allocate memory for the mutex lock
pthread_mutex_t lock; 

// Function to perform the sequence number operation 
void * sequence_number_operation(void* arg) {
    FILE *file;

    int sequence_number = 0;

    //Lock
    pthread_mutex_lock(&lock); 


    // Open the file for reading and writing
 
    file = fopen(FILE_PATH, "r+");


    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }



    // Read the sequence number from the file using fscanf()
    fscanf(file, "%d",&sequence_number);


       


    // Print the sequence number and the process PID
    printf("Thread's TID: %lu, Sequence Number: %d\n", pthread_self(), sequence_number);





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

    //Unlock
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL); 

}

// Main function to create processes 
int main() {


 

    //the thread identifier
    pthread_t tid;


    //default attribute for all of the threads
   
    pthread_attr_t attr; 


    //set the default attributes for all of the threads
   
    pthread_attr_init(&attr);
    
   //The array of all of the children pthreads 
    pthread_t tids[10]; 


    //Initialize the lock
    if(pthread_mutex_init(&lock, NULL) != 0){
    	printf("mutex error"); 
    }

    //Create all of the 10 threads    
    for(int i = 0; i < NUM_PROCESSES;i++){
    	pthread_create(&tid, &attr, &sequence_number_operation, NULL);
	tids[i] = tid; 
    }

    //Wait for all of the threads to finish 
    for(int i = 0; i < NUM_PROCESSES;i++)
    	pthread_join(tids[i], NULL); 
    

    //Dealocate the mutex lock
    pthread_mutex_destroy(&lock);  

    return 0;
   
}
