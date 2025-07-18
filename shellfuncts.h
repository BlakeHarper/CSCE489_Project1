/*************************************************************************************
 * shellfuncts.h - Header file for the basic shell functions
 *
 *************************************************************************************/

// Replace these with something useful
void printShellHeader();
void create(char fileName[]);
void update(char fileName[], int num, char text[]);
void list(char fileName[]);
void dir();
void halt();
void help();
char* getCWD();
bool togglePID(bool printPID);
void delete(char fileName[]);
char *cd(char *path);