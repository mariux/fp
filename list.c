#include "list.h"

struct List *new_list(void)
{
    struct List *n;
    
    if((n = malloc(sizeof(struct List))) == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    
    n->first = NULL;
    n->last = NULL;
    n->current = NULL;
    n->count = 0;
    
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
    
    /* leere Liste */
    if(l->first == NULL) {
        l->first = e;
        l->last = e;
    } else {
        l->last->next = e;
        l->last = e;
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
        printf("Die Liste ist NULL\n");
    
    l->current = l->first;
    
    while(l->current != NULL) {
         print_node(l->current->node);
         printf("\n");
         
         l->current = l->current->next;
    }
    
    if(l->count == 0)
        printf("Die Liste ist leer\n");
    else {
        if(l->count == 1)
            printf("Die Liste hat ein Element\n");
        else
            printf("Die Liste hat %d Elemente\n", l->count);
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
