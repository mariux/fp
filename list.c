/*
    formelparser - list.c

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

#include <stdlib.h>
#include <stdio.h>

#include "list.h"

struct List *new_list(void)
{
    struct List *n;
    
    if((n = malloc(sizeof(struct List))) == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    
    n->first   = NULL;
    n->last    = NULL;
    n->current = NULL;
    n->count   = 0;
    
    return(n);
}

struct Element *new_element(struct Node *n)
{
    struct Element *e;
    
    if((e = malloc(sizeof(struct Element))) == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    
    e->node = n;
    e->next = NULL;
    
    return(e);
}

void add_node(struct List *l, struct Node *n)
{
    struct Element *e;

#ifdef DEBUG    
    if(l == NULL || n == NULL) {
        fprintf(stderr, "add_node (l == NULL || n == NULL)  l=0x%08x n=0x%08x\n", l, n);
        exit(EXIT_FAILURE);
    }
#endif
    
    e = new_element(n);
    
    /* empty list */
    if(l->first == NULL) {
        l->first = e;
        l->last  = e;
    } else {
        l->last->next = e;
        l->last       = e;
    }
    
    l->current = l->first;
    l->count++;
}

void delete_list_without_nodes(struct List *old)
{
    struct Element *current, *delete;
    
    if(old == NULL)
        return;
    
    current = old->first;
    
    while(current != NULL) {
        delete = current;
        current = current->next;
        free(delete);
    }
    
    free(old);
}

void delete_list(struct List *old)
{
    struct Element *current, *delete;
    
    if(old == NULL)
        return;
    
    current = old->first;
    
    while(current != NULL) {
        delete_node(current->node);
        delete = current;
        current = current->next;
        free(delete);
    }
    
    free(old);
}

void print_list(struct List *l)
{
    if(l == NULL)
        printf("list is NULL\n");
    
    l->current = l->first;
    
    while(l->current != NULL) {
         print_node(l->current->node);
         printf("\n");
         
         l->current = l->current->next;
    }
    
    if(l->count == 0)
        printf("the list is empty\n");
    else {
        if(l->count == 1)
            printf("the list has one element\n");
        else
            printf("the list has %d elements\n", l->count);
    }
    
    rewind_list(l);
}

void rewind_list(struct List *l)
{
    if(l == NULL)
        return;
    
    l->current = l->first;
}

void next_element(struct List *l)
{
    if(l == NULL || l->current == NULL)
        return;
    
    l->current = l->current->next;
}
