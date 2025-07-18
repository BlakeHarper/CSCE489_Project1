/*************************************************************************************
 * myshell - student code for Project 1 of CSCE 489 
 *
 *************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "shellfuncts.h"
#include <sys/types.h>
#include <sys/wait.h>

int main(int argv, const char *argc[]) {
	(void) argv;                      // Make compile warnings go away
	(void) argc;                      // Make compile warnings go away

	printShellHeader();               // Print the shell header

	char input[100];                  // Buffer for user input
	char *command;                    // Pointer to hold the command entered by the user
	static bool printPID = true;      // Default is to print the PID to the console
	char* cwd;                        // Variable to hold the current working directory
	int argLen; 					  // Variable to hold the number of arguments
	char *arg1;						  // Pointer to hold the first argument
	char *arg2; 					  // Pointer to hold the second argument as an string
	char *arg3; 				      // Pointer to hold the third argument as a string
	char *arg4; 				      // Pointer to hold the fourth argument as a string
	char *temp;                       // Temporary pointer for parsing arguments
	bool isBackground = false;        // Flag to check if the command should run in the background
	//Any command can be run with a "&" at the end to run it in the background

	while(1) {
		cwd = getCWD(); //left this inside the loop in case I want to add the ability to change directories later
		if (printPID) {
			printf("\033[45;34mParent PID: %d \033[0;45m\n", getpid());
		}
		printf("%s> ", cwd);
		fgets(input, sizeof(input), stdin);
		command = strtok(input, "\n"); //remove the newline from the user input before parsing
		command = strtok(input, " ");

		if (command == NULL || strcmp(command, "") == 0) {
			continue; // Skip if the command is empty
		}

		temp = strtok(NULL, " ");
		argLen = 0; // Reset argument length
		arg1 = NULL; // Initialize argument pointers to NULL
		arg2 = 0; // Initialize integer argument to 0
		arg3 = NULL; // Initialize string argument to NULL
		arg4 = NULL; // Initialize string argument to NULL
		while (temp != NULL) {
			if (strcmp(temp, "&") == 0) {
				isBackground = true; // Set the flag if the command is to be run in the background
			}

			argLen++;
			if (argLen == 1) {
				arg1 = temp; // Get the first argument
			} 
			else if (argLen == 2) {
				arg2 = temp; // Get the second argument as a string
			} 
			else if (argLen == 3) {
				arg3 = temp; // Get the third argument as a string
			} 
			else if (argLen == 4) {
				arg4 = temp; // Get the fourth argument as a string
			}
			temp = strtok(NULL, " "); // Get the next argument
		}
	
		if (strcmp(command, "halt") == 0) {
			if (argLen > 1) {
				printf("\033[45;31mError: '\033[45;33mhalt\033[45;31m' command does not take any arguments.\033[0;45m\n");
				continue;
			}
			else if (argLen == 1 && strcmp(arg1, "&") == 0) {
				printf("\033[45;31mError: '\033[45;33mhalt\033[45;31m' command cannot be run in the background.\033[0;45m\n");
				continue;
			}
			else if (argLen == 1 && strcmp(arg1, "&") != 0) {
				printf("\033[45;31mError: '\033[45;33mhalt\033[45;31m' command does not take any arguments.\033[0;45m\n");
				continue;
			}
			printf("\033[0m\n"); // Reset background color to default
			halt();
		}
		else if (strcmp(command, "") == 0 || strcmp(command, " ") == 0 || strcmp(command, "\n") == 0) {
			//do nothing if no command entered
			continue;}
		//The following two commands do not fork because they interact directly with variables used in the parent
		else if (strcmp(command, "cd") == 0){
			if (argLen != 1) {
				printf("\033[45;31mError: '\033[45;33mcd\033[45;31m' command requires exactly one argument.\033[0;45m\n");
				continue;
			}
			else if (strcmp(arg1, "&") == 0) {
				printf("\033[45;31mError: '\033[45;33mcd\033[45;31m' command cannot be run in the background.\033[0;45m\n");
				continue;
			}
			cwd = cd(arg1);
		}
		else if (strcmp(command, "togglePID") == 0){
			if (argLen > 0 && strcmp(arg1, "&") == 0) {
				printf("\033[45;31mError: '\033[45;33mtogglePID\033[45;31m' command cannot be run in the background.\033[0;45m\n");
				continue;
			}
			else if (argLen > 1) {
				printf("\033[45;31mError: '\033[45;33mtogglePID\033[45;31m' command does not take more than one argument.\033[0;45m\n");
				continue;
			}
			printPID = togglePID(printPID);
		}
		else {
			pid_t pid = fork(); //create a child process
			if (pid < 0) {
				perror("\033[45;31mError: Child process creation failed\033[0;45m\n");
				exit(EXIT_FAILURE);
			}
			else if (pid == 0) { //child process
				if (isBackground) {
					printf("\n");
				}
				if (printPID) {
					printf("\033[45;34mChild PID: %d \033[0;45m\n", getpid());
				}
				if (strcmp(command, "dir") == 0){
					if (argLen > 0 && strcmp(arg1, "&") != 0) {
						printf("\033[45;31mError: '\033[45;33mdir\033[45;31m' command does not take any arguments.\033[0;45m\n");
						exit(EXIT_FAILURE);
					}
					dir();
				}
				else if (strcmp(command, "create") == 0) {
					if (argLen > 1 && strcmp(arg2, "&") != 0) {
						printf("\033[45;31mError: '\033[45;33mcreate\033[45;31m' command takes only one argument.\033[0;45m\n");
						exit(EXIT_FAILURE);
					}
					create(arg1);
				}
				else if (strcmp(command, "list") == 0) {
					if (argLen > 1 && strcmp(arg2, "&") != 0) {
						printf("\033[45;31mError: '\033[45;33mlist\033[45;31m' command takes only one argument.\033[0;45m\n");
						exit(EXIT_FAILURE);
					}
					list(arg1);
				}
				else if (strcmp(command, "update") == 0) {
					if (argLen < 3 || argLen > 4 || (argLen == 4 && strcmp(arg4, "&") != 0)) {
						printf("\033[45;31mError: '\033[45;33mupdate\033[45;31m' command requires exactly three arguments.\033[0;45m\n");
						exit(EXIT_FAILURE);
					}
					//ensure that the second argument is a valid integer
					int num = atoi(arg2);
					update(arg1, num, arg3);
				}
				else if (strcmp(command, "help") == 0){
					if (argLen > 0 && strcmp(arg1, "&") != 0) {
						printf("\033[45;31mError: '\033[45;33mhelp\033[45;31m' command does not take any arguments.\033[0;45m\n");
						exit(EXIT_FAILURE);
					}
					help();
				}
				else if (strcmp(command, "delete") == 0){
					if (argLen > 1 && strcmp(arg2, "&") != 0) {
						printf("\033[45;31mError: '\033[45;33mdelete\033[45;31m' command takes only one argument.\033[0;45m\n");
						exit(EXIT_FAILURE);
					}
					delete(arg1);
				}
				else {
					printf("\033[45;31mUnknown command: \033[45;33m%s\033[45;31m\033[0;45m\n", command);
					printf("Type '\033[45;33mhelp\033[45;31m' for a list of commands.\n");
					exit(EXIT_FAILURE);
				}

				_exit(0); //exit child process after executing command
			}
			else {
				if (isBackground) {
					printf("\033[45;32mRunning command in the background...\033[0;45m\n");
					isBackground = false; // Reset the background flag
				} else {
					// Wait for the child process to complete if not running in the background
					wait(NULL); //wait for child process to complete
				}
			}
		}
	}
}