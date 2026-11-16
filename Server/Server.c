/**********************************************************************/
/* Author/Editor:  Tre Hamilton (CS), Patrick Walter (CS)                                                                                                               
/* Creation Date:  November 12, 2026
/* Last Modified:  November 13, 2026                                                    
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

char wordlist[10][6] = {"SWORD", "APPLE", "GHAST", "BROWN", "MUAVE", "MOVIE", "GROUP", "CHANT", "SMILE", "SHELL"};

int numThreads;
int passiveTCP(const int port, int qlen);
void *threadWorker(void *arg);
int verifyPort(int port);

int main(int argc, char *argv[])
{
    struct sockaddr_in clientaddr;
    unsigned int    clientlen;
    int psock, asock, tid;
    int port = 65000;
    int qlen = 10;
    numThreads = 0;
    switch (argc)
    {
        case 1:
            break;
        case 2:
            port = atoi(argv[1]);
            if (!verifyPort(port));
            exit(1);
        break;
        default:
            perror("usage: serverprogram [port]");
    }

    psock = passiveTCP(port, qlen);
    for(;;)
    {
        pthread_t child;
        clientlen = sizeof(clientaddr);
        asock = accept(psock, (struct sockaddr *)&clientaddr, &clientlen);
        if (psock < 0)
        {
            if (errno == EINTR)
                continue;
            perror("error on accept");
        }
        tid = pthread_create(&child, NULL, threadWorker, (void *)&asock);
        if (tid < 0)
        {
            perror("failure creating child thread");
        }
        pthread_detach(child);
        numThreads++;
        printf("Client connected. There are currently %d thread(s).\n", numThreads);
    }
}

void *threadWorker(void *arg)
{
    char  buf[6];
    char  word[6];
    int *socket = (int *)arg;
    int num;
    srandom(pthread_self());
    if (write(*socket, "HELLO", sizeof("HELLO")) < 0)
    {
        perror("error writing hello");
    }
    while (1)
    {
        num = random() % 10;
        if (read(*socket, buf, sizeof(buf)) < 0)
        {
            perror("Error waiting for wordle");
            break;
        }
        if (strcmp(buf, "WORD") == 0)
        {
            printf("Recieved %s from client.\n", buf);
            if (write(*socket, wordlist[num], sizeof(wordlist[num])) == 0)
            {
                perror("error sending SWORD");
                break;
            }
        }
        else if (strcmp(buf, "QUIT") == 0)
        {
            printf("Recieved %s from client.\n", buf);
            break;
        }
        else
        {
            printf("Recieved %s from client.\n", buf);
            perror("Invalid response");
            break;
        }

    }
    numThreads--;
    printf("Client disconnected. %d threads remain.", numThreads);
    pthread_exit(NULL);
}

/**********************************************************************/
/*
/* MODIFICATION HISTORY
/*
/**********************************************************************/
/* Author:     Tre Hamilton                                                   
/* Date:       November 12, 2026
/* Update:     Created file, wrote comments based off of Design Document
/* Time Spent: 10 minutes
/*
/**********************************************************************/
/* Author:     Patrick Walter                                                  
/* Date:       November 13, 2026
/* Update:     Integrated his version of the server code.
/* Time Spent: 
/*
/**********************************************************************/