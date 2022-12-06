// CSC-400: OS Final Project -- File Server
// Authors: Carrie Eierman, Julia Gersey, and Dana Rabung
// Date: 12/6/2022

#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/unistd.h> 

#define RESOURCE_SERVER_PORT 1032
#define BUF_SIZE 256

#define BUFFER_SIZE 256

// read function
void readFile(char * path, char * response) {
    char * fileContents = malloc(BUFFER_SIZE);

    // removes newline character in path
    for (int i = 0; ; i++) {
        if (path[i] == '\n') {
            path[i] = '\0';
            break;
        }
    }

    FILE * stream = fopen(path, "r");
        
    if (stream != 0) {
    	int fileLength = 0;
    	char file[256];
    	file[0] = '\0';

    	while (fgets(fileContents, BUFFER_SIZE, stream)) {
        	fileLength += strlen(fileContents);
        	strcat(file, fileContents);
   	}

    	printf("%d:%s", fileLength, file);
        snprintf(response, 256, "%d:%s", fileLength, file);
        fclose(stream); 
    }
    else {
	printf("0:"); 
        snprintf(response, 256, "0:");
    }
    
    free(fileContents);
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
    fclose(goHere);
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

// We make this a global so that we can refer to it in our signal handler
int serverSocket;

/*
 We need to make sure we close the connection on signal received, otherwise we have to wait
 for server to timeout.
 */
void closeConnection() {
    printf("\nClosing Connection with file descriptor: %d \n", serverSocket);
    close(serverSocket);
    exit(1);
}

// Create a separate method for

void * processClientRequest(void * request) {
    int connectionToClient = *(int *)request;
    char receiveLine[BUF_SIZE];
    char sendLine[BUF_SIZE];
    
    int bytesReadFromClient = 0;
    // Read the request that the client has
    while ( (bytesReadFromClient = read(connectionToClient, receiveLine, BUF_SIZE)) > 0) {
        // Need to put a NULL string terminator at end
        receiveLine[bytesReadFromClient] = 0;
        
        // Show what client sent
        printf("Received: %s\n", receiveLine);

	// put our code
	//char command[BUFFER_SIZE];
    	char * call = malloc(7);
    	char * filename = malloc(32);
    	char * contents = malloc(32);
    	char * path = malloc(BUFFER_SIZE);
    	char * input = malloc(BUFFER_SIZE);

    	// relative vs. absolute path
    	//FILE * config = fopen("fs.cfg", "r");
    	FILE * config = fopen("/stu/ceierman19/os/final_Project/FinalProject/FinalProject/fs.cfg", "r");

    	char save_dir[1024];
    	save_dir[0] = '\0';

    	while (fgets(input, BUFFER_SIZE, config)) {
        	strcat(save_dir, input);
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
    	//fgets(receiveLine, BUFFER_SIZE, stdin);

    	// gets function to call & filename
    	for (int i = 0; i < 32; i++) {
        	if (receiveLine[i] == ' ') {
            		strncpy(call, receiveLine, i);
            		call[i] = '\0';

            		int count = 0;
            		while (count < strlen(receiveLine)) {
                	filename[count] = receiveLine[(i + 2) + count - 1];
                	count++;
            	}

            	receiveLine[i] = '\0';
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

    	// adds filename to end of path
    	strcat(path, filename);

    	if (strcmp(receiveLine, "read") == 0) {
        	// pass path to read function 
        	readFile(path, &sendLine[0]);
    	} else if (strcmp(receiveLine, "write") == 0) {
        	// pass path to write function
        	writeFile(path, contents);
    	} else {
        	// pass path to delete function
        	deleteFile(path);
    	}

    	fclose(config);
    	free(call);
    	free(filename);
    	free(contents);
    	free(path);
    	free(input);
     
        // Print text out to buffer, and then write it to client (connfd)
        //snprintf(sendLine, sizeof(sendLine), "true");
      
        printf("Sending %s\n", sendLine);
        write(connectionToClient, sendLine, strlen(sendLine));
        
        // Zero out the receive line so we do not get artifacts from before
        bzero(&receiveLine, sizeof(receiveLine));
        close(connectionToClient);
    }
}

int main(int argc, char *argv[]) {
    int connectionToClient, bytesReadFromClient;
  
    // Create a server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddress;
    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family      = AF_INET;
    
    // INADDR_ANY means we will listen to any address
    // htonl and htons converts address/ports to network formats
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port        = htons(RESOURCE_SERVER_PORT);
    
    // Bind to port
    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
        printf("Unable to bind to port just yet, perhaps the connection has to be timed out\n");
        exit(-1);
    }
    
    // Before we listen, register for Ctrl+C being sent so we can close our connection
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = closeConnection;
    sigIntHandler.sa_flags = 0;
    
    sigaction(SIGINT, &sigIntHandler, NULL);
    
    // Listen and queue up to 10 connections
    listen(serverSocket, 10);
    
    while (1) {
        /*
         Accept connection (this is blocking)
         2nd parameter you can specify connection
         3rd parameter is for socket length
         */
        connectionToClient = accept(serverSocket, (struct sockaddr *) NULL, NULL);
        
        // Kick off a thread to process request
        pthread_t someThread;
        pthread_create(&someThread, NULL, processClientRequest, (void *)&connectionToClient);
        
    }
}

