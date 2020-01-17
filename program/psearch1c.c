#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/wait.h> 

#define MAXCHAR 1000

char * findWord(char*, int, char*, char*);

int fd[2];

int main(int argc, char *argv[]) {

	char *wordToFind = argv[1];
	int numberOfFiles = atoi(argv[2]);
    char *outputFile = argv[numberOfFiles + 3];
	FILE *fileOut = fopen(outputFile, "w");

	pipe(fd);
	//fd[0]; //-> for using read end
	//fd[1]; //-> for using write end

	pid_t  n; 
    
	char *updateChar;
    char *lastChar = "";
    
    char concat_str[100]; 
    n = fork(); 
    
	if (n < 0) {
    	fprintf(stderr, "Fork Failed");
    	exit(-1);
	} else if (n == 0) {
		
		for (int i = 1; i <= numberOfFiles; i++) {
			char* filename = argv[2 + i];
			updateChar = findWord(wordToFind, i, outputFile, filename);
			asprintf(&lastChar, "%s%s", lastChar, updateChar);
		}

		write(fd[1], lastChar, strlen(lastChar)+1); 

	} else {
		wait (NULL);

		read(fd[0], concat_str, 100);
		fprintf(fileOut, "%s", concat_str);
		
	}
    
	close(fd[0]);
	close(fd[1]);

    fclose(fileOut);
	return 0;
}


char * findWord(char *wordToFind, int numberOfFiles, char *outputFile, char *filename) {	

   	FILE *fileIn = fopen(filename, "r");

    char * returnChar;

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
						asprintf(&returnChar, "%s, %d: %s\n", filename, lineNumber, oneLine);
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
	return returnChar;


}
