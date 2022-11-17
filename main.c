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

// delete function
void delete(char * input) {
    unlink(input);
}

int main() {
    char command[32];
    char *call = malloc(7);
    char filename[32];

    // gets input
    printf("Enter command: ");
    fgets(command, 32, stdin);

    // gets function to call, filename, and the rest
    for (int i = 0; i < 32; i++) {
        if ((command[i] == ' ')) {
            // gets the name of function to call
            strncpy(call, command, i);

            // gets filename
            int count = 0;
            while (count < strlen(command) ) {
                filename[count] = command[(i + 2) + count - 1];
                count++;
            }
            //filename[count] = '\0';
            printf("%s", filename);
        }
        // gives the rest to write
    }

    if (strcmp(command, "read") == 0) {
        // pass to read function
    } else if (strcmp(command, "write") == 0) {
        // pass to write
    } else {
        // pass to delete
        delete(filename);
    }

}
