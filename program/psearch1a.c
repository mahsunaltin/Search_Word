#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h> 

#define MAXCHAR 1000

void findWord(char*, int, char*, char*);

int main(int argc, char *argv[]) {

	char *wordToFind = argv[1];
	int numberOfFiles = atoi(argv[2]);
    char *outputFile = argv[numberOfFiles + 3];
	FILE *fileOut = fopen(outputFile, "w");

	pid_t  n; 

    n = fork(); 
    
    for (int i = 1; i <= numberOfFiles; i++) {
   		char* filename = argv[2 + i];

		if (n < 0) {
	    	fprintf(stderr, "Fork Failed");
	    	exit(-1);
		} else if (n == 0) { /* child process*/
			findWord(wordToFind, i, outputFile, filename);
		} else { /* parent process */
			wait (NULL);

			char readTempsNumber[20];
	  		sprintf(readTempsNumber, "%d", i);

			char *outputFile_P = strdup(outputFile);
			char *newChar = strcat(strtok(outputFile_P, "."), strcat(readTempsNumber, ".txt"));
	  	
			FILE *fileIn = fopen(newChar, "r");
			
			char tempStr[MAXCHAR];

			while(fgets(tempStr, MAXCHAR, fileIn) != NULL){
				fprintf(fileOut, "%s", tempStr);
			}

			fclose(fileIn);
			unlink(newChar);
			
		}
    }

    fclose(fileOut);
	return 0;
}


void findWord(char *wordToFind, int numberOfFiles, char *outputFile, char *filename) {

	char stringNum[20];
  	sprintf(stringNum, "%d", numberOfFiles);	
  	char *outputFile_P = strdup(outputFile);

   	FILE *fileIn = fopen(filename, "r");
 	FILE *fileOut = fopen(strcat(strtok(outputFile_P, "."), strcat(stringNum, ".txt")), "w");	

    char str[MAXCHAR];
   	char *oneLine;
 	int lineNumber = 1;
    
    if(fileIn == NULL){
        printf("Could not open file %s", filename);
    }

    while(fgets(str, MAXCHAR, fileIn) != NULL){
        oneLine = strtok (str, "\n");
        while(oneLine != NULL){
        	const char* p = oneLine;
			for(;;){
				p = strstr(p, wordToFind);
				if (p == NULL) break;
				if ((p == oneLine) || !isalnum((unsigned char)p[-1])){
				   	p += strlen(wordToFind);
				   	if (!isalnum((unsigned char)*p)){
						if (fileOut == NULL){
						    printf("Error opening file!\n");
						    exit(1);
						}
						fprintf(fileOut, "%s, %d: %s\n", filename, lineNumber, oneLine);
						break;
				   	}
				}
				p+=1;
			}
  			oneLine = strtok (str, "\n");
	  		break;
	  	}
        lineNumber++;
    }

    fclose(fileIn);
	fclose(fileOut);
	free(outputFile_P);
}
