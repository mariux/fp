/*
    formelparser - grammar.c

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

#include <stdio.h>
#include <math.h>
#include <ctype.h>

#include "node.h"
#include "grammar.h"

/* Prototype of static functions */

static GRAMMAR_PARSER(T);
static GRAMMAR_PARSER(S);
static GRAMMAR_PARSER(P);
static GRAMMAR_PARSER(O);
static GRAMMAR_PARSER(K);
static GRAMMAR_PARSER(Num);
static GRAMMAR_PARSER(N);
static GRAMMAR_PARSER(Z);
static GRAMMAR_PARSER(Var);
static GRAMMAR_PARSER(B);

struct Node *parse(char *string)
{
    struct Tokenizer *tokenizer;
    struct Node      *root;
    int position;
    char c;
    
    tokenizer = create_tokenizer(string);
    
    root     = T(tokenizer);
    
    position = tokenizer->position;
    c        = CURRENT_TOKEN;
    
    free_tokenizer(tokenizer);

    if(!root) {
        if(c == '\0')
            printf("unexpected end of string\n");
        else
            printf("syntax error at position %d near '%c'\n", position, c);
        
        return(NULL);
    }
    
    if(c != '\0') {
        printf("expecting end at position %d near '%c'\n", position, c);
        delete_tree(root);
        return(NULL);
    }

    return(root);
}

/*
 * Grammar:
 * T   -> S | S ? S : S
 * S   -> P | P + P | P - P
 * P   -> O | O * O | O / O | OVar
 * O   -> K | K ^ K
 * K   -> -K | (T) | Num | Var
 * Num -> N | N 'E' Z | N 'E' - Z
 * N   -> Z | Z . Z
 * Z   -> [0-9]+
 * Var -> B | BZ
 * B   -> [a-z]
 */
 
/*
 * function for conditional expression operators
 * T -> S | S '?' S ':' S
 */
GRAMMAR_PARSER(T)
{
    struct Node *condition, 
                *true, 
                *false;
    
    condition = S(tokenizer);
    
    if(!condition)
        /* syntax error in S -> return error */
        return(NULL);
    
    while(CURRENT_TOKEN == '?') {
        SKIP_TOKEN;
        
        true = S(tokenizer);
        
        if(!true) {
            /* syntax error in S -> cleanup -> return error */
            delete_tree(condition);
            return(NULL);            
        }
        
        if(CURRENT_TOKEN != ':') {
            /* local syntax error -> cleanup -> return error */
            delete_tree(true);
            delete_tree(condition);
            return(NULL);
        }
        
        SKIP_TOKEN;
        
        false = S(tokenizer);
        
        if(!false) {
            /* syntax error in S -> cleanup -> return error */
            delete_tree(condition);
            delete_tree(true);
            return(NULL);
        }

        condition = new_conditional_node(condition, true, false);
    }
    
    return(condition);
}

/*
 * function for addition and subtraction signs
 * S -> P | P '+' P | P '-' P
 */
static GRAMMAR_PARSER(S)
{
    struct Node *subtree, 
                *right, 
                *op;
    
    subtree = P(tokenizer);
    
    if(!subtree)
        return(NULL);
    
    while(CURRENT_TOKEN == '+' || CURRENT_TOKEN == '-') {
        op = new_operator_node(CURRENT_TOKEN);
        
        SKIP_TOKEN;
        
        right = P(tokenizer);
        
        if(!right) {
            /* cleanup and return Error */
            delete_node(op);
            delete_tree(subtree);
            return(NULL);
        }
        
        subtree = set_childs(op, subtree, right);
    }
    
    return(subtree);
}

/*
 * function for multiplication and division signs
 * P -> O | O '*' O | O '/' O | OVar
 */
static GRAMMAR_PARSER(P)
{
    struct Node *subtree, *right, *op;
    
    subtree = O(tokenizer);
    
    if(!subtree)
        return(NULL);
    
    while(islower(CURRENT_TOKEN)) {
        right = Var(tokenizer);
        
        if(!right) {
            delete_node(subtree);
            return(NULL);
        }
        
        subtree = set_childs(new_operator_node('*'),
                             subtree, right);
    }
    
    while(CURRENT_TOKEN == '*' || CURRENT_TOKEN == '/') {
        op = new_operator_node(CURRENT_TOKEN);
        
        SKIP_TOKEN;
        
        right = O(tokenizer);
        
        if(!right) {
            delete_node(op);
            delete_tree(subtree);
            return(NULL);
        }
        
        subtree = set_childs(op, subtree, right);
    }
    
    return(subtree);
}

/*
 * function for exponentiations
 * O -> K | K '^' K
 */
static GRAMMAR_PARSER(O)
{
    struct Node *subtree, *right;
    
    subtree = K(tokenizer);
    
    if(!subtree)
        return(NULL);
    
    while(CURRENT_TOKEN == '^') {
        SKIP_TOKEN;
        
        right = K(tokenizer);
        
        if(!right) {
            delete_tree(subtree);
            return(NULL);
        }
        
        subtree = set_childs(new_operator_node('^'), subtree, right);
    }
    
    return(subtree);
}

/*
 * function for signed terms, braces, numbers and variables
 * K -> -K | (T) | Num | Var
 */
static GRAMMAR_PARSER(K)
{
    struct Node *subtree;
    
    /* K -> -K */
    if(CURRENT_TOKEN == '-') {
        SKIP_TOKEN;

        subtree = K(tokenizer);

        if(!subtree)
            return(NULL);

        subtree  = set_childs(new_operator_node('*'),
                              new_number_node(-1),
                              subtree);
        return(subtree);
    }

    /* K -> (T) */
    if(CURRENT_TOKEN == '(') {
        SKIP_TOKEN;
        
        subtree = T(tokenizer);
        
        if(!subtree)
            return(NULL);
        
        if(CURRENT_TOKEN == ')') {
            SKIP_TOKEN;
            return(subtree);
        }
        
        /* cleanup and return Error */
        delete_tree(subtree);
        return(NULL);
    }
    
    
    /* K -> Num */
    if(isdigit(CURRENT_TOKEN)) {
        subtree = Num(tokenizer); 
        return(subtree);
    }
    
    /* K -> Var */
    if(islower(CURRENT_TOKEN)) {
        subtree = Var(tokenizer);
        return(subtree);
    }
    
    /* Syntax Error */
    return(NULL);
}

/*
 * function for numbers (maybe with an 'E')
 * Num -> N | N 'E' Z | N 'E' '-' Z
 */
static GRAMMAR_PARSER(Num)
{
    struct Node *subtree, *right;
    
    subtree = N(tokenizer); /* get number */
    
    if(!subtree)
        return(NULL);
    
    if(CURRENT_TOKEN == 'E') {
        /* skip E symbol */
        SKIP_TOKEN;        

        if(CURRENT_TOKEN == '-') {
            /* skip subtraction sign */
            SKIP_TOKEN;            

            /* get number */
            right = Z(tokenizer);
            
            if(!right) {
                delete_tree(subtree);
                return(NULL);
            }
            
            right = set_childs(new_operator_node('*'),
                               new_number_node(-1), right);
            
            subtree = set_childs(new_operator_node('E'),
                                 subtree, right);
        } else {
            /* get number */
            right = Z(tokenizer);
            
            if(!right) {
                delete_tree(subtree);
                return(NULL);
            }
            
            subtree = set_childs(new_operator_node('E'),
                                 subtree, right);
            printf("subtree->right = %Lf\n", subtree->data.op.right->data.value);
        }
    }
    
    return(subtree);
}

/*
 * function for numbers
 * N -> Z | Z '.' Z
 */
static GRAMMAR_PARSER(N)
{
    struct Node *subtree;
    long double nr;
    int digits;
    
    subtree = Z(tokenizer);
    
    if(!subtree)
        return(NULL);
    
    if(CURRENT_TOKEN == '.' || CURRENT_TOKEN == ',') {
        /* skip point */
        SKIP_TOKEN;        

        nr = subtree->data.value;
        digits = 0;
        
        while(isdigit(CURRENT_TOKEN)) {
            digits++;
            
            nr += (CURRENT_TOKEN - '0') / pow(10.0, digits);
            
            SKIP_TOKEN;
        }
        
        subtree->data.value = nr;
    }
    
    return(subtree);
}

/*
 * function for digits
 * Z -> [0-9]+
 */
static GRAMMAR_PARSER(Z)
{
    struct Node *subtree;
    long double number;
    
    subtree = NULL;
    number = 0.0;
    
    if(!isdigit(CURRENT_TOKEN))
        return(NULL);
    
    while(isdigit(CURRENT_TOKEN)) {    
        number = number * 10.0 + (CURRENT_TOKEN - '0');
        
        SKIP_TOKEN;
    }
    
    subtree = new_number_node(number);
    
    return(subtree);
}

/*
 * function for variables
 * Var -> B | BZ
 */
static GRAMMAR_PARSER(Var)
{
    struct Node *subtree, *right;
    
    subtree = B(tokenizer);
        
    if(!subtree)
        return(NULL);
    
    if(isdigit(CURRENT_TOKEN)) {
        right = Z(tokenizer);
        
        if(!right) {
            delete_tree(subtree);
            return(NULL);
        }
        
        subtree = set_childs(new_operator_node('^'),
                             subtree, right);
    }
    
    return(subtree);
}

/*
 * function for characters
 * B -> [a-z]
 */
static GRAMMAR_PARSER(B)
{
    struct Node *subtree;
    
    subtree = NULL;
    
    if(islower(CURRENT_TOKEN)) {
        subtree = new_variable_node(CURRENT_TOKEN);
        
        SKIP_TOKEN;
    }
    
    return(subtree);
}
