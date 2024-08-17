/*Name: Mansoor Ali, Illya Gordyy
 * Desciption: The program is intended to create a zombie child process for 10 seconds, after that it executes the process listing command and destroys the zombie child prorcess 
 *
 *
 * */

#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<time.h>




int main(){
	pid_t pid; 
	pid = fork();
       	int status; 	

	//Exit the child process 
	if(pid == 0){ 
		exit(EXIT_SUCCESS);
	
	}
	//Put the parent process onto sleep mode, execute the ps -l command after 10 seconds, and finally destroy the child process
	if(pid > 0){
		sleep(10); 

		char command[50]; 
		sprintf(command, "ps -l"); 
		system(command); 

		wait(NULL); 
			
	}
}
