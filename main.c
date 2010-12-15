/*
    formula parser - main.c

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

#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "grammar.h"
#include "formelparser.h"

void print_usage()
{
    printf("usage: formelparser [OPTIONS] FORMULA...\n"
           "possible options:\n"
           "    -f [FILE]         read folmulas from file\n"
           "    -p [PRECISION]    set the precision of the output\n"
           "    -n                just print results\n");
}

int main(int argc, char *argv[])
{
    struct Node *parse_tree;
    long double result;
    int i;
    short precision;
    char fromfile, just_print, c;
    char read[LINE_MAX];
    char *term, *filename;
    FILE *file;
    
    filename = term = NULL;
    fromfile = just_print = 0;
    i = 1;
    precision = 5;
    
    /* no arguments */
    if(argc == 1) {
        print_usage();
        return (0);
    }
    
    /* read arguments */
    while((c = getopt(argc, argv, "f:p:hn-0123456789E^*/+-")) != -1) {
        switch(c) {
            /* get file name */
            case 'f':
                filename = optarg;
                fromfile = 1;
                break;
            
            case 'n':
                just_print = 1;
                break;
            
            /* get precision */
            case 'p':
                precision = atoi(optarg);
                
                if(precision < 0)
                    precision = 0;
                
                if(precision > 65)
                    precision = 65;
                
                i = 3;
                break;
            
            /* print help */
            case 'h':
                print_usage();
                return(1);
                break;
            
            /* wrong arguments */
            case '?':
                if(optopt == 'f' || optopt == 'p')
                    printf("option -%c requires an argument\n", optopt);
                else
                    printf("unknown option %c!\n", c);
                
                return(1);
                break;
        }
    }
    
    if(fromfile) {
        /* open file */
        if((file = fopen(filename, "r")) == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
        
        /* empty file */
        if(fgets(read, LINE_MAX - 1, file) == NULL) {
            fclose(file);
            return(0);
        }
    }
    
    do {
        if(fromfile)
            term = read;
        else
            term = argv[i];
        
        term[strlen(term) - 1] = '\0';

        if(term == NULL)
            break;
        
        /* empty string */
        if(strlen(term) == 0) {
            i++;
            continue;
        }
        
        /* create parse tree */
        if((parse_tree = parse(term)) == NULL) {
            fprintf(stderr, "cannot create parse tree for %s\n", term);
            i++;
            continue;
        } else {
            reduce(parse_tree);
            
            /* replace variables of tree */
            replace_variables(&parse_tree);

            reduce(parse_tree);
            
            /* calculate value of parse tree */
            result = calculate_parse_tree(parse_tree);
            
            /* print result */
            if((argc == 2 && !fromfile) || just_print)
                printf("%.*Lf\n", precision, result);
            else
                printf("%s = %.*Lf\n", term, precision, result);
            
            /* free memory of parse tree */
            delete_tree(parse_tree);
        }
        
        i++;
    } while(fromfile ? (fgets(read, LINE_MAX - 1, file) != NULL) : (argv[i] != NULL));
    
    /* close file */
    if(fromfile)
        fclose(file);
    
    return(0);
}
