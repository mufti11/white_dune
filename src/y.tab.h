/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    BRACKET_ON = 258,
    BRACKET_OFF = 259,
    WING_BRACKET_ON = 260,
    WING_BRACKET_OFF = 261,
    SCRIPT = 262,
    COMPOSED_SHADER = 263,
    PACKAGED_SHADER = 264,
    SHADER_PROGRAM = 265,
    VRML_CUT = 266,
    EXPORT = 267,
    IMPORT = 268,
    AS = 269,
    ID = 270,
    ID_X3D = 271,
    STRING = 272,
    INT_NUM = 273,
    FLOAT_NUM = 274,
    DEF = 275,
    EXTERNPROTO = 276,
    FALSE_TOK = 277,
    IS = 278,
    NULL_TOK = 279,
    PROTO = 280,
    ROUTE = 281,
    TO = 282,
    TRUE_TOK = 283,
    USE = 284,
    EVENT_IN = 285,
    EVENT_OUT = 286,
    EXPOSED_FIELD = 287,
    INPUT_OUTPUT = 288,
    FIELD = 289,
    X3D = 290,
    PROFILE = 291,
    COMPONENT = 292,
    UNIT = 293,
    META = 294,
    VRML1 = 295
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 170 "parser.y" /* yacc.c:1909  */

    int                  int32;
    int                  id;
    Node                *node;
    NodeList            *nodeList;
    Element             *element;
    FieldValue          *value;
    float                sffloat;
    StringArray         *stringArray;
    BoolArray           *boolArray;
    IntArray            *intArray;
    DoubleArray         *doubleArray;

#line 109 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
