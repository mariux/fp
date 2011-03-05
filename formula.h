/*
    formelparser - formula.h

    Copyright (C) 2010 Matthias Ruester <ruester@molgen.mpg.de>
    Copyright (C) 2010 Max Planck Institut for Molecular Genetics

    Formelparser is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Formelparser is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FORMELPARSER_FORMELPARSER_H
#define FORMELPARSER_FORMELPARSER_H

extern void reduce(struct Node *);
extern void replace_variables(struct Node **);
extern long double calculate_parse_tree(struct Node *root);

#endif
