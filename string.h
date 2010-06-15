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
