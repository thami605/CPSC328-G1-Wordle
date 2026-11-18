/**********************************************************************
 * Author/Editor:  Tre Hamilton (CS), Patrick Walter (CS), 
 *                 Caleb Miller (CS)                                                                                                               
 * Creation Date:  November 10, 2026
 * Last Modified:  November 16, 2026                                                    
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
#include <errno.h>

/* Variables for setting colored text */
#define TILE_GREEN "\033[42;30m"
#define TILE_YELLOW "\033[43;30m"
#define TILE_GRAY "\033[100;97m"
#define TILE_WHITE "\033[47;30m"
#define TILE_RESET "\033[0m"

int connectTCP(const char *host, const int port); //Included in library
void serverConnect(const char *host, int port); //Connects client to server
int wordle(char word[5]); //The actual wordle game using provided word
int verifyPort(int port); //Verifies port input
int validateInput(char word[], int size); //Validates wordle input
char convertToUpper(char word[]); //Converts strings to uppercase
void printHeader(void); //prints wordle game header
void printResult(const char guess[], const char result[]); //prints result of wordle
void printLetters(const char letterlist[]); //prints letters used, guessed and unguessed
int errexit(const char *format, ...); //Method for handling errors

/*
 *  Method: main
 *  Purpose: Main driver for the program, verifies CLAs and calls function to connect to server
 *  Parameters: int argc - number of CLAs, char *argv[] - array of CLAs
 *  Returns - none
 */
int main(int argc, char *argv[])
{
   int port = 65000; //default port
   char *host; //store host
   switch(argc) //switch case based on number of arguments
   {
      case 3: //server and port
         port = atoi(argv[2]); //get port
         if (!verifyPort(port)) //verify correct port
               exit(1);
      case 2:
         host = argv[1]; //just server
         break;
      default:
         fprintf(stderr, "usage: %s [host [port]]\n", argv[0]); //exit if CLA was mishandled
         exit(1);
   }
   serverConnect(host, port); //connect to server and play wordle
   exit(0); //exit when done
}

/*
 *  Method: serverConnect
 *  Purpose: Establish connection with the Wordle server and call function to play wordle
 *  Parameters: char *host - the hostname of the server, int port - the port to connect to
 *  Return - none
 */
void serverConnect(const char *host, int port)
{
   char buf[6]; //buffer for recieving up to 5 letters from server (size of 6 to store null terminator)
   int s; //socket descriptor
   int replay = 0; //boolean int for replay
   s = connectTCP(host, port); //create and return socket
   if (read(s, buf, sizeof(buf)) > 0) //read Hello from server
   {
      printf("Read %s from server\n", buf);
   }
   else //error handle if socket read failed
   {
      errexit("HELLO read failed: %s\n", strerror(errno));
   }
   printHeader(); //print wordle game header

   //while loop for recieving word and playing wordle
   do
   {
      if (write(s, "WORD", sizeof("WORD")) < 0) //send message to get word to server, and handle possible error
      {
         errexit("Failed sending word request to server: %s\n", strerror(errno));
      }
      printf("Requesting word from server.\n");
      if (read(s, buf, sizeof(buf)) < 0 ) //read word from server, and handle possible error
      {
         errexit("WORD read failed: %s\n", strerror(errno));
      }
      else
      {
         printf("Recieved word from server.\n", buf); //print to console that getting word was successful
      }
      buf[strlen(buf)] = '\0'; //add null terminator to end
      replay = wordle(buf); //play wordle and store replay variable
   }
   while(replay == 1); //repeat while replay is true
   if (write(s, "QUIT", sizeof("QUIT")) < 0) //write quit to the server
   {
      errexit("Failed sending QUIT to server: %s\n", strerror(errno));
   }
   close(s); //close socket
}


/*
 *  Method: wordle
 *  Purpose: plays wordle with the given word
 *  Parameters: char word[] - the word given by the server
 *  Return int - 1 or 0 depending on if user wants to replay
 *
 *  For color code in result and letters
 *  x - Unused Letter (letter list only)
 *  ! - Green (right letter right position)
 *  # - yellow (right letter wrong position)
 *  / - Grey (wrong letter)
 */
int wordle(char word[])
{
   char guess[7]; //guess provided by user
   char display[7]; //copy of guess for displaying results
   char result[6]; //logical form of result using special characters
   char buffer[6]; //buffer for
   char letters[27]; //list of used letters
   char replay[3]; //response of user to replay

   int valid = 1; //int for checking if input is valid
   result[5] = '\0'; //add null terminator to end of result
   memset(letters, 'x', sizeof letters); //fill letters with x to represent unused characters
   letters[26] = '\0'; //place null terminator in last slot

   printf("Wordle Game Started\n");

   //Loop 6 times for 6 guesses
   for (int w = 0; w < 6; w++)
   {
      //repeatedly ask user for guess until proper input
      do
      {
         printf("Enter Guess: ");
         if (fgets(guess, sizeof guess, stdin) == 0) //fgets has proved more reliable than scanf
            valid = 0;
         else
            valid = 1;
      }
      while (!valid || !validateInput(guess, 5)); //validate user input
      convertToUpper(guess); //convert guess to uppercase to make case insensitive
      printf("\n"); //print nextline
      strcpy(buffer, word); //copy the given word into a manipulatable buffer
      strcpy(display, guess); //copy the guess to leave unedited for displaying

      //first loop to look for greens (right letter right place)
      for (int i = 0; i < 5; i++)
      {
         if (guess[i] == word[i]) //check both positions
         {
            letters[(int)guess[i]-65] = '!'; //set that letter to green in used letter list
            buffer[i] = '!'; //set buffer to ! to represent found letter to be skipped when compared against guess
            result[i] = '!'; //set result to ! to represent green letter
            guess[i] = '!'; //set guess to ! to represent found letter, to skip over already found letters
         }
      }

      //second loop for checking yellows
      for (int i = 0; i < 5; i++)
      {
         int found = 0; //check for seeing if a letter was found in the word
         if (guess[i] == '!') //skip already green letters
            continue;
         for (int j = 0; j < 5; j++) //loop over every letter in the buffer
         {
            if (guess[i] == buffer[j])
            {
               found = 1; //set found variable
               buffer[j] = '!'; //set buffer position to found
               result[i] = '#'; //set to character representing yellow
               letters[(int)guess[i]-65] = '#'; //same as above
               break;
            }
            if (!found) //if letter was not found, set to appropriate characters
            {
               result[i] = '/';
               letters[(int)guess[i]-65] = '/';
            }
         }
      }
      printResult(display, result); //print result of guess
      if (strcmp(result, "!!!!!") == 0) //check if word is complete match
      {
         printf("\nYou won in %d guess%s!\n", w+1, (w==0) ? "" : "es");
         break;
      }
      if (w == 5) //check if ran out of guesses
         printf("\nNo guesses Left! The word was %s. \n", word);

      else //print remaining letters and guesses
      {
         printLetters(letters);
         printf("Guesses remaining: %d\n\n", 5-w);
      }
   }
   //Loop while invalid input
   do
   {
      //Get input from user to repeat or not
      printf("Enter Y to play again, anything else to quit: ");
      if (fgets(replay, sizeof replay, stdin) == 0)
         valid = 0;
      else
         valid = 1;
   }
   while(!valid || !validateInput(replay, 1)); //validate input
   convertToUpper(replay); //convert to uppercase
   if (replay[0] == 'Y') //return if game should repeat
      return 1;
   else
      return 0;

}

/*
 *  Method: printResult
 *  Purpose: Displays the result of the guess with wordle formatting
 *  Parameters: char guess[] - the unedited guess, char result[] - the result of the guess
 *  Return: none
 */
void printResult(const char guess[], const char result[])
{
   printf(" ");
   for (int i = 0; i < 5; i++) //loop over the word
   {
      const char *color;

      if (result[i] == '!') //character for green in result
         color = TILE_GREEN;
      else if (result[i] == '#') //character for yellow in result
         color = TILE_YELLOW;
      else
         color = TILE_GRAY; //otherwise missing
         printf("%s %c %s", color, guess[i], TILE_RESET); //print the character and loop again
   }
   printf("\n");
}

/*
 *  Method: printHeader
 *  Purpose: prints wordle game header and information
 *  Parameters: none
 *  Return: none
 */
void printHeader(void)
{
   printf("\n  ----------------------------------\n");
   printf("               WORDLE\n");
   printf("  ----------------------------------\n");
   printf("  Guess the five-letter word in six tries.\n\n");
   printf("  %s A %s Correct position\n", TILE_GREEN, TILE_RESET);
   printf("  %s A %s Present, wrong position\n", TILE_YELLOW, TILE_RESET);
   printf("  %s A %s Not in the word\n\n", TILE_GRAY, TILE_RESET);
}

/*
 *  Method: printLetters
 *  Purpose: prints unguessed/guessed letters
 *  Parameters: char letterlist[] - list of characters representing letter state alphebetically
 *  Return: none
 */
void printLetters(const char letterlist[])
{
    printf("\nRemaining Leters\n");
    for (int i = 0; i < 26; i++) //loop over array
    {
      if ((i+1) % 9 == 0) //formatting, start on new line after 9 characters
         printf("\n");
      const char *color;
      char letter = ((char)i+65); //get ACSII letter correlating to index

      if (letterlist[i] == '!') //green code
         color = TILE_GREEN;
      else if (letterlist[i] == '#') //yellow code
         color = TILE_YELLOW;
      else if (letterlist[i] == '/') //grey code
         color = TILE_GRAY;
      else
         color = TILE_WHITE; //set to white if unguessed
         printf("%s %c %s", color, letter, TILE_RESET); //print and repeat
   }
   printf("\n");
}
 

/**********************************************************************
 *
 * MODIFICATION HISTORY
 *
 **********************************************************************/
/**********************************************************************
 * Author:     Tre Hamilton                                                   
 * Date:       November 10, 2026
 * Update:     Created file, wrote comments based off of Design Document
 * Time Spent: 10 minutes
 *
 **********************************************************************/ 
/**********************************************************************
 * Author:     Tre Hamilton                                                   
 * Date:       November 12, 2026
 * Update:     Added main function, arg line check, socket creation, and 
 *             connection to server
 * Time Spent: 5:30-6:30 | 1 hr
 *
 **********************************************************************/
/**********************************************************************
 * Author:     Tre Hamilton                                                   
 * Date:       November 12, 2026
 * Update:     Added main function, arg line check, socket creation, and 
 *             connection to server, added base functionally for wordle.
 * Time Spent: 8:00-10:00 | 2 hrs
 *
 **********************************************************************/
/**********************************************************************
 * Author:     Patrick Walter                                                   
 * Date:       November 12, 2026
 * Update:     Overhauled program from scratch, moved socket creation 
 *             to library, enabled input from CLA (hostname and 
 *             optional port) for hostname and port to be properly 
 *             resolved established communication with server program, 
 *             and tested mock back and forth with server. (Recieve 
 *             HELLO, send WORD request, recieve word, send another 
 *             word request, read word, send quit)
 * Time Spent: ~4 hours
 *
 **********************************************************************/
/**********************************************************************
 * Author:     Patrick Walter                                                 
 * Date:       November 12, 2026
 * Update:     Fixed formatting errors with strings and added basic 
 *             implementation for wordle using test word recieved 
 *             from server
 * Time Spent: ~1.5 hours
 *
 **********************************************************************/
/**********************************************************************
 * Author:     Patrick Walter                                                 
 * Date:       November 14, 2026
 * Update:     Added replay, functionality
 * Time Spent: ~0.5 hours
 *
 **********************************************************************/
/**********************************************************************
 * Author:     Patrick Walter                                                 
 * Date:       November 15, 2026
 * Update:     Initial completion of wordle game logic and output 
 *             formatting.
 * Time Spent: ~2 hours
 *
 **********************************************************************/
/**********************************************************************
 * Author:     Patrick Walter                                                 
 * Date:       November 15, 2026
 * Update:     Completed input validation
 * Time Spent: ~2 hours
 *
 **********************************************************************/
/**********************************************************************
 * Author:     Caleb Miller
 * Date:       November 16, 2026
 * Update:     Added start of final formatting for wordle, including 
 *             colored tiles and proper user feedback.
 * Time Spent: ~2 hours
 *
 **********************************************************************/
/**********************************************************************
 * Author:     Patrick Waler
 * Date:       November 16, 2026
 * Update:     Finished final output formatting, added unguessed letters, 
 *             updated error handling to use library method, and added 
 *             documentation. Client program can be considered complete 
 *             aside from minor tweaks as they come up.
 * Time Spent: ~2 hours
 *
 **********************************************************************/
/**********************************************************************
 * Author:     Tre Hamilton
 * Date:       November 17, 2026
 * Update:     Universalized Documentation for all files
 * Time Spent: ~15 minutes
 *
 **********************************************************************/