/* A Bison parser, made by GNU Bison 3.6.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    BRACKET_ON = 258,              /* BRACKET_ON  */
    BRACKET_OFF = 259,             /* BRACKET_OFF  */
    WING_BRACKET_ON = 260,         /* WING_BRACKET_ON  */
    WING_BRACKET_OFF = 261,        /* WING_BRACKET_OFF  */
    SCRIPT = 262,                  /* SCRIPT  */
    COMPOSED_SHADER = 263,         /* COMPOSED_SHADER  */
    PACKAGED_SHADER = 264,         /* PACKAGED_SHADER  */
    SHADER_PROGRAM = 265,          /* SHADER_PROGRAM  */
    VRML_CUT = 266,                /* VRML_CUT  */
    EXPORT = 267,                  /* EXPORT  */
    IMPORT = 268,                  /* IMPORT  */
    AS = 269,                      /* AS  */
    ID = 270,                      /* ID  */
    ID_X3D = 271,                  /* ID_X3D  */
    STRING = 272,                  /* STRING  */
    INT_NUM = 273,                 /* INT_NUM  */
    FLOAT_NUM = 274,               /* FLOAT_NUM  */
    DEF = 275,                     /* DEF  */
    EXTERNPROTO = 276,             /* EXTERNPROTO  */
    FALSE_TOK = 277,               /* FALSE_TOK  */
    IS = 278,                      /* IS  */
    NULL_TOK = 279,                /* NULL_TOK  */
    PROTO = 280,                   /* PROTO  */
    ROUTE = 281,                   /* ROUTE  */
    TO = 282,                      /* TO  */
    TRUE_TOK = 283,                /* TRUE_TOK  */
    USE = 284,                     /* USE  */
    EVENT_IN = 285,                /* EVENT_IN  */
    EVENT_OUT = 286,               /* EVENT_OUT  */
    EXPOSED_FIELD = 287,           /* EXPOSED_FIELD  */
    INPUT_OUTPUT = 288,            /* INPUT_OUTPUT  */
    FIELD = 289,                   /* FIELD  */
    X3D = 290,                     /* X3D  */
    PROFILE = 291,                 /* PROFILE  */
    COMPONENT = 292,               /* COMPONENT  */
    UNIT = 293,                    /* UNIT  */
    META = 294,                    /* META  */
    VRML1 = 295                    /* VRML1  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define BRACKET_ON 258
#define BRACKET_OFF 259
#define WING_BRACKET_ON 260
#define WING_BRACKET_OFF 261
#define SCRIPT 262
#define COMPOSED_SHADER 263
#define PACKAGED_SHADER 264
#define SHADER_PROGRAM 265
#define VRML_CUT 266
#define EXPORT 267
#define IMPORT 268
#define AS 269
#define ID 270
#define ID_X3D 271
#define STRING 272
#define INT_NUM 273
#define FLOAT_NUM 274
#define DEF 275
#define EXTERNPROTO 276
#define FALSE_TOK 277
#define IS 278
#define NULL_TOK 279
#define PROTO 280
#define ROUTE 281
#define TO 282
#define TRUE_TOK 283
#define USE 284
#define EVENT_IN 285
#define EVENT_OUT 286
#define EXPOSED_FIELD 287
#define INPUT_OUTPUT 288
#define FIELD 289
#define X3D 290
#define PROFILE 291
#define COMPONENT 292
#define UNIT 293
#define META 294
#define VRML1 295

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 170 "parser.y"

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

#line 160 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
