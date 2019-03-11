/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 24 "rcparse.y" /* yacc.c:339  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <res.h>
#include "rc.h"

static RCNode *newNode(int type, const char *name, int id, int x, int y,
                       int width, int height, int children);
static RCString *newString(int id, const char *str);
static RCNode *newControl(const char *name, int id, const char *type,
                          int flags, int x, int y, int width, int height);
static RCNode *append(RCNode *node1, RCNode *node2);
static RCBitmap *newBitmap(int id, const char *filename);
static void addAccelerator(int key, int modifiers, int command);
static int getVirtkey(const char *str);

void yyerror (char const *s);

RCBitmap **bitmaps = NULL;
RCNode **nodes = NULL;
RCString *rcStrings = NULL;

int numBitmaps = 0;
int numNodes = 0;
int numStrings = 0;

static int bitmapsAllocated = 0;
static int nodesAllocated = 0;
static int stringsAllocated = 0;
static RCNode *curDlg = NULL;

#line 100 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
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
#line 59 "rcparse.y" /* yacc.c:355  */

    RCNode *node;
    int i;
    const char *s;

#line 330 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 347 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   946

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  106
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  64
/* YYNRULES -- Number of rules.  */
#define YYNRULES  206
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  439

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   347

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    99,    94,     2,
     104,   105,    97,    95,   102,    96,     2,    98,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   103,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    93,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    92,     2,   100,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,   101
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   114,   114,   116,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     139,   142,   144,   148,   150,   155,   157,   162,   163,   166,
     171,   172,   173,   174,   175,   176,   182,   189,   196,   195,
     202,   203,   206,   208,   211,   213,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   228,   229,   233,   234,
     235,   236,   237,   238,   239,   243,   244,   245,   246,   251,
     252,   253,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   268,   274,   276,   282,   287,   292,   296,   300,   302,
     306,   310,   312,   316,   317,   318,   319,   325,   330,   331,
     335,   337,   339,   344,   345,   346,   350,   351,   352,   353,
     354,   355,   361,   366,   367,   371,   373,   375,   377,   379,
     381,   383,   391,   396,   397,   401,   403,   410,   416,   419,
     421,   425,   426,   427,   428,   434,   437,   439,   440,   444,
     445,   449,   452,   454,   455,   456,   457,   458,   459,   460,
     469,   471,   472,   475,   477,   480,   482,   488,   489,   490,
     496,   498,   502,   503,   504,   505,   508,   510,   514,   515,
     516,   517,   518,   519,   520,   526,   527,   531,   532,   536,
     537,   538,   539,   543,   544,   548,   549,   550,   551,   552,
     553,   556,   558,   563,   564,   570,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,   581
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "BEG", "END", "ACCELERATORS", "VIRTKEY",
  "ASCII", "NOINVERT", "SHIFT", "CONTROL", "ALT", "BITMAP", "CURSOR",
  "DIALOG", "DIALOGEX", "EXSTYLE", "CAPTION", "CLASS", "STYLE",
  "AUTO3STATE", "AUTOCHECKBOX", "AUTORADIOBUTTON", "CHECKBOX", "COMBOBOX",
  "CTEXT", "DEFPUSHBUTTON", "EDITTEXT", "GROUPBOX", "LISTBOX", "LTEXT",
  "PUSHBOX", "PUSHBUTTON", "RADIOBUTTON", "RTEXT", "SCROLLBAR", "STATE3",
  "USERBUTTON", "BEDIT", "HEDIT", "IEDIT", "FONT", "ICON", "LANGUAGE",
  "CHARACTERISTICS", "VERSIONK", "MENU", "MENUEX", "MENUITEM", "SEPARATOR",
  "POPUP", "CHECKED", "GRAYED", "HELP", "INACTIVE", "MENUBARBREAK",
  "MENUBREAK", "ES_READONLY", "ES_MULTILINE", "ES_PASSWORD",
  "BS_AUTOCHECKBOX", "BS_AUTORADIOBUTTON", "TOOLBAR", "BUTTON",
  "MESSAGETABLE", "RCDATA", "STRINGTABLE", "VERSIONINFO", "FILEVERSION",
  "PRODUCTVERSION", "FILEFLAGSMASK", "FILEFLAGS", "FILEOS", "FILETYPE",
  "FILESUBTYPE", "BLOCKSTRINGFILEINFO", "BLOCKVARFILEINFO", "VALUE",
  "BLOCK", "MOVEABLE", "FIXED", "PURE", "IMPURE", "PRELOAD", "LOADONCALL",
  "DISCARDABLE", "NOT", "QUOTEDSTRING", "STRING", "NUMBER", "SIZEDSTRING",
  "DLGINIT", "'|'", "'^'", "'&'", "'+'", "'-'", "'*'", "'/'", "'%'", "'~'",
  "NEGATE", "','", "'='", "'('", "')'", "$accept", "stmts", "stmt",
  "accelerator", "acc_entries", "acc_entry", "acc_event", "acc_options",
  "acc_option", "bitmap", "cursor", "dialog", "$@1", "dlg_token",
  "exstyle", "styles", "controls", "control", "params", "opt_control_data",
  "font", "icon", "language", "dlginit", "dlgdataitems", "dlgdataitem",
  "hexdata", "hexdatum", "menu", "menuitems", "menuitem", "menuitem_flags",
  "menuitem_flag", "menuex", "menuexitems", "menuexitem", "toolbar",
  "buttons", "button", "messagetable", "rcdata", "optrcdata_data",
  "rcdata_data", "stringtable", "string_data", "user", "versioninfo",
  "fixedverinfo", "verblocks", "vervals", "vertrans", "id", "suboptions",
  "suboption", "memflags", "memflag", "file_name", "optstyleexpr",
  "styleexpr", "parenstyle", "style", "optexpr", "optstringc", "expr", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   124,    94,    38,    43,    45,    42,    47,    37,
     126,   347,    44,    61,    40,    41
};
# endif

#define YYPACT_NINF -384

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-384)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -384,    90,  -384,   -59,  -384,  -384,  -384,   -58,  -384,  -384,
    -384,  -384,  -384,  -384,  -384,  -384,  -384,  -384,  -384,  -384,
    -384,  -384,  -384,  -384,  -384,   187,   -79,   237,  -384,  -384,
    -384,  -384,  -384,  -384,  -384,  -384,  -384,  -384,  -384,  -384,
    -384,  -384,    40,  -384,  -384,   -59,  -384,   -59,   159,   159,
    -384,  -384,  -384,  -384,  -384,  -384,  -384,  -384,  -384,   247,
     263,   263,   263,   263,   332,   346,   444,   263,   355,   103,
    -384,   138,   227,  -384,    46,   -53,  -384,   159,   159,   159,
     309,   309,  -384,  -384,  -384,  -384,  -384,  -384,  -384,  -384,
    -384,  -384,   528,  -384,   199,  -384,   159,   159,   159,   159,
     159,   159,   159,    70,   -52,   159,   199,  -384,  -384,   -54,
     -59,  -384,  -384,   507,   159,   159,   159,   159,   159,   159,
     159,   159,    61,     4,    37,   159,  -384,    51,   -46,   309,
      13,   539,   550,   309,   309,   309,   309,   309,  -384,  -384,
     -18,    19,   561,   115,  -384,    56,  -384,  -384,   835,   380,
     146,    54,    54,  -384,  -384,  -384,  -384,  -384,  -384,    44,
    -384,  -384,    -4,    60,  -384,  -384,    73,    75,  -384,    32,
    -384,   264,  -384,   162,   177,   159,   159,   159,  -384,   159,
    -384,  -384,   -59,  -384,    80,  -384,    81,     2,  -384,  -384,
     309,   116,   118,   572,   583,   594,   605,   110,   -59,    16,
     159,  -384,   159,   135,   202,   144,   159,   159,   159,   159,
     929,  -384,  -384,  -384,  -384,  -384,  -384,  -384,  -384,   890,
    -384,   616,   137,     1,  -384,  -384,   -59,  -384,  -384,  -384,
     627,   638,   649,   660,  -384,  -384,  -384,  -384,  -384,  -384,
       5,  -384,   285,   140,  -384,   159,  -384,  -384,   159,  -384,
      24,     3,   159,   159,   159,   159,   929,  -384,  -384,   671,
     141,    18,   220,   148,  -384,   159,   309,   309,   309,   309,
    -384,   159,  -384,  -384,  -384,   159,  -384,   125,   682,   -55,
    -384,   309,   160,   671,   151,   159,   145,   154,  -384,   468,
    -384,   250,  -384,   309,  -384,  -384,  -384,   159,   170,   -59,
     361,   159,   -59,   159,   159,   -59,  -384,   535,   309,  -384,
    -384,  -384,  -384,  -384,  -384,  -384,   394,   458,  -384,   168,
    -384,  -384,   693,   164,   309,   309,  -384,   189,  -384,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,  -384,  -384,   163,   384,   180,   -59,  -384,   171,
     -59,  -384,   -59,  -384,  -384,  -384,  -384,  -384,  -384,  -384,
    -384,  -384,  -384,  -384,  -384,  -384,  -384,  -384,  -384,  -384,
    -384,  -384,  -384,   -59,  -384,   394,  -384,   172,  -384,  -384,
     175,   182,   183,  -384,   159,   192,   159,   -59,   704,   184,
     715,   185,   159,   361,   159,   159,   726,   -66,   737,   748,
     159,   159,   159,   159,   309,   759,   770,   781,   159,   159,
     159,   792,   803,   814,   159,   361,   266,   159,   825,   168,
     199,  -384,   309,   159,   289,   671,  -384,   266,  -384
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,   160,   159,   157,     0,     3,     4,
       5,     6,     7,     9,    10,    11,     8,    12,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,   158,   160,
     166,   166,    40,    41,   166,   166,   160,   160,   166,   166,
     160,   142,     0,   166,   160,     0,   136,     0,     0,     0,
     168,   169,   170,   171,   172,   173,   174,   161,   162,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    42,     0,    86,     0,     0,   195,     0,     0,     0,
     163,   165,    21,   175,   176,   167,    36,    37,    84,    85,
      98,   113,     0,   127,   129,   150,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   129,   140,   135,     0,
       0,   198,   197,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   131,     0,   130,   132,
       0,     0,     0,   145,   146,   147,   148,   149,    87,    89,
       0,     0,     0,     0,   137,     0,   164,   196,   206,   205,
     204,   202,   203,   199,   200,   201,    20,    25,    22,     0,
      26,    97,     0,     0,    99,   112,     0,     0,   114,     0,
     128,     0,   141,     0,     0,     0,     0,     0,    43,     0,
     139,   138,     0,   101,     0,   103,   115,     0,   123,   133,
     134,     0,     0,     0,     0,     0,     0,    23,     0,     0,
       0,   113,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   103,    98,   106,   107,   108,   109,   110,   111,     0,
     105,   116,     0,     0,   122,   126,     0,   124,   153,   155,
       0,     0,     0,     0,    30,    31,    32,    33,    34,    35,
      24,    27,   100,     0,   104,     0,   118,   113,     0,   125,
       0,     0,     0,     0,     0,     0,     0,    29,   102,   191,
       0,     0,     0,     0,   152,     0,   143,   144,    91,    38,
      28,     0,   117,   119,   113,     0,   151,     0,     0,    90,
      44,   192,     0,   191,     0,     0,    94,    93,    92,     0,
     120,     0,   154,   156,    96,    95,    56,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   113,     0,    48,    45,
      46,   186,   185,   187,   188,   189,     0,     0,   190,    47,
     179,   183,     0,     0,    53,    55,    52,     0,    39,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,    57,   180,     0,     0,     0,     0,   121,     0,
       0,    58,     0,    59,    60,    62,    63,    65,    67,    68,
      69,    73,    66,    74,    75,    76,    77,    78,    79,    80,
      61,    70,    72,     0,   184,     0,   181,    49,    54,   194,
       0,     0,     0,   182,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,     0,    51,     0,     0,     0,     0,     0,
       0,     0,   177,     0,     0,     0,    82,     0,     0,   178,
     129,    81,    71,     0,     0,   191,    83,    82,    64
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -384,  -384,  -384,  -384,  -384,  -384,  -384,  -384,  -234,  -384,
    -384,  -384,  -384,  -384,  -384,  -384,  -384,  -384,   248,  -143,
    -384,  -384,  -384,  -384,  -384,  -384,  -384,  -384,  -384,    84,
    -384,    86,    83,  -384,  -183,  -384,  -384,  -384,  -384,  -384,
    -384,  -105,  -384,  -384,  -384,  -384,  -384,  -384,  -384,  -384,
    -384,    -1,   167,  -384,   576,   432,   469,  -384,  -383,  -263,
     -17,  -280,  -304,   -39
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     8,     9,   122,   158,   159,   240,   241,    10,
      11,    12,   280,    43,   105,   289,   307,   352,   361,   431,
      13,    14,    15,    16,   103,   139,   279,   288,    17,   123,
     164,   199,   220,    18,   124,   168,    19,   203,   227,    20,
      21,   127,   128,    22,    74,    23,    24,    69,   130,   250,
     251,   318,    27,    57,    60,    85,    86,   426,   319,   320,
     321,   272,   362,   129
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint16 yytable[] =
{
      25,   143,    26,   291,   247,   201,   257,   264,   161,    80,
      81,   234,   235,   236,   237,   238,   239,   172,   222,   212,
     407,   274,   270,    45,    44,   360,   355,    92,   262,     5,
       6,    28,   286,   144,   287,   188,   411,     7,   111,   112,
     113,   165,   429,    70,    73,   183,    75,   383,   145,   110,
     108,   141,   162,   353,   163,   170,   171,   131,   132,   133,
     134,   135,   136,   137,   260,   156,   142,   213,   214,   215,
     216,   217,   218,   109,   138,   148,   149,   150,   151,   152,
     153,   154,   155,   184,   177,   166,   169,   167,   173,   174,
       2,   282,   386,   114,   115,   116,   117,   118,   119,   120,
     121,   263,   140,   248,   202,   265,    95,   256,   178,   146,
     114,   115,   116,   117,   118,   119,   120,   121,   219,   180,
     275,   160,   393,   327,   114,   115,   116,   117,   118,   119,
     120,   121,   190,     3,     5,     6,   193,   194,   195,   224,
     196,   246,     7,   181,   258,   273,   182,   185,   157,     5,
       6,   119,   120,   121,   104,   437,     4,     7,     5,     6,
     186,   221,   187,   223,   290,   191,     7,   230,   231,   232,
     233,    96,    97,    98,    99,   100,   101,   102,     5,     6,
     192,   197,   198,   200,   225,   166,     7,   167,   162,   166,
     163,   167,    29,   358,   204,   205,    59,   211,   226,    30,
      31,    32,    33,    64,    65,   228,   259,    68,   166,   261,
     167,    72,   210,   266,   267,   268,   269,    50,    51,    52,
      53,    54,    55,    56,   276,   249,   278,   284,    34,    35,
     106,   229,   281,    36,    37,   277,   283,   166,   292,   167,
      46,   117,   118,   119,   120,   121,   293,   294,    76,    38,
      82,    39,    40,   306,    41,    77,   295,   309,   308,    78,
     355,   359,   322,    79,   324,   325,   357,   387,   384,   430,
      47,    48,    49,   389,   394,     5,     6,   395,    42,   399,
      47,    48,    49,     7,   396,   397,   403,   405,    76,   126,
      47,    48,    49,   436,   438,    77,   243,   242,   310,    78,
     354,   323,   244,    79,   326,     0,    50,    51,    52,    53,
      54,    55,    56,     0,    83,    84,    50,    51,    52,    53,
      54,    55,    56,     0,     0,   434,    50,    51,    52,    53,
      54,    55,    56,     0,     0,    90,   213,   214,   215,   216,
     217,   218,    50,    51,    52,    53,    54,    55,    56,    91,
      83,    84,     0,    76,   189,   398,   388,   400,    94,   390,
      77,   391,     0,   406,    78,   408,   409,     0,    79,     0,
       0,   414,   415,   416,   417,    47,    48,    49,     0,   421,
     422,   423,   392,     0,     0,   428,     0,   219,   432,    47,
      48,    49,     0,     0,   435,     0,   401,     0,    47,    48,
      49,   114,   115,   116,   117,   118,   119,   120,   121,     0,
       0,    50,    51,    52,    53,    54,    55,    56,   311,   312,
     313,   314,   315,     0,     0,    50,    51,    52,    53,    54,
      55,    56,     0,     0,    50,    51,    52,    53,    54,    55,
      56,   311,   312,   313,   314,   315,     0,   316,     0,     5,
       6,   311,   312,   313,   314,   315,     0,     7,     0,    58,
       0,     0,     0,     0,     0,   317,     0,     0,     0,     0,
     385,   296,     5,     6,   116,   117,   118,   119,   120,   121,
       7,     0,     5,     6,   297,   298,   299,   300,   317,     0,
       7,    58,     0,     0,     0,     0,    58,    58,   317,     0,
      58,     0,     0,     0,    58,     0,     0,     0,     0,   301,
       0,   302,   303,   304,   305,   311,   312,   313,   314,   315,
       0,     0,     0,    50,    51,    52,    53,    54,    55,    56,
      87,    88,    89,    76,     0,     0,    93,     0,     0,   328,
      77,   107,     0,     0,    78,   329,     5,     6,    79,     0,
       0,     0,     0,     0,     7,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,     0,   351,     0,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   379,   380,   381,   382,   114,
     115,   116,   117,   118,   119,   120,   121,    61,     0,     0,
      62,    63,   147,     0,    66,    67,     0,     0,     0,    71,
     114,   115,   116,   117,   118,   119,   120,   121,     0,     0,
     125,   114,   115,   116,   117,   118,   119,   120,   121,     0,
       0,   175,   114,   115,   116,   117,   118,   119,   120,   121,
       0,     0,   176,   114,   115,   116,   117,   118,   119,   120,
     121,     0,     0,   179,   114,   115,   116,   117,   118,   119,
     120,   121,     0,     0,   206,   114,   115,   116,   117,   118,
     119,   120,   121,     0,     0,   207,   114,   115,   116,   117,
     118,   119,   120,   121,     0,     0,   208,   114,   115,   116,
     117,   118,   119,   120,   121,     0,     0,   209,   114,   115,
     116,   117,   118,   119,   120,   121,     0,     0,   245,   114,
     115,   116,   117,   118,   119,   120,   121,     0,     0,   252,
     114,   115,   116,   117,   118,   119,   120,   121,     0,     0,
     253,   114,   115,   116,   117,   118,   119,   120,   121,     0,
       0,   254,   114,   115,   116,   117,   118,   119,   120,   121,
       0,     0,   255,   114,   115,   116,   117,   118,   119,   120,
     121,     0,     0,   271,   114,   115,   116,   117,   118,   119,
     120,   121,     0,     0,   285,   114,   115,   116,   117,   118,
     119,   120,   121,     0,     0,   356,   114,   115,   116,   117,
     118,   119,   120,   121,     0,     0,   402,   114,   115,   116,
     117,   118,   119,   120,   121,     0,     0,   404,   114,   115,
     116,   117,   118,   119,   120,   121,     0,     0,   410,   114,
     115,   116,   117,   118,   119,   120,   121,     0,     0,   412,
     114,   115,   116,   117,   118,   119,   120,   121,     0,     0,
     413,   114,   115,   116,   117,   118,   119,   120,   121,     0,
       0,   418,   114,   115,   116,   117,   118,   119,   120,   121,
       0,     0,   419,   114,   115,   116,   117,   118,   119,   120,
     121,     0,     0,   420,   114,   115,   116,   117,   118,   119,
     120,   121,     0,     0,   424,   114,   115,   116,   117,   118,
     119,   120,   121,     0,     0,   425,   114,   115,   116,   117,
     118,   119,   120,   121,     0,     0,   427,   114,   115,   116,
     117,   118,   119,   120,   121,     0,     0,   433,   115,   116,
     117,   118,   119,   120,   121,   234,   235,   236,   237,   238,
     239,   213,   214,   215,   216,   217,   218
};

static const yytype_int16 yycheck[] =
{
       1,   106,     3,   283,     3,     3,   240,     4,     4,    48,
      49,     6,     7,     8,     9,    10,    11,     4,   201,     3,
     403,     3,   256,   102,    25,   329,    92,    66,     4,    88,
      89,    89,    87,    87,    89,     3,   102,    96,    77,    78,
      79,     4,   425,     3,    45,    49,    47,   351,   102,   102,
       4,   103,    48,   316,    50,     4,   102,    96,    97,    98,
      99,   100,   101,   102,   247,     4,   105,    51,    52,    53,
      54,    55,    56,    74,     4,   114,   115,   116,   117,   118,
     119,   120,   121,    87,   102,    48,   125,    50,    75,    76,
       0,   274,   355,    92,    93,    94,    95,    96,    97,    98,
      99,    77,   103,   102,   102,   102,     3,   102,    89,   110,
      92,    93,    94,    95,    96,    97,    98,    99,   102,     4,
     102,   122,   385,   306,    92,    93,    94,    95,    96,    97,
      98,    99,   171,    43,    88,    89,   175,   176,   177,     4,
     179,     4,    96,    87,     4,     4,   102,    87,    87,    88,
      89,    97,    98,    99,    16,   435,    66,    96,    88,    89,
      87,   200,    87,   202,     4,     3,    96,   206,   207,   208,
     209,    68,    69,    70,    71,    72,    73,    74,    88,    89,
       3,   182,   102,   102,    49,    48,    96,    50,    48,    48,
      50,    50,     5,     4,    78,    77,    29,   198,    63,    12,
      13,    14,    15,    36,    37,     3,   245,    40,    48,   248,
      50,    44,   102,   252,   253,   254,   255,    79,    80,    81,
      82,    83,    84,    85,     4,   226,   265,   102,    41,    42,
       3,    87,   271,    46,    47,    87,   275,    48,    87,    50,
       3,    95,    96,    97,    98,    99,   285,   102,    89,    62,
       3,    64,    65,     3,    67,    96,   102,    87,   297,   100,
      92,    87,   301,   104,   303,   304,   102,    87,   105,     3,
      43,    44,    45,   102,   102,    88,    89,   102,    91,    87,
      43,    44,    45,    96,   102,   102,   102,   102,    89,    90,
      43,    44,    45,     4,   437,    96,   212,   211,   299,   100,
     317,   302,   219,   104,   305,    -1,    79,    80,    81,    82,
      83,    84,    85,    -1,    87,    88,    79,    80,    81,    82,
      83,    84,    85,    -1,    -1,   430,    79,    80,    81,    82,
      83,    84,    85,    -1,    -1,     3,    51,    52,    53,    54,
      55,    56,    79,    80,    81,    82,    83,    84,    85,     3,
      87,    88,    -1,    89,    90,   394,   357,   396,     3,   360,
      96,   362,    -1,   402,   100,   404,   405,    -1,   104,    -1,
      -1,   410,   411,   412,   413,    43,    44,    45,    -1,   418,
     419,   420,   383,    -1,    -1,   424,    -1,   102,   427,    43,
      44,    45,    -1,    -1,   433,    -1,   397,    -1,    43,    44,
      45,    92,    93,    94,    95,    96,    97,    98,    99,    -1,
      -1,    79,    80,    81,    82,    83,    84,    85,    57,    58,
      59,    60,    61,    -1,    -1,    79,    80,    81,    82,    83,
      84,    85,    -1,    -1,    79,    80,    81,    82,    83,    84,
      85,    57,    58,    59,    60,    61,    -1,    86,    -1,    88,
      89,    57,    58,    59,    60,    61,    -1,    96,    -1,    27,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      86,     3,    88,    89,    94,    95,    96,    97,    98,    99,
      96,    -1,    88,    89,    16,    17,    18,    19,   104,    -1,
      96,    59,    -1,    -1,    -1,    -1,    64,    65,   104,    -1,
      68,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    41,
      -1,    43,    44,    45,    46,    57,    58,    59,    60,    61,
      -1,    -1,    -1,    79,    80,    81,    82,    83,    84,    85,
      61,    62,    63,    89,    -1,    -1,    67,    -1,    -1,     4,
      96,    72,    -1,    -1,   100,    10,    88,    89,   104,    -1,
      -1,    -1,    -1,    -1,    96,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    42,    -1,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,    92,
      93,    94,    95,    96,    97,    98,    99,    31,    -1,    -1,
      34,    35,   105,    -1,    38,    39,    -1,    -1,    -1,    43,
      92,    93,    94,    95,    96,    97,    98,    99,    -1,    -1,
     102,    92,    93,    94,    95,    96,    97,    98,    99,    -1,
      -1,   102,    92,    93,    94,    95,    96,    97,    98,    99,
      -1,    -1,   102,    92,    93,    94,    95,    96,    97,    98,
      99,    -1,    -1,   102,    92,    93,    94,    95,    96,    97,
      98,    99,    -1,    -1,   102,    92,    93,    94,    95,    96,
      97,    98,    99,    -1,    -1,   102,    92,    93,    94,    95,
      96,    97,    98,    99,    -1,    -1,   102,    92,    93,    94,
      95,    96,    97,    98,    99,    -1,    -1,   102,    92,    93,
      94,    95,    96,    97,    98,    99,    -1,    -1,   102,    92,
      93,    94,    95,    96,    97,    98,    99,    -1,    -1,   102,
      92,    93,    94,    95,    96,    97,    98,    99,    -1,    -1,
     102,    92,    93,    94,    95,    96,    97,    98,    99,    -1,
      -1,   102,    92,    93,    94,    95,    96,    97,    98,    99,
      -1,    -1,   102,    92,    93,    94,    95,    96,    97,    98,
      99,    -1,    -1,   102,    92,    93,    94,    95,    96,    97,
      98,    99,    -1,    -1,   102,    92,    93,    94,    95,    96,
      97,    98,    99,    -1,    -1,   102,    92,    93,    94,    95,
      96,    97,    98,    99,    -1,    -1,   102,    92,    93,    94,
      95,    96,    97,    98,    99,    -1,    -1,   102,    92,    93,
      94,    95,    96,    97,    98,    99,    -1,    -1,   102,    92,
      93,    94,    95,    96,    97,    98,    99,    -1,    -1,   102,
      92,    93,    94,    95,    96,    97,    98,    99,    -1,    -1,
     102,    92,    93,    94,    95,    96,    97,    98,    99,    -1,
      -1,   102,    92,    93,    94,    95,    96,    97,    98,    99,
      -1,    -1,   102,    92,    93,    94,    95,    96,    97,    98,
      99,    -1,    -1,   102,    92,    93,    94,    95,    96,    97,
      98,    99,    -1,    -1,   102,    92,    93,    94,    95,    96,
      97,    98,    99,    -1,    -1,   102,    92,    93,    94,    95,
      96,    97,    98,    99,    -1,    -1,   102,    92,    93,    94,
      95,    96,    97,    98,    99,    -1,    -1,   102,    93,    94,
      95,    96,    97,    98,    99,     6,     7,     8,     9,    10,
      11,    51,    52,    53,    54,    55,    56
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   107,     0,    43,    66,    88,    89,    96,   108,   109,
     115,   116,   117,   126,   127,   128,   129,   134,   139,   142,
     145,   146,   149,   151,   152,   157,   157,   158,    89,     5,
      12,    13,    14,    15,    41,    42,    46,    47,    62,    64,
      65,    67,    91,   119,   157,   102,     3,    43,    44,    45,
      79,    80,    81,    82,    83,    84,    85,   159,   161,   158,
     160,   160,   160,   160,   158,   158,   160,   160,   158,   153,
       3,   160,   158,   157,   150,   157,    89,    96,   100,   104,
     169,   169,     3,    87,    88,   161,   162,   162,   162,   162,
       3,     3,   169,   162,     3,     3,    68,    69,    70,    71,
      72,    73,    74,   130,    16,   120,     3,   162,     4,   157,
     102,   169,   169,   169,    92,    93,    94,    95,    96,    97,
      98,    99,   110,   135,   140,   102,    90,   147,   148,   169,
     154,   169,   169,   169,   169,   169,   169,   169,     4,   131,
     157,   103,   169,   147,    87,   102,   157,   105,   169,   169,
     169,   169,   169,   169,   169,   169,     4,    87,   111,   112,
     157,     4,    48,    50,   136,     4,    48,    50,   141,   169,
       4,   102,     4,    75,    76,   102,   102,   102,    89,   102,
       4,    87,   102,    49,    87,    87,    87,    87,     3,    90,
     169,     3,     3,   169,   169,   169,   169,   157,   102,   137,
     102,     3,   102,   143,    78,    77,   102,   102,   102,   102,
     102,   157,     3,    51,    52,    53,    54,    55,    56,   102,
     138,   169,   140,   169,     4,    49,    63,   144,     3,    87,
     169,   169,   169,   169,     6,     7,     8,     9,    10,    11,
     113,   114,   137,   135,   138,   102,     4,     3,   102,   157,
     155,   156,   102,   102,   102,   102,   102,   114,     4,   169,
     140,   169,     4,    77,     4,   102,   169,   169,   169,   169,
     114,   102,   167,     4,     3,   102,     4,    87,   169,   132,
     118,   169,   140,   169,   102,   102,    87,    89,   133,   121,
       4,   167,    87,   169,   102,   102,     3,    16,    17,    18,
      19,    41,    43,    44,    45,    46,     3,   122,   169,    87,
     157,    57,    58,    59,    60,    61,    86,   104,   157,   164,
     165,   166,   169,   157,   169,   169,   157,   140,     4,    10,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    42,   123,   165,   166,    92,   102,   102,     4,    87,
     168,   124,   168,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   168,   105,    86,   165,    87,   157,   102,
     157,   157,   157,   165,   102,   102,   102,   102,   169,    87,
     169,   157,   102,   102,   102,   102,   169,   164,   169,   169,
     102,   102,   102,   102,   169,   169,   169,   169,   102,   102,
     102,   169,   169,   169,   102,   102,   163,   102,   169,   164,
       3,   125,   169,   102,   147,   169,     4,   167,   125
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   106,   107,   107,   108,   108,   108,   108,   108,   108,
     108,   108,   108,   108,   108,   108,   108,   108,   108,   108,
     109,   110,   110,   111,   111,   112,   112,   113,   113,   113,
     114,   114,   114,   114,   114,   114,   115,   116,   118,   117,
     119,   119,   120,   120,   121,   121,   121,   121,   121,   121,
     121,   121,   121,   121,   121,   121,   122,   122,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   124,   125,   125,   126,   127,   128,   129,   130,   130,
     131,   132,   132,   133,   133,   133,   133,   134,   135,   135,
     136,   136,   136,   137,   137,   137,   138,   138,   138,   138,
     138,   138,   139,   140,   140,   141,   141,   141,   141,   141,
     141,   141,   142,   143,   143,   144,   144,   145,   146,   147,
     147,   148,   148,   148,   148,   149,   150,   150,   150,   151,
     151,   152,   153,   153,   153,   153,   153,   153,   153,   153,
     154,   154,   154,   155,   155,   156,   156,   157,   157,   157,
     158,   158,   159,   159,   159,   159,   160,   160,   161,   161,
     161,   161,   161,   161,   161,   162,   162,   163,   163,   164,
     164,   164,   164,   165,   165,   166,   166,   166,   166,   166,
     166,   167,   167,   168,   168,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       6,     0,     2,     3,     5,     1,     1,     1,     3,     2,
       1,     1,     1,     1,     1,     1,     4,     4,     0,    16,
       1,     1,     0,     3,     0,     3,     3,     3,     3,     5,
       9,    11,     3,     3,     5,     3,     0,     2,     2,     2,
       2,     2,     2,     2,    17,     2,     2,     2,     2,     2,
       2,    13,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    12,     0,     3,     4,     4,     4,     5,     0,     2,
       8,     0,     2,     1,     1,     2,     2,     6,     0,     2,
       5,     2,     6,     0,     3,     2,     1,     1,     1,     1,
       1,     1,     6,     0,     2,     2,     4,     7,     5,     7,
       9,    12,     9,     0,     2,     2,     1,     4,     6,     0,
       1,     1,     1,     3,     3,     5,     0,     3,     4,     6,
       4,     6,     0,     9,     9,     3,     3,     3,     3,     3,
       0,     8,     7,     0,     5,     0,     5,     1,     2,     1,
       0,     2,     1,     2,     4,     2,     0,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     2,     1,
       2,     3,     4,     1,     3,     1,     1,     1,     1,     1,
       1,     0,     2,     0,     2,     1,     3,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 20:
#line 139 "rcparse.y" /* yacc.c:1646  */
    {}
#line 1847 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 149 "rcparse.y" /* yacc.c:1646  */
    { addAccelerator((yyvsp[-2].i), 0, (yyvsp[0].i)); }
#line 1853 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 151 "rcparse.y" /* yacc.c:1646  */
    { addAccelerator((yyvsp[-4].i), (yyvsp[0].i), (yyvsp[-2].i)); }
#line 1859 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 156 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[0].s)[0]; }
#line 1865 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 158 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = getVirtkey(yytext); }
#line 1871 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 164 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) | (yyvsp[0].i); }
#line 1877 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 167 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-1].i) | (yyvsp[0].i); }
#line 1883 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 171 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 1889 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 172 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 1895 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 173 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 1901 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 174 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = SW_SHIFT; }
#line 1907 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 175 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = SW_CONTROL; }
#line 1913 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 176 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = SW_ALT; }
#line 1919 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 183 "rcparse.y" /* yacc.c:1646  */
    { newBitmap((yyvsp[-3].i), (yyvsp[0].s)); }
#line 1925 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 189 "rcparse.y" /* yacc.c:1646  */
    {}
#line 1931 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 196 "rcparse.y" /* yacc.c:1646  */
    { curDlg = newNode(RCT_DIALOG, "", (yyvsp[-10].i), (yyvsp[-6].i), (yyvsp[-4].i), (yyvsp[-2].i), (yyvsp[0].i), -1); }
#line 1937 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 198 "rcparse.y" /* yacc.c:1646  */
    { curDlg->children = (yyvsp[-1].node)->index; curDlg = NULL; }
#line 1943 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 214 "rcparse.y" /* yacc.c:1646  */
    { if (curDlg) curDlg->name = strdup((yyvsp[0].s)); }
#line 1949 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 228 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = NULL; }
#line 1955 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 229 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = append((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1961 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 233 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); (yyval.node)->type = RCT_BUTTON; }
#line 1967 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 234 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); (yyval.node)->type = RCT_CHECKBOX; }
#line 1973 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 235 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); (yyval.node)->type = RCT_RADIOBUTTON; }
#line 1979 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 236 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); }
#line 1985 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 237 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); (yyval.node)->type = RCT_CHECKBOX; }
#line 1991 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 238 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); (yyval.node)->type = RCT_COMBOBOX; }
#line 1997 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 241 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = newControl((yyvsp[-15].s), (yyvsp[-14].i), (yyvsp[-12].s), (yyvsp[-10].i), (yyvsp[-8].i), 
                                                          (yyvsp[-6].i), (yyvsp[-4].i), (yyvsp[-2].i)); }
#line 2004 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 243 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); }
#line 2010 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 244 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); (yyval.node)->type = RCT_LABEL; }
#line 2016 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 245 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); (yyval.node)->type = RCT_BUTTON; }
#line 2022 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 246 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); (yyval.node)->type = RCT_TEXTEDIT;
                                          if (!((yyval.node)->flags & SW_MULTI_LINE))
                                               (yyval.node)->flags |= SW_SINGLE_LINE;
                                          if (!((yyval.node)->flags & SW_NON_EDITABLE))
                                               (yyval.node)->flags |= SW_EDITABLE; }
#line 2032 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 251 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); (yyval.node)->type = RCT_GROUPBOX; }
#line 2038 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 252 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); }
#line 2044 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 254 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = newNode(-1, (yyvsp[-11].s), (yyvsp[-10].i), (yyvsp[-6].i), (yyvsp[-4].i), (yyvsp[-2].i),
                                                       (yyvsp[0].i), -1); }
#line 2051 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 256 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); }
#line 2057 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 257 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); }
#line 2063 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 258 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); }
#line 2069 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 259 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); (yyval.node)->type = RCT_BUTTON; }
#line 2075 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 260 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); (yyval.node)->type = RCT_RADIOBUTTON; }
#line 2081 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 261 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); }
#line 2087 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 262 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); (yyval.node)->type = RCT_SCROLLBAR; }
#line 2093 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 263 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); }
#line 2099 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 264 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); }
#line 2105 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 270 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = newNode(-1, (yyvsp[-11].s), (yyvsp[-10].i), (yyvsp[-8].i), (yyvsp[-6].i), (yyvsp[-4].i), (yyvsp[-2].i), -1);
          (yyval.node)->flags = (yyvsp[-1].i); }
#line 2112 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 283 "rcparse.y" /* yacc.c:1646  */
    {}
#line 2118 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 288 "rcparse.y" /* yacc.c:1646  */
    {}
#line 2124 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 297 "rcparse.y" /* yacc.c:1646  */
    {}
#line 2130 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 307 "rcparse.y" /* yacc.c:1646  */
    {}
#line 2136 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 316 "rcparse.y" /* yacc.c:1646  */
    {}
#line 2142 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 317 "rcparse.y" /* yacc.c:1646  */
    {}
#line 2148 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 318 "rcparse.y" /* yacc.c:1646  */
    {}
#line 2154 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 319 "rcparse.y" /* yacc.c:1646  */
    {}
#line 2160 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 326 "rcparse.y" /* yacc.c:1646  */
    { newNode(RCT_MENUBAR, "", (yyvsp[-5].i), 0, 0, 0, 0, (yyvsp[-1].node)->index); }
#line 2166 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 330 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = NULL; }
#line 2172 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 331 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = append((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2178 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 336 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = newNode(RCT_MENUITEM, (yyvsp[-3].s), (yyvsp[-1].i), 0, 0, (yyvsp[0].i), 0, -1); }
#line 2184 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 338 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = newNode(RCT_SEPARATOR, "---", -1, 0, 0, 0, 0, -1); }
#line 2190 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 340 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = newNode(RCT_MENU, (yyvsp[-4].s), -1, 0, (yyvsp[-3].i), 0, 0, (yyvsp[-1].node)->index); }
#line 2196 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 344 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 2202 "y.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 345 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) | (yyvsp[0].i); }
#line 2208 "y.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 346 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-1].i) | (yyvsp[0].i); }
#line 2214 "y.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 350 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = SW_MENU_CHECKED; }
#line 2220 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 351 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = SW_MENU_DISABLED; }
#line 2226 "y.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 352 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 2232 "y.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 353 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 2238 "y.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 354 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 2244 "y.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 355 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 2250 "y.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 362 "rcparse.y" /* yacc.c:1646  */
    { newNode(RCT_MENU, "", (yyvsp[-5].i), 0, 0, 0, 0, (yyvsp[-1].node)->index); }
#line 2256 "y.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 366 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = NULL; }
#line 2262 "y.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 367 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = append((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2268 "y.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 372 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = newNode(RCT_MENUITEM, (yyvsp[0].s), -1, 0, 0, 0, 0, -1); }
#line 2274 "y.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 374 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = newNode(RCT_MENUITEM, (yyvsp[-2].s), (yyvsp[0].i), 0, 0, 0, 0, -1); }
#line 2280 "y.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 376 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = newNode(RCT_MENUITEM, (yyvsp[-5].s), (yyvsp[-3].i), 0, 0, 0, 0, -1); }
#line 2286 "y.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 378 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = newNode(RCT_MENU, (yyvsp[-3].s), -1, 0, 0, 0, 0, (yyvsp[-1].node)->index); }
#line 2292 "y.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 380 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = newNode(RCT_MENU, (yyvsp[-5].s), (yyvsp[-3].i), 0, 0, 0, 0, (yyvsp[-1].node)->index); }
#line 2298 "y.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 382 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = newNode(RCT_MENU, (yyvsp[-7].s), (yyvsp[-5].i), 0, 0, 0, 0, (yyvsp[-1].node)->index); }
#line 2304 "y.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 385 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = newNode(RCT_MENU, (yyvsp[-10].s), (yyvsp[-8].i), 0, 0, 0, 0, (yyvsp[-1].node)->index); }
#line 2310 "y.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 392 "rcparse.y" /* yacc.c:1646  */
    { newNode(RCT_TOOLBAR, "", (yyvsp[-8].i), 0, 0, 0, 0, (yyvsp[-1].node)->index); }
#line 2316 "y.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 396 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = NULL; }
#line 2322 "y.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 397 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = append((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2328 "y.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 402 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = newNode(RCT_BUTTON, "", (yyvsp[0].i), 0, 0, 0, 0, -1); }
#line 2334 "y.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 404 "rcparse.y" /* yacc.c:1646  */
    { (yyval.node) = newNode(RCT_SEPARATOR, "", -1, 0, 0, 0, 0, -1); }
#line 2340 "y.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 410 "rcparse.y" /* yacc.c:1646  */
    {}
#line 2346 "y.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 416 "rcparse.y" /* yacc.c:1646  */
    {}
#line 2352 "y.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 425 "rcparse.y" /* yacc.c:1646  */
    {}
#line 2358 "y.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 426 "rcparse.y" /* yacc.c:1646  */
    {}
#line 2364 "y.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 439 "rcparse.y" /* yacc.c:1646  */
    { newString((yyvsp[-1].i), (yyvsp[0].s)); }
#line 2370 "y.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 440 "rcparse.y" /* yacc.c:1646  */
    { newString((yyvsp[-2].i), (yyvsp[0].s)); }
#line 2376 "y.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 444 "rcparse.y" /* yacc.c:1646  */
    {}
#line 2382 "y.tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 445 "rcparse.y" /* yacc.c:1646  */
    {}
#line 2388 "y.tab.c" /* yacc.c:1646  */
    break;

  case 141:
#line 449 "rcparse.y" /* yacc.c:1646  */
    {}
#line 2394 "y.tab.c" /* yacc.c:1646  */
    break;

  case 157:
#line 488 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[0].i); }
#line 2400 "y.tab.c" /* yacc.c:1646  */
    break;

  case 158:
#line 489 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = -(yyvsp[0].i); }
#line 2406 "y.tab.c" /* yacc.c:1646  */
    break;

  case 159:
#line 490 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = -1; }
#line 2412 "y.tab.c" /* yacc.c:1646  */
    break;

  case 177:
#line 531 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 2418 "y.tab.c" /* yacc.c:1646  */
    break;

  case 178:
#line 532 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[0].i); }
#line 2424 "y.tab.c" /* yacc.c:1646  */
    break;

  case 179:
#line 536 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[0].i); }
#line 2430 "y.tab.c" /* yacc.c:1646  */
    break;

  case 180:
#line 537 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 2436 "y.tab.c" /* yacc.c:1646  */
    break;

  case 181:
#line 538 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) | (yyvsp[0].i); }
#line 2442 "y.tab.c" /* yacc.c:1646  */
    break;

  case 182:
#line 539 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-3].i); }
#line 2448 "y.tab.c" /* yacc.c:1646  */
    break;

  case 183:
#line 543 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[0].i); }
#line 2454 "y.tab.c" /* yacc.c:1646  */
    break;

  case 184:
#line 544 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-1].i); }
#line 2460 "y.tab.c" /* yacc.c:1646  */
    break;

  case 185:
#line 548 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = SW_MULTI_LINE; }
#line 2466 "y.tab.c" /* yacc.c:1646  */
    break;

  case 186:
#line 549 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = SW_NON_EDITABLE; }
#line 2472 "y.tab.c" /* yacc.c:1646  */
    break;

  case 187:
#line 550 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = SW_PASSWORD; }
#line 2478 "y.tab.c" /* yacc.c:1646  */
    break;

  case 188:
#line 551 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = SW_AUTOCHECKBOX; }
#line 2484 "y.tab.c" /* yacc.c:1646  */
    break;

  case 189:
#line 552 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = SW_AUTORADIOBUTTON; }
#line 2490 "y.tab.c" /* yacc.c:1646  */
    break;

  case 190:
#line 553 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 2496 "y.tab.c" /* yacc.c:1646  */
    break;

  case 193:
#line 563 "rcparse.y" /* yacc.c:1646  */
    { (yyval.s) = ""; }
#line 2502 "y.tab.c" /* yacc.c:1646  */
    break;

  case 194:
#line 565 "rcparse.y" /* yacc.c:1646  */
    { (yyval.s) = (yyvsp[-1].s); }
#line 2508 "y.tab.c" /* yacc.c:1646  */
    break;

  case 195:
#line 570 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[0].i); }
#line 2514 "y.tab.c" /* yacc.c:1646  */
    break;

  case 196:
#line 571 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-1].i); }
#line 2520 "y.tab.c" /* yacc.c:1646  */
    break;

  case 197:
#line 572 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = ~ (yyvsp[0].i); }
#line 2526 "y.tab.c" /* yacc.c:1646  */
    break;

  case 198:
#line 573 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = - (yyvsp[0].i); }
#line 2532 "y.tab.c" /* yacc.c:1646  */
    break;

  case 199:
#line 574 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) * (yyvsp[0].i); }
#line 2538 "y.tab.c" /* yacc.c:1646  */
    break;

  case 200:
#line 575 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) / (yyvsp[0].i); }
#line 2544 "y.tab.c" /* yacc.c:1646  */
    break;

  case 201:
#line 576 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) % (yyvsp[0].i); }
#line 2550 "y.tab.c" /* yacc.c:1646  */
    break;

  case 202:
#line 577 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) + (yyvsp[0].i); }
#line 2556 "y.tab.c" /* yacc.c:1646  */
    break;

  case 203:
#line 578 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) - (yyvsp[0].i); }
#line 2562 "y.tab.c" /* yacc.c:1646  */
    break;

  case 204:
#line 579 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) & (yyvsp[0].i); }
#line 2568 "y.tab.c" /* yacc.c:1646  */
    break;

  case 205:
#line 580 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) ^ (yyvsp[0].i); }
#line 2574 "y.tab.c" /* yacc.c:1646  */
    break;

  case 206:
#line 581 "rcparse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) | (yyvsp[0].i); }
#line 2580 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2584 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 584 "rcparse.y" /* yacc.c:1906  */


void
yyerror(const char *s)
{
    fprintf(stderr, "%s:%d: %s\n", rc_filename, rc_lineno, s);
    exit(1);
}

static RCNode *
newNode(int type, const char *name, int id, int x, int y,
        int width, int height, int children)
{
    RCNode *node = (RCNode *) malloc(sizeof(RCNode));
    if (nodesAllocated == 0) {
        nodesAllocated = 16;
        nodes = (RCNode **) malloc(nodesAllocated * sizeof(RCNode *));
    } else if (numNodes >= nodesAllocated) {
        nodesAllocated *= 2;
        nodes = (RCNode **) realloc(nodes, nodesAllocated * sizeof(RCNode *));
    }
    node->type = type;
    node->name = strdup(name);
    node->id = id;
    node->x = x;
    node->y = y;
    node->width = width;
    node->height = height;
    node->index = numNodes;
    node->flags = 0;
    node->children = children;
    node->next = -1;

    nodes[numNodes++] = node;
    return node;
}

static RCNode *
newControl(const char *name, int id, const char *type, int flags, int x, int y,
           int width, int height)
{
    if (strcmp(type, "Button") == 0) {
        if (flags & SW_AUTOCHECKBOX) {
            return newNode(RCT_CHECKBOX, name, id, x, y, width, height, -1);
        } else if (flags & SW_AUTORADIOBUTTON) {
            return newNode(RCT_RADIOBUTTON, name, id, x, y, width, height, -1);
        }
    }
    fprintf(stderr, "warning: unhandled type(\"%s\")/style in control\n", type);
    return newNode(-1, name, id, x, y, width, height, -1);
}

static RCString *
newString(int id, const char *str)
{
    RCString *s;
    if (stringsAllocated == 0) {
        stringsAllocated = 16;
        rcStrings = (RCString *) malloc(stringsAllocated * sizeof(RCString));
    } else if (numStrings >= stringsAllocated) {
        stringsAllocated *= 2;
        rcStrings = (RCString *) realloc(rcStrings,
                                         stringsAllocated * sizeof(RCString));
    }
    s = rcStrings + numStrings++;
    s->id = id;
    s->str = strdup(str);
    return s;
}

static short
readShort(FILE *f)
{
    unsigned char c1, c2;

    c1 = getc(f);
    c2 = getc(f);

    return c1 | c2 << 8;
}

static long
readLong(FILE *f)
{
    unsigned char c1, c2, c3, c4;

    c1 = getc(f);
    c2 = getc(f);
    c3 = getc(f);
    c4 = getc(f);

    return c1 | c2 << 8 | c3 << 16 | c4 << 24;
}

static RCBitmap *
newBitmap(int id, const char *filename)
{
    BMPHeader header;
    RCBitmap *bitmap;
    FILE *f;
    int ncolours;
    int y, width_in_bytes, i;
    int pad;
    char padbuf[4];

    f = fopen(filename, "rb");

    if (!f)
    {
        perror(filename);
        return NULL;
    }

    header.type = readShort(f);
    header.size = readLong(f);
    header.reserved1 = readShort(f);  /* reserved */
    header.reserved2 = readShort(f);
    header.offset_to_data = readLong(f);
    header.header_size = readLong(f);
    header.width = readLong(f);
    header.height = readLong(f);
    header.planes = readShort(f);
    header.bits_per_pixel = readShort(f);
    header.compression_type = readLong(f);
    header.size_in_bytes = readLong(f);
    header.x_pixels_per_meter = readLong(f);
    header.y_pixels_per_meter = readLong(f);
    header.colours = readLong(f);
    header.important_colours = readLong(f);

    bitmap = (RCBitmap *) malloc(sizeof(RCBitmap));

    bitmap->id = id;
    bitmap->width = header.width;
    bitmap->height = header.height;
    bitmap->bits_per_pixel = header.bits_per_pixel;

    if (bitmapsAllocated == 0) {
        bitmapsAllocated = 16;
        bitmaps = (RCBitmap **) malloc(bitmapsAllocated * sizeof(RCNode *));
    } else if (numBitmaps >= bitmapsAllocated) {
        bitmapsAllocated *= 2;
        bitmaps = (RCBitmap **) realloc(bitmaps, bitmapsAllocated * sizeof(RCNode *));
    }

    bitmap->size = header.size_in_bytes;
    bitmap->data = (unsigned char *) malloc(header.size);
    ncolours = 1 << bitmap->bits_per_pixel;
    bitmap->palette = (int *) malloc (ncolours * sizeof(int));
    for (i = 0; i < ncolours; i++) {
        unsigned char b = getc(f);
        unsigned char g = getc(f);
        unsigned char r = getc(f);
        (void) getc(f);
        bitmap->palette[i] = (b << 16) | (g << 8) | r;
    }
    width_in_bytes = bitmap->width * bitmap->bits_per_pixel / 8;
    pad = (4 - width_in_bytes) % 4;
    for (y = 0; y < bitmap->height; y++) {
        while (fread(bitmap->data + (bitmap->height - 1 - y) * width_in_bytes,
                     1, width_in_bytes, f) == -1);
        if (pad > 0) while (fread(padbuf, 1, pad, f) == -1);
    }
    fclose (f);
    bitmaps[numBitmaps++] = bitmap;
    return bitmap;
}

static void
addAccelerator(int key, int modifiers, int command)
{
    int i;
    
    /* search for menu corresponding to command */
    for (i = 0; i < numNodes; i++) {
        if (nodes[i]->type == RCT_MENUITEM && nodes[i]->id == command) {
            nodes[i]->x = key;
            nodes[i]->y = modifiers;
        }
    }
}

static RCNode *
append(RCNode *node1, RCNode *node2)
{
    RCNode *s;

    if (node1) {
        for (s = node1; s->next != -1; s = nodes[s->next])
            ;
        s->next = node2->index;
        return node1;
    } else {
        return node2;
    }
}

static int
getVirtkey(const char *str)
{
    if (!strcmp(str, "VK_INSERT"))
        return SW_INSERT;
    if (!strcmp(str, "VK_DELETE"))
        return SW_DELETE;
    return 0;
}
