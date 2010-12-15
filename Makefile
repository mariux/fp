#COMPILING AND LINKING
CC	= gcc
CFLAGS	= -Wall -g -pedantic
LDFLAGS = -lm
OBJECTS = node.o tokenizer.o list.o grammar.o formelparser.o main.o

#PROJECT
PROJECT  = formelparser
VERSION  = 0
REVISION = 3

#PACKAGE
TARDIR  = $(PROJECT)-$(VERSION).$(REVISION)
TARFILE = $(TARDIR).tar

all: $(PROJECT)

formelparser: $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

package:
	rm -rf $(TARFILE) $(TARDIR) $(TARFILE).bz2
	mkdir $(TARDIR)
	cp -f *.c *.h Makefile LICENSE $(TARDIR)
	tar cf $(TARFILE) $(TARDIR)
	bzip2 -z9 $(TARFILE)
	rm -rf $(TARDIR)

clean:
	rm -f $(OBJECTS) $(PROJECT)
