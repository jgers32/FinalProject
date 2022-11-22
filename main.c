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
// sample output:   32:contents of file
// fgets() and keep track of how many bytes you read

// write function
// fputs()
void saveFile(char * filename, char * rest){
    char * size = malloc(2);
    char * toSave = malloc(32);

    for (int i = 0; i < 32; i++) {
        if (rest[i] == ':') {
            strncpy(size, rest, i);
            size[i] = '\0';
            int count = 0;
            while (count < strlen(rest) - 3) {
                toSave[count] = rest[(i + 2) + count - 1];
                count++;
            }
            toSave[count] = '\0';
            break;
        }
    }

    printf("size: %s\n", size);
    printf("contents: %s\n", toSave);

}

// delete function
void delete(char *input) {
    unlink(input);
}

int main() {
    char command[32];
    char *call = malloc(7);
    char filename[32];
    char contents[32];

    // gets input -- in future this will be from the config file
    printf("Enter command: ");
    fgets(command, 32, stdin);

    // gets function to call & filename
    for (int i = 0; i < 32; i++) {
        if (command[i] == ' ') {
            strncpy(call, command, i);
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

    printf("function: %s\n", call);
    printf("filename: %s\n", filename);
    printf("contents: %s\n", contents);

    if (strcmp(command, "read") == 0) {
        // pass to read function
        printf("Read function...");
    } else if (strcmp(command, "write") == 0) {
        // pass to write
        printf("Write function...\n");
        saveFile(&filename, &contents);
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
