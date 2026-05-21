CC = gcc
CFLAGS = -Wall -Wextra -g

OBJ = main.o parser.o executor.o builtins.o

all: mysh

mysh: $(OBJ)
	$(CC) $(CFLAGS) -o mysh $(OBJ)

%.o: %.c shell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f mysh $(OBJ)