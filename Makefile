# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -lpthread

# Executable names
SERVER = server
CLIENT = client

# Source files
SERVER_SRC = Server.c validationUtils.c passiveTCP.c
CLIENT_SRC = Client.c validationUtils.c connectTCP.c

# Object files
SERVER_OBJ = $(SERVER_SRC:.c=.o)
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)

# Default target
all: $(SERVER) $(CLIENT)

# Build the server executable
$(SERVER): $(SERVER_OBJ)
	$(CC) $(CFLAGS) -o $(SERVER) $(SERVER_OBJ)

# Build the client executable
$(CLIENT): $(CLIENT_OBJ)
	$(CC) $(CFLAGS) -o $(CLIENT) $(CLIENT_OBJ)

# Compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(SERVER) $(CLIENT) $(SERVER_OBJ) $(CLIENT_OBJ)

# Phony targets
.PHONY: all clean
