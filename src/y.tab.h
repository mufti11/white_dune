/* A Bison parser, made by GNU Bison 3.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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
    EXPORT = 266,
    IMPORT = 267,
    AS = 268,
    ID = 269,
    ID_X3D = 270,
    STRING = 271,
    INT_NUM = 272,
    FLOAT_NUM = 273,
    DEF = 274,
    EXTERNPROTO = 275,
    FALSE_TOK = 276,
    IS = 277,
    NULL_TOK = 278,
    PROTO = 279,
    ROUTE = 280,
    TO = 281,
    TRUE_TOK = 282,
    USE = 283,
    EVENT_IN = 284,
    EVENT_OUT = 285,
    EXPOSED_FIELD = 286,
    INPUT_OUTPUT = 287,
    FIELD = 288,
    X3D = 289,
    PROFILE = 290,
    COMPONENT = 291,
    UNIT = 292,
    META = 293,
    VRML1 = 294
  };
#endif
/* Tokens.  */
#define BRACKET_ON 258
#define BRACKET_OFF 259
#define WING_BRACKET_ON 260
#define WING_BRACKET_OFF 261
#define SCRIPT 262
#define COMPOSED_SHADER 263
#define PACKAGED_SHADER 264
#define SHADER_PROGRAM 265
#define EXPORT 266
#define IMPORT 267
#define AS 268
#define ID 269
#define ID_X3D 270
#define STRING 271
#define INT_NUM 272
#define FLOAT_NUM 273
#define DEF 274
#define EXTERNPROTO 275
#define FALSE_TOK 276
#define IS 277
#define NULL_TOK 278
#define PROTO 279
#define ROUTE 280
#define TO 281
#define TRUE_TOK 282
#define USE 283
#define EVENT_IN 284
#define EVENT_OUT 285
#define EXPOSED_FIELD 286
#define INPUT_OUTPUT 287
#define FIELD 288
#define X3D 289
#define PROFILE 290
#define COMPONENT 291
#define UNIT 292
#define META 293
#define VRML1 294

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 168 "parser.y"

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

#line 149 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
