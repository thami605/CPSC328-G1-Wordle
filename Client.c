/**********************************************************************/
/* Author/Editor:  Tre Hamilton (CS)                                                                                                               
/* Creation Date:  November 10, 2026
/* Last Modified:  November 12, 2026                                                    
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

/*
-Client-
1. **Connect to Server**
   - Establish a connection to the server on the chosen port.
   - Handle connection errors and retry if necessary.

2. **Receive Random Word**
   - Communicate with the server to receive a random word for the game.
   - Ensure the word is properly received and stored.

3. **Play Wordle Game**
   - Implement the logic for playing a game of Wordle using the provided random word.
   - Validate user input and provide feedback based on the game state.

4. **Provide Game State Information**
   - Display information to the user about the current game state (e.g., guesses remaining, correct letters).

5. **Determine Game End State**
   - Check if the word has been guessed correctly or if the user has used all 6 guesses.
   - Handle both win and lose scenarios appropriately.

6. **Ask to Play Again**
   - Prompt the user to decide whether they want to play another game.

7. **Request New Word**
   - If the user wants to play again, request a new random word from the server.

8. **Close Connection**
   - If the user does not want to play again, close the connection with the server gracefully.

9. **Handle Connection Loss**
   - Detect if the connection to the server is lost.
   - Attempt to re-establish the connection and handle errors appropriately.
*/

#define SIZE sizeof(struct sockaddr_in)
#define IPADDR "127.0.0.1"

/* -- Temp Functions -- */
//need to go into library

// Convert a string to lowercase
void toLowerCase(char *str) {
   for (int i = 0; str[i] != '\0'; i++) {
       str[i] = tolower(str[i]);
   }
}//end toLowerCase

//return 1 if guess is correct, 0 if not
int checkGuess(const char *guess, const char *answer) {
   return strcmp(guess, answer) == 0;
}//end checkGuess

void displayFeedback(const char *guess, const char *answer) {
   
}


int main(int argc, char **argv) {

   int sockfd;
   struct sockaddr_in server = {AF_INET, 7000};
   
   /* Validate Command Line Arguments */
   if (argc != 3) {
      printf("\nusage: %s <hostname> <port_number>\n\n", argv[0]);
      exit(1);
   }

   //Convert and store the server's IP address
   server.sin_addr.s_addr = inet_addr(argv[1]);

   /* Create socket */
   if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      perror("ERROR: Unable to create socket");
      exit(1);
   }

   // connect the socket to the server's address                                    
   if (connect(sockfd, (struct sockaddr *)&server, SIZE) == -1) {   
      perror("ERROR: connect call failed");
      exit(-1);
   }
   
   printf("Connected to server successfully.\n");


   //delete later
   printf("DEBUG: Client connected to server at %s:%s\n", argv[1], argv[2]);
   printf("DEBUG: closing socket connection\n");

   close(sockfd);
   return 0;
}//end main


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
/*             connection to server
/* Time Spent: 8:00 | 
/*
/**********************************************************************/