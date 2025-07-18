/*************************************************************************************
 * shellfuncts.c - Contains all functions needed for the basic shell program.
 *
 *************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include <stdbool.h>
#include "shellfuncts.h"
#include <string.h>

/*************************************************************************************
 * printShellHeader - Prints a nice header to boot up my shell because every good
 *					  shell has this functionality.
 *
 *************************************************************************************/

void printShellHeader() {
	printf("\033[45m\n"); // Set background color to magenta
	printf("--------------------------------------------------------------------------------------------------------------\n");
	printf("\n");
	printf("\033[45;33m"); // Set text color to yellow
	printf("              _.-''|''-._\n");
    printf("           .-'     |     `-.\n");
    printf("         .'\\       |       /`.\n");
    printf("       .'   \\      |      /   `.\n");
    printf("       \\     \\     |     /     /\n");
    printf("        `\\    \\    |    /    /'\n");
    printf("          `\\   \\   |   /   /'\n");
    printf("            `\\  \\  |  /  /'\n");
    printf("           _.-`\\ \\ | / /'-._\n");
    printf("          {_____]\\\\|//[_____}\n");
    printf("                  `-'\n\n");
	printf("\033[45;34m"); // Set text color to blue
    printf("[simple]\n");
	printf("\033[45;33m"); // Set text color to yellow
    printf("           88                     88 88  \n");
    printf("           88                     88 88  \n");
    printf("           88                     88 88  \n");
    printf(" ,adPPYba, 88,dPPYba,   ,adPPYba, 88 88  \n");
    printf(" I8[    \"\" 88P'    \"8a a8P_____88 88 88  \n");
    printf("  `\"Y8ba,  88       88 8PP\"\"\"\"\"\"\" 88 88  \n");
    printf(" aa    ]8I 88       88 \"8b,   ,aa 88 88  \n");
    printf(" `\"YbbdP\"' 88       88  `\"Ybbd8\"' 88 88  \n");
    printf("\n");
	printf("\033[0;45m"); // Reset text color to default
	printf("--------------------------------------------------------------------------------------------------------------\n");
    printf("\n");
	printf("\n");
    printf("Type \'help' for a list of commands\n");
	printf("\n");
}

/*************************************************************************************
 * help - Prints a list of commands with a brief description of each.

 *************************************************************************************/

void help() {
	printf("\n");
	printf("\033[45;33mcd \033[45;34m<path>\033[0;45m                             Changes the current working directory to the specified path\n");
    printf("\033[45;33mcreate \033[45;34m<filename> \033[0;45m                    Creates a new file in the current directory\n");
	printf("\033[45;33mdelete \033[45;34m<filename> \033[0;45m                    Deletes a file in the current directory\n");
    printf("\033[45;33mdir \033[0;45m                                  Lists the names of all files in the current directory\n");
    printf("\033[45;33mhalt \033[0;45m                                 Terminates the shell\n");
    printf("\033[45;33mhelp \033[0;45m                                 Lists all commands with a brief description\n");
    printf("\033[45;33mlist \033[45;34m<filename> \033[0;45m                      Displays the contents of a file\n");
	printf("\033[45;33mtogglePID \033[0;45m                            Toggles the PID being printed to the console (default=on)\n");
    printf("\033[45;33mupdate \033[45;34m<filename> \033[45;31m<num> \033[45;32m<text>\033[0;45m        Appends text to the end of a file for the specified number of times\n");
	printf("\n");
}

/*************************************************************************************
 * getCWD - Returns the current working directory as a string.
 *
 *************************************************************************************/

char* getCWD() {
	static char cwd[PATH_MAX];
	getcwd(cwd, sizeof(cwd));
	return cwd;
}

/*************************************************************************************
 * togglePID - Toggles the boolean value of printPID.
 *
 *************************************************************************************/

bool togglePID(bool printPID) {
	return !printPID;
}

/*************************************************************************************
 * create - Creates a file with the specified name in the current working directory.
 *
 *************************************************************************************/

void create(char fileName[]) {
	// Check if the file name is provided
	if (fileName == NULL) {
		printf("\033[45;31mError: No file name provided for create command.\033[0;45m\n");
		exit(EXIT_FAILURE);
	}

	// Check if the file already exists
	FILE *file = fopen(fileName, "r");
	if (file != NULL) {
		printf("\033[45;31mError: File '\033[45;33m%s\033[45;31m' already exists.\033[0;45m\n", fileName);
		fclose(file);
		exit(EXIT_FAILURE);
	}

	FILE *f = fopen(fileName, "w"); // Open the file in write mode
	if (f == NULL) { // If the file cannot be created, print an error message
		perror("\033[45;31mError creating file\033[0;45m");
		exit(EXIT_FAILURE);
	}
	printf("\033[45;32mFile '\033[45;33m%s\033[45;32m' created successfully.\033[0;45m\n", fileName); // Write a success message to the file
}

/*************************************************************************************
 * update - Appends text to a file num times, sleeping for several seconds after each append.
 *
 *************************************************************************************/

void update(char fileName[], int num, char text[]) {
	if (fileName == NULL || strlen(fileName) == 0) {
		printf("\033[45;31mError: No file name provided for update command.\033[0;45m\n");
		exit(EXIT_FAILURE);
	}

	FILE *f = fopen(fileName, "a"); // Open the file in append mode
	for (int i=0; i<num; i++) { // Loop to append text num times
		if (f == NULL) {
			// If the file does not exist, print an error message
			printf("\033[45;31mError: Could not open file '\033[45;33m%s\033[45;31m' for updating.\033[0;45m\n", fileName);
			exit(EXIT_FAILURE);
		}
		else {
			// If the file exists, attempt to append the text
			// and sleep for a few seconds
			if (num <= 0) { //Check if num is a valid positive integer
				printf("\033[45;31mError: Invalid number of times to update.\033[0;45m\n");
				fclose(f);
				exit(EXIT_FAILURE);
			}
			else if (text == NULL) { // Check if text is provided
				printf("\033[45;31mError: No text provided for update command.\033[0;45m\n");
				fclose(f);
				exit(EXIT_FAILURE);
			}
			else if (text[0] != '"') { // Check if text starts with a double quote
				printf("\033[45;31mError: Text must be enclosed in double quotes.\033[0;45m\n");
				fclose(f);
				exit(EXIT_FAILURE);
			}
			else if (text[strlen(text)-1] != '"') { // Check if text ends with a double quote
				printf("\033[45;31mError: Text must be enclosed in double quotes.\033[0;45m\n");
				fclose(f);
				exit(EXIT_FAILURE);
			}

			char *tempText = strdup(text);
			tempText[strlen(tempText)-1] = '\0'; // Remove the trailing double quote
			tempText++; // Move the pointer to the first character after the opening double quote
			// Append the text to the file
			fprintf(f, "%s\n", tempText);
			fflush(f);
			sleep(strlen(text)/5);
		}
	}
	fclose(f); // Close the file after appending

	printf("\033[45;32mFile '\033[45;33m%s\033[45;32m' updated successfully.\033[0;45m\n", fileName); // Print success message
}

/*************************************************************************************
 * list - Displays the contents of a file.
 *
 *************************************************************************************/

void list(char fileName[]) {
	FILE *f = fopen(fileName, "r"); // Open the file in read mode
	if (f==NULL) {
		// If the file does not exist, print an error message
		printf("\033[45;31mError: File '\033[45;33m%s\033[45;31m' does not exist.\033[0;45m\n", fileName);
		fclose(f);
		exit(EXIT_FAILURE);
	}
	else {
		// File exists, construct and execute cat command
		char command[256];
		snprintf(command, sizeof(command), "cat %s", fileName);
		execl("/bin/sh", "sh", "-c", command, NULL);
		fclose(f);
	}
}

/*************************************************************************************
 * dir - Lists the names of all files in the current working directory.
 *
 *************************************************************************************/

void dir() {
	execl("/bin/ls", "ls", NULL); //list the names of all files in cwd
}

/*************************************************************************************
 * halt - Terminates the shell.
 *
 *************************************************************************************/

void halt() {
	exit(0); //terminate shell
}

/*************************************************************************************
 * delete - Deletes a file with the specified name.
 *
 *************************************************************************************/

void delete(char fileName[]) {
	//Delete the file with the given name
	if (remove(fileName) == 0) {
		printf("\033[45;32mFile '\033[45;33m%s\033[45;32m' deleted successfully.\033[0;45m\n", fileName);
	} else {
		//If the file does not exist, print an error message
		perror("\033[45;31mError deleting file\033[0;45m");
	}
}

/*************************************************************************************
 * cd - Changes the current working directory to the specified path.
 *
 *************************************************************************************/

char *cd(char *path) {
	//Change the current working directory to the specified path
	if (chdir(path) != 0) {
		//If the path does not exist, print an error message
		perror("\033[45;31mError changing directory\033[0;45m");
	} else {
		//If the path exists, print the new current working directory
		printf("\033[45;32mChanged directory to: '\033[45;33m%s\033[45;32m'\033[0;45m\n", path);
	}
	return path; //Return the new current working directory
}