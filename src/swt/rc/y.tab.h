/* A Bison parser, made by GNU Bison 3.4.2.  */

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
/* Tokens.  */
#define BEG 258
#define END 259
#define ACCELERATORS 260
#define VIRTKEY 261
#define ASCII 262
#define NOINVERT 263
#define SHIFT 264
#define CONTROL 265
#define ALT 266
#define BITMAP 267
#define CURSOR 268
#define DIALOG 269
#define DIALOGEX 270
#define EXSTYLE 271
#define CAPTION 272
#define CLASS 273
#define STYLE 274
#define AUTO3STATE 275
#define AUTOCHECKBOX 276
#define AUTORADIOBUTTON 277
#define CHECKBOX 278
#define COMBOBOX 279
#define CTEXT 280
#define DEFPUSHBUTTON 281
#define EDITTEXT 282
#define GROUPBOX 283
#define LISTBOX 284
#define LTEXT 285
#define PUSHBOX 286
#define PUSHBUTTON 287
#define RADIOBUTTON 288
#define RTEXT 289
#define SCROLLBAR 290
#define STATE3 291
#define USERBUTTON 292
#define BEDIT 293
#define HEDIT 294
#define IEDIT 295
#define FONT 296
#define ICON 297
#define LANGUAGE 298
#define CHARACTERISTICS 299
#define VERSIONK 300
#define MENU 301
#define MENUEX 302
#define MENUITEM 303
#define SEPARATOR 304
#define POPUP 305
#define CHECKED 306
#define GRAYED 307
#define HELP 308
#define INACTIVE 309
#define MENUBARBREAK 310
#define MENUBREAK 311
#define ES_READONLY 312
#define ES_MULTILINE 313
#define ES_PASSWORD 314
#define BS_AUTOCHECKBOX 315
#define BS_AUTORADIOBUTTON 316
#define TOOLBAR 317
#define BUTTON 318
#define MESSAGETABLE 319
#define RCDATA 320
#define STRINGTABLE 321
#define VERSIONINFO 322
#define FILEVERSION 323
#define PRODUCTVERSION 324
#define FILEFLAGSMASK 325
#define FILEFLAGS 326
#define FILEOS 327
#define FILETYPE 328
#define FILESUBTYPE 329
#define BLOCKSTRINGFILEINFO 330
#define BLOCKVARFILEINFO 331
#define VALUE 332
#define BLOCK 333
#define MOVEABLE 334
#define FIXED 335
#define PURE 336
#define IMPURE 337
#define PRELOAD 338
#define LOADONCALL 339
#define DISCARDABLE 340
#define NOT 341
#define QUOTEDSTRING 342
#define STRING 343
#define NUMBER 344
#define SIZEDSTRING 345
#define DLGINIT 346
#define NEGATE 347

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 59 "rcparse.y"

    RCNode *node;
    int i;
    const char *s;

#line 247 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
