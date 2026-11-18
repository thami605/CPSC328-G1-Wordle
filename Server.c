/********************************************************************** 
 * Author/Editor:  Tre Hamilton (CS), Patrick Walter (CS)                                                                                                               
 * Creation Date:  November 12, 2026
 * Last Modified:  November 13, 2026                                                    
 * Due Date:       November 23, 2026                                                   
 * Course:         CPSC 328                                                              
 * Professor Name: Prof. Walther                                                         
 * Assignment:     Group Project: Wordle Client/Server Application
 * Filename:       Server.c
 * Purpose:        Server side application for the Wordle game. This 
 *                 file listens for clients, sends a random word to the
 *                 client, and handles multiple clients.
 * Modifications:  Listed at the bottom of the file. 
 **********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>

#define MAX_WORDS 1000
#define WORD_LENGTH 6 //5 letters + '/0' 

char wordlist[MAX_WORDS][WORD_LENGTH];
int wordCount = 0;

int numThreads; //global number of threads since we are not tracking them in a structure currently
int passiveTCP(const int port, int qlen); //passive TCP socket method
void *threadWorker(void *arg); //method for thread
void loadWords(const char *filename); //method for loading words from file
int verifyPort(int port); //port verificatino method
int errexit(const char *format, ...); //Method for handling errors

/*
 *    Method: main
 *    Purpose: Driver method for running server
 *    Parameters: int argc - number of CLAs, char *argv[] - array of CLAs
 */
int main(int argc, char *argv[])
{
    struct sockaddr_in clientaddr;
    unsigned int    clientlen;
    int psock, asock, tid;
    int port = 65000;
    int qlen = 10;
    numThreads = 0;

    // Load words from file
    loadWords("wordBank.txt");

    //switch based on number of arguments
    switch (argc)
    {
        case 1: //no arguments
            break;
        case 2: //specified port
            port = atoi(argv[1]);
            if (!verifyPort(port)) //verify port is valid
            {
                exit(1);
            }
            break;
        default:
            perror("usage: serverprogram [port]"); //print proper usage of program
    }

    psock = passiveTCP(port, qlen); //create passive socket
    for(;;) //loop indefinitely
    {
        pthread_t child; //create child pthread
        clientlen = sizeof(clientaddr); //set size of the length
        asock = accept(psock, (struct sockaddr *)&clientaddr, &clientlen); //create active socket using the information
        if (asock < 0) //check for error
        {
            if (errno == EINTR)
                continue;
            errexit("Error accepting incoming socket: ", strerror(errno));

        }

        //create thread and pass the active socket as a parameter
        tid = pthread_create(&child, NULL, threadWorker, (void *)&asock);
        if (tid < 0) //check for error
        {
            perror("failure creating child thread");
        }
        pthread_detach(child); //detach the thread so when it closes the memory is automatically freed
        numThreads++; //increase number of threads
        printf("Client connected. There are currently %d thread(s).\n", numThreads); //print number of connected threads
    }
}

/*
 *    Method: threadWorker
 *    Purpose: method for threads
 *    Parameters: void *arg - given arguments
 *    Return: none
 */
void *threadWorker(void *arg)
{
    char  buf[6]; // send and recieve buffer
    int *socket = (int *)arg; //convert arg to proper type
    int num; //int for random number to grab random word
    srandom((unsigned int)time(NULL) ^ (unsigned int)pthread_self()); //set random seed

    //write initial hello to server
    if (write(*socket, "HELLO", sizeof("HELLO")) < 0)
    {
        errexit("Error writing HELLO to client\n", strerror(errno));
    }

    //loop infinitely while client remains connected
    while (1)
    {
        //get psuedorandom number
        num = random() % 10;

        if (read(*socket, buf, sizeof(buf)) < 1) //read word from client, client closes on 0 and errors on -1
        {
            printf("Client disconnected. Closing thread %d.\n", pthread_self());
            numThreads--;
            break; //break out of loop since client disconnected
        }
        if (strcmp(buf, "WORD") == 0) //check to see if the client sent WORD
        {
            printf("Recieved %s from client.\n", buf); //print to console
            if (write(*socket, wordlist[num], sizeof(wordlist[num])) == 0) //send random word to client
            {
                errexit("Error sending word to client", strerror(errno)); //handle error
                break;
            }
        }
        else if (strcmp(buf, "QUIT") == 0) //check to see if the client sent QUIT
        {
            printf("Recieved %s from client. Closing thread %d.\n", buf, pthread_self()); //acknowledge
            numThreads--; //decrement
            break; //break out of thread
        }
        else
        {
            printf("Invalid response"); //we shouldn't get this since the client sends specific messages, but this is here in case
            break;
        }

    }
    pthread_exit(NULL); //close thread
}

/*
 *    Method: loadWords
 *    Purpose: Load words from our wordbank file into the wordlist array
 *    Parameters: const char *filename - name of the file containing words
 *    Return: void
 */
void loadWords(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error: Unable to open word file");
        exit(EXIT_FAILURE);
    }

    char buffer[WORD_LENGTH];
    while (fgets(buffer, sizeof(buffer), file) && wordCount < MAX_WORDS) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
        if (strlen(buffer) == 5) { // Ensure the word is exactly 5 letters
            strcpy(wordlist[wordCount], buffer);
            wordCount++;
        }
    }

    fclose(file);

    if (wordCount == 0) {
        fprintf(stderr, "Error: No valid words found in the file.\n");
        exit(EXIT_FAILURE);
    }

    printf("Loaded %d words from %s.\n", wordCount, filename);
}

/********************************************************************** 
 *
 * MODIFICATION HISTORY
 *
 **********************************************************************/
/********************************************************************** 
 * Author:     Tre Hamilton                                                   
 * Date:       November 12, 2026
 *  Update:     Created file, wrote comments based off of Design Document
 * Time Spent: 10 minutes
 *
 **********************************************************************/
/********************************************************************** 
 * Author:     Patrick Walter                                                  
 * Date:       November 12, 2026
 * Update:     Connected server program to library to create passive 
 *             sockets, allowed for user defined port, put active socket 
 *             connection in a thread, and tested basic communication 
 *             with client (HELLO, WORD, QUIT). Also sent intial test 
 *             word.
 * Time Spent: ~4 hours
 *
 **********************************************************************/
/********************************************************************** 
 * Author:     Patrick Walter                                                  
 * Date:       November 12, 2026
 * Update:     Added array for word bank and let server send client a 
 *             random word.
 * Time Spent: ~30 minutes
 *
 **********************************************************************/
/********************************************************************** 
 * Author:     Patrick Walter                                                  
 * Date:       November 15, 2026
 * Update:     Added port verification and numthreads tracker.
 * Time Spent: ~20 minutes
 *
 **********************************************************************/
/********************************************************************** 
 * Author:     Patrick Walter                                                  
 * Date:       November 17, 2026
 * Update:     Updated error handlers, fixed error that caused server 
 *             to crash if client force closed their session, and added 
 *             documentation
 * Time Spent: ~1 hour
 *
 **********************************************************************/
/********************************************************************** 
 * Author:     Tre Hamilton
 * Date:       November 17, 2026
 * Update:     Universalized Documentation for all files
 * Time Spent: ~15 minutes
 *
 **********************************************************************/
/********************************************************************** 
 * Author:     Tre Hamilton
 * Date:       November 17, 2026
 * Update:     Added implementation for loadWords to read from 
 *             wordBank.txt and store in wordlist array
 * Time Spent: ~45 minutes
 *
 **********************************************************************/
