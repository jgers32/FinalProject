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

// if file exists
    // FILE *inputStream = fopen(filename, "r");
    // save the contents of the file to a string
    // count number of bytes
    // return number: contents
// if file doesn't exist
    // return 0:


// write function


// delete function

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
        } // gets the filename
        // gives the rest to write
    }

    printf("%s", call);
    //printf("%s\n", program);

    if (strcmp(call, "read") == 0) {
        //read(filename);

    } else if (strcmp(call, "write") == 0) {
        // pass to write

    } else {
        // pass to delete
    }

}

