#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define BUFFER_SIZE 256
// read function
// sample output:   32:contents of file
// fgets() and keep track of how many bytes you read
// write function
// delete function
void delete(char * input) {
    unlink(input);
}

int main() {
    // gets input
    printf("enter command: ");
    char command[32];
    char *call = malloc(7);
    fgets(command, 32, stdin);
    // gets function, filename, and the rest
    for (int i = 0; i < 32; i++) {
        if ((command[i] == ' ')) {
            // gets the name of function to call
            strncpy(call, command, i);
            char filename[32];
            int count = 0;
            while (count < strlen(command) ) {
                filename[count] = command[(i + 2) + count - 1];
                count++;
            }
            //filename[count] = '\0';
            printf("%s", filename);
            delete(filename);
        } // gets the filename
        // gives the rest to write
    }

    //printf("%s\n", program);
    if (strcmp(command, "read") == 0) {
        // pass to read function
    } else if (strcmp(command, "write") == 0) {
        // pass to write
    } else {
        // pass to delete
    }

}
