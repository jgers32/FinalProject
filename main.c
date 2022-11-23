#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/unistd.h>

#define BUFFER_SIZE 256

// read function
void readFile(char * filename) {
    char * string = malloc(BUFFER_SIZE);

    for (int i = 0;; i++) {
        if (filename[i] == '\n') {
            filename[i] = '\0';
            break;
        }
    }

    FILE * stream = fopen(filename, "r");
    int fileLength = 0;
    char file[1024];
    file[0] = '\0';

    while (fgets(string, BUFFER_SIZE, stream)) {
        fileLength += strlen(string);
        strcat(file, string);
    }

    printf("%d:%s", fileLength, file);
    fclose(stream);
}

// write function
void saveFile(char * filename, char * rest, char * path) {
    strcat(path, filename);
    FILE * goHere = fopen(path, "w");
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
void deleteFile(char * input) {
    unlink(input);
}

int main() {
    char * string = malloc(BUFFER_SIZE);
    FILE * config = fopen("fs.cfg", "r");
    char save_dir[1024];
    save_dir[0] = '\0';

    while (fgets(string, BUFFER_SIZE, config)) {
        strcat(save_dir, string);
    }

    char * path = malloc(BUFFER_SIZE);
    for (int i = 0; i < 32; i++) {
        if (save_dir[i] == '=' && save_dir[i + 1] == ' ') {
            int count = 0;
            while (count < strlen(save_dir)) {
                path[count] = save_dir[i + 2];
                count++;
                i++;
            }
            for (int k = 0; k < 32; k++) {
                if (path[k] == '\n') {
                    path[k] = '\0';
                    break;
                }
            }
            break;
        }
    }

    char command[BUFFER_SIZE];
    char * call = malloc(7);
    char * filename = malloc(32);
    char * contents = malloc(32);

    // gets input -- in future this will be from the server?
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

    if (strcmp(command, "read") == 0) {
        // pass to read function
        readFile(filename);
    } else if (strcmp(command, "write") == 0) {
        // pass to write function
        saveFile(filename, contents, path);
    } else {
        // remove newline character and pass to delete function
        for (int i = 0; i < 32; i++) {
            if (filename[i] == '\n') {
                filename[i] = '\0';
                break;
            }
        }
        deleteFile(filename);
    }
}
