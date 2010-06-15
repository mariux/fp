/*
    formelparser - string.h

    Copyright (C) 2010 Matthias Ruester

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h> /* fuer malloc/free */
#include <stdio.h>  /* fuer perror/printf */
#include <string.h> /* fuer strlen */

#ifndef STRING_H
#define STRING_H

struct String {
    char *current;
    char *str;
    int i;
};

extern struct String *new_string(char s[]);
extern void next_char(struct String *t);
extern void rewind_string(struct String *t);
extern void delete_string(struct String *s);

#endif
