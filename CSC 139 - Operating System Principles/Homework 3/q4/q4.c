/*Name: Mansoor Ali, Illya Gordyy
 * Desciption: The program is intended to create a deadlock between two child processes, and then kill the first child process and unlock the semaphores in order for the second child process to complete  *
 * */

#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>


#define SEMAPHORE1 "/semaphore1"
#define SEMAPHORE2 "/semaphore2"


//Testing our deadlock
void deadlock_test(sem_t *sem1, sem_t *sem2){
	
	//Locking semaphores 
	sem_wait(sem1);
	printf("Locked the first semaphore\n");
	sem_wait(sem2); 
        printf("Locked the second semaphore\n");		
	
	sleep(5);
	//Unlocking semaphores 
	sem_post(sem1);
	sem_post(sem2);
	
        printf("A child process is complete\n");
} 



int main(){

	//Unlinking semaphores just in case
	sem_unlink(SEMAPHORE1);
	sem_unlink(SEMAPHORE2);
	

	//Creating and opennng the two semaphores
	sem_t *sem1 = sem_open(SEMAPHORE1, O_CREAT, 0666, 1);
       	sem_t *sem2 = sem_open(SEMAPHORE2, O_CREAT, 0666, 1);	
	if (sem1 == SEM_FAILED || sem2 == SEM_FAILED) {
        	perror("Error creating semaphore");
        	exit(1);
    	}
	//Forkin 1
	pid_t pid1 = fork();
       	if(pid1 == 0){
		deadlock_test(sem1, sem2); 
		exit(0);
	}	
	//Forking 2
	pid_t pid2 = fork();
	if(pid2 == 0){
		deadlock_test(sem2, sem1);
		exit(0);
	}


	//Waits for the deadlock to occur
	sleep(3);

	//Kills the first child process and opens up both semaphores
	//in order for the second child process
	//to continue and finish
	kill(pid1, 9);
	sem_post(sem2); 
	sem_post(sem1);



	//Waiting for the child processes
	wait(NULL);
	wait(NULL);
        
	sem_close(sem1);
    	sem_close(sem2);
    	sem_unlink(SEMAPHORE1);
    	sem_unlink(SEMAPHORE2);


	
}


