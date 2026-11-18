/**********************************************************************
 * Author/Editor:  Patrick Walter (CS)                                                                                                               
 * Creation Date:  November 12, 2026
 * Last Modified:  November 16, 2026                                                    
 * Due Date:       November 23, 2026                                                   
 * Course:         CPSC 328                                                              
 * Professor Name: Prof. Walther                                                         
 * Assignment:     Group Project: Wordle Client/Server Application
 * Filename:       passiveTCP.c
 * Purpose:        Focused for server side, this program is able to 
 *                 create a TCP socket and bind it to a port to listen 
 *                 on.
 * Modifications:  Listed at the bottom of the file. 
 **********************************************************************/

//libraries
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>

int errexit(const char *format, ...); //Method for errors

/*
 *  Method: passiveTCP
 *  Purpose: Create and return TCP socket that listens on specific port
 *  Parameters: int port - the port to listen on, int qlen - length of connections to be allowed in queue
 *  Return: int s - socket file descriptor
 */
int passiveTCP(const int port, int qlen)
{
    struct sockaddr_in sockin; //socket in information
    int s; //socket descriptor

    memset(&sockin, 0, sizeof(sockin)); //zero out address for socket in
    sockin.sin_family = AF_INET; //set socket in to internet
    sockin.sin_addr.s_addr = INADDR_ANY; //allow any address in
    sockin.sin_port = htons(port); //convert port and set it

    s = socket(AF_INET, SOCK_STREAM, 0); //create socket
    if (s < 0) //handle errors
    {
        errexit("Failed to create socket: %s\n", strerror(errno));

    }

    if (bind(s, (struct sockaddr *)&sockin, sizeof(sockin)) < 0) //bind socket
    {
        errexit("Failed to bind socket to port: %s\n", port, strerror(errno));
    }

    if (listen(s, qlen) < 0) //set socket to listen
    {
        errexit("Failed to listen on port: %s\n", strerror(errno));
    }
    return s; //return socket descriptor
}


/**********************************************************************
 *
 * MODIFICATION HISTORY
 *
 **********************************************************************/
/**********************************************************************
 * 
 * Author:     Patrick Walter
 * Date:       November 12, 2026
 * Update:     Finished file
 * Time Spent: ~30 minutes
 *
 **********************************************************************/
/**********************************************************************
 * 
 * Author:     Patrick Walter
 * Date:       November 16, 2026
 * Update:     Added documentation and updated error methods
 * Time Spent: ~20 minutes
 *
 **********************************************************************/