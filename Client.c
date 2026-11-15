/**********************************************************************/
/* Author/Editor:  Tre Hamilton (CS), Patrick Walter (CS)                                                                                                               
/* Creation Date:  November 10, 2026
/* Last Modified:  November 13, 2026                                                    
/* Due Date:       November 23, 2026                                                   
/* Course:         CPSC 328                                                              
/* Professor Name: Prof. Walther                                                         
/* Assignment:     Group Project: Wordle Client/Server Application
/* Filename:       Client.c
/* Purpose:        Client side application for the Wordle game.
/*                 This file connects to the server, receives 
/*                 a random word, and allows the user to play, then 
/*                 replay if necessary.
/* Modifications:  Listed at the bottom of the file. 
/**********************************************************************/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>

int connectTCP(const char *host, const int port);
int testConnection(const char *host, int port);
bool wordle(char word[5]);

int main(int argc, char *argv[])
{
   int port = 65000;
   char *host;
   switch(argc)
   {
      case 3:
            port = atoi(argv[2]);
      case 2:
            host = argv[1];
            break;
      default:
            fprintf(stderr, "usage: TCPecho [host [port]]\n");
            exit(1);
   }
   testConnection(host, port);
   exit(0);
}

int testConnection(const char *host, int port)
{
   char buf[6];
   int s;
   bool replay = false;
   s = connectTCP(host, port);
   if (read(s, buf, sizeof(buf)) > 0)
   {
      printf("Read %s from server\n", buf);
   }
   else
   {
      perror("Error reading TCP: ");
   }
   do
   {
      if (write(s, "WORD", sizeof("WORD")) < 0)
      {
            perror("Error sending WORD to server");
      }
      printf("Sent WORD to server\n");
      if (read(s, buf, sizeof(buf)) < 0)
      {
            perror("Error reading word from server");
      }
      else
      {
            printf("Read %s from server.\n", buf);
      }
      buf[strlen(buf)] = '\0';
      replay = wordle(buf);
   }
   while(replay == true);
   if (write(s, "QUIT", sizeof("QUIT")) < 0)
   {
      perror("Error sending WORD to server");
   }
   return 0;
}

bool wordle(char word[])
{
   char guess[5];
   char result[6];
   printf("Wordle Game Started\n");
   for (int w = 0; w < 6; w++)
   {
      printf("Enter Guess: ");
      scanf(" %s", guess);
      {
            for (int i = 0; i < 5; i++)
            {
               bool found = false;
               for (int j = 0; j < 5; j++)
               {
                  if (guess[i] == word[j])
                  {
                        found = true;
                        if (i == j)
                        {
                           result[i] = word[i];
                           break;
                        }
                        else
                        {
                           result[i] = '#';
                           break;
                        }
                  }
               }
               if (!found)
                  result[i] = '/';
            }
      }

      if (strcmp(guess, word) == 0)
            break;
      printf("%s\n%s\n%s\n%d", result, word, guess, strlen(result));
   }
   return false;
}
 

/**********************************************************************/
/*
/* MODIFICATION HISTORY
/*
/**********************************************************************/
/* Author:     Tre Hamilton                                                   
/* Date:       November 10, 2026
/* Update:     Created file, wrote comments based off of Design Document
/* Time Spent: 10 minutes
/*
/**********************************************************************/
/* Author:     Tre Hamilton                                                   
/* Date:       November 12, 2026
/* Update:     Added main function, arg line check, socket creation, and 
/*             connection to server
/* Time Spent: 5:30-6:30 | 1 hr
/*
/**********************************************************************/
/* Author:     Tre Hamilton                                                   
/* Date:       November 12, 2026
/* Update:     Added main function, arg line check, socket creation, and 
/*             connection to server, added base functionally for wordle.
/* Time Spent: 8:00-10:00 | 2 hrs
/*
/**********************************************************************
/* Author:     Patrick Walter                                                   
/* Date:       November 13, 2026
/* Update:     integrated his version.
/* Time Spent: 
/*
/**********************************************************************/