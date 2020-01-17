#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
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


	// START
	key_t shm_key = 6166529;
	const int shm_size = 1024;

	int shm_id;
	char* shmaddr, *ptr;
	char* shared_memory[1];
	int *p;

	shm_id = shmget (shm_key, shm_size, IPC_CREAT | S_IRUSR | S_IWUSR);
	shmaddr = (char*) shmat (shm_id, 0, 0);
   
	pid_t  n;

    	n = fork(); 

    	if (n < 0) {
	    	fprintf(stderr, "Fork Failed");
	    	exit(-1);
		} else if (n == 0) {
			char* listOfFiles = "";
			char* filename = "";
			for (int i = 1; i <= numberOfFiles; i++) {
   				filename = argv[2 + i];
   				if(i==1)
   					listOfFiles = filename;
   				else	
					asprintf(&listOfFiles, "%s;%s;", listOfFiles, filename);
   			}
  	 		char charNumberOfFiles[20];
  	 		sprintf(charNumberOfFiles, "%d", numberOfFiles);
			char *args[]={"./a", wordToFind, listOfFiles, charNumberOfFiles, NULL}; 
			execvp(args[0], args);
		} else {
			wait (NULL);
			p = (int *)shmaddr;
			ptr = shmaddr + sizeof (int) * 2;
			shared_memory[0] = "";
			shared_memory[0] = ptr;
			
			fprintf(fileOut, "%.*s%s", 4, argv[3], shared_memory[0]);

		}
    
	shmdt (shmaddr);
	fclose(fileOut);

	return 0;
}
