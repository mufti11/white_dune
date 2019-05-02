/*
 * parser.h
 *
 * Copyright (C) 1999 Stephen F. White
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (see the file "COPYING" for details); if 
 * not, write to the Free Software Foundation, Inc., 675 Mass Ave, 
 * Cambridge, MA 02139, USA.
 */

class Scene;
class Node;

#include "config.h"

extern "C" {

#ifdef HAVE_LIBZ
# include "zlib.h"
#endif

extern int lineno;
extern void error(const char *s, ...);
extern int yyparse(void);
extern void yyerror(const char *s);
extern int yywrap(void);
extern int yylex(void);
#ifdef HAVE_LIBZ 
 extern gzFile inputFile;
#else
# include <stdio.h>
 extern FILE *inputFile;
#endif
extern const char *parseX3d();
extern int isInProtoLibrary;
extern Scene *scene;
extern Node *targetNode;
extern int targetField;
extern int scanFor;

extern void addToCurrentComment(char *string);
extern void nodeComment(void);
extern void insideNodeComment(void);
extern void MFComment(void);
extern void insideMFComment(void);
extern void forgetNameTranslation(void);
extern void setVrml(void);
extern void setX3d(void);
}
