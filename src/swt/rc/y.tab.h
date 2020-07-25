/* A Bison parser, made by GNU Bison 3.3.2.  */

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
    BEG = 258,
    END = 259,
    ACCELERATORS = 260,
    VIRTKEY = 261,
    ASCII = 262,
    NOINVERT = 263,
    SHIFT = 264,
    CONTROL = 265,
    ALT = 266,
    BITMAP = 267,
    CURSOR = 268,
    DIALOG = 269,
    DIALOGEX = 270,
    EXSTYLE = 271,
    CAPTION = 272,
    CLASS = 273,
    STYLE = 274,
    AUTO3STATE = 275,
    AUTOCHECKBOX = 276,
    AUTORADIOBUTTON = 277,
    CHECKBOX = 278,
    COMBOBOX = 279,
    CTEXT = 280,
    DEFPUSHBUTTON = 281,
    EDITTEXT = 282,
    GROUPBOX = 283,
    LISTBOX = 284,
    LTEXT = 285,
    PUSHBOX = 286,
    PUSHBUTTON = 287,
    RADIOBUTTON = 288,
    RTEXT = 289,
    SCROLLBAR = 290,
    STATE3 = 291,
    USERBUTTON = 292,
    BEDIT = 293,
    HEDIT = 294,
    IEDIT = 295,
    FONT = 296,
    ICON = 297,
    LANGUAGE = 298,
    CHARACTERISTICS = 299,
    VERSIONK = 300,
    MENU = 301,
    MENUEX = 302,
    MENUITEM = 303,
    SEPARATOR = 304,
    POPUP = 305,
    CHECKED = 306,
    GRAYED = 307,
    HELP = 308,
    INACTIVE = 309,
    MENUBARBREAK = 310,
    MENUBREAK = 311,
    ES_READONLY = 312,
    ES_MULTILINE = 313,
    ES_PASSWORD = 314,
    BS_AUTOCHECKBOX = 315,
    BS_AUTORADIOBUTTON = 316,
    TOOLBAR = 317,
    BUTTON = 318,
    MESSAGETABLE = 319,
    RCDATA = 320,
    STRINGTABLE = 321,
    VERSIONINFO = 322,
    FILEVERSION = 323,
    PRODUCTVERSION = 324,
    FILEFLAGSMASK = 325,
    FILEFLAGS = 326,
    FILEOS = 327,
    FILETYPE = 328,
    FILESUBTYPE = 329,
    BLOCKSTRINGFILEINFO = 330,
    BLOCKVARFILEINFO = 331,
    VALUE = 332,
    BLOCK = 333,
    MOVEABLE = 334,
    FIXED = 335,
    PURE = 336,
    IMPURE = 337,
    PRELOAD = 338,
    LOADONCALL = 339,
    DISCARDABLE = 340,
    NOT = 341,
    QUOTEDSTRING = 342,
    STRING = 343,
    NUMBER = 344,
    SIZEDSTRING = 345,
    DLGINIT = 346,
    NEGATE = 347
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 59 "rcparse.y" /* yacc.c:1921  */

    RCNode *node;
    int i;
    const char *s;

#line 157 "y.tab.h" /* yacc.c:1921  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
