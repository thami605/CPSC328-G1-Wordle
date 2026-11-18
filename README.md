# **CPSC328-G1-Wordle**
 
# -- Team Members --
**Tre Hamilton**
  Library, github
**Patrick Walter**
  Client, Server
**Christian Collins**
  Project Manager, Client
**Caleb Miller**
  Client, Making it pretty

# -- Description --


# -- Directions --
  Our goal was to make this project super easy to run. We used a Makefile to get that done. After all files have been downloaded, the following instructions can be followed to get your very Wordle game running.
## How to compile
  Within the terminal run: 'make'
  If the user wishes to run only one side of the program: 'make server' or 'make client'
## How to run the client
  After being compiled, the user can run './client <hostname> <port (optional)>' with hostname being the a viable server side host and port being a selected port number. './client localhost' can be utilized if the server is running on the same machine.
## How to run the server
  After being compiled, the user can run './server <port (optional)>' with *port* being an available port number to run on. './server' can be utilized if the user does not have a specific port in mind.

# -- Library --
**validationUtils**
  This library contains methods shared between the client and server. It contains most of the shared verification and validation methods used throughout the program.
**passiveTCP**
  This library contains methods on the server side. Its main method is passiveTCP which creates and returns a TCP socket that will listen on selected ports.
**connectTCP** 
  This library is the counterpart to the previous library. It is the client side protocol handler that lets a client create a TCP socket and connect to a specified server with a selected port.

# -- Protocols -- 
  This project uses only TCP protocol. Our team decided a slightly slower but more reliable connection should be the priority for this project.

# -- Teamwork -- 



