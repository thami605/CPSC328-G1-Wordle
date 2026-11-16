/**********************************************************************/
/* Author/Editor:  Patrick Walter (CS)                                                                                                               
/* Creation Date:  November 15, 2026
/* Last Modified:  November 15, 2026                                                    
/* Due Date:       November 23, 2026                                                   
/* Course:         CPSC 328                                                              
/* Professor Name: Prof. Walther                                                         
/* Assignment:     Group Project: Wordle Client/Server Application
/* Filename:       validationUtils.c
/* Purpose:        Program contains functions to validate user input 
/*                 and port numbers for the Wordle game.
/* Modifications:  Listed at the bottom of the file. 
/**********************************************************************/
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int verifyPort(int port)
{
    if (port < 1 || port > 65535)
    {
        printf("Specified port, %d invalid", port);
        return 0;
    }
    return 1;
}

int validateGuess(char guess[])
{
    int ch;
    int len = strlen(guess);
    if (guess[len-1] != '\n')
    {
        printf("Invalid guess\n");
        while ((ch = getchar()) != '\n' && ch != EOF);
        return 0;
    }

    if (len > 0 && guess[len - 1 ] == '\n')
        guess[--len] = '\0';

    if (len != 5)
    {
        printf("Invalid length of guess");
        return 0;
    }

    for (int i = 0; i < 5; i++)
    {
        if (!isalpha(guess[i]))
        {
            printf("Guess contains invalid characters. Try again: ");
            return 0;
        }
    }
    return 1;
}

//int validate
