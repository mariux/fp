/*
    formelparser - list.h

    Copyright (C) 2010 Matthias Ruester <ruester@molgen.mpg.de> 
    Copyright (C) 2010 Max Planck Institut for Molecular Genetics

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

#include "node.h"

#ifndef LIST_H
#define LIST_H

struct List {
    struct Element *first;
    struct Element *last;
    struct Element *current;
    unsigned int count;
};

struct Element {
    struct Node *node;
    struct Element *next;
};

extern struct List *new_list(void);
extern struct Element *new_element(struct Node *n);
extern void add_node(struct List *l, struct Node *n);
extern void delete_list(struct List *old);
extern void print_list(struct List *l);
extern void delete_list_without_nodes(struct List *old);
extern void rewind_list(struct List *l);
extern void next_element(struct List *l);

#endif
