/**********************************************************************
 * Author/Editor:  Patrick Walter (CS)                                                                                                               
 * Creation Date:  November 15, 2026
 * Last Modified:  November 17, 2026                                                    
 * Due Date:       November 23, 2026                                                   
 * Course:         CPSC 328                                                              
 * Professor Name: Prof. Walther                                                         
 * Assignment:     Group Project: Wordle Client/Server Application
 * Filename:       validationUtils.c
 * Purpose:        Methods shared between client and server, mostly 
 *                 verification. Also includes method for handling 
 *                 errors as provided in given examples
 * Modifications:  Listed at the bottom of the file. 
 **********************************************************************/
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

/*
 *  Method: verifyPort
 *  Purpose: verifies that port input is in valid range
 *  Parameters: int port - specified port
 *  Return: 1 or 0 for true or false
 */
int verifyPort(int port)
{
    if (port < 1 || port > 65535) //check for valid port range
    {
        printf("Specified port, %d invalid", port);
        return 0;
    }
    printf("Verified Port\n");
    return 1;
}
/*
 *  Method validateInput
 *  Purpose: Validate given input, including given size
 */
int validateInput(char input[], int size)
{
    int ch; //character to check
    int len = strlen(input); //length of the input
    if (input[len-1] != '\n') //check if newline character is read, meaning the correct size was input
    {
        printf("Invalid input.\n");
        while ((ch = getchar()) != '\n' && ch != EOF); //clear stdin
        return 0;
    }

    if (len > 0 && input[len - 1 ] == '\n') //manuipulate the input length and set proper null terminator
        input[--len] = '\0';

    if (len != size) //check if string is shorter, above check handles longer strings
    {
        printf("Invalid length of input\n");
        return 0;
    }

    for (int i = 0; i < size; i++) //check each character to ensure it is alphanumeric
    {
        if (!isalpha(input[i]))
        {
            printf("Input contains invalid characters.\n");
            return 0;
        }
    }
    return 1; //return 1 if validated
}

/*
 *  Method: convertToUpper
 *  Purpose: converts all characters in a string to uppercase
 *  Parameters: char input[] - a string
 *  Return: input - the modified string
 */
char* convertToUpper(char input[])
{
    //loop through every character and make uppercase
    for (int i = 0; i < strlen(input); i++)
    {
        input[i] = toupper(input[i]);
    }
    return input;
}

/*
 *  Author: Comer, Stevens
 *  Title: Internetworking with TCP/IP, Vol 3, Linux / POSIX Sockets Version
 *  Publisher: Pretice Hall
 *  Page 87
 *  Retrived November 16, 2026 from library examples provided by Professor Walther
 *
 *  Method: errexit
 *  Purpose: Handle errors in multiple instances
 *  Parameters: char *format - string for error, ... - extra parameters for more information
 */
int errexit(const char *format, ...)
{
    //macro for errors. Handles multiple arguments of multiple types
    va_list	args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    exit(1);
}

/**********************************************************************
 *
 * MODIFICATION HISTORY
 *
 **********************************************************************/
/**********************************************************************
 * 
 * Author:     Patrick Walter
 * Date:       November 15, 2026
 * Update:     Added initial verification methods for port, input, and 
 *             convertToUpper
 * Time Spent: ~2 hours
 *
 **********************************************************************/
/**********************************************************************
 * Author:     Patrick Walter
 * Date:       November 16, 2026
 * Update:     Added errexit as exampled in provided TCPecho program, 
 *             and added documentation
 * Time Spent: ~30 minutes
 *
 **********************************************************************/
/**********************************************************************
 * 
 * Author:     Tre Hamilton
 * Date:       November 17, 2026
 * Update:     Universalized Documentation for all files
 * Time Spent: ~15 minutes
 *
 **********************************************************************/
