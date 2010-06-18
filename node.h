/*
    formelparser - node.h

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

#ifndef FORMELPARSER_NODE_H
#define FORMELPARSER_NODE_H

/* node types */
#define OPERATOR     0
#define NUMBER       1
#define VARIABLE     2
#define CONDITIONAL  3

/* Operatoren */
#define ADD       4
#define MINUS     5
#define MULTIPLY  6
#define DIVIDE    7
#define POWER     8
#define E_SYMBOL  9

/* Error */
#define ERROR 10

struct Operator {
    int          operator;
    struct Node *left;
    struct Node *right;
};

struct Conditional {
    struct Node *condition;
    struct Node *true;
    struct Node *false;
};

union Data {
    long double value;
    
    char name;
    
    struct Conditional con;
    struct Operator op;
};

struct Node {
    int   type;
    char *formula;
    
    union Data data;
};

extern struct Node *new_operator_node(char);
extern struct Node *new_variable_node(char);
extern struct Node *new_number_node(double);
extern struct Node *new_conditional_node(struct Node *, struct Node *, struct Node *);
extern struct Node *new_node(void);
extern void delete_node(struct Node *);
extern void delete_tree(struct Node *);
extern struct Node *set_childs(struct Node *, struct Node *, struct Node *);
extern char otoa(int);
extern int atoo(char);
extern int cmp_nodes(struct Node *, struct Node *);
extern void print_node(struct Node *);
extern char *get_formula(struct Node *);
extern int cmp_trees(struct Node *, struct Node *);
extern struct Node *get_parent(struct Node *, struct Node *);
extern void print_formula(struct Node *, int);
extern void sort_tree(struct Node *);
extern struct List *get_operands(struct Node *, int);
extern void update(struct Node *);

#endif /* FORMELPARSER_NODE_H */
