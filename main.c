// free memory to prevent memory leaks
// do we need the newline character for the read function?
// edit the serverWithPopupThread.c file on the server

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/unistd.h> // needed to prevent warning on server

#define BUFFER_SIZE 256

// read function
void readFile(char * path) {
    char * string = malloc(BUFFER_SIZE);

    // removes newline character in path
    for (int i = 0; ; i++) {
        if (path[i] == '\n') {
            path[i] = '\0';
            break;
        }
    }

    FILE * stream = fopen(path, "r");
    int fileLength = 0;
    char file[1024];
    file[0] = '\0';

    while (fgets(string, BUFFER_SIZE, stream)) {
        fileLength += strlen(string);
        strcat(file, string);
    }

    printf("%d:%s", fileLength, file);
    fclose(stream);
    free(string);
}

// write function
void writeFile(char * path, char * rest) {
    FILE * goHere = fopen(path, "w");
    char * toWrite = malloc(32);

    int toCopyFlag = 0;
    int n = 0;
    for (int i = 0; i < 32; i++) {
        if (rest[i] == ':') {
            toCopyFlag = 1;
            continue;
        }
        if (toCopyFlag) {
            if (rest[i] == '\n') {
                toWrite[n] = '\0';
            }
            else {
                toWrite[n] = rest[i];
                n++;
            }
        }
    }
    fputs(toWrite, goHere);
    free(toWrite);
}

// delete function
void deleteFile(char * path) {
    // removes newline character in path
    for (int i = 0; ; i++) {
        if (path[i] == '\n') {
            path[i] = '\0';
            break;
        }
    }
    unlink(path);
}

int main() {
    char command[BUFFER_SIZE];
    char * call = malloc(7);
    char * filename = malloc(32);
    char * contents = malloc(32);
    char * path = malloc(BUFFER_SIZE);
    char * string = malloc(BUFFER_SIZE);
    FILE * config = fopen("/stu/ceierman19/os/final_Project/FinalProject/FinalProject/fs.cfg", "r");
    char save_dir[1024];

    save_dir[0] = '\0';

    while (fgets(string, BUFFER_SIZE, config)) {
        strcat(save_dir, string);
    }

    // removes newline character in save_dir so path is correct
    for (int i = 0; ; i++) {
        if (save_dir[i] == '\n') {
            save_dir[i] = '\0';
            break;
        }
    }

    for (int i = 0; i < 32; i++) {
        if (save_dir[i] == '=' && save_dir[i + 1] == ' ') {
            int count = 0;
            while (count < strlen(save_dir)) {
                path[count] = save_dir[i + 2];
                count++;
                i++;
            }
            break;
        }
    }

    // gets input -- in the future this will be from the server
    printf("Enter command: ");
    fgets(command, BUFFER_SIZE, stdin);

    // gets function to call & filename
    for (int i = 0; i < 32; i++) {
        if (command[i] == ' ') {
            strncpy(call, command, i);
            call[i] = '\0';

            int count = 0;
            while (count < strlen(command)) {
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
            while (count < strlen(filename)) {
                contents[count] = filename[(i + 2) + count - 1];
                count++;
            }

            filename[i] = '\0';
            break;
        }
    }

    strcat(path, filename); // adds filename to end of path

    if (strcmp(command, "read") == 0) {
        // pass path to read function
        readFile(path);
    } else if (strcmp(command, "write") == 0) {
        // pass path to write function
        writeFile(path, contents);
    } else {
        // pass path to delete function
        deleteFile(path);
    }

    //free(call);
    //free(filename);
    //free(contents);
    //free(path);
    //free(string);
}
