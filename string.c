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
