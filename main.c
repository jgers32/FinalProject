#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 256

// note: strcat for the config file

// read function
void readFile(char * filename) {
    char * string = malloc(BUFFER_SIZE);
    for(int i = 0;; i++) {
        if(filename[i] == '\n') {
            filename[i] = '\0';
            break;
        }
    }
    FILE *stream = fopen(filename, "r");
    int fileLength = 0;
    while (fgets(string, BUFFER_SIZE, stream)) {
        for (int j = 0; string[j] != '\0'; j++) {
            fileLength++;
        }
    }
    printf("Length of Str is %d", fileLength);
    fclose(stream);
}

// write function
// fputs()
void saveFile(char * filename, char * rest){
    FILE *goHere = fopen(filename, "w");
    char * size = malloc(2);
    char * toSave = malloc(32);

    for (int i = 0; i < 32; i++) {
        if (rest[i] == ':') {
            strncpy(size, rest, i);
            size[i] = '\0';
            int count = 0;
            while (count < strlen(rest) - strlen(size) - 1) {
                toSave[count] = rest[(i + 2) + count - 1];
                count++;
            }
            toSave[count] = '\0';
            break;
        }
    }
    fputs(toSave, goHere);
}

// delete function
void delete(char *input) {
    unlink(input);
}

int main() {
    char command[BUFFER_SIZE];
    char *call = malloc(7);
    char *filename = malloc(32);
    char *contents = malloc(32);
    // gets input -- in future this will be from the config file
    printf("Enter command: ");
    fgets(command, BUFFER_SIZE, stdin);
    // gets function to call & filename
    for (int i = 0; i < 32; i++) {
        if (command[i] == ' ') {
            strncpy(call, command, i);
            call[i] = '\0';
            int count = 0;
            while (count < strlen(command) ) {
                filename[count] = command[(i + 2) + count - 1];
                count++;
            }
            command[i] = '\0';
            break;
        }
    }
    // gets the contents for the write function
    for (int i = 0; i < 32; i++) {
        if (filename[i] == ' ') {
            strncpy(contents, filename, i);
            int count = 0;
            while (count < strlen(filename) ) {
                contents[count] = filename[(i + 2) + count - 1];
                count++;
            }
            filename[i] = '\0';
            break;
        }
    }

    if (strcmp(command, "read") == 0) {
        // pass to read function
        readFile(filename);
    } else if (strcmp(command, "write") == 0) {
        // pass to write
        saveFile(filename, contents);
    } else {
        // remove newline character and pass to delete
        for (int i = 0; i < 32; i++) {
            if (filename[i] == '\n') {
                filename[i] = '\0';
                break;
            }
        }
        delete(filename);
    }
}