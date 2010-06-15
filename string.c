/*
    formelparser - string.c

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

#include "string.h"

void next_char(struct String *t)
{
    if(t->current == NULL)
        return;
    
    t->current++;
    
    if(t->current == '\0')
        return;
    
    while(*(t->current) == ' ')
        t->current++;
    
    t->i++;
}

struct String *new_string(char *s)
{
    struct String *n;
    
    if(s == NULL)
        return(NULL);
    
    if((n = malloc(sizeof(struct String))) == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    
    if((n->str = calloc(strlen(s) + 1, sizeof(char))) == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }
    
    memcpy(n->str, s, strlen(s) * sizeof(char));
    n->current = n->str;
    n->i = 1;
    
    return(n);
}

void rewind_string(struct String *t)
{
    t->current = t->str;
    t->i = 1;
}

void delete_string(struct String *s)
{
    free(s->str);
    free(s);
}
