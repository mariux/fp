/*
    formelparser - tokenizer.c

    Copyright (C) 2010 Matthias Ruester
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
#include <string.h>

#include "tokenizer.h"

/*****************************************************************************/

void next_token(struct Tokenizer *tokenizer)
{
    if(tokenizer->current_token == NULL)
        return;
    
    tokenizer->current_token++;
    
    if(CURRENT_TOKEN == '\0')
        return;
    
    while(CURRENT_TOKEN == ' ')
        tokenizer->current_token++;
    
    tokenizer->position++;
}

/*****************************************************************************/

struct Tokenizer *create_tokenizer(char *string)
{
    struct Tokenizer *tokenizer;
    
    if(string == NULL)
        return(NULL);
        
    tokenizer = malloc(sizeof(struct Tokenizer));
    
    if(!tokenizer) {
        perror("malloc(tokenizer)");
        exit(EXIT_FAILURE);
    }
    
    tokenizer->string = strdup(string);
    
    if(!(tokenizer->string)) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }
    
    tokenizer->current_token = tokenizer->string;
    tokenizer->position      = 1;
    
    return(tokenizer);
}

/*****************************************************************************/

void free_tokenizer(struct Tokenizer *t)
{
    free(t->string);
    free(t);
}

/*****************************************************************************/
