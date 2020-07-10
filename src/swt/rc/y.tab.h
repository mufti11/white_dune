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
    BEG = 258,                     /* BEG  */
    END = 259,                     /* END  */
    ACCELERATORS = 260,            /* ACCELERATORS  */
    VIRTKEY = 261,                 /* VIRTKEY  */
    ASCII = 262,                   /* ASCII  */
    NOINVERT = 263,                /* NOINVERT  */
    SHIFT = 264,                   /* SHIFT  */
    CONTROL = 265,                 /* CONTROL  */
    ALT = 266,                     /* ALT  */
    BITMAP = 267,                  /* BITMAP  */
    CURSOR = 268,                  /* CURSOR  */
    DIALOG = 269,                  /* DIALOG  */
    DIALOGEX = 270,                /* DIALOGEX  */
    EXSTYLE = 271,                 /* EXSTYLE  */
    CAPTION = 272,                 /* CAPTION  */
    CLASS = 273,                   /* CLASS  */
    STYLE = 274,                   /* STYLE  */
    AUTO3STATE = 275,              /* AUTO3STATE  */
    AUTOCHECKBOX = 276,            /* AUTOCHECKBOX  */
    AUTORADIOBUTTON = 277,         /* AUTORADIOBUTTON  */
    CHECKBOX = 278,                /* CHECKBOX  */
    COMBOBOX = 279,                /* COMBOBOX  */
    CTEXT = 280,                   /* CTEXT  */
    DEFPUSHBUTTON = 281,           /* DEFPUSHBUTTON  */
    EDITTEXT = 282,                /* EDITTEXT  */
    GROUPBOX = 283,                /* GROUPBOX  */
    LISTBOX = 284,                 /* LISTBOX  */
    LTEXT = 285,                   /* LTEXT  */
    PUSHBOX = 286,                 /* PUSHBOX  */
    PUSHBUTTON = 287,              /* PUSHBUTTON  */
    RADIOBUTTON = 288,             /* RADIOBUTTON  */
    RTEXT = 289,                   /* RTEXT  */
    SCROLLBAR = 290,               /* SCROLLBAR  */
    STATE3 = 291,                  /* STATE3  */
    USERBUTTON = 292,              /* USERBUTTON  */
    BEDIT = 293,                   /* BEDIT  */
    HEDIT = 294,                   /* HEDIT  */
    IEDIT = 295,                   /* IEDIT  */
    FONT = 296,                    /* FONT  */
    ICON = 297,                    /* ICON  */
    LANGUAGE = 298,                /* LANGUAGE  */
    CHARACTERISTICS = 299,         /* CHARACTERISTICS  */
    VERSIONK = 300,                /* VERSIONK  */
    MENU = 301,                    /* MENU  */
    MENUEX = 302,                  /* MENUEX  */
    MENUITEM = 303,                /* MENUITEM  */
    SEPARATOR = 304,               /* SEPARATOR  */
    POPUP = 305,                   /* POPUP  */
    CHECKED = 306,                 /* CHECKED  */
    GRAYED = 307,                  /* GRAYED  */
    HELP = 308,                    /* HELP  */
    INACTIVE = 309,                /* INACTIVE  */
    MENUBARBREAK = 310,            /* MENUBARBREAK  */
    MENUBREAK = 311,               /* MENUBREAK  */
    ES_READONLY = 312,             /* ES_READONLY  */
    ES_MULTILINE = 313,            /* ES_MULTILINE  */
    ES_PASSWORD = 314,             /* ES_PASSWORD  */
    BS_AUTOCHECKBOX = 315,         /* BS_AUTOCHECKBOX  */
    BS_AUTORADIOBUTTON = 316,      /* BS_AUTORADIOBUTTON  */
    TOOLBAR = 317,                 /* TOOLBAR  */
    BUTTON = 318,                  /* BUTTON  */
    MESSAGETABLE = 319,            /* MESSAGETABLE  */
    RCDATA = 320,                  /* RCDATA  */
    STRINGTABLE = 321,             /* STRINGTABLE  */
    VERSIONINFO = 322,             /* VERSIONINFO  */
    FILEVERSION = 323,             /* FILEVERSION  */
    PRODUCTVERSION = 324,          /* PRODUCTVERSION  */
    FILEFLAGSMASK = 325,           /* FILEFLAGSMASK  */
    FILEFLAGS = 326,               /* FILEFLAGS  */
    FILEOS = 327,                  /* FILEOS  */
    FILETYPE = 328,                /* FILETYPE  */
    FILESUBTYPE = 329,             /* FILESUBTYPE  */
    BLOCKSTRINGFILEINFO = 330,     /* BLOCKSTRINGFILEINFO  */
    BLOCKVARFILEINFO = 331,        /* BLOCKVARFILEINFO  */
    VALUE = 332,                   /* VALUE  */
    BLOCK = 333,                   /* BLOCK  */
    MOVEABLE = 334,                /* MOVEABLE  */
    FIXED = 335,                   /* FIXED  */
    PURE = 336,                    /* PURE  */
    IMPURE = 337,                  /* IMPURE  */
    PRELOAD = 338,                 /* PRELOAD  */
    LOADONCALL = 339,              /* LOADONCALL  */
    DISCARDABLE = 340,             /* DISCARDABLE  */
    NOT = 341,                     /* NOT  */
    QUOTEDSTRING = 342,            /* QUOTEDSTRING  */
    STRING = 343,                  /* STRING  */
    NUMBER = 344,                  /* NUMBER  */
    SIZEDSTRING = 345,             /* SIZEDSTRING  */
    DLGINIT = 346,                 /* DLGINIT  */
    NEGATE = 347                   /* NEGATE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
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

#line 256 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
