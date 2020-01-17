#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAXCHAR 1000

typedef struct node {
    char *data;
    struct node *right;
} node;

void insert(node **tree, char *val) { 
    node *temp = NULL;
    if (!(*tree)) {
        temp = (node *) malloc(sizeof(node));
        char *val_copy = strdup(val);

        temp->data = val_copy;
        temp->right = NULL;

        *tree = temp;
        return;
    }
    insert(&(*tree)->right, val);
}

void inorder(node *tree, FILE* fp) {
    if (tree) {
        fprintf(fp, "%s\n", tree->data);
        inorder(tree->right, fp);
    }
}

typedef struct {
    char *filename;
    const char *wordToFind;
    node **list;
} fileFeatures;

void *findWord(void *args) {
    fileFeatures *actual_args = args;

    FILE *fileIn = fopen(actual_args->filename, "r");

    char str[MAXCHAR];
    char *oneLine;
    int lineNumber = 1;
    
    char * returnChar;


    if(fileIn == NULL){
        printf("Could not open file %s", actual_args->filename);
    }

    while(fgets(str, MAXCHAR, fileIn) != NULL){
        oneLine = strtok (str, "\n");
        while(oneLine != NULL){
            const char* p = oneLine;
            for(;;){
                p = strstr(p, actual_args->wordToFind);
                if (p == NULL) break;
                if ((p == oneLine) || !isalnum((unsigned char)p[-1])){
                    p += strlen(actual_args->wordToFind);
                    if (!isalnum((unsigned char)*p)){
                        char tmp[MAXCHAR];
                        sprintf(tmp, "%s, %d: %s", actual_args->filename, lineNumber, oneLine);
                        insert((actual_args->list), tmp);
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

    return 0;
}


int main(int argc, char **argv) {
    const char *const wordToFind = argv[1];
    const int numberOfFiles = atoi(argv[2]);

    int err;
    pthread_t tid[numberOfFiles];

    node *inputFile[numberOfFiles];
    fileFeatures *pointerOfFile[numberOfFiles];

    for (int i = 0; i < numberOfFiles; i++) {
        inputFile[i] = NULL;
        pointerOfFile[i] = malloc(sizeof *pointerOfFile[0]);
    }

    for (int i = 0; i < numberOfFiles; i++) {
        pointerOfFile[i]->filename = argv[i + 3];
        pointerOfFile[i]->wordToFind = wordToFind;
        pointerOfFile[i]->list = &inputFile[i];

        err = pthread_create(&(tid[i]), NULL, &findWord, pointerOfFile[i]);

        if (err != 0)
            printf("\nThere is a problem on thread :[%s]", strerror(err));

        pthread_join(tid[i], NULL);
    }

    FILE *fileOut;

    fileOut = fopen(argv[numberOfFiles + 3], "w+");

    for (int i = 0; i < numberOfFiles; i++) 
        inorder(inputFile[i], fileOut);

    fclose(fileOut);

    for (int i = 0; i < numberOfFiles; i++) {
        free(inputFile[i]);
        free(pointerOfFile[i]);
    }
}