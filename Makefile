CC=gcc

CFLAGS=-Wall -Wextra

SRC=src/main.c src/server.c

all:
	$(CC) $(CFLAGS) $(SRC) -o sws -lws2_32

clean:
	rm -f sws.exe sws