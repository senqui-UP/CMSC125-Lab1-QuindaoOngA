CC = gcc
CFLAGS = -Wall -Wextra

all:
	$(CC) $(CFLAGS) main.c parser.c executor.c builtins.c -o mysh

clean:
	rm -f mysh