# formelparser - Makefile
#
# Copyright (C) 2011 Matthias Ruester <ruester@molgen.mpg.de>
# Copyright (C) 2011 Max Planck Institut for Molecular Genetics
#
# Formelparser is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Formelparser is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

#COMPILING AND LINKING
CC	= gcc
CFLAGS	= -Wall -g -pedantic
LDFLAGS = -lm
OBJECTS = node.o tokenizer.o list.o grammar.o formula.o main.o

#PROJECT
PROJECT  = formelparser
VERSION  = 0
REVISION = 6

#PACKAGE
TARDIR  = $(PROJECT)-$(VERSION).$(REVISION)
TARFILE = $(TARDIR).tar

all: $(PROJECT)

$(PROJECT): $(OBJECTS)
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
