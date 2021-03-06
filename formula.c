/*
    fp - formula.c

    Copyright (C) 2011 Matthias Ruester <ruester@molgen.mpg.de>
    Copyright (C) 2011 Max Planck Institut for Molecular Genetics

    fp is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    fp is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>

#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include "node.h"
#include "list.h"
#include "grammar.h"

/* calculates the value of a parse tree
 * 1. argument: pointer of the parse tree
 * return value: the value of the parse tree */
long double calculate_parse_tree(struct Node *root)
{
    long double left, right;

    switch (root->type) {
    case NUMBER:
        return (root->data.value);
        break;

    case OPERATOR:
        left = calculate_parse_tree(root->data.op.left);
        right = calculate_parse_tree(root->data.op.right);

        switch (root->data.op.operator) {
        case ADD:
            return (left + right);
            break;

        case MINUS:
            return (left - right);
            break;

        case MULTIPLY:
            return (left * right);
            break;

        case DIVIDE:
            return (left / right);
            break;

        case POWER:
            return (pow(left, right));
            break;

        case E_SYMBOL:
            return (left * pow(10.0, right));
            break;
        }
        break;

    case CONDITIONAL:
        if (calculate_parse_tree(root->data.con.condition))
            return (calculate_parse_tree(root->data.con.true));
        else
            return (calculate_parse_tree(root->data.con.false));
        break;
    }

    return (0);
}

/* finds all variables in a tree
 * and save them in the 2nd argument
 * 1. argument: pointer of the tree
 * 2. argument: adress of the pointer of a string
 *              (first call with empty string)
 * return value: none */
static void find_variables(struct Node *root, char **var)
{
    char *i;
    size_t temp;

    /* check type of node */
    switch (root->type) {
    case CONDITIONAL:
        /* traverse tree */
        find_variables(root->data.con.condition, var);
        find_variables(root->data.con.true, var);
        find_variables(root->data.con.false, var);
        break;

    case OPERATOR:
        /* traverse tree */
        find_variables(root->data.op.left, var);
        find_variables(root->data.op.right, var);
        break;

    case VARIABLE:
        for (i = *var; *i != '\0'; i++)
            if (*i == root->data.name)
                return;

        temp = strlen(*var);

        if ((*var = realloc(*var, (temp + 2)
                            * sizeof(char))) == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        (*var)[temp] = root->data.name;

        (*var)[temp + 1] = '\0';
        break;
    }
}

/* searches a tree for variables
 * 1. argument: pointer of the tree
 * return value: 0 if tree has no variables
 *               1 if tree has variables */
static int has_variables(struct Node *root)
{
    int ret;

    ret = 0;

    /* check type of node */
    switch (root->type) {
    case CONDITIONAL:
        /* check subtree */
        ret = has_variables(root->data.con.condition);

        if (ret)
            break;              /* subtree has a variable */

        ret = has_variables(root->data.con.true);

        if (ret)
            break;

        ret = has_variables(root->data.con.false);
        break;

    case OPERATOR:
        ret = has_variables(root->data.op.left);

        if (ret)
            break;

        ret = has_variables(root->data.op.right);
        break;

    case VARIABLE:
        /* leaf is a variable */
        return (1);
        break;
    }

    return (ret);
}

/* replaces one specific variable in a tree
 * 1. argument: variable
 * 2. argument: adress of the pointer of the tree
 * 3. argument: value of the variable (as a node)
 * return value: none */
static void replace(char b, struct Node **root, struct Node *n)
{
    /* check type of node */
    switch ((*root)->type) {
    case CONDITIONAL:
        /* traverse tree */
        replace(b, &((*root)->data.con.condition), n);
        replace(b, &((*root)->data.con.true), n);
        replace(b, &((*root)->data.con.false), n);
        break;

    case OPERATOR:
        /* traverse tree */
        replace(b, &((*root)->data.op.left), n);
        replace(b, &((*root)->data.op.right), n);
        break;

    case VARIABLE:
        /* check variable */
        if ((*root)->data.name == b)
            *root = memcpy(*root, n, sizeof(struct Node));
        break;
    }
}

/* remove trivial things like "0 * a" or "b - b"
 * 1. argument: pointer of the tree
 * return value: none */
void reduce(struct Node *root)
{
    struct Node *left, *right, *old,
        *current, *current2, *a, *b, *c, *d, *parent;
    struct List *all, *numbers, *operators, *variables;
    struct Element *rem;

    all = NULL;
    numbers = NULL;
    operators = NULL;

    if (root == NULL)
        return;

    sort_tree(root);

    switch (root->type) {
    case OPERATOR:
        reduce(root->data.op.left);
        reduce(root->data.op.right);

        left = root->data.op.left;
        right = root->data.op.right;

        if (left == NULL || right == NULL)
            return;

        switch (root->data.op.operator) {
        case ADD:
            if (right->type == NUMBER && left->type == NUMBER) {
                root->type = NUMBER;
                root->data.value = left->data.value + right->data.value;
                delete_node(left);
                delete_node(right);
                break;
            }

            if (left->type == NUMBER && left->data.value == 0.0) {
                memcpy(root, right, sizeof(struct Node));
                delete_node(left);
                delete_node(right);
                break;
            }

            if (right->type == NUMBER && right->data.value == 0.0) {
                memcpy(root, left, sizeof(struct Node));
                delete_node(right);
                delete_node(left);
                break;
            }

            if (cmp_nodes(left, right)) {
                root->data.op.operator = MULTIPLY;
                left->type = NUMBER;
                left->data.value = 2.0;
                break;
            }

            if (cmp_trees(left, right)) {
                root->data.op.operator = MULTIPLY;
                delete_tree(left);
                root->data.op.left = new_number_node(2.0);
                break;
            }

            all = get_operands(root, ADD);

            if (all == NULL)
                break;

            numbers = new_list();
            operators = new_list();
            variables = new_list();

            while (all->current != NULL) {
                current = all->current->node;

                if (current->type == OPERATOR)
                    add_node(operators, current);
                else if (current->type == NUMBER)
                    add_node(numbers, current);
                else if (current->type == VARIABLE)
                    add_node(variables, current);

                next_element(all);
            }

            rewind_list(operators);

            /* case: a*4+a*7 -> a*11 */
            while (operators->current != NULL) {
                current = operators->current->node;

                rem = operators->current;

                if (current->data.op.operator == MULTIPLY) {
                    next_element(operators);

                    while (operators->current != NULL) {
                        current2 = operators->current->node;

                        if (current2->data.op.operator == MULTIPLY) {
                            a = current->data.op.left;
                            b = current->data.op.right;
                            c = current2->data.op.left;
                            d = current2->data.op.right;

                            if (a->type == OPERATOR || b->type == OPERATOR
                                || c->type == OPERATOR
                                || d->type == OPERATOR) {
                                next_element(operators);
                                continue;
                            }

                            /* 4 cases: */
                            if (a->type == VARIABLE && b->type == NUMBER
                                && c->type == VARIABLE
                                && d->type == NUMBER)
                                b->data.value += d->data.value;
                            else if (a->type == VARIABLE
                                     && b->type == NUMBER
                                     && c->type == NUMBER
                                     && d->type == VARIABLE)
                                b->data.value += c->data.value;
                            else if (a->type == NUMBER
                                     && b->type == VARIABLE
                                     && c->type == VARIABLE
                                     && d->type == NUMBER)
                                a->data.value += d->data.value;
                            else if (a->type == NUMBER
                                     && b->type == VARIABLE
                                     && c->type == NUMBER
                                     && d->type == VARIABLE)
                                a->data.value += c->data.value;
                            else {
                                next_element(operators);
                                continue;
                            }

                            parent = get_parent(root, current2);

                            old = parent->data.op.left;
                            memcpy(parent, parent->data.op.left,
                                   sizeof(struct Node));
                            delete_node(old);

                            delete_tree(current2);
                        }

                        next_element(operators);
                    }
                }

                operators->current = rem;
                next_element(operators);
            }

            rewind_list(operators);

            /* case: 2*a+a -> 3*a */
            while (operators->current != NULL) {
                current = operators->current->node;

                if (current->data.op.operator == MULTIPLY) {
                    rewind_list(variables);

                    a = current->data.op.left;
                    b = current->data.op.right;

                    while (variables->current != NULL) {
                        c = variables->current->node;

                        /* 2 cases */
                        if (a->type == VARIABLE && b->type == NUMBER
                            && a->data.name == c->data.name)
                            b->data.value += 1.0;
                        else if (a->type == NUMBER && b->type == VARIABLE
                                 && b->data.name == c->data.name)
                            a->data.value += 1.0;
                        else {
                            next_element(variables);
                            continue;
                        }

                        parent = get_parent(root, c);

                        old = parent->data.op.left;
                        memcpy(parent, parent->data.op.left,
                               sizeof(struct Node));
                        delete_node(old);
                        delete_node(c);

                        next_element(variables);
                    }
                }

                next_element(operators);
            }

            delete_list_without_nodes(operators);
            delete_list_without_nodes(numbers);
            delete_list_without_nodes(variables);
            delete_list_without_nodes(all);
            break;

        case MINUS:
            if (left->type == NUMBER && right->type == NUMBER) {
                memcpy(root, left, sizeof(struct Node));
                root->data.value = left->data.value - right->data.value;
                delete_node(left);
                delete_node(right);
                break;
            }

            if (cmp_nodes(left, right)) {
                root->type = NUMBER;
                root->data.value = 0.0;
                delete_node(left);
                delete_node(right);
                break;
            }

            if (right->type == NUMBER && right->data.value == 0.0) {
                memcpy(root, left, sizeof(struct Node));
                delete_node(right);
                delete_node(left);
                break;
            }

            if (cmp_trees(left, right)) {
                root->type = NUMBER;
                root->data.value = 0.0;
                delete_tree(left);
                delete_tree(right);
                break;
            }

            all = get_operands(root, MINUS);

            if (all == NULL)
                break;

            delete_list_without_nodes(all);
            break;

        case MULTIPLY:
            if (left->type == NUMBER && right->type == NUMBER) {
                memcpy(root, left, sizeof(struct Node));
                root->data.value = left->data.value * right->data.value;
                delete_node(left);
                delete_node(right);
                break;
            }

            if ((left->type == NUMBER && left->data.value == 0.0)
                || (right->type == NUMBER && right->data.value == 0.0)) {
                root->type = NUMBER;
                root->data.value = 0.0;
                delete_tree(left);
                delete_tree(right);
                break;
            }

            if (left->type == NUMBER && left->data.value == 1.0) {
                memcpy(root, right, sizeof(struct Node));
                delete_node(left);
                delete_node(right);
                break;
            }

            if (right->type == NUMBER && right->data.value == 1.0) {
                memcpy(root, left, sizeof(struct Node));
                delete_node(right);
                delete_node(left);
                break;
            }

            if (cmp_nodes(left, right)) {
                root->data.op.operator = POWER;
                right->type = NUMBER;
                right->data.value = 2.0;
                break;
            }

            all = get_operands(root, MULTIPLY);

            if (all == NULL)
                break;

            delete_list_without_nodes(all);
            break;

        case DIVIDE:
            if (left->type == NUMBER && right->type == NUMBER) {
                memcpy(root, left, sizeof(struct Node));
                root->data.value = left->data.value / right->data.value;
                delete_node(left);
                delete_node(right);
                break;
            }

            if (left->type == NUMBER && left->data.value == 0.0) {
                root->type = NUMBER;
                root->data.value = 0.0;
                delete_tree(left);
                delete_tree(right);
                break;
            }

            if (right->type == NUMBER && right->data.value == 0.0) {
                root->type = NUMBER;
                root->data.value = 1.0 / 0.0;
                delete_node(left);
                delete_node(right);
                break;
            }

            if (cmp_nodes(left, right)) {
                root->type = NUMBER;
                root->data.value = 1.0;
                delete_node(left);
                delete_node(right);
                break;
            }

            if (right->type == NUMBER && right->data.value == 1.0) {
                memcpy(root, left, sizeof(struct Node));
                delete_node(right);
                delete_node(left);
                break;
            }

            if (cmp_trees(left, right)) {
                root->type = NUMBER;
                root->data.value = 1.0;
                delete_tree(left);
                delete_tree(right);
                break;
            }

            all = get_operands(root, DIVIDE);

            if (all == NULL)
                break;

            delete_list_without_nodes(all);
            break;

        case POWER:
            if (left->type == NUMBER && right->type == NUMBER) {
                memcpy(root, left, sizeof(struct Node));
                root->data.value =
                    pow(left->data.value, right->data.value);
                delete_node(left);
                delete_node(right);
                break;
            }

            if (left->type == NUMBER && left->data.value == 0.0) {
                root->type = NUMBER;
                root->data.value = 0.0;
                delete_node(left);
                delete_node(right);
                break;
            }

            if (right->type == NUMBER && right->data.value == 0.0) {
                root->type = NUMBER;
                root->data.value = 1.0;
                delete_node(left);
                delete_node(right);
                break;
            }

            if (right->type == NUMBER && right->data.value == 1.0) {
                memcpy(root, left, sizeof(struct Node));
                delete_node(left);
                delete_node(right);
                break;
            }

            if (left->type == NUMBER && left->data.value == 1.0) {
                memcpy(root, left, sizeof(struct Node));
                delete_node(right);
                delete_node(left);
                break;
            }

            all = get_operands(root, POWER);

            if (all == NULL)
                break;

            delete_list_without_nodes(all);
            break;

        case E_SYMBOL:
            if (left->type == NUMBER && right->type == NUMBER) {
                root->type = NUMBER;
                root->data.value =
                    left->data.value * pow(10.0, right->data.value);
                delete_node(left);
                delete_node(right);
                break;
            }
            break;
        }
        break;

    case CONDITIONAL:
        old = root->data.con.condition;
        left = root->data.con.true;
        right = root->data.con.false;

        reduce(root->data.con.condition);
        reduce(root->data.con.true);
        reduce(root->data.con.false);

        if (root->data.con.condition->type == NUMBER) {
            if (root->data.con.condition->data.value) {
                memcpy(root, left, sizeof(struct Node));
                delete_node(old);
                delete_node(left);
                delete_tree(right);
            } else {
                memcpy(root, right, sizeof(struct Node));
                delete_node(old);
                delete_tree(left);
                delete_node(right);
            }
        }
        break;
    }
}

/* replace all variables in a tree by asking the user for values
 * 1. argument: adress of the pointer of the tree
 * return value: none
 */
void replace_variables(struct Node **root)
{
    char *variables, *i;
    char input[MAX_INPUT];
    struct Node *value;

    while (has_variables(*root)) {
        /* create emtpy string */
        if ((variables = calloc(1, sizeof(char))) == NULL) {
            perror("calloc");
            exit(EXIT_FAILURE);
        }

        /* find all variables in tree */
        find_variables(*root, &variables);

        for (i = variables; *i != '\0'; i++) {
            /* ask for value of variable */
            printf("value of variable %c: ", *i);
            fgets(input, MAX_INPUT - 1, stdin);
            input[strlen(input) - 1] = '\0';

            /* create parse tree */
            value = parse(input);

            if (value == NULL) {
                fprintf(stderr, "cannot create parse tree\n");
                i--;
                continue;
            }

            reduce(value);

            /* replace variable in tree */
            replace(*i, root, value);

            delete_node(value);
        }

        /* free memory of string */
        free(variables);
    }
}
