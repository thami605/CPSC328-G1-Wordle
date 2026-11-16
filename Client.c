/**********************************************************************
 * Author/Editor:  Tre Hamilton (CS), Patrick Walter (CS), 
 *                 Caleb Miller (CS)                                                                                                               
 * Creation Date:  November 10, 2026
 * Last Modified:  November 13, 2026                                                    
 * Due Date:       November 23, 2026                                                   
 * Course:         CPSC 328                                                              
 * Professor Name: Prof. Walther                                                         
 * Assignment:     Group Project: Wordle Client/Server Application
 * Filename:       Client.c
 * Purpose:        Client side application for the Wordle game.
 *                 This file connects to the server, receives 
 *                 a random word, and allows the user to play, then 
 *                 replay if necessary.
 * Modifications:  Listed at the bottom of the file. 
 **********************************************************************/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>
#include <ctype.h>

/* Terminal colors used only for displaying Wordle guesses. */
#define TILE_GREEN  "\033[42;30m"  /* Correct position */
#define TILE_YELLOW "\033[43;30m"  /* Present, wrong position */
#define TILE_GRAY   "\033[100;97m" /* Not in the word */
#define TILE_RESET  "\033[0m"

/* Display functions: can be copied into the newer client without changing validation. */
void printWordleHeader(void)
{
    printf("\n  ----------------------------------\n");
    printf("               WORDLE\n");
    printf("  ----------------------------------\n");
    printf("  Guess the five-letter word in six tries.\n\n");
    printf("  %s A %s Correct position\n", TILE_GREEN, TILE_RESET);
    printf("  %s A %s Present, wrong position\n", TILE_YELLOW, TILE_RESET);
    printf("  %s A %s Not in the word\n\n", TILE_GRAY, TILE_RESET);
}
// Display tile colors with guess tile requested
void printGuessTiles(const char originalGuess[], const char result[])
{
   printf("  ");
   for (int i = 0; i < 5; i++)
   {
      const char *color;

      /* The existing game logic stores exact matches as letters,
         misplaced matches as #, and missing letters as /. */
      if (result[i] == originalGuess[i])
         color = TILE_GREEN; // Correct
      else if (result[i] == '#')
         color = TILE_YELLOW; // Misplaced
      else
         color = TILE_GRAY; // Not In Word

      printf("%s %c %s ", color, originalGuess[i], TILE_RESET);
   }
   printf("\n");
}

int connectTCP(const char *host, const int port);
int testConnection(const char *host, int port);
bool wordle(char word[5]);
int verifyPort(int port);
int validateGuess(char word[]);

int main(int argc, char *argv[])
{
   int port = 65000;
   char *host;
   switch(argc)
   {
      case 3:
         port = atoi(argv[2]);
         if (!verifyPort(port))
            exit(1);
      case 2:
         host = argv[1];
         break;
      default:
         fprintf(stderr, "usage: %s [host [port]]\n", argv[0]);
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
   char guess[7];
   char result[6];
   char buffer[6];
   char replay[2];
   char shownGuess[6];  /* Original letters, saved before the existing matcher changes guess. */
   bool valid = true;
   result[5] = '\0';
   printWordleHeader();
   for (int w = 0; w < 6; w++)
   {

      do
      {
         printf("  Guess %d of 6: ", w + 1);
         if (fgets(guess, sizeof guess, stdin) == 0)
            valid = false;
      }
      while (!valid || !validateGuess(guess));

	// Convert the guess to uppercase
	for (int i = 0;	i < 5; i++)
	{
		guess[i] = toupper((unsigned char)guess[i]);
	}

      memcpy(shownGuess, guess, 5);
      shownGuess[5] = '\0';
      strcpy(buffer, word);
      for (int i = 0; i < 5; i++)
      {
         if (guess[i] == word[i])
         {
            buffer[i] = '!';
            result[i] = guess[i];
            guess[i] = '!';
         }
      }
      for (int i = 0; i < 5; i++)
      {
         bool found = false;
         if (guess[i] == '!')
            continue;
         for (int j = 0; j < 5; j++)
         {
            if (guess[i] == buffer[j])
            {
               found = true;
               buffer[j] = '!';
               result[i] = '#';
               break;
            }
            if (!found)
               result[i] = '/';
         }
      }
      printGuessTiles(shownGuess, result);
      if (strcmp(word, result) == 0)
      {
         printf("\n  You won in %d guess%s!\n", w + 1, (w == 0) ? "" : "es");
         break;
      }
      if (w == 5)
         printf("\n  No guesses left! The word was %s.\n", word);
      else
         printf("  Guesses remaining: %d\n\n", 5 - w);
   }
    
   while (1)
   {
      printf("\n  Play again? (Y/N): ");

      if (fgets(replay, sizeof(replay), stdin) == NULL)
         return false;

      if (toupper((unsigned char)replay[0]) == 'Y' &&
         (replay[1] == '\n' || replay[1] == '\0'))
      {
         return true;
      }
      else if (toupper((unsigned char)replay[0]) == 'N' &&
               (replay[1] == '\n' || replay[1] == '\0'))
      {
         return false;
      }
      else
      {
         printf("  Invalid input. Please enter Y or N.\n");
      }
   }
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
/* Author:     Caleb Miller                                                  
/* Date:       November 13, 2026
/* Update:     Made formatting changes and fixed case sensitivity issues
/* Time Spent: 
/*
/**********************************************************************/