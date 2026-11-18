/**********************************************************************
 * Author/Editor:  Patrick Walter (CS)                                                                                                               
 * Creation Date:  November 12, 2026
 * Last Modified:  November 16, 2026                                                    
 * Due Date:       November 23, 2026                                                   
 * Course:         CPSC 328                                                              
 * Professor Name: Prof. Walther                                                         
 * Assignment:     Group Project: Wordle Client/Server Application
 * Filename:       connectTCP.c
 * Purpose:        Focused for client side, this program is able to 
 *                 take a given hostname and port number and to return 
 *                 a created TCP socket.
 * Modifications:  Listed at the bottom of the file. 
 **********************************************************************/

//libraries
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

int errexit(const char *format, ...); //Method for handling errors

/*
 *    Method: connectTCP
 *    Purpose: create and return a TCP socket connected to a given hostname and port
 *    Parameters: char *host - server hostname, int port - desired port number
 *    Return: int s - the connected socket descriptor
 */
int connectTCP(const char *host, const int port)
{
    struct sockaddr_in *server; //socket address in
    struct addrinfo *servaddrinfo = (struct addrinfo *)malloc(sizeof(struct addrinfo)); //server address info
    struct sockaddr saddr; //for sizeof
    int s; //socket descriptor
    int rv; //check for error
    char ipstr[INET_ADDRSTRLEN]; //string for hostnames ip address

    //set up connection
    servaddrinfo->ai_family = AF_INET;
    servaddrinfo->ai_flags = 0;
    servaddrinfo->ai_protocol = 0;
    servaddrinfo-> ai_socktype = SOCK_STREAM;

    //get server information using given hostname
    rv=getaddrinfo(host, NULL, servaddrinfo, &servaddrinfo);
    if (rv != 0) //check if cannot resolve
    {
        printf("Error in getaddrinfo call: %s\n", gai_strerror(rv));
        errexit("Cannot resolve host for %s\n", host);
    }

    server = (struct sockaddr_in *)servaddrinfo ->ai_addr; //save needed information

    //Converting and outputting server ip to string for convenience
    if (inet_ntop(AF_INET, &server->sin_addr, ipstr, INET_ADDRSTRLEN) == NULL)
        perror("Error converting IPv4 address to a string: ");
    else
        printf("IP address for %s is %s\n", host, ipstr);

    server->sin_port = htons(port); //use given port and call htons
    s = socket(AF_INET, SOCK_STREAM, 0); //create socket
    if (s < 0) //check for error
    {
        errexit("Cannot create socket: %s\n", strerror(errno));

    }

    if (connect(s, (struct sockaddr *)server, sizeof(saddr)) < 0) //connect socket to server, check for error
    {
        errexit("Cannot connect socket: %s\n", host, port, strerror(errno));
    }
    free(servaddrinfo); //free memory
    return s; //return created socket
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
 * Update:     Added Everything
 * Time Spent: ~40 minutes
 *
 **********************************************************************/
/**********************************************************************
 * 
 * Author:     Patrick Walter
 * Date:       November 16, 2026
 * Update:     Updated error methods and added documentation
 * Time Spent: ~20 minutes
 *
 **********************************************************************/