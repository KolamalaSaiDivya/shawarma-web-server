CC=gcc
CFLAGS=-Wall -Wextra

SRC=src/main.c src/server.c

all:
	$(CC) $(CFLAGS) $(SRC) -o sws

clean:
	rm -f sws