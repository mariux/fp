/*
    formelparser - node.c

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

#include <math.h>   /* for HUGE_VAL */
#include <stdlib.h> /* for exit */
#include <string.h> /* for strdup/memcpy/strlen/... */
#include <stdio.h>  /* for printf/sprintf/... */

#include "node.h"
#include "list.h"

struct Node *new_node(void)
{
    struct Node *n;
    
    n = calloc(1, sizeof(struct Node));
    
    if(n == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }
    
    return(n);
}

struct Node *new_operator_node(char op)
{
    struct Node *n;
    
    n = new_node();
    
    n->type = OPERATOR;
    n->data.op.operator = atoo(op);
    
    return(n);
}

struct Node *new_variable_node(char var)
{
    struct Node *n;
    
    n = new_node();
    
    n->type = VARIABLE;
    n->data.name = var;
    
    return(n);
}

struct Node *new_number_node(double nr)
{
    struct Node *n;
    
    n = new_node();
    
    n->type = NUMBER;
    n->data.value = nr;
    
    return(n);
}

struct Node *new_conditional_node(struct Node *condition, struct Node *true, struct Node *false)
{
    struct Node *node;
    
    node = new_node();
    
    node->type               = CONDITIONAL;
    node->data.con.condition = condition;
    node->data.con.true      = true;
    node->data.con.false     = false;
    
    return(node);
}

void delete_node(struct Node *old)
{
    if(old == NULL)
        return;

    free(old->formula);
    free(old);
}

void delete_tree(struct Node *old)
{
    if(old == NULL)
        return;
    
    switch(old->type) {
        case OPERATOR:
            delete_tree(old->data.op.left);
            delete_tree(old->data.op.right);
            break;
        
        case CONDITIONAL:
            delete_tree(old->data.con.condition);
            delete_tree(old->data.con.true);
            delete_tree(old->data.con.false);
	    break;
        
        case ERROR:
            printf("ERROR\n");
            break;
    }
    
    /* leaf */
    delete_node(old);
}

struct Node *set_childs(struct Node *root, struct Node *left, struct Node *right)
{
    if(root->type == OPERATOR) {
        root->data.op.left  = left;
        root->data.op.right = right;
    }
    
    return(root);
}

char otoa(int operator)
{
    switch(operator) {
        case ADD:
            return('+');
        
        case MINUS:
            return('-');
        
        case MULTIPLY:
            return('*');
        
        case DIVIDE:
            return('/');
        
        case POWER:
            return('^');
        
        case E_SYMBOL:
            return('E');
        
        default:
            return('#');
    }
}

int atoo(char operator)
{
    switch(operator) {
        case '+':
            return(ADD);
        
        case '-':
            return(MINUS);
        
        case '*':
            return(MULTIPLY);
        
        case '/':
            return(DIVIDE);
        
        case '^':
            return(POWER);
        
        case 'E':
            return(E_SYMBOL);
        
        default:
            return(ERROR);
    }
}

void print_node(struct Node *root)
{
    if(root == NULL) {
        printf("NULL");
        return;
    }
    
    switch(root->type) {
        case CONDITIONAL:
            printf("?");
            break;
        
        case OPERATOR:
            switch(root->data.op.operator) {
                case ADD: 
		    printf("+");
                    break;
                
                case MINUS: 
		    printf("-");
                    break;
                
                case MULTIPLY: 
		    printf("*");
                    break;
                
                case DIVIDE: 
		    printf("/");
                    break;
                
                case E_SYMBOL: 
		    printf("E");
                    break;
                
                case POWER: 
		    printf("^");
                    break;
            }
            break;
        
        case NUMBER:
            printf("%.2Lf", root->data.value);
            break;
        
        case VARIABLE:
            printf("%c", root->data.name);
            break;
    }
}

void print_tree(struct Node *root)
{
    if(root == NULL)
        return;
    
    switch(root->type) {
        case CONDITIONAL:
            printf("? ");
            printf("(condition: ");
            print_node(root->data.con.condition);
            printf(" true: ");
            print_node(root->data.con.true);
            printf(" false: ");
            print_node(root->data.con.false);
            printf(")\n");
            break;
        
        case OPERATOR:
            print_tree(root->data.op.left);
            print_tree(root->data.op.right);
            
            switch(root->data.op.operator) {
                case ADD:
                    printf("+ ");
                    printf("(left: ");
                    print_node(root->data.op.left);
                    printf(" right: ");
                    print_node(root->data.op.right);
                    printf(")\n");
                    break;
                
                case MINUS:
                    printf("- ");
                    printf("(left: ");
                    print_node(root->data.op.left);
                    printf(" right: ");
                    print_node(root->data.op.right);
                    printf(")\n");
                    break;
                
                case MULTIPLY:
                    printf("* ");
                    printf("(left: ");
                    print_node(root->data.op.left);
                    printf(" right: ");
                    print_node(root->data.op.right);
                    printf(")\n");
                    break;
                
                case DIVIDE:
                    printf("/ ");
                    printf("(left: ");
                    print_node(root->data.op.left);
                    printf(" right: ");
                    print_node(root->data.op.right);
                    printf(")\n");
                    break;
                
                case E_SYMBOL:
                    printf("E ");
                    printf("(left: ");
                    print_node(root->data.op.left);
                    printf(" right: ");
                    print_node(root->data.op.right);
                    printf(")\n");
                    break;
                
                case POWER:
                    printf("^ ");
                    printf("(left: ");
                    print_node(root->data.op.left);
                    printf(" right: ");
                    print_node(root->data.op.right);
                    printf(")\n");
                    break;
            }
            break;
        
        case NUMBER:
            /* printf("%d\n", root->data.value); */
            break;
        
        case VARIABLE:
            /* printf("%c\n", root->data.name); */
            break;
        
        default:
            printf("ERROR\n");
            break;
    }
}

long double my_fabs(long double d)
{
    return(d < 0 ? -d : d);    
}

char *ldtostr(long double d)
{
    unsigned int digits;
    char *ret;
    long double h;
    
    if(d == HUGE_VAL || d == -HUGE_VAL) {
        return(strdup("inf"));
    }
    
    if(d == 0.0) {
        digits = 1;
    } else {
        digits = 0;
    }
    
    if(d < 0) {
        digits++;
    }
    
    for(h = d; my_fabs(h) > 0.0; h /= 10.0) {
        digits++;
    }
    
    /* digit-characters '.' precision-characters '\0' */
    if((ret = calloc(digits + 1 + 65 + 1, sizeof(char))) == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }
    
    sprintf(ret, "%.65Lf", d);
    
    return(ret);
}

char *get_formula(struct Node *root)
{
    static int f = 0;
    static char *formula = NULL;
    char temp[2];
    char *h;
    
    if(root == NULL) {
        return(NULL);
    }
    
    if(f == 0 && (formula = calloc(1, sizeof(char))) == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }
    
    f++;
    
    switch(root->type) {
        case CONDITIONAL:
            if((formula = realloc(formula, (strlen(formula) + 2) * sizeof(char))) == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            
            strcat(formula, "(");
            get_formula(root->data.con.condition);
            
            if((formula = realloc(formula, (strlen(formula) + 2) * sizeof(char))) == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            
            strcat(formula, ")?(");
            get_formula(root->data.con.true);
            
            if((formula = realloc(formula, (strlen(formula) + 2) * sizeof(char))) == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            
            strcat(formula, "):(");
            get_formula(root->data.con.false);
            
            if((formula = realloc(formula, (strlen(formula) + 2) * sizeof(char))) == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            
            strcat(formula, ")");
            break;
        
        case OPERATOR:
            if(root->data.op.left->type == OPERATOR && root->data.op.left->data.op.operator != root->data.op.operator) {
                if((formula = realloc(formula, (strlen(formula) + 2) * sizeof(char))) == NULL) {
                    perror("realloc");
                    exit(EXIT_FAILURE);
                }

                strcat(formula, "(");
            }
            
            get_formula(root->data.op.left);
            
            if(root->data.op.left->type == OPERATOR  && root->data.op.left->data.op.operator != root->data.op.operator) {
                if((formula = realloc(formula, (strlen(formula) + 2) * sizeof(char))) == NULL) {
                    perror("realloc");
                    exit(EXIT_FAILURE);
                }

                strcat(formula, ")");
            }
            
            sprintf(temp, "%c", otoa(root->data.op.operator));
            temp[1] = '\0';
            
            if((formula = realloc(formula, (strlen(formula) + 2) * sizeof(char))) == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            
            strcat(formula, temp);
            
            if(root->data.op.right->type == OPERATOR && root->data.op.right->data.op.operator != root->data.op.operator) {
                if((formula = realloc(formula, (strlen(formula) + 2) * sizeof(char))) == NULL) {
                    perror("realloc");
                    exit(EXIT_FAILURE);
                }

                strcat(formula, "(");
            }
            
            get_formula(root->data.op.right);
            
            if(root->data.op.right->type == OPERATOR  && root->data.op.right->data.op.operator != root->data.op.operator) {
                if((formula = realloc(formula, (strlen(formula) + 2) * sizeof(char))) == NULL) {
                    perror("realloc");
                    exit(EXIT_FAILURE);
                }

                strcat(formula, ")");
            }
            break;
        
        case NUMBER:
            h = ldtostr(root->data.value);
            
            if((formula = realloc(formula, (strlen(formula) + strlen(h) + 1) * sizeof(char))) == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            
            strcat(formula, h);
            
            free(h);
            break;
        
        case VARIABLE:
            if((formula = realloc(formula, (strlen(formula) + 2) * sizeof(char))) == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            
            sprintf(temp, "%c", root->data.name);
            temp[1] = '\0';
            strcat(formula, temp);
            break;
        
        case E_SYMBOL:
            if((formula = realloc(formula, (strlen(formula) + 2) * sizeof(char))) == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            
            sprintf(temp, "E");
            temp[1] = '\0';
            strcat(formula, temp);
            break;
        
        default:
            printf("ERROR\n");
            break;
    }
    
    f--;
    
    if(f == 0) {
        return(formula);
    }
    
    return(NULL);
}

int cmp_nodes(struct Node *n1, struct Node *n2)
{
    if(n1->type == n2->type) {
        switch(n1->type) {
            case NUMBER:
                if(n1->data.value == n2->data.value) {
                    return(1);
		}
                break;
            
            case VARIABLE:
                if(n1->data.name == n2->data.name) {
                    return(1);
		}
                break;
        }
    }
        
    return(0);
}

struct Node *get_parent(struct Node *root, struct Node *search)
{
    struct Node *parent;
    
    if(root == NULL) {
        return(NULL);
    }
    
    switch(root->type) {
        case OPERATOR:
            if((parent = get_parent(root->data.op.left, search)) != NULL
               || (parent = get_parent(root->data.op.right, search)) != NULL)
                return(parent);
            
            if(root->data.op.left == search
               || root->data.op.right == search)
                return(root);
            break;
        
        case NUMBER:

#ifdef DEBUG
            if(root == search)
                printf("match\n");
#endif

            break;
        
        case CONDITIONAL:
            if((parent = get_parent(root->data.con.condition, search)) != NULL
               || (parent = get_parent(root->data.con.true, search)) != NULL
               || (parent = get_parent(root->data.con.false, search)) != NULL) {
                return(parent);
            }
            if(root->data.con.condition == search
              || root->data.con.true == search
              || root->data.con.false == search) {
                return(root);
	    }
            break;
    }
    
    return(NULL);
}

int cmp_trees(struct Node *a, struct Node *b)
{
    int ret;
    char *f, *g;
    
    f = get_formula(a);
    g = get_formula(b);
    
    ret = strcmp(f, g);
    
    free(f);
    free(g);
    
    return(!ret);
}

void sort_numbers(struct List *l)
{
    struct Element *current, *current2;
    long double temp;
    
    current = l->first;
    
    while(current != NULL) {
        current2 = current->next;
        
        while(current2 != NULL) {
            if(current2->node->data.value < current->node->data.value) {
                temp = current2->node->data.value;
                current2->node->data.value = current->node->data.value;
                current->node->data.value = temp;
            }
            
            current2 = current2->next;
        }
        
        current = current->next;
    }
}

void sort_variables(struct List *l)
{
    struct Element *current, *current2;
    char temp;
    
    current = l->first;
    
    while(current != NULL) {
        current2 = current->next;
        
        while(current2 != NULL) {
            if(current2->node->data.name < current->node->data.name) {
                temp = current2->node->data.name;
                current2->node->data.name = current->node->data.name;
                current->node->data.name = temp;
            }
            
            current2 = current2->next;
        }
        
        current = current->next;
    }
}

void sort_operators(struct List *l)
{
    struct Element *current, *current2;
    struct Node *temp;
    
    temp = new_operator_node('#');
    
    current = l->first;
    
    while(current != NULL) {
        current2 = current->next;
        
        while(current2 != NULL) {
            if(current2->node->data.op.operator < current->node->data.op.operator) {
                memcpy(temp, current->node, sizeof(struct Node));
                memcpy(current->node, current2->node, sizeof(struct Node));
                memcpy(current2->node, temp, sizeof(struct Node));
            }
            
            current2 = current2->next;
        }
        
        current = current->next;
    }
    
    delete_node(temp);
}

void sort_conditional(struct List *l)
{
    /* no idea */
}

void sort(struct Node *root, struct List *l, int operator)
{
    if(root == NULL) {
        return;
    }
    
    if(root->type == OPERATOR && root->data.op.operator == operator) {
        if(root->data.op.left->type == OPERATOR && root->data.op.left->data.op.operator == operator
           && root->data.op.right->type == OPERATOR && root->data.op.right->data.op.operator == operator) {
            sort(root->data.op.left, l, operator);
            sort(root->data.op.right, l, operator);
            return;
        }
        
        if(root->data.op.right->type == OPERATOR && root->data.op.right->data.op.operator == operator) {
            sort(root->data.op.right, l, operator);
            
            if(l->current == NULL) {
                printf("ERROR\n");
                exit(EXIT_FAILURE);
            }
            
            root->data.op.left = l->current->node;
            next_element(l);
            
            return;
        }
        
        if(root->data.op.left->type == OPERATOR && root->data.op.left->data.op.operator == operator) {
            sort(root->data.op.left, l, operator);
            
            if(l->current == NULL) {
                printf("ERROR\n");
                exit(EXIT_FAILURE);
            }
            
            root->data.op.right = l->current->node;
            next_element(l);
            
            return;
        }
        
        root->data.op.left = l->current->node;
        next_element(l);

        if(l->current == NULL) {
            printf("ERROR\n");
            exit(EXIT_FAILURE);
        }

        root->data.op.right = l->current->node;
        next_element(l);
    }
}

void sort_tree(struct Node *root)
{
    struct List *variables, *operators, *numbers,
                *operands, *sorted, *conditional;
    
    operands = NULL;
    
    switch(root->type) {
        case CONDITIONAL:
            sort_tree(root->data.con.condition);
            sort_tree(root->data.con.true);
            sort_tree(root->data.con.false);
            break;
        
        case OPERATOR:
            sort_tree(root->data.op.left);
            sort_tree(root->data.op.right);
            
            if(root->data.op.operator == MINUS
               || root->data.op.operator == DIVIDE
               || root->data.op.operator == POWER)
                return; /* do not sort */
            
            variables = new_list();
            operators = new_list();
            numbers = new_list();
            conditional = new_list();
            sorted = new_list();
            
            operands = get_operands(root, root->data.op.operator);
            
            if(operands == NULL) {
                printf("ERROR\n");
                exit(EXIT_FAILURE);
            }
            
            rewind_list(operands);
            
            while(operands->current != NULL) {
                if(operands->current->node->type == NUMBER)
                    add_node(numbers, operands->current->node);
                
                if(operands->current->node->type == VARIABLE)
                    add_node(variables, operands->current->node);
                    
                if(operands->current->node->type == OPERATOR)
                    add_node(operators, operands->current->node);
                
                if(operands->current->node->type == CONDITIONAL)
                    add_node(conditional, operands->current->node);
                
                next_element(operands);
            }
            
            sort_numbers(numbers);
            sort_variables(variables);
            sort_operators(operators);
            sort_conditional(conditional);
            
            while(numbers->current != NULL) {
                add_node(sorted, numbers->current->node);
                next_element(numbers);
            }
            
            while(variables->current != NULL) {
                add_node(sorted, variables->current->node);
                next_element(variables);
            }
            
            while(operators->current != NULL) {
                add_node(sorted, operators->current->node);
                next_element(operators);
            }
            
            while(conditional->current != NULL) {
                add_node(sorted, conditional->current->node);
                next_element(conditional);
            }
            
            rewind_list(sorted);
            sort(root, sorted, root->data.op.operator);
            
            delete_list_without_nodes(variables);
            delete_list_without_nodes(operators);
            delete_list_without_nodes(numbers);
            delete_list_without_nodes(operands);
            delete_list_without_nodes(conditional);
            delete_list_without_nodes(sorted);
            break;
    }
}

void update(struct Node *root)
{
    if(root == NULL)
        return;
    
    switch(root->type) {
        case CONDITIONAL:
            update(root->data.con.condition);
            update(root->data.con.true);
            update(root->data.con.false);
            
            if(root->formula != NULL)
                free(root->formula);
            
            root->formula = get_formula(root);
            break;
        
        case OPERATOR:
            update(root->data.op.left);
            update(root->data.op.right);
            
            if(root->formula != NULL)
                free(root->formula);
            
            root->formula = get_formula(root);
            break;
    }
}

void print_formula(struct Node *root, int precision)
{
    static int f = 0;
    
    if(root == NULL)
        return;
    
    f++;
    
    switch(root->type) {
        case CONDITIONAL:
            printf("(");
            print_formula(root->data.con.condition, precision);
            printf(")?(");
            print_formula(root->data.con.true, precision);
            printf("):(");
            print_formula(root->data.con.false, precision);
            printf(")");
            break;
        
        case OPERATOR:
            if(root->data.op.left->type == OPERATOR && root->data.op.left->data.op.operator != root->data.op.operator)
                printf("(");
                
            print_formula(root->data.op.left, precision);
            
            if(root->data.op.left->type == OPERATOR && root->data.op.left->data.op.operator != root->data.op.operator)
                printf(")");
            
            printf("%c", otoa(root->data.op.operator));
            
            if(root->data.op.right->type == OPERATOR && root->data.op.right->data.op.operator != root->data.op.operator)
                printf("(");
            
            print_formula(root->data.op.right, precision);
            
            if(root->data.op.right->type == OPERATOR && root->data.op.right->data.op.operator != root->data.op.operator)
                printf(")");
            break;
        
        case NUMBER:
            printf("%.*Lf", precision, root->data.value);
            break;
        
        case VARIABLE:
            printf("%c", root->data.name);
            break;
        
        default:
            printf("ERROR\n");
            break;
    }
    
    f--;
    
    if(f == 0)
        printf("\n");
}

void add_subtrees(struct Node *root, struct List *l, int operator)
{
    if(root == NULL)
        return;
    
    if(root->type == OPERATOR && root->data.op.operator == operator) {
        if(root->data.op.left->type == OPERATOR && root->data.op.left->data.op.operator == operator
           && root->data.op.right->type == OPERATOR && root->data.op.right->data.op.operator == operator) {
           add_subtrees(root->data.op.left, l, operator);
           add_subtrees(root->data.op.right, l, operator);
           return;
        }
        
        if(root->data.op.right->type == OPERATOR && root->data.op.right->data.op.operator == operator) {
            add_subtrees(root->data.op.right, l, operator);
            add_node(l, root->data.op.left);
            return;
        }
        
        if(root->data.op.left->type == OPERATOR && root->data.op.left->data.op.operator == operator) {
            add_subtrees(root->data.op.left, l, operator);
            add_node(l, root->data.op.right);
            return;
        }
        
        add_node(l, root->data.op.left);
        add_node(l, root->data.op.right);
    }
}

struct List *get_operands(struct Node *root, int operator)
{
    struct List *l;
    
    l = new_list();
    
    add_subtrees(root, l, operator);

#ifdef DEBUG
    print_list(l);
#endif

    if(l->count == 0) {
        free(l);
        return(NULL);
    }
    
    return(l);
}
