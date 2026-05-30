CC = gcc

CFLAGS = -Wall -Wextra

SRC = src/main.c src/server.c src/router.c

TARGET = sws

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) -lws2_32

clean:
	rm -f $(TARGET) $(TARGET).exe