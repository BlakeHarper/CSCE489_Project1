/*************************************************************************************
 * myshell - student code for Project 1 of CSCE 489 
 *
 *************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "shellfuncts.h"

int main(int argv, const char *argc[]) {
	(void) argv; // Make compile warnings go away - be sure to delete this line if you use the param
	(void) argc; // Make compile warnings go away - be sure to delete this line if you use the param

	printShellHeader();

	char input[100];
	char *command; //maybe make this bigger to prevent overflow by user

	while(1) {
		char* cwd = getCWD();
		printf("%s> ", cwd);
		fgets(input, sizeof(input), stdin);
		command = strtok(input, "\n"); //remove the newline from the user input before parsing
		command = strtok(input, " "); //need to loop this later to get all args

		if (strcmp(command, "halt") == 0) {
			halt();
		}
		else {
			help();
		}
	}

	exit(0);

	//use getpid() to get and print the process id for this process loop
	//prompt user for next command
	//parse command, terminate immediately if halt()
	//fork() a child process to execute the command
	//print the process id of the child process
	//if the command is to be executed in the foreground then wait for it to complete
}