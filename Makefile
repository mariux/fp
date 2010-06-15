CC	=/usr/bin/gcc
CFLAGS	=-Wall -g -pedantic
LDFLAGS =-lm
OBJECTS =node.o string.o list.o

all: formelparser

formelparser: formelparser.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $<

touch:
	touch *.h

debug:
	time valgrind -v --leak-check=full --show-reachable=yes --smc-check=all ./formelparser $(ARG)

clean:
	rm -f *.o
	rm -f formelparser
