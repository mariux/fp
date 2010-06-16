/*
    formelparser - node.h

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

#ifndef FORMELPARSER_NODE_H
#define FORMELPARSER_NODE_H

/* Knoten-Typen */
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

extern struct Node *new_operator_node(char op);
extern struct Node *new_variable_node(char var);
extern struct Node *new_number_node(double nr);
extern struct Node *new_ternary_node(void);
extern struct Node *new_node(void);
extern void delete_node(struct Node *old);
extern void delete_tree(struct Node *old);
extern struct Node *set_childs(struct Node *root, struct Node *left, struct Node *right);
extern struct Node *set_3childs(struct Node *root, struct Node *cond, struct Node *t, struct Node *f);
extern char otoa(int operator);
extern int atoo(char op);
extern int cmp_nodes(struct Node *n1, struct Node *n2);
extern void print_node(struct Node *root);
extern char *get_formula(struct Node *root);
extern int cmp_trees(struct Node *a, struct Node *b);
extern struct Node *get_parent(struct Node *root, struct Node *search);
extern void print_formula(struct Node *root, int precision);
extern void sort_tree(struct Node *root);
extern struct List *get_operands(struct Node *root, int operator);
extern void update(struct Node *root);

#endif /* FORMELPARSER_NODE_H */
