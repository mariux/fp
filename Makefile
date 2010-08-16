CC	=/usr/bin/gcc
CFLAGS	=-Wall -g -pedantic
LDFLAGS =-lm
OBJECTS =node.o tokenizer.o list.o grammar.o formelparser.o

all: formelparser

formelparser: $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f $(OBJECTS)
	rm -f formelparser
