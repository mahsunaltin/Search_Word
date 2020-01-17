#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/shm.h> 
#include <sys/wait.h> 

#define SIZE 4096
#define SHM_NAME "foobar"
#define MAXCHAR 1000

char * findWord(char*, char*);

int main(int argc, char *argv[]) {

	char *wordToFind = argv[1];
	char *filename = argv[2];
	int numberOfFiles = atoi(argv[3]);

	char filenamearray[strlen(filename) + 1];
	strcpy(filenamearray, filename);

	char* rest = filenamearray; 

	char *updateChar;
    char *lastChar = "";
	
	char* oneFileInput;
	
  	while ((oneFileInput = strtok_r(rest, ";", &rest))){		
		updateChar = findWord(wordToFind, oneFileInput);
		asprintf(&lastChar, "%s%s", lastChar, updateChar);
  	}

	key_t shm_key = 6166529;
	const int shm_size = 1024;

	int shm_id;
	char* shmaddr, * ptr;
	int next[1];

	shm_id = shmget (shm_key, shm_size, IPC_CREAT | S_IRUSR | S_IWUSR);
	shmaddr = (char*) shmat (shm_id, 0, 0);
	
	// Write
	ptr = shmaddr + sizeof (next);
	next[0] = sprintf (ptr, "%s", lastChar) + 1;	
	memcpy(shmaddr, &next, sizeof (next));

	shmdt (shmaddr);
	shmctl (shm_id, IPC_RMID, 0);

	return 0;
}

char * findWord(char *wordToFind, char *filename) {
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
