CC=gcc
LN=ld
CFLAGS=-Wall -std=c99

HEADERS=net.h
SOURCES=net.c
OBJECTS=net.o

.PHONY: default
default: build link

build: $(HEADERS) $(SOURCES)
	$(CC) $(CFLAGS) -c $(SOURCES)
link: $(OBJECTS)
	$(LN) -r $(OBJECTS) -o net_test.o
clean:
	rm -f *.o *.i *.s
