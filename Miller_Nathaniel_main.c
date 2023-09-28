/**************************************************************
* Class:  
* Name: Nathaniel Miller	
*
* Description: This is a simple shell that reads input from the user as 
* 	a sting and runs it as a command in the terminal. 
*
**************************************************************/




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define BUFFER_SIZE 123


//int execvp(const char *command, char *const argv[]);
//provide an array of pointers to null-terminated strings 
//that represent the argument list available to the new program

int main(int argc, char* argv[]) {
	char input[BUFFER_SIZE];	//the entire string from user before broken into cmd-line args
	char *argvShell[BUFFER_SIZE];	//mimicking the argv[] array of pointers for the simple shell

	char *prompt = argv[1];		//the second command line argument will be the prompt

	int runningShell = 1;		
	while(runningShell){

	if(prompt != NULL)
		printf("%s ", prompt);
	else
		//default prompt input character if none specified
		printf("> ");	

	
	fgets(input, BUFFER_SIZE, stdin);
	input[strlen(input)-1] = '\0';	//replace \n at end with null
	
	//I learned how and got this from 
	//tutorialspoint strtok() page/chris fietkiewicz's execvp lecture
	argvShell[0] = strtok(input, " ");
	int i=0;
	while(argvShell[i] != NULL){
		argvShell[++i] = strtok(NULL, " ");
	}
	
	////////
	
	//return to bash shell/exit current shell with "exit" command
	//if(!strcmp(argvShell[0], "exit")) {
	//	runningShell=0;
	//	execvp(argvShell[0], argvShell);
	//}

	//create a fork so that the command from input can be 
	//called without ending the current process
	pid_t pid = fork();
	int status;

	if(!strcmp(argvShell[0], "exit")) {
		runningShell=0;
		execvp(argvShell[0], argvShell);
	}
	//condition: once forked and inside the child process 	
	// 	run the commands from parsed input while parent waits.
	if(pid == 0){
		//child
		execvp(argvShell[0], argvShell);
	}
	if(pid > 0) {
		//parent
		wait(NULL);
	}

	//after child terminates print it's PID as well as the exit status
	waitpid(pid, &status, 0);
	if(WEXITSTATUS(status)){
		printf("Child %d exited with %d\n",pid,  WIFEXITED(status));
	}	
    }
}

