#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h> 

#define MAXCHAR 1000

int main(int argc, char *argv[]) {

	char *wordToFind = argv[1];
	int numberOfFiles = atoi(argv[2]);
    char *outputFile = argv[numberOfFiles + 3];
	FILE *fileOut = fopen(outputFile, "w");
   
	pid_t  n; 

    for (int i = 1; i <= numberOfFiles; i++) {
   		char* filename = argv[2 + i];

    	n = fork(); 

    	if (n < 0) {
	    	fprintf(stderr, "Fork Failed");
	    	exit(-1);
		} else if (n == 0) {
			char *args[]={"./a", wordToFind, filename, NULL}; 
			execvp(args[0], args);
		} else {
			wait (NULL);

			FILE *fileOutShared = fopen("outputFile.txt", "r");
			char tempStr[MAXCHAR];

			while(fgets(tempStr, MAXCHAR, fileOutShared) != NULL){
					fprintf(fileOut, "%s", tempStr);
			}

			fclose(fileOutShared);
			unlink("outputFile.txt");
		
		}
    }

	fclose(fileOut);

	return 0;
}
