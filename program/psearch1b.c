#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAXCHAR 1000
#define SIZE 4096
#define SHM_NAME "foobar"

char * findWord(char*, int, char*, char*);

int main(int argc, char *argv[]) {

	char *wordToFind = argv[1];
	int numberOfFiles = atoi(argv[2]);
    char *outputFile = argv[numberOfFiles + 3];

	int fileOut = open(outputFile, O_RDWR | O_CREAT, 0666);
    int r = ftruncate(fileOut, SIZE);
    char *fileInMemory = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fileOut, 0);

    char *updateChar;
    char *lastChar = "";

    for (int i = 1; i <= numberOfFiles; i++) {
   		char* filename = argv[2 + i];
		updateChar = findWord(wordToFind, i, outputFile, filename);
		asprintf(&lastChar, "%s%s", lastChar, updateChar);

    }
    
	strcpy(fileInMemory, lastChar);

    r = munmap(fileInMemory, SIZE);
    r = shm_unlink(SHM_NAME);

	return 0;
}

char * findWord(char *wordToFind, int numberOfFiles, char *outputFile, char *filename) {

   	FILE *fileIn = fopen(filename, "r");

    char str[MAXCHAR];
   	char *oneLine;
 	int lineNumber = 1;
	
	char * returnChar;


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
