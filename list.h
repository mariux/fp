#include <stdlib.h> /* fuer malloc/free */
#include <stdio.h>  /* fuer perror/printf */

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
