/**********************************************************************/
/* Author/Editor:  Tre Hamilton (CS)                                                                                                               
/* Creation Date:  November 12, 2026
/* Last Modified:  November 12, 2026                                                    
/* Due Date:       November 23, 2026                                                   
/* Course:         CPSC 328                                                              
/* Professor Name: Prof. Walther                                                         
/* Assignment:     Group Project: Wordle Client/Server Application
/* Filename:       Server.c
/* Purpose:        Server side application for the Wordle game. This 
/*                 file listens for clients, sends a random word to the
/*                 client, and handles multiple clients.
/* Modifications:  Listed at the bottom of the file. 
/**********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 5
#define WORD_LENGTH 5
#define MAX_WORDS 100

int main(int argc, char **argv) {
    //User can specify a port number as a command line argument, otherwise default to 8080
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[256];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Error: Unable to create socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    #define PORT 8080 // Define a valid port number
        server_addr.sin_port = htons(PORT);

    // Bind socket to the specified port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error: Unable to bind socket");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Error: Unable to listen on socket");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    
    // Accept a client connection on a forked process

    /* -- Fork process -- */
    // Send "Hello" to the client

    // Listen for "READY" message from the client

    // Select a random word from the word list
    // Send the random word to the client

    // Listen for "READY"

    // Listen for "BYE" message from the client
    // Close the client connection

    // Close the server socket

    return 0;
}