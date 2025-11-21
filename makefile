CC = gcc
CFLAGS = -Wall -O2

all: docker-lite

docker-lite: docker-lite.c
	$(CC) $(CFLAGS) -o docker-lite docker-lite.c

clean:
	rm -f docker-lite
