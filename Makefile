# Define the compiler
CC=gcc

# Define any compile-time flags
CFLAGS=-Wextra -Wall -g

# Define the source files
SRCS=main.c house.c logger.c utils.c ghost.c hunter.c room.c evidence.c

# Define the object files 
OBJS=$(SRCS:.c=.o)

# Define the executable file 
MAIN=ghost_hunt_simulator

.PHONY: clean

all: $(MAIN)
	@echo Simple ghost hunt simulator compiled

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) -pthread

# Rule to create object files
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o *~ $(MAIN)