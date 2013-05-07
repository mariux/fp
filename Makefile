# fp - Makefile
#
# Copyright (C) 2011 Matthias Ruester <ruester@molgen.mpg.de>
# Copyright (C) 2011 Max Planck Institut for Molecular Genetics
#
# fp is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# fp is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

#COMPILING AND LINKING
CC      = gcc
CFLAGS  = -Wall -Wextra -g -pedantic
LDFLAGS = -lm
OBJECTS = node.o tokenizer.o list.o grammar.o formula.o main.o

#PROJECT
PROJECT  = fp
VERSION  = 0
REVISION = 8

#INSTALLATION
DESTDIR =
PREFIX  = /usr/local
BINDIR  = ${PREFIX}/bin

all: $(PROJECT)

$(PROJECT): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

install: all
	@mkdir -pv ${DESTDIR}${BINDIR}
	@cp -vf ${PROJECT} ${DESTDIR}${BINDIR}

clean:
	@rm -f $(OBJECTS) $(PROJECT)
