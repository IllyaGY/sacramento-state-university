#
/*Students: Mansoor Ali, Illya Gordyy
 *Date: 07/12/2024
Class: CSC 139 - 02
Description: This program uses the system calls in order to demonstrate the multiprocessing using fork(), execl()
getppid(), getpid(), wait(), and exit()
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h> 

int main(){
	pid_t pid;
	int status; 	
 

	//Create a duplicate process
	pid = fork();

	//If it is a child process, get its process id and its parent process id
       	if (pid == 0){	
		printf("Parent process ID: %d\n", getppid());
		printf("Child process ID: %d\n", getpid());
	}

	//If it is a parent process, stop it, and run a separate process
	else{
		 
		printf("Stopped the parent process \n");
		wait(&status);
	       	printf("Parent process is up\n");	
		execl("/bin/ls", "ls", "-la", NULL);

	}
	
	//Terminates the child process
	if(pid == 0){
		printf("Terminating the child process\n");
	 	exit(0);
	}
	
		 
	return 0; 

	

}
