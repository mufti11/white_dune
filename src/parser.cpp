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
#line 24 "parser.y" /* yacc.c:339  */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"

#ifdef _WIN32
/* Ugly hack: redefine alloca to malloc */
#define alloca malloc
#endif

#include "stdafx.h"

#include "parser.h"
#include "Scene.h"
#include "Element.h"
#include "EventIn.h"
#include "EventOut.h"
#include "ExposedField.h"
#include "Field.h"
#include "FieldValue.h"

#include "resource.h"

#include "SFMFTypes.h"

#include "Node.h"
#include "NodeScript.h"
#include "NodeNurbsSurface.h"
#include "NodeNurbsTrimmedSurface.h"
#include "NodeNurbsCurve.h"
#include "NodeNurbsPositionInterpolator.h"
#include "NodeGeoCoordinate.h"
#include "NodeGeoElevationGrid.h"
#include "NodeGeoLocation.h"
#include "NodeGeoLOD.h"
#include "NodeGeoOrigin.h"
#include "NodeGeoPositionInterpolator.h"
#include "NodeGeoViewpoint.h"

#ifdef NURBS_CURVE_ANIMATION_COMPATIBILTY
#include "NodeCurveAnimation.h"
#endif

#include "NodeComment.h"
#include "NodeExport.h"
#include "NodeImport.h"
#include "Proto.h"

#include "Stack.h"

extern void stopProto(void);

#define SYMB(id) (scene->getSymbol(id))
#define IS_OUTSIDE_PROTO "IS statement used outside PROTO"

#define BISON_MID_RULE_TYPE 1
#ifdef BISON_MID_RULE_TYPE
# define RET(type) $<type>$
#else
# define RET(type) $$
#endif

Scene *scene = NULL;

#ifdef HAVE_LIBZ 
 gzFile inputFile;
#else
 FILE *inputFile;
#endif
int inputFileSize;

int lineno = 1;

Node *targetNode;
int targetField;
int scanFor = SCAN_FOR_BOTH;

static Stack<Node *> nodeStack;
static Stack<Proto *> protoStack;
static int defName = -1;
static int currentType;
static NodeList commentNodeList;

static List<MyString> commentList;


static void    route(const MyString &srcNode, const MyString &srcField,
                     const MyString &dstNode, const MyString &dstField);
static Node   *addExport(const MyString &srcNode, const MyString &dstNode);
static Node   *addImport(const MyString &srcNode, const MyString &importedNode,
                         const MyString &dstNode);
                     
static Node   *newNode(const MyString &nodeType);

static int     checkField(Node *node, const MyString &fieldName);
static Field  *createField(int type, const MyString &name);
static void    setField(Node *node, int index, FieldValue *value);
static void    isField(Node *node, const MyString &fieldName, 
                       const MyString &isName);
static FieldValue *boolsToType(BoolArray *bools, int type);
static FieldValue *intsToType(IntArray *ints, int type);
static FieldValue *floatsToType(DoubleArray *floats, int type);
static FieldValue *stringToType(const char* string, int type);
static int fieldTypeToEnum(const char* str);
static DoubleArray *intsToFloats(IntArray *ints);
static DoubleArray *intsToDoubles(IntArray *ints);
static FieldValue *emptyMF(int type);
static FieldValue *emptyMFNodeOrNULL(int type);
static FieldValue *SillyDefaultValue(int type);
static MyString uniqName(const MyString name);
static MyString checkName(const MyString name);
static void addCommentsToNode(Node* node);
static void addCommentsToNodeList(NodeList *nodelist);
static int addDynamicElement(Element *element);
static int addDynamicExposedElement(Element *element);
static void illegalX3DV(void);
class nameTranslation {
public:
    MyString oldName;
    MyString newName;
    nameTranslation(MyString newN, MyString oldN)
       {
       newName = newN;
       oldName = oldN;
       }
};

Array<nameTranslation*> NameTranslation;

static bool x3d = false;

void setX3d(void) 
{
    x3d = true;
    scene->setX3dv();
} 

void setVrml(void) 
{
    x3d = false;
    scene->setVrml();
} 

#line 210 "y.tab.c" /* yacc.c:339  */

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
#line 168 "parser.y" /* yacc.c:355  */

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

#line 342 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 359 "y.tab.c" /* yacc.c:358  */

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
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   354

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  61
/* YYNRULES -- Number of rules.  */
#define YYNRULES  134
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  225

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   294

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    40,     2,     2,     2,
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
      35,    36,    37,    38,    39
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   213,   213,   219,   225,   226,   229,   234,   237,   238,
     239,   240,   241,   244,   245,   245,   246,   249,   250,   253,
     254,   257,   257,   279,   284,   287,   294,   297,   297,   301,
     301,   305,   306,   307,   310,   316,   317,   317,   322,   321,
     347,   354,   357,   358,   359,   363,   367,   373,   380,   382,
     386,   388,   392,   394,   397,   397,   411,   411,   428,   429,
     432,   433,   436,   437,   438,   442,   443,   446,   447,   448,
     455,   463,   470,   479,   482,   485,   486,   490,   495,   498,
     501,   505,   509,   513,   516,   518,   519,   520,   522,   523,
     524,   525,   526,   527,   528,   532,   536,   543,   544,   547,
     548,   551,   552,   553,   556,   559,   560,   564,   566,   569,
     574,   580,   581,   590,   591,   592,   593,   594,   595,   597,
     598,   599,   600,   603,   606,   609,   610,   613,   615,   616,
     619,   622,   623,   626,   629
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "BRACKET_ON", "BRACKET_OFF",
  "WING_BRACKET_ON", "WING_BRACKET_OFF", "SCRIPT", "COMPOSED_SHADER",
  "PACKAGED_SHADER", "SHADER_PROGRAM", "EXPORT", "IMPORT", "AS", "ID",
  "ID_X3D", "STRING", "INT_NUM", "FLOAT_NUM", "DEF", "EXTERNPROTO",
  "FALSE_TOK", "IS", "NULL_TOK", "PROTO", "ROUTE", "TO", "TRUE_TOK", "USE",
  "EVENT_IN", "EVENT_OUT", "EXPOSED_FIELD", "INPUT_OUTPUT", "FIELD", "X3D",
  "PROFILE", "COMPONENT", "UNIT", "META", "VRML1", "'.'", "$accept",
  "vrmlScene", "statements", "statement", "nodeStatement", "$@1",
  "protoStatement", "protoStatements", "proto", "$@2", "protoBody",
  "interfaceDeclarations", "fieldDeclaration", "$@3",
  "exposedFieldDeclaration", "$@4", "restrictedInterfaceDeclaration",
  "interfaceDeclaration", "$@5", "externproto", "$@6",
  "externInterfaceDeclarations", "externInterfaceDeclaration",
  "routeStatement", "exportStatement", "importStatement", "URLList",
  "empty", "node", "@7", "@8", "nodeBody", "dynamicNode", "shaderNode",
  "scriptBody", "scriptBodyElement", "nodeBodyElement", "fieldName",
  "nodeName", "nodeType", "fieldType", "profileId", "componentId",
  "fieldValue", "bools", "sfboolValue", "ints", "floats", "mfstringValue",
  "strings", "nodeStatements", "id", "x3dHeaderStatement",
  "headerStatement", "profileStatement", "componentStatements",
  "componentStatement", "metaStatements", "metaStatement",
  "unitStatements", "unitStatement", YY_NULLPTR
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
      46
};
# endif

#define YYPACT_NINF -121

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-121)))

#define YYTABLE_NINF -54

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      27,  -121,  -121,  -121,     5,   295,  -121,  -121,   -16,  -121,
    -121,  -121,  -121,  -121,  -121,  -121,    97,    97,  -121,  -121,
      97,    97,    97,    97,    97,  -121,  -121,  -121,  -121,  -121,
    -121,  -121,  -121,  -121,    40,  -121,    49,  -121,   295,    97,
     136,    52,  -121,    22,  -121,  -121,  -121,    35,  -121,  -121,
    -121,  -121,  -121,    53,    97,    61,    19,    48,    45,    55,
      55,    45,    97,    97,   339,    88,    93,    97,  -121,  -121,
    -121,  -121,    97,   103,  -121,    55,  -121,  -121,  -121,  -121,
    -121,  -121,    91,  -121,  -121,  -121,   127,  -121,   170,  -121,
     112,   116,  -121,  -121,    97,    39,  -121,    69,  -121,    97,
    -121,    97,    97,    97,    97,  -121,   198,   220,  -121,  -121,
    -121,  -121,   242,   125,  -121,  -121,  -121,  -121,  -121,     1,
      97,    97,    97,    97,    97,  -121,   120,    97,    97,    97,
     242,   242,  -121,  -121,   118,    97,  -121,    97,  -121,  -121,
     264,  -121,  -121,  -121,  -121,    97,  -121,  -121,  -121,  -121,
       2,  -121,   121,   124,    97,  -121,  -121,    97,   140,  -121,
    -121,  -121,    97,    97,    97,    97,    97,  -121,    97,    97,
    -121,    97,   137,   145,    97,    97,  -121,  -121,  -121,   101,
      46,    99,     4,   280,  -121,  -121,  -121,  -121,  -121,  -121,
    -121,  -121,    28,  -121,  -121,  -121,  -121,  -121,   317,   162,
     169,  -121,  -121,    97,  -121,    97,    97,  -121,  -121,  -121,
    -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,
     242,  -121,  -121,   295,  -121
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    53,   123,    53,     0,     2,     7,    53,   113,     5,
       4,     1,    60,    62,    64,    63,     0,     0,   112,   111,
       0,     0,     0,     0,     0,     6,     8,     9,    17,    18,
      10,    12,    11,    13,     0,    61,     0,    79,     3,     0,
     114,    48,    78,     0,    14,    38,    21,     0,    16,    56,
      54,   124,    81,     0,     0,     0,   115,   125,   122,   128,
     119,   131,     0,     0,     0,     0,     0,     0,    53,    53,
      82,   127,     0,     0,   116,   117,   126,   121,   129,   120,
     132,    49,    50,    15,    53,    53,     0,    66,     0,    59,
       0,     0,   130,   118,     0,     0,    41,     0,    26,     0,
      57,     0,     0,     0,     0,    76,     0,     0,    68,    75,
      65,    67,     0,    77,    55,    58,   134,   133,    51,     0,
       0,     0,     0,     0,     0,    40,     0,     0,     0,     0,
       0,     0,    35,    25,     0,     0,    80,     0,    29,    27,
       0,    83,   100,   104,    98,     0,    86,    97,    85,    33,
      90,    96,    88,    89,     0,    34,    73,     0,     0,   105,
      39,    52,     0,     0,     0,     0,     0,    53,     0,     0,
      36,     0,    31,    32,     0,     0,    94,   107,   109,     0,
       0,     0,     0,     0,    71,    95,    99,   103,   102,   101,
      72,    74,     0,    42,    43,    45,    46,    44,     0,     0,
      20,    31,    32,     0,    47,     0,     0,    30,    28,    93,
      91,    92,    84,   108,    87,   110,   106,    53,    19,    22,
       0,    69,    70,    23,    37
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -121,  -121,    -6,  -121,    -5,  -121,   -74,  -121,  -121,  -121,
    -121,  -121,    83,  -121,    84,  -121,    89,  -121,  -121,  -121,
    -121,  -121,  -121,   -64,  -121,  -121,  -121,    10,   132,  -121,
    -121,  -121,  -121,  -121,  -121,  -121,    98,  -121,    -2,   108,
      42,  -121,  -121,  -100,    47,  -120,    57,    64,  -121,    51,
    -121,   -14,  -121,  -121,  -121,   153,  -121,   123,  -121,    25,
    -121
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,     5,    25,   148,    64,    27,   198,    28,    66,
     199,    97,   106,   175,   107,   174,   108,   133,   203,    29,
      65,    95,   125,    30,    31,    32,   160,     6,    33,    69,
      68,    90,    34,    35,    88,   110,   111,   112,    41,    36,
     135,    51,    71,   149,   150,   151,   152,   153,   161,   182,
     183,    37,     7,     8,    40,    56,    57,    58,    59,    60,
      61
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      26,    38,    42,    42,   158,    11,    42,   155,   212,    42,
      42,     9,   156,    10,   105,    43,   105,   159,    44,    39,
     213,    47,    48,   144,   109,    52,   109,   -53,     1,   147,
     185,   155,   216,    26,   -53,   -53,   -53,   -53,   -53,   -53,
      72,   -53,   -53,   119,   213,    49,   -53,   -53,    42,    82,
     210,   -53,   -53,    86,    50,   -53,    54,    55,    91,   185,
      81,     2,    63,   186,   187,    62,     3,    70,   120,   121,
     122,   123,   124,   126,   113,    67,   113,    73,    87,    89,
      42,    75,    54,    77,    53,    42,    80,   136,   136,   136,
     136,    84,   118,    55,    96,    98,    85,   134,   127,   128,
     129,   103,   104,   211,    94,   209,   136,   136,   136,   136,
     136,    18,    19,   136,   136,   136,   188,   189,   114,    92,
     224,   172,   144,   173,   218,   167,    18,    19,   147,    45,
      46,   184,    21,   116,   117,   178,    22,    23,   186,   187,
     190,   188,   189,   191,   137,   138,   139,   157,   193,   194,
     195,   196,   197,    99,   201,   202,   177,   204,   171,   205,
     207,   208,   162,   163,   164,   165,   166,   206,   219,   168,
     169,   170,    53,    54,    55,   -24,   100,   200,   215,    74,
     130,   131,    78,    79,    18,    19,   132,   179,   115,   220,
      21,   221,   222,   217,    22,    23,    83,   180,    93,   101,
     102,   140,   103,   104,   181,    12,    13,    14,    15,   192,
      76,   223,    18,    19,   141,   142,   143,    20,    26,   144,
     145,   146,     0,   140,     0,   147,    24,    12,    13,    14,
      15,     0,     0,     0,    18,    19,   141,   142,   143,    20,
       0,   144,   154,   146,     0,   140,     0,   147,    24,    12,
      13,    14,    15,     0,     0,     0,    18,    19,   141,   142,
     143,    20,     0,   144,     0,   146,     0,     0,   176,   147,
      24,    12,    13,    14,    15,     0,     0,     0,    18,    19,
     177,   142,   143,    20,   214,   144,     0,    12,    13,    14,
      15,   147,    24,     0,    18,    19,     0,     0,     0,    20,
       0,     0,    12,    13,    14,    15,    16,    17,    24,    18,
      19,     0,     0,     0,    20,    21,     0,     0,     0,    22,
      23,     0,     0,    24,    12,    13,    14,    15,     0,     0,
       0,    18,    19,     0,     0,     0,    20,    21,     0,     0,
       0,    22,     0,     0,     0,    24,    12,    13,    14,    15,
       0,     0,     0,    18,    19
};

static const yytype_int16 yycheck[] =
{
       5,     7,    16,    17,     3,     0,    20,   107,     4,    23,
      24,     1,   112,     3,    88,    17,    90,    16,    20,    35,
      16,    23,    24,    21,    88,    39,    90,     0,     1,    27,
     150,   131,     4,    38,     7,     8,     9,    10,    11,    12,
      54,    14,    15,     4,    16,     5,    19,    20,    62,    63,
       4,    24,    25,    67,     5,    28,    37,    38,    72,   179,
      62,    34,    40,    17,    18,    13,    39,    14,    29,    30,
      31,    32,    33,     4,    88,    40,    90,    16,    68,    69,
      94,    56,    37,    58,    36,    99,    61,   101,   102,   103,
     104,     3,    94,    38,    84,    85,     3,    99,    29,    30,
      31,    32,    33,     4,    13,     4,   120,   121,   122,   123,
     124,    14,    15,   127,   128,   129,    17,    18,     6,    16,
     220,   135,    21,   137,   198,     5,    14,    15,    27,    21,
      22,   145,    20,    17,    18,   140,    24,    25,    17,    18,
     154,    17,    18,   157,   102,   103,   104,    22,   162,   163,
     164,   165,   166,    26,   168,   169,    16,   171,    40,    22,
     174,   175,   120,   121,   122,   123,   124,    22,     6,   127,
     128,   129,    36,    37,    38,     6,     6,   167,   183,    56,
      97,    97,    59,    60,    14,    15,    97,   140,    90,   203,
      20,   205,   206,   198,    24,    25,    64,   140,    75,    29,
      30,     3,    32,    33,   140,     7,     8,     9,    10,   158,
      57,   217,    14,    15,    16,    17,    18,    19,   223,    21,
      22,    23,    -1,     3,    -1,    27,    28,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    19,
      -1,    21,    22,    23,    -1,     3,    -1,    27,    28,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    19,    -1,    21,    -1,    23,    -1,    -1,     4,    27,
      28,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    19,     4,    21,    -1,     7,     8,     9,
      10,    27,    28,    -1,    14,    15,    -1,    -1,    -1,    19,
      -1,    -1,     7,     8,     9,    10,    11,    12,    28,    14,
      15,    -1,    -1,    -1,    19,    20,    -1,    -1,    -1,    24,
      25,    -1,    -1,    28,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    -1,    -1,    -1,    19,    20,    -1,    -1,
      -1,    24,    -1,    -1,    -1,    28,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,    34,    39,    42,    43,    68,    93,    94,    68,
      68,     0,     7,     8,     9,    10,    11,    12,    14,    15,
      19,    20,    24,    25,    28,    44,    45,    47,    49,    60,
      64,    65,    66,    69,    73,    74,    80,    92,    43,    35,
      95,    79,    92,    79,    79,    80,    80,    79,    79,     5,
       5,    82,    92,    36,    37,    38,    96,    97,    98,    99,
     100,   101,    13,    40,    46,    61,    50,    40,    71,    70,
      14,    83,    92,    16,    98,   100,    96,   100,    98,    98,
     100,    79,    92,    69,     3,     3,    92,    68,    75,    68,
      72,    92,    16,    98,    13,    62,    68,    52,    68,    26,
       6,    29,    30,    32,    33,    47,    53,    55,    57,    64,
      76,    77,    78,    92,     6,    77,    17,    18,    79,     4,
      29,    30,    31,    32,    33,    63,     4,    29,    30,    31,
      53,    55,    57,    58,    79,    81,    92,    81,    81,    81,
       3,    16,    17,    18,    21,    22,    23,    27,    45,    84,
      85,    86,    87,    88,    22,    84,    84,    22,     3,    16,
      67,    89,    81,    81,    81,    81,    81,     5,    81,    81,
      81,    40,    92,    92,    56,    54,     4,    16,    45,    85,
      87,    88,    90,    91,    92,    86,    17,    18,    17,    18,
      92,    92,    90,    92,    92,    92,    92,    92,    48,    51,
      68,    92,    92,    59,    92,    22,    22,    92,    92,     4,
       4,     4,     4,    16,     4,    45,     4,    45,    47,     6,
      92,    92,    92,    43,    84
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    41,    42,    42,    42,    42,    43,    43,    44,    44,
      44,    44,    44,    45,    46,    45,    45,    47,    47,    48,
      48,    50,    49,    51,    51,    52,    52,    54,    53,    56,
      55,    57,    57,    57,    57,    58,    59,    58,    61,    60,
      62,    62,    63,    63,    63,    63,    63,    64,    65,    65,
      66,    66,    67,    68,    70,    69,    71,    69,    72,    72,
      73,    73,    74,    74,    74,    75,    75,    76,    76,    76,
      76,    76,    76,    77,    77,    77,    77,    78,    79,    80,
      81,    82,    83,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    85,    85,    86,    86,    87,
      87,    88,    88,    88,    88,    89,    89,    90,    90,    91,
      91,    92,    92,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    94,    95,    96,    96,    97,    98,    98,
      99,   100,   100,   101,   101
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     2,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     0,     4,     2,     1,     1,     2,
       1,     0,     9,     3,     1,     2,     1,     0,     4,     0,
       4,     3,     3,     2,     2,     1,     0,     5,     0,     7,
       2,     1,     3,     3,     3,     3,     3,     8,     2,     4,
       4,     6,     1,     0,     0,     5,     0,     5,     2,     1,
       1,     1,     1,     1,     1,     2,     1,     1,     1,     5,
       5,     3,     3,     2,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     1,     3,     1,     1,
       1,     3,     3,     3,     2,     2,     1,     1,     1,     2,
       1,     2,     2,     2,     1,     1,     3,     1,     2,     1,
       2,     1,     1,     1,     2,     3,     4,     4,     5,     3,
       4,     4,     3,     1,     2,     1,     2,     2,     1,     2,
       3,     1,     2,     4,     4
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
        case 2:
#line 213 "parser.y" /* yacc.c:1646  */
    { 
                                  nodeComment();
                                  addCommentsToNodeList((yyvsp[0].nodeList));
                                  scene->addNodes(targetNode, targetField, (yyvsp[0].nodeList),
                                                  scanFor); 
                                 }
#line 1644 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 219 "parser.y" /* yacc.c:1646  */
    { 
                                  nodeComment();
                                  addCommentsToNodeList((yyvsp[0].nodeList));
                                  scene->addNodes(targetNode, targetField, (yyvsp[0].nodeList),
                                                  scanFor); 
                                }
#line 1655 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 229 "parser.y" /* yacc.c:1646  */
    {
                                  if ((yyvsp[0].node) != NULL) 
                                      (yyvsp[0].node)->appendTo((yyvsp[-1].nodeList)); 
                                  (yyval.nodeList) = (yyvsp[-1].nodeList); 
                                }
#line 1665 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 234 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeList) = new NodeList(); }
#line 1671 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 237 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); }
#line 1677 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 238 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = NULL; }
#line 1683 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 239 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = NULL; }
#line 1689 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 240 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); }
#line 1695 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 241 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); }
#line 1701 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 244 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); }
#line 1707 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 245 "parser.y" /* yacc.c:1646  */
    { defName = (yyvsp[0].id); }
#line 1713 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 245 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); }
#line 1719 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 246 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = scene->use(checkName(SYMB((yyvsp[0].id)))); }
#line 1725 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 257 "parser.y" /* yacc.c:1646  */
    { 
                                   Proto *proto = scene->getProto(SYMB((yyvsp[0].id)));
                                   if ((proto == NULL) || 
                                       proto->isScriptedProto())
                                       proto = new Proto(scene, SYMB((yyvsp[0].id)));
                                   else if (!proto->isExternProto())
                                       proto->deleteElements(); 
                                   protoStack.push(proto);
                                   if (!scene->addProtoName(SYMB((yyvsp[0].id))))
                                      scene->warning(IDS_PROTO_ALREADY_DEFINED,
                                                     (const char*)SYMB((yyvsp[0].id)));
                                   if (TheApp->isImportingProtoLibrary())
                                       proto->setFromProtoLibrary();
                                 }
#line 1744 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 273 "parser.y" /* yacc.c:1646  */
    { 
                                   scene->addProto(SYMB((yyvsp[-7].id)), protoStack.pop());
                                   stopProto();
                                 }
#line 1753 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 280 "parser.y" /* yacc.c:1646  */
    { 
                                 protoStack.peek()->define((yyvsp[-1].node), (yyvsp[0].nodeList)); 
                                 protoStack.peek()->setLoaded(true);
                                 }
#line 1762 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 288 "parser.y" /* yacc.c:1646  */
    {
            if (protoStack.empty()) 
                yyerror("syntax error");
            else 
                protoStack.peek()->addOrUpdateElement((yyvsp[0].element));
          }
#line 1773 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 297 "parser.y" /* yacc.c:1646  */
    { currentType = (yyvsp[0].int32); }
#line 1779 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 298 "parser.y" /* yacc.c:1646  */
    { (yyval.element) = createField((yyvsp[-2].int32), SYMB((yyvsp[0].id))); }
#line 1785 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 301 "parser.y" /* yacc.c:1646  */
    { currentType = (yyvsp[0].int32); }
#line 1791 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 302 "parser.y" /* yacc.c:1646  */
    { (yyval.element) = createField((yyvsp[-2].int32), SYMB((yyvsp[0].id))); }
#line 1797 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 305 "parser.y" /* yacc.c:1646  */
    { (yyval.element) = new EventIn((yyvsp[-1].int32), SYMB((yyvsp[0].id))); }
#line 1803 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 306 "parser.y" /* yacc.c:1646  */
    { (yyval.element) = new EventOut((yyvsp[-1].int32), SYMB((yyvsp[0].id))); }
#line 1809 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 307 "parser.y" /* yacc.c:1646  */
    { (yyval.element) = new Field((yyvsp[-1].element)->getType(), 
                                                         (yyvsp[-1].element)->getName(x3d), (yyvsp[0].value)); 
                                        }
#line 1817 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 311 "parser.y" /* yacc.c:1646  */
    { (yyval.element) = new ExposedField((yyvsp[-1].element)->getType(), 
                                                     (yyvsp[-1].element)->getName(x3d), (yyvsp[0].value)); 
                                        }
#line 1825 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 317 "parser.y" /* yacc.c:1646  */
    { currentType = (yyvsp[0].int32); }
#line 1831 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 318 "parser.y" /* yacc.c:1646  */
    { (yyval.element) = new ExposedField((yyvsp[-3].int32), SYMB((yyvsp[-1].id)), (yyvsp[0].value)); }
#line 1837 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 322 "parser.y" /* yacc.c:1646  */
    { 
                                  Proto *proto = new Proto(scene, SYMB((yyvsp[0].id)));
                                  protoStack.push(proto);
                                  if (!scene->belongsToNodeWithExternProto(
                                          SYMB((yyvsp[0].id)))) {
                                      if (!scene->addProtoName(SYMB((yyvsp[0].id))))
                                          scene->warning(IDS_PROTO_ALREADY_DEFINED,
                                                         (const char*)SYMB((yyvsp[0].id)));
                                      if (TheApp->isImportingProtoLibrary())
                                          proto->setFromProtoLibrary();
                                  }
                                }
#line 1854 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 335 "parser.y" /* yacc.c:1646  */
    {                       
                                  if (!scene->hasProtoName(SYMB((yyvsp[-5].id))) &&
                                      (!scene->belongsToNodeWithExternProto(
                                          SYMB((yyvsp[-5].id))))) {
                                      scene->addProto(SYMB((yyvsp[-5].id)), protoStack.pop()); 
                                      scene->getProto(SYMB((yyvsp[-5].id)))->addURLs((yyvsp[0].value));
                                  } 
                                  if (!scene->hasProtoName(SYMB((yyvsp[-5].id))))
                                      stopProto();
                                }
#line 1869 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 348 "parser.y" /* yacc.c:1646  */
    {
            if (protoStack.empty()) 
                yyerror("syntax error");
            else 
                protoStack.peek()->addElement((yyvsp[0].element));
          }
#line 1880 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 357 "parser.y" /* yacc.c:1646  */
    { (yyval.element) = new EventIn((yyvsp[-1].int32), SYMB((yyvsp[0].id))); }
#line 1886 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 358 "parser.y" /* yacc.c:1646  */
    { (yyval.element) = new EventOut((yyvsp[-1].int32), SYMB((yyvsp[0].id))); }
#line 1892 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 359 "parser.y" /* yacc.c:1646  */
    { 
                                          (yyval.element) = new Field((yyvsp[-1].int32), SYMB((yyvsp[0].id)),
                                                         SillyDefaultValue((yyvsp[-1].int32)));
                                        }
#line 1901 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 363 "parser.y" /* yacc.c:1646  */
    {
                                          (yyval.element) = new ExposedField((yyvsp[-1].int32), SYMB((yyvsp[0].id)),
                                                SillyDefaultValue((yyvsp[-1].int32)));
                                        }
#line 1910 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 367 "parser.y" /* yacc.c:1646  */
    {
                                          (yyval.element) = new ExposedField((yyvsp[-1].int32), SYMB((yyvsp[0].id)),
                                                SillyDefaultValue((yyvsp[-1].int32)));
                                        }
#line 1919 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 374 "parser.y" /* yacc.c:1646  */
    { 
               route(checkName(SYMB((yyvsp[-6].id))), SYMB((yyvsp[-4].id)), 
                     checkName(SYMB((yyvsp[-2].id))), SYMB((yyvsp[0].id))); 
             }
#line 1928 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 381 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = addExport(checkName(SYMB((yyvsp[0].id))), ""); }
#line 1934 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 383 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = addExport(checkName(SYMB((yyvsp[-2].id))), SYMB((yyvsp[0].id))); }
#line 1940 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 387 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = addImport(checkName(SYMB((yyvsp[-2].id))), SYMB((yyvsp[0].id)), ""); }
#line 1946 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 389 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = addImport(checkName(SYMB((yyvsp[-4].id))), SYMB((yyvsp[-2].id)), SYMB((yyvsp[0].id))); }
#line 1952 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 397 "parser.y" /* yacc.c:1646  */
    { 
                                        (yyval.node) = newNode(SYMB((yyvsp[-1].id)));
                                        Node *node = (yyval.node);
                                        if (node != NULL) {
                                            addCommentsToNode((yyval.node));
                                            nodeStack.push((yyval.node));
                                            if (defName != -1) { 
                                                scene->def(uniqName(SYMB(
                                                       defName)), (yyval.node));
                                                defName = -1;
                                            }
                                        }
                                      }
#line 1970 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 410 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = nodeStack.pop(); }
#line 1976 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 411 "parser.y" /* yacc.c:1646  */
    { 
                                        (yyval.node) = (yyvsp[-1].node);
                                        addCommentsToNode((yyval.node)); 
                                        nodeStack.push((yyval.node));
                                        if (defName != -1) { 
                                            scene->def(uniqName(SYMB(defName)), 
                                                       (yyval.node));
                                            defName = -1;
                                        }
                                      }
#line 1991 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 421 "parser.y" /* yacc.c:1646  */
    { 
                                        (yyval.node) = nodeStack.pop(); 
                                        ((NodeScript *) (yyval.node))->update(); 
                                      }
#line 2000 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 432 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = new NodeScript(scene); }
#line 2006 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 433 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = (yyvsp[0].node); }
#line 2012 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 436 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = newNode("ComposedShader"); }
#line 2018 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 437 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = newNode("ShaderProgram"); }
#line 2024 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 438 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = newNode("PackagedShader"); }
#line 2030 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 446 "parser.y" /* yacc.c:1646  */
    { (yyval.element) = NULL; }
#line 2036 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 447 "parser.y" /* yacc.c:1646  */
    { addDynamicElement((yyvsp[0].element)); }
#line 2042 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 448 "parser.y" /* yacc.c:1646  */
    { if (protoStack.empty()) 
                                              yyerror(IS_OUTSIDE_PROTO); 
                                          (yyval.element) = new EventIn((yyvsp[-3].int32), SYMB((yyvsp[-2].id)));
                                          addDynamicElement((yyval.element));
                                          isField(nodeStack.peek(),
                                                  SYMB((yyvsp[-2].id)), SYMB((yyvsp[0].id))); 
                                        }
#line 2054 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 455 "parser.y" /* yacc.c:1646  */
    { if (protoStack.empty()) 
                                              yyerror(IS_OUTSIDE_PROTO); 
                                          (yyval.element) = new EventOut((yyvsp[-3].int32), SYMB((yyvsp[-2].id))); 
                                          addDynamicElement((yyval.element));
                                          isField(nodeStack.peek(),
                                                  SYMB((yyvsp[-2].id)), SYMB((yyvsp[0].id))); 
                                        }
#line 2066 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 463 "parser.y" /* yacc.c:1646  */
    { if (protoStack.empty())
                                              yyerror(IS_OUTSIDE_PROTO);
                                          (yyval.element) = (yyvsp[-2].element);
                                          addDynamicElement((yyval.element));
                                          isField(nodeStack.peek(), 
                                                  (yyvsp[-2].element)->getName(x3d), SYMB((yyvsp[0].id)));
                                        }
#line 2078 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 470 "parser.y" /* yacc.c:1646  */
    { if (protoStack.empty()) 
                                              yyerror(IS_OUTSIDE_PROTO); 
                                          (yyval.element) = (yyvsp[-2].element);
                                          addDynamicExposedElement((yyval.element));
                                          isField(nodeStack.peek(),
                                                  (yyvsp[-2].element)->getName(x3d), SYMB((yyvsp[0].id))); 
                                        }
#line 2090 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 479 "parser.y" /* yacc.c:1646  */
    { setField(nodeStack.peek(),
                                                   (yyvsp[-1].int32), (yyvsp[0].value)); 
                                        }
#line 2098 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 482 "parser.y" /* yacc.c:1646  */
    { isField(nodeStack.peek(),
                                                  SYMB((yyvsp[-2].id)), SYMB((yyvsp[0].id))); 
                                        }
#line 2106 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 490 "parser.y" /* yacc.c:1646  */
    { (yyval.int32) = checkField(nodeStack.peek(),
                                                          SYMB((yyvsp[0].id))); 
                                        }
#line 2114 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 501 "parser.y" /* yacc.c:1646  */
    { (yyval.int32) = fieldTypeToEnum(SYMB((yyvsp[0].id))); }
#line 2120 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 513 "parser.y" /* yacc.c:1646  */
    { (yyval.value) = stringToType(SYMB((yyvsp[0].id)),
                                                            currentType); 
                                        }
#line 2128 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 517 "parser.y" /* yacc.c:1646  */
    { (yyval.value) = new MFString((yyvsp[-1].stringArray)); }
#line 2134 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 518 "parser.y" /* yacc.c:1646  */
    { (yyval.value) = new SFNode((yyvsp[0].node)); }
#line 2140 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 519 "parser.y" /* yacc.c:1646  */
    { (yyval.value) = emptyMFNodeOrNULL(currentType); }
#line 2146 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 521 "parser.y" /* yacc.c:1646  */
    { (yyval.value) = new MFNode((yyvsp[-1].nodeList)); }
#line 2152 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 522 "parser.y" /* yacc.c:1646  */
    { (yyval.value) = intsToType((yyvsp[0].intArray), currentType); }
#line 2158 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 523 "parser.y" /* yacc.c:1646  */
    { (yyval.value) = floatsToType((yyvsp[0].doubleArray), currentType); }
#line 2164 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 524 "parser.y" /* yacc.c:1646  */
    { (yyval.value) = boolsToType((yyvsp[0].boolArray), currentType); }
#line 2170 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 525 "parser.y" /* yacc.c:1646  */
    { (yyval.value) = intsToType((yyvsp[-1].intArray), currentType); }
#line 2176 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 526 "parser.y" /* yacc.c:1646  */
    { (yyval.value) = floatsToType((yyvsp[-1].doubleArray), currentType); }
#line 2182 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 527 "parser.y" /* yacc.c:1646  */
    { (yyval.value) = boolsToType((yyvsp[-1].boolArray), currentType); }
#line 2188 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 528 "parser.y" /* yacc.c:1646  */
    { (yyval.value) = emptyMF(currentType); }
#line 2194 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 532 "parser.y" /* yacc.c:1646  */
    { 
                                (yyvsp[-1].boolArray)->append(((SFBool *)(yyvsp[0].value))->getValue()); 
                                (yyval.boolArray) = (yyvsp[-1].boolArray); 
                                }
#line 2203 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 536 "parser.y" /* yacc.c:1646  */
    { 
                                (yyval.boolArray) = new BoolArray(); 
                                (yyval.boolArray)->append(((SFBool *)(yyvsp[0].value))->getValue()); 
                                }
#line 2212 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 543 "parser.y" /* yacc.c:1646  */
    { (yyval.value) = new SFBool(true); }
#line 2218 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 544 "parser.y" /* yacc.c:1646  */
    { (yyval.value) = new SFBool(false); }
#line 2224 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 547 "parser.y" /* yacc.c:1646  */
    { (yyvsp[-1].intArray)->append((yyvsp[0].int32)); (yyval.intArray) = (yyvsp[-1].intArray); }
#line 2230 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 548 "parser.y" /* yacc.c:1646  */
    { (yyval.intArray) = new IntArray(); (yyval.intArray)->append((yyvsp[0].int32)); }
#line 2236 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 551 "parser.y" /* yacc.c:1646  */
    { (yyvsp[-1].doubleArray)->append((yyvsp[0].sffloat)); (yyval.doubleArray) = (yyvsp[-1].doubleArray); }
#line 2242 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 552 "parser.y" /* yacc.c:1646  */
    { (yyvsp[-1].doubleArray)->append((float) (yyvsp[0].int32)); (yyval.doubleArray) = (yyvsp[-1].doubleArray); }
#line 2248 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 553 "parser.y" /* yacc.c:1646  */
    { (yyval.doubleArray) = intsToFloats((yyvsp[-1].intArray)); (yyval.doubleArray)->append((yyvsp[0].sffloat)); 
                                  delete (yyvsp[-1].intArray); 
                                }
#line 2256 "y.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 556 "parser.y" /* yacc.c:1646  */
    { (yyval.doubleArray) = new DoubleArray(); (yyval.doubleArray)->append((yyvsp[0].sffloat)); }
#line 2262 "y.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 559 "parser.y" /* yacc.c:1646  */
    { (yyval.value) = new MFString(SYMB((yyvsp[0].id))); }
#line 2268 "y.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 561 "parser.y" /* yacc.c:1646  */
    { (yyval.value) = new MFString((yyvsp[-1].stringArray)); }
#line 2274 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 564 "parser.y" /* yacc.c:1646  */
    { (yyval.stringArray) = new StringArray();
                                  (yyval.stringArray)->append(SYMB((yyvsp[0].id))); }
#line 2281 "y.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 566 "parser.y" /* yacc.c:1646  */
    { (yyvsp[-1].stringArray)->append(SYMB((yyvsp[0].id))); (yyval.stringArray) = (yyvsp[-1].stringArray); }
#line 2287 "y.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 569 "parser.y" /* yacc.c:1646  */
    {
                                          (yyval.nodeList) = new NodeList();
                                          if ((yyvsp[0].node)) 
                                              (yyvsp[0].node)->appendTo((yyval.nodeList));
                                        }
#line 2297 "y.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 574 "parser.y" /* yacc.c:1646  */
    { 
                                          if ((yyvsp[0].node)) 
                                              (yyvsp[0].node)->appendTo((yyvsp[-1].nodeList)); 
                                          (yyval.nodeList) = (yyvsp[-1].nodeList);
                                        }
#line 2307 "y.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 580 "parser.y" /* yacc.c:1646  */
    { (yyval.id) = (yyvsp[0].id); }
#line 2313 "y.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 581 "parser.y" /* yacc.c:1646  */
    {  
                                          if (x3d)
                                              scene->warning(
                                                    IDS_ILLEGAL_X3D_CHARACTER,
                                                    (const char *)SYMB((yyvsp[0].id)));
                                          (yyval.id) = (yyvsp[0].id); 
                                        }
#line 2325 "y.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 590 "parser.y" /* yacc.c:1646  */
    { illegalX3DV(); }
#line 2331 "y.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 619 "parser.y" /* yacc.c:1646  */
    { scene->addMeta(SYMB((yyvsp[-1].id)), SYMB((yyvsp[0].id))); }
#line 2337 "y.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 626 "parser.y" /* yacc.c:1646  */
    { scene->addUnit(SYMB((yyvsp[-2].id)), SYMB((yyvsp[-1].id)), 
                                                         (yyvsp[0].sffloat)); 
                                        }
#line 2345 "y.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 629 "parser.y" /* yacc.c:1646  */
    { scene->addUnit(SYMB((yyvsp[-2].id)), SYMB((yyvsp[-1].id)), 
                                                         (yyvsp[0].int32)); 
                                        }
#line 2353 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2357 "y.tab.c" /* yacc.c:1646  */
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
#line 633 "parser.y" /* yacc.c:1906  */


int yywrap(void)
{
    return 1;
}

void yyerror(const char *s)
{
    scene->setErrorLineNumber(lineno);
#ifdef HAVE_LIBZ
    scene->errorf("%s in line %d\n", s, lineno);
#else
    if (strcmp(s,"parse error") == 0)
        scene->errorf("%s (or %s) in line %d\n",
                      s, "unsupported compression (no gzip in this version)",
                      lineno);
    else
        scene->errorf("%s in line %d\n", s, lineno);
#endif
}

static Node *
newNode(const MyString &nodeType)
{
    Proto *proto;

    if (TheApp->getPrefix() != NULL) {
        proto = scene->getExtensionProto(scene->getNodeWithPrefix(nodeType));
        if (proto)
            return proto->create(scene);
    }
    proto = scene->getExtensionProto(nodeType);
    if (!proto) {
        scene->errorf("invalid node type \"%s\" in line %d\n", 
                      (const char *) nodeType, lineno);
        return NULL;
    } else {
        return proto->create(scene);
    }
}

static FieldValue *
intsToType(IntArray *ints, int type)
{
    FieldValue *r = NULL;
    const int  *data = ints->getData();
    int len = ints->size();

    switch(type) {
      case SFCOLOR:
        if (len != 3) {
            yyerror("SFColor must have 3 values");
        } else {
            r = new SFColor((float) data[0], (float) data[1], (float) data[2]);
        }
        break;
      case SFCOLORRGBA:
        if (len != 4) {
            yyerror("SFColorRGBA must have 4 values");
        } else {
            r = new SFColorRGBA((float) data[0], (float) data[1],
                                (float) data[2], (float) data[3]);
        }
        break;
      case SFDOUBLE:
        if (len != 1) {
            yyerror("SFDouble must have 1 value");
        } else {
            r = new SFDouble((double) data[0]);
        }
        break;
      case SFFLOAT:
        if (len != 1) {
            yyerror("SFFloat must have 1 float value");
        } else {
            r = new SFFloat((float) data[0]);
        }
        break;
      case SFIMAGE:
        if (len < 3) {
            yyerror("SFImage must have at least 3 values");
        } else {
            int width = data[0];
            int height = data[1];
            int depth = data[2];

            if (len - 3 != width * height) {
                char buf[1024];
                mysnprintf(buf, 1024, "SFImage data must have %d values\n", 
                        width * height);
                yyerror(buf);
            } else {
                int *pixels = new int[len - 3];
                for (int i = 0; i < (len - 3); i++)
                    pixels[i] = data[i + 3];
                r = new SFImage(width, height, depth, pixels);
            }
        }
        break;
      case SFINT32:
        if (len != 1) {
            yyerror("SFInt32 must have 1 integer value");
        } else {
            r = new SFInt32(data[0]);
        }
        break;
      case SFROTATION:
        if (len != 4) {
            yyerror("SFRotation must have 4 values");
        } else {
            r = new SFRotation((float) data[0], (float) data[1], 
                               (float) data[2], (float) data[3]);
        }
        break;
      case SFTIME:
        if (len != 1) {
            yyerror("SFTime must have 1 value");
        } else {
            r = new SFTime((double) data[0]);
        }
        break;
      case SFVEC2D:
        if (len != 2) {
            yyerror("SFVec2f must have 2 values");
        } else {
            r = new SFVec2f((double) data[0], (double) data[1]);
        }
        break;
      case SFVEC2F:
        if (len != 2) {
            yyerror("SFVec2f must have 2 values");
        } else {
            r = new SFVec2f((float) data[0], (float) data[1]);
        }
        break;
      case SFVEC3D:
        if (len != 3) {
            yyerror("SFVec3d must have 3 values");
        } else {
            r = new SFVec3d((double) data[0], (double) data[1], 
                            (double) data[2]);
        }
        break;
      case SFVEC3F:
        if (len != 3) {
            yyerror("SFVec3f must have 3 values");
        } else {
            r = new SFVec3f((float) data[0], (float) data[1], (float) data[2]);
        }
        break;
      case SFVEC4F:
        if (len != 4) {
            yyerror("SFVec4f must have 4 values");
        } else {
            r = new SFVec4f((float) data[0], (float) data[1], (float) data[2],
                            (float) data[3]);
        }
        break;
      case SFMATRIX3F:
        if (len != 9) {
            yyerror("SFMatrix3f must have 9 values");
        } else {
            r = new SFMatrix3f((float) data[0], 
                               (float) data[1], 
                               (float) data[2], 
                               (float) data[3], 
                               (float) data[4], 
                               (float) data[5], 
                               (float) data[6], 
                               (float) data[7], 
                               (float) data[8]);
        }
        break;
      case SFMATRIX4F:
        if (len != 16) {
            yyerror("SFMatrix4f must have 16 values");
        } else {
            r = new SFMatrix4f((float) data[0], 
                               (float) data[1], 
                               (float) data[2], 
                               (float) data[3], 
                               (float) data[4], 
                               (float) data[5], 
                               (float) data[6], 
                               (float) data[7], 
                               (float) data[8],
                               (float) data[9],
                               (float) data[10],
                               (float) data[11],
                               (float) data[12],
                               (float) data[13],
                               (float) data[14],
                               (float) data[15]);
        }
        break;
      case MFINT32:
        r = new MFInt32(ints->extractData(), len);
        break;
      case MFCOLOR:
      case MFCOLORRGBA:
      case MFFLOAT:
      case MFROTATION:
      case MFTIME:
      case MFVEC2F:
      case MFVEC3F:
      case MFVEC4F:
      case MFMATRIX3F:
      case MFMATRIX4F:
        r = floatsToType(intsToFloats(ints), type);
        break;
      case MFDOUBLE:
      case MFVEC2D:
      case MFVEC3D:
        r = floatsToType(intsToDoubles(ints), type);
        break;
      default:
        yyerror("type mismatch ");
        break;
    }
    delete ints;
    return r;
}

static FieldValue *
boolsToType(BoolArray *bools, int type)
{
    FieldValue *r = NULL;
    const bool *data = bools->getData();
    int len = bools->size();

    switch(type) {
      case SFBOOL:
        if (len != 1) {
            yyerror("SFBool must have 1 bool value");
        } else {
            r = new SFBool(data[0]);
        }
        break;
      case MFBOOL:
        r = new MFBool(bools->extractData(), len);
        break;
      default:
        yyerror("type mismatch ");
        break;
    }
    delete bools;
    return r;
}

static FieldValue *
floatsToType(DoubleArray *floats, int type)
{
    FieldValue     *r = NULL;
    const double   *data = floats->getData();
    int len = floats->size();

    switch(type) {
      case SFCOLOR:
        if (len != 3) {
            yyerror("SFColor must have 3 values");
        } else {
            r = new SFColor(data[0], data[1], data[2]);
        }
        break;
      case SFCOLORRGBA:
        if (len != 4) {
            yyerror("SFColorRGBA must have 3 values");
        } else {
            r = new SFColorRGBA(data[0], data[1], data[2], data[3]);
        }
        break;
      case SFDOUBLE:
        if (len != 1) {
            yyerror("SFDouble must have 1 float value");
        } else {
            r = new SFDouble(data[0]);
        }
        break;
      case SFFLOAT:
        if (len != 1) {
            yyerror("SFFloat must have 1 float value");
        } else {
            r = new SFFloat(data[0]);
        }
        break;
      case SFROTATION:
        if (len != 4) {
            yyerror("SFRotation must have 4 values");
        } else {
            r = new SFRotation(data[0], data[1], data[2], data[3]);
        }
        break;
      case SFTIME:
        if (len != 1) {
            yyerror("SFTime must have 1 value");
        } else {
            r = new SFTime((double) data[0]);
        }
        break;
      case SFVEC2D:
        if (len != 2) {
            yyerror("SFVec2d must have 2 values");
        } else {
            r = new SFVec2d(data[0], data[1]);
        }
        break;
      case SFVEC2F:
        if (len != 2) {
            yyerror("SFVec2f must have 2 values");
        } else {
            r = new SFVec2f(data[0], data[1]);
        }
        break;
      case SFVEC3D:
        if (len != 3) {
            yyerror("SFVec3d must have 3 values");
        } else {
            r = new SFVec3d(data[0], data[1], data[2]);
        }
        break;
      case SFVEC3F:
        if (len != 3) {
            yyerror("SFVec3f must have 3 values");
        } else {
            r = new SFVec3f(data[0], data[1], data[2]);
        }
        break;
      case SFVEC4F:
        if (len != 4) {
            yyerror("SFVec4f must have 4 values");
        } else {
            r = new SFVec4f((float) data[0], (float) data[1], (float) data[2],
                            (float) data[3]);
        }
        break;
      case SFMATRIX3F:
        if (len != 9) {
            yyerror("SFMatrix3f must have 9 values");
        } else {
            r = new SFMatrix3f(data[0], data[1], data[2], 
                               data[3], data[4], data[5], 
                               data[6], data[7], data[8]);
        }
        break;
      case SFMATRIX4F:
        if (len != 16) {
            yyerror("SFMatrix4f must have 16 values");
        } else {
            r = new SFMatrix4f(data[0], data[1], data[2], data[3], 
                               data[4], data[5], data[6], data[7],
                               data[8], data[9], data[10], data[11],
                               data[12], data[13], data[14], data[15]);
        }
        break;
      case MFCOLOR:
        if (len % 3 != 0) {
            yyerror("MFColor must be a multiple of 3 values");
        } else {
            r = new MFColor(floats->extractData(), len);
        }
        break;
      case MFCOLORRGBA:
        if (len % 4 != 0) {
            yyerror("MFColorRGBA must be a multiple of 4 values");
        } else {
            r = new MFColorRGBA(floats->extractData(), len);
        }
        break;
      case MFDOUBLE:
        r = new MFDouble(floats->extractData(), len);
        break;
      case MFFLOAT:
        r = new MFFloat(floats->extractData(), len);
        break;
      case MFROTATION:
        if (len % 4 != 0) {
            yyerror("MFRotation must be a multiple of 4 values");
        } else {
            r = new MFRotation(floats->extractData(), len);
        }
        break;
      case MFTIME:
        r = new MFTime(data, len);
        break;
      case MFVEC2D:
        if (len % 2 != 0) {
            yyerror("MFVec2d must be a multiple of 2 values");
        } else {
            r = new MFVec2d(floats->extractData(), len);
        }
        break;
      case MFVEC2F:
        if (len % 2 != 0) {
            yyerror("MFVec2f must be a multiple of 2 values");
        } else {
            r = new MFVec2f(floats->extractData(), len);
        }
        break;
      case MFVEC3D:
        if (len % 3 != 0) {
            yyerror("MFVec3d must be a multiple of 3 values");
        } else {
            r = new MFVec3d(floats->extractData(), len);
        }
        break;
      case MFVEC3F:
        if (len % 3 != 0) {
            yyerror("MFVec3f must be a multiple of 3 values");
        } else {
            r = new MFVec3f(floats->extractData(), len);
        }
        break;
      case MFVEC4F:
        if (len % 4 != 0) {
            yyerror("MFVec4f must be a multiple of 4 values");
        } else {
            r = new MFVec4f(floats->extractData(), len);
        }
        break;
      case MFMATRIX3F:
        if (len % 9 != 0) {
            yyerror("MFMatrix3f must be a multiple of 9 values");
        } else {
            r = new MFMatrix3f(floats->extractData(), len);
        }
        break;
      case MFMATRIX4F:
        if (len % 16 != 0) {
            yyerror("MFMatrix4f must be a multiple of 16 values");
        } else {
            r = new MFMatrix4f(floats->extractData(), len);
        }
        break;
      default:
        yyerror("type mismatch ");
        break;
    }
    delete floats;
    return r;
}

static FieldValue *
stringToType(const char *string, int type)
{
    FieldValue *r = NULL;

    switch(type) {
      case SFSTRING:
        r=new SFString(string);
        break;
      case MFSTRING:
        r=new MFString(string);
        break;
      default:
        yyerror("type mismatch ");
        break;
    }
    return r;
}

static int 
fieldTypeToEnum(const char* str)
{
    int ret = typeStringToEnum(str);
    if (ret == -1) {
        MyString errorMesg = "";
        errorMesg += "unknown type \"";
        errorMesg += str;
        errorMesg += "\"";
        yyerror(errorMesg);
        }
    return ret;
}

static FieldValue *
emptyMFNodeOrNULL(int type)
{
    if (type==SFNODE)
       return new SFNode(NULL);
    else if (type==MFNODE)
       return emptyMF(type);
    else {
       /* NULL only allowed for Node types
          see VRML97 Grammar 
          http://www.web3d.org/x3d/specifications/vrml/ISO-IEC-14772-IS-VRML97WithAmendment1/part1/grammar.html
          sfnodeValue ::= 
              nodeStatement | 
              NULL ; 
       
       */
       yyerror("NULL only allowed for Node types, assuming \"[]\"");
       return emptyMF(type);
    }
}

static FieldValue *
emptyMF(int type)
{
    return typeDefaultValue(type);
}

/*
 * Currently, EXTERNPROTO Definitions are not read
 * field need a senseful default value
 */

static FieldValue *
SillyDefaultValue(int type)
{
    if (FieldValue *value = typeDefaultValue(type))
       return value;
    else {
       yyerror("intern error: type no supported");
       return NULL;
    }
}

static int
repairField(Node *node, int index)
{
    return node->repairField(index);
}

//
// checkField() - verify a field reference
//
// check that the node "node" has the field "fieldName"
// if not, print an error
// if so, stash its type in the lexer, and return its index

static int
checkField(Node *node, const MyString &fieldName)
{
    if (!node) return INVALID_INDEX;

    Proto *proto = node->getProto();

    if (!proto) return INVALID_INDEX;

    int index = proto->lookupField(fieldName, x3d);
    if (index == INVALID_INDEX)
        index = proto->lookupExposedField(fieldName, x3d);
    if (index == INVALID_INDEX) {
        if (TheApp->getKambiMode())
           if (node->getType() == KAMBI_KAMBI_NAVIGATION_INFO)
               if (strcmp(fieldName, "headBobbingDistance") == 0)
                   scene->errorf("KambiNavigationInfo.headBobbingDistance is deprecated/not supported any more");
        scene->invalidField(proto->getName(x3d), fieldName);
    } else {
        currentType = proto->getField(repairField(node, index))->getType();
    }

    return index;
}

static FieldValue *
convertSFToMFValue(FieldValue *value, int fieldType)
{
    if (fieldType == MFSTRING && value->getType() == SFSTRING) {
        FieldValue *newValue = new MFString(((SFString *) value)->getValue());
        value->unref();
        return newValue;
    } else if (fieldType == MFNODE && value->getType() == SFNODE) {
        NodeList *list = new NodeList();
        list->append(((SFNode *) value)->getValue());
        FieldValue *newValue = new MFNode(list);
        value->unref();
        return newValue;
    } else if (fieldType == MFBOOL && value->getType() == SFBOOL) {
        FieldValue *newValue = new MFBool(((SFBool *) value)->getValue());
        value->unref();
        return newValue;
    } else
        return value;
}

static Field *
createField(int type, const MyString &name)
{
    if (type == -1) {
        return NULL;
    }
    return new Field(type, name, typeDefaultValue(type));
}

static void
setField(Node *node, int index, FieldValue *value)
{
    if (!node || !value || index < 0) return;

    Proto *proto = node->getProto();

    if (!proto) return;

    int newIndex = repairField(node, index);
    Field *field = proto->getField(newIndex);

    if (field == NULL) {
        scene->errorf("unknown/unsupported field 8-(\n");
        delete value;
        return;
    }

    value = convertSFToMFValue(value, field->getType());

    if (value->getType() != field->getType()) {
        scene->errorf("type mismatch:  field \"%s\"\n", 
                      (const char *)field->getName(x3d));
        delete value;
        value = typeDefaultValue(field->getType());
    } 

    scene->setField(node, newIndex, value);
    if (TheApp->is4Catt())
        node->generateTreeLabel();
}

static void
isField(Node *node, const MyString &fieldName, const MyString &isName)
{
    int srcField, srcEventIn, srcEventOut, srcExposedField;
    int dstField, dstEventIn, dstEventOut, dstExposedField;

    if (!node) return;

    Proto *source = node->getProto();

    if (!source) return;

    if (protoStack.empty()) {
        scene->errorf("IS statement used outside PROTO\n");
        return;
    }

    Proto *proto = protoStack.peek(); // wrong for recursive PROTOs 8-(

    if ((srcExposedField = source->lookupExposedField(fieldName, x3d)) != -1) {
        dstEventIn = proto->lookupEventIn(isName, x3d);
        dstEventOut = proto->lookupEventOut(isName, x3d);

        if ((dstExposedField = proto->lookupExposedField(isName, x3d)) != -1) {
            ExposedField *expField = proto->getExposedField(dstExposedField);
            if (expField != NULL) {
                srcField = source->lookupField(fieldName, x3d);
                if (srcField != -1)
                    expField->addIs(node, srcField, EL_EXPOSED_FIELD,
                                    proto, dstExposedField);
                else 
                    scene->invalidField(source->getName(x3d), fieldName);
            }
        } else if ((dstField = proto->lookupField(isName, x3d)) != -1) {
            Field *field = proto->getField(dstField);
            if (field != NULL) {
                srcField = source->lookupField(fieldName, x3d);
                if (srcField != -1)
                    field->addIs(node, srcField, EL_EXPOSED_FIELD,
                                 proto, dstField);
                else 
                    scene->invalidField(source->getName(x3d), fieldName);
            }
        } else if ((dstEventIn = proto->lookupEventIn(isName, x3d)) != -1) {
            EventIn *eventIn = proto->getEventIn(dstEventIn);
            if (eventIn != NULL)
                eventIn->addIs(node, srcExposedField, EL_EXPOSED_FIELD,
                               proto, dstEventIn, EIF_IS);
        } else if ((dstEventOut = proto->lookupEventOut(isName, x3d)) != -1) {
            EventOut *eventOut = proto->getEventOut(dstEventOut);
            if (eventOut != NULL)
                eventOut->addIs(node, srcExposedField, EL_EXPOSED_FIELD,
                                proto, dstEventOut, EOF_IS);
        } else {
            scene->invalidField(source->getName(x3d), isName);
        }
    } else if ((srcField = source->lookupField(fieldName, x3d)) != -1) {
        dstField = proto->lookupField(isName, x3d);
        if (dstField != -1) {
            Field *field = proto->getField(dstField);
            if (field != NULL)
                field->addIs(node, srcField, EL_FIELD, proto, dstField);
        } else {
            scene->invalidField(source->getName(x3d), isName);
        }
    } else if ((srcEventIn = source->lookupEventIn(fieldName, x3d)) != -1) {
        dstEventIn = proto->lookupEventIn(isName, x3d);
        if (dstEventIn != -1) {
            EventIn *eventIn = proto->getEventIn(dstEventIn);
            if (eventIn != NULL)
                eventIn->addIs(node, srcEventIn, EL_EVENT_IN, 
                               proto, dstEventIn, EIF_IS);
        } else {
            scene->invalidField(source->getName(x3d), isName);
        }
    } else if ((srcEventOut = source->lookupEventOut(fieldName, x3d)) != -1) {
        dstEventOut = proto->lookupEventOut(isName, x3d);
        if (dstEventOut != -1) {
            EventOut *eventOut = proto->getEventOut(dstEventOut);
            if (eventOut != NULL)
                eventOut->addIs(node, srcEventOut, EL_EVENT_OUT, 
                                proto, dstEventOut, EOF_IS);
        } else {
            scene->invalidField(source->getName(x3d), isName);
        }
    } else {
        scene->invalidField(source->getName(x3d), fieldName);
    }
}

static void
route(const MyString &srcNode, const MyString &srcField,
      const MyString &dstNode, const MyString &dstField)
{
    int eventIn = INVALID_INDEX;
    int eventOut = INVALID_INDEX;
    bool valid = true;

    Node *src = scene->use(srcNode);
    if (!src) {
        scene->invalidNode(srcNode);
        valid = false;
        
    } else
        eventOut = src->lookupEventOut(srcField, x3d);

    Node *dst = scene->use(dstNode);
    if (!dst) {
        scene->invalidNode(dstNode);
        valid = false;
    } else {
        eventIn = dst->lookupEventIn(dstField, x3d);
        if (eventIn == -1) {
            int iexposedField = dst->getProto()->lookupExposedField(dstField, 
                                                                    x3d);
            if (iexposedField != -1) {
                ExposedField *exposedField = dst->getProto()->getExposedField(
                                             iexposedField);
                eventIn = exposedField->getEventIn();
            }
        }        
    }        

    if ((eventIn == INVALID_INDEX) && (eventOut == INVALID_INDEX)) {
        valid = false;
        scene->errorf("invalid ROUTE command -both end types unknown- in line %d -ignoring\n",
                      lineno); 
    }
 
    if (valid && (eventIn == INVALID_INDEX)) {
        // create matching eventIn for IMPORT command
        if (dst->getType() == X3D_IMPORT) {
            NodeImport *node = (NodeImport *)dst;
            int type = src->getProto()->getEventOut(eventOut)->getType();
            node->addEventIn(type, dstField);
            eventIn = dst->lookupEventIn(dstField, x3d);
        } else {
            scene->errorf("node \"%s\" has no eventIn \"%s\"\n",
                          (const char *) dstNode, (const char *) dstField);
            valid = false;
        }
    }

    if (valid && (eventOut == INVALID_INDEX)) {
        // create matching eventOut for IMPORT command
        if (src->getType() == X3D_IMPORT) {
            NodeImport *node = (NodeImport *)src;
            int type = dst->getProto()->getEventIn(eventIn)->getType();
            node->addEventOut(type, srcField);
            eventOut = src->lookupEventOut(srcField, x3d);
        } else {
            scene->errorf("node \"%s\" has no eventOut \"%s\"\n",
                          (const char *) srcNode, (const char *) srcField);
            valid = false;
        }
    }


    if (valid) {
        src->update();
        dst->update();
        if (!scene->addRoute(src, eventOut, dst, eventIn))
            scene->errorf("invalid ROUTE command in line %d\n",lineno);        
    }

}

static Node *    
addExport(const MyString &srcNode, const MyString &dstNode) {
    NodeExport *node = (NodeExport *) scene->createNode("EXPORT");
    node->localDEF(new SFString(srcNode));
    if (dstNode.length() > 0)
        scene->def(dstNode, node);
    return node;
}

static Node *  
addImport(const MyString &srcNode, const MyString &importedNode,
          const MyString &dstNode) {
    NodeImport *node = (NodeImport *) scene->createNode("IMPORT");
    if (dstNode.length() > 0)
        scene->def(dstNode, node);
    node->inlineDEF(new SFString(srcNode));
    node->importedDEF(new SFString(importedNode));
    return node;
}


static DoubleArray *
intsToFloats(IntArray *ints)
{
    if (ints == NULL) return NULL;

    int len = ints->size();
    DoubleArray *r = new DoubleArray(len);
    const int *d = ints->getData();
    for (int i = 0; i < len; i++) {
        r->set(i, (float) d[i]);
    }
    return r;
}

static DoubleArray *
intsToDoubles(IntArray *ints)
{
    if (ints == NULL) return NULL;

    int len = ints->size();
    DoubleArray *r = new DoubleArray(len);
    const int *d = ints->getData();
    for (int i = 0; i < len; i++) {
        r->set(i, (double) d[i]);
    }
    return r;
}

/* avoid double DEFs while file import */

static MyString
uniqName(const MyString name)
{
    int i = 0;
    if (scene->hasAlreadyName(name)) {
        while (true) {
            int len = strlen((const char*) name) + 512;
            char* buf=(char*) malloc(len);
            mysnprintf(buf, len, "%s_%d", (const char*) name, i++);
            MyString* newName=new MyString(buf);
            free(buf);
            if (!scene->hasAlreadyName(*newName)) {
                NameTranslation.append(new nameTranslation(*newName,name));
                return *newName;
            }
        }
    }
    return name;
}

/* replace double DEFs while file import */

static MyString
checkName(const MyString name)
{
    for (int i = NameTranslation.size() - 1; i >= 0; i--)
        if (name == NameTranslation[i]->oldName) {
            return NameTranslation[i]->newName;
        }
    return name;
}

void forgetNameTranslation(void)
{
    NameTranslation.resize(0);
}

static void 
addCommentsToNode(Node* node)
{
    if (commentNodeList.size() > 0) {
        for (int i = 0;i < commentNodeList.size(); i++)
            node->appendComment(commentNodeList[i]);
        commentNodeList.resize(0);
    }
}

static void 
addCommentsToNodeList(NodeList *nodelist)
{
    if (commentNodeList.size() > 0) {
        for (int i = 0;i < commentNodeList.size(); i++)
            nodelist->append(commentNodeList[i]);
        commentNodeList.resize(0);
    }    
}

static int addDynamicElement(Node *node, Element *element)
{
    int fieldIndex = -1;
    if (nodeStack.empty()) 
        yyerror("syntax error");
    else if (element) { 
        node->getProto()->addElement(element);
        node->update();
        if (element->getElementType() == EL_FIELD) {
            Field *field = (Field *)element;
            // search for number of field to use setField 
            Proto *proto = node->getProto();
            for (int i = 0; i < proto->getNumFields(); i++)
                if (proto->getField(i)->getName(x3d) == field->getName(x3d)) {
                    FieldValue *value = field->getDefault(x3d);
                    value = convertSFToMFValue(value, field->getType());
                    setField(node, i, value);
                    fieldIndex = i;
                }
        }
    }
    return fieldIndex;
}


static int addDynamicExposedElement(Node *node, Element *element)
{
    int fieldIndex = -1;
    if (nodeStack.empty()) 
        yyerror("syntax error");
    else if (element) { 
        node->getProto()->addElement(element);
        node->update();
        if (element->getElementType() == EL_EXPOSED_FIELD) {
            ExposedField *field = (ExposedField *)element;
            // search for number of field to use setField 
            Proto *proto = node->getProto();
            for (int i = 0; i < proto->getNumExposedFields(); i++)
                if (proto->getField(i)->getName(x3d) == field->getName(x3d)) {
                    FieldValue *value = proto->getField(i)->getDefault(x3d);
                    value = convertSFToMFValue(value, field->getType());
                    setField(node, i, value);
                    fieldIndex = i;
                }
        }
    }
    return fieldIndex;
}

static int addDynamicElement(Element *element)
{
    return addDynamicElement(nodeStack.peek(), element);
} 

static int addDynamicExposedElement(Element *element)
{
    return addDynamicExposedElement(nodeStack.peek(), element);
} 

void
addToCurrentComment(char *string)
{
   if (strlen(string) > 0)
       commentList.append(string);
}

void
nodeComment(void)
{
    if (commentList.size() != 0) {
        StringArray *mfString = new StringArray();
        for (List<MyString>::Iterator* commentpointer = commentList.first();
            commentpointer != NULL; commentpointer = commentpointer->next()) {
            const char *string = commentpointer->item();
            bool isVRMLheader = false;
            if (strcmp(string, "#VRML V2.0 utf8") == 0) 
                isVRMLheader = true;
            if (!isVRMLheader) {
                if (string[0] == '\n')
                    string++;
                StringArray array;
                MyString str = "";
                str += string;
                str.split(&array, "\n");
                for (int i = 0; i < array.size(); i++)
                    if (array[i][0] == '#')
                        mfString->append(mystrdup(array[i]) + 1);
                    else
                        mfString->append(mystrdup(array[i]));
            }
        }
        if (mfString->size() > 0) {
            NodeComment *node = (NodeComment *) scene->createNode("#");
            node->comment(new MFString(mfString));
            commentNodeList.append(node);
        }
        commentList.removeAll();
    }
}

void
insideNodeComment(void)
{
}

void
MFComment(void)
{
}

void
insideMFComment(void)
{
}

void 
illegalX3DV(void)
{
    scene->warning(IDS_PROFILE_MISSING);
}

#ifdef HAVE_LIBEXPAT
/*
 * XML parsing code from doug sanden
 *
 * Copyright (C) 2010 Doug Sanden
 * 
 * This source is licensed under the GNU LGPL v2.1
 * Please read http://www.gnu.org/copyleft/lgpl.html for more information
 *
 * This software comes with the standard NO WARRANTY disclaimer for any
 * purpose. Use it at your own risk. If there's a problem you get to fix it.
 *
 *
 * I, Doug Sanden, hereby grant the permission to change the License on the 
 * white_dune XML parsing code from GPL to LGPL.
 */



//  if the parser complains with "not well-formed (invalid token)"
//  you should use something like
//     <?xml version="1.0" encoding="iso-8859-1"?>
//  instead of the usual 
//     <?xml version="1.0" encoding="utf-8"?>
//  header in your XML file (or just fix the nonmatching characters in the file)

# include <expat.h>
# include <ctype.h>
static int depth = 0; //xml elements
static int level = 0; //x3d nodes

class DataNode {
public:
    DataNode() {}
    DataNode(void *v) {}
    NodeList *nodeList;
    IntArray *containerFields;
};

static Stack<DataNode> nodes;

#define STATUS_NONE 0
#define STATUS_NODE 1
#define STATUS_USE 2
#define STATUS_ROUTE 3
#define STATUS_SCRIPT 4
#define STATUS_SCRIPT_ELEMENT 5
#define STATUS_PROTO 6
#define STATUS_PROTOINTERFACE -7
#define STATUS_PROTO_ELEMENT 8
#define STATUS_PROTOBODY 9
#define STATUS_PROTOINSTANCE_FIELDVALUE 10
#define STATUS_HEAD 11
#define STATUS_IMPORTEXPORT 12
#define STATUS_SHADERPART 13

static Stack<int> status;
static Stack<int> currentFieldIndex;
static bool done;
static NodeList *rootNodes;
char *currentURL = NULL;
char *parentURL = NULL;
static Array<XML_Parser> x3dParser;
static int contextIndex = -1; //contextIndex 0=scene, protobody 1, protobody 2..
static int doingcdata = 0;
static int gotcdata = 0;

#ifdef __sgi
# define XMLCALL
#endif

extern void startProto(void);

static void XMLCALL startCDATA (void *userData) 
{
    if (currentURL != NULL) {
        free(currentURL);
        currentURL = NULL;
    }
    currentURL = (char *) malloc(1);
    currentURL[0] = '\0';
    doingcdata = 1;
    gotcdata = 0;

}

static void XMLCALL endCDATA (void *userData) 
{
    doingcdata = 0;
}

static void XMLCALL handleCDATA (void *userData, const char *string, int len) 
{
    if (doingcdata) {
        if (len == 1)
            if (string[0] == '\r')
                return;
        int curlen = strlen(currentURL);
        int newlen = curlen + len + 1;

        // == awk gsub("\r", "", string)
        char *temp = (char *)malloc(len + 2);
        strncpy(temp, string, len);
        temp[len] = '\0';
        temp[len + 1] = '\0';
        char *firstToken = strtok(temp, "\r");
        currentURL = (char *)realloc(currentURL, newlen);
        currentURL[curlen] = 0;
        if (firstToken == NULL)
            currentURL = strcat(currentURL, temp);
        else {
            char *token = strtok(firstToken, "\r");
            while (token != NULL) {
                currentURL = strcat(currentURL, token);
                token = strtok(token + strlen(token) + 1, "\r");
            }                
        }
        free(temp);
        gotcdata = strlen(currentURL);
    }
}


static void XMLCALL handleComment(void *userData, const XML_Char *data)
{
    char *mydata = strdup(data);
    addToCurrentComment(mydata);
    nodeComment(); //converts from list of strings to list of comment nodes
    if (protoStack.peek())
        addCommentsToNodeList(nodes.peek().nodeList);
    else if (nodeStack.peek() == NULL)
        addCommentsToNodeList(rootNodes);
    else {
        addCommentsToNodeList(nodes.peek().nodeList);
    }
}

static void mfstrsplit(Array<const char *> *toksArray, char *string)
{
    /* special version of strsplit just for mf strings */
    int count = 0;
 
    int len = strlen(string);
    if (len == 0)
       (*toksArray)[count] = string;
    else {
        //xml cases 
        // a) url='"one" "two" "three"'  //normal case, we get str ["one" "two" "three"]
        // b) url="one"  //no '', we get [one]
        // c) url=' \t\n\r"one"\t\n\r "two"\n \t \r"three"\n\r' //crazy whitespace between sfstrings
        const char dq = '"';
        const char *wseps = " '\t\n\r,";  //just skip single quotes if they are there
        int start, end, i, j;
        
        //find first non-white space
        start = 0;
        bool contLoop = true;
        for (j = 0; (j < len) && contLoop; j++) {
            contLoop = false;
            for (int k = 0; k < strlen(wseps); k++) 
                if (string[j] == wseps[k]) {
                    contLoop = true;
                    break;
                }
            if (!contLoop)
                start = j;
        }
        j = j - 1;
        if (string[j] == dq) {
            j++; //if its ", step inside the SFString
            start = j;
        }
        bool insf = true; //else assume we're already inside, no leading "
        count++;
        for (i = j; i < len; i++) {
            //this loop ignors everything outside of doublequote pairs
            //and assumes we start inside a double quote pair
            if (string[i] == dq) {
                insf = !insf;
                if (insf) {
                    count ++;
                    start = i + 1;
                } else {
                    end = i;
                    int tlen = end - start;
                    char *sfs = (char *)malloc(tlen + 1);
                    strncpy(sfs, string + start, tlen);
                    sfs[tlen] = '\0';
                    (*toksArray)[count-1] = sfs;
                }
            }
        }
        if (insf) {
            //no end "
            end = i;
            int tlen = end - start;
            if (tlen > 0) {
                char *sfs = (char *)malloc(tlen + 1);
                strncpy(sfs, string + start, tlen);
                sfs[tlen] = '\0';
                (*toksArray)[count-1] = sfs;
            }
        }
    }
}

static char **strsplit(char *string, char *seps, int *tcount)
{
    int count = 0;
    char **toksArray;
    char *token;
    int toksSize = 256;
    toksArray = (char **)malloc(sizeof(char*) * toksSize);
    token = strtok(string, seps);
    while (token != NULL) {
        /* While there are tokens in "string" */
        count++;
        if (count + 1 > toksSize) {
            toksSize = 2 * count;
            toksArray = (char **)realloc(toksArray,sizeof(char*) * toksSize);
        }
        toksArray[count - 1] = token;
        /* Get next token: */
        token = strtok(NULL, seps);
    }
    toksArray[count] = NULL;
    *tcount = count;
    return toksArray;
}

#define TYPE_BOOL   0
#define TYPE_STRING 1
#define TYPE_FLOAT  2
#define TYPE_INT    3
#define TYPE_NODE   4

static FieldValue *
parseFieldValue(int curtype, const char* value)
{
    int sftype = curtype;
    int fmtype = -1;
    if (isMFType(curtype))
        sftype = getSFType(curtype);

    switch (sftype) {
      case SFBOOL:
        fmtype = TYPE_BOOL; 
        break;
      case SFSTRING:
        fmtype = TYPE_STRING;
        break;
      case SFCOLOR:
      case SFCOLORRGBA:
      case SFDOUBLE:
      case SFFLOAT:
      case SFMATRIX3F:
      case SFMATRIX4F:
      case SFROTATION:
      case SFTIME:
      case SFVEC2D:
      case SFVEC2F:
      case SFVEC3D:
      case SFVEC3F:
      case SFVEC4F:
        fmtype = TYPE_FLOAT;
        break;
      case SFIMAGE:
      case SFINT32:
        fmtype = TYPE_INT;
        break;
      case SFNODE:
        fmtype = TYPE_NODE; 
        break;
      default:
        assert("internal error: no fmtype");
    }

    FieldValue *fv = NULL;
    char seps[]  = " ,\t\n";
    char **tokens;
    int tcount;
    switch (fmtype) {
      case TYPE_BOOL:
        {
        char cbool[6];
        int len = strlen(value);
        if (len == 4 || len == 5) {
            strcpy(cbool, value);
            for (int i = 0; i < len; i++) { 
                // upper casing covers: false, False, FALSE
                cbool[i] = toupper(cbool[i]); 
            }
            if (!strcmp(cbool, "FALSE")) 
                fv = new SFBool(false);
            if (!strcmp(cbool,"TRUE")) 
                fv = new SFBool(true); 
        }
        if ((strlen(value) == 0) && (curtype == MFBOOL))
            fv = new MFBool();
        }
        break;
      case TYPE_STRING:
        {
        StringArray *strings = new StringArray();
        // doesn't split on blank (but does on ") 
        // in case a filename has a blank in it.
        Array<const char *> tokens;
        mfstrsplit(&tokens, (char *)value);
        tcount = tokens.size();
        for (int i = 0; i < tcount; i++) {
            strings->append(MyString(tokens[i]));
        }
        if (tcount == 0) {
            strings->append(value);
            swDebugf("missing double quotes in SF/MFSTRING, assuming \"%s\"\n",
                     (unsigned char *)value);
        }
        if (curtype == MFSTRING)
            fv = new MFString(strings);  // '"WALK" "ANY"' 
        else if (curtype == SFSTRING)
            fv = new SFString(strings->get(0));
        }
        break;
      case TYPE_FLOAT:
        {                                     
        DoubleArray *floats = new DoubleArray();
        tokens = strsplit((char *)value, seps, &tcount);
        double tmp;
        for (int i = 0; i < tcount; i++) {
            sscanf(tokens[i], "%lf", &tmp);
            floats->append(tmp);
        }
        fv = floatsToType(floats, curtype);
        free(tokens);
        }
        break;
      case TYPE_INT:
        {
        IntArray *ints = new IntArray();
        tokens = strsplit((char *) value, seps,&tcount);
        for (int i = 0; i < tcount; i++) {
            int icnt;
            int itmp;
            /* check for hex */
            if ((strchr(tokens[i], 'x') == NULL) &&
                (strchr(tokens[i], 'X') == NULL))
                icnt = sscanf(tokens[i],"%d",&itmp);
            else
                icnt = sscanf(tokens[i],"%x",&itmp); 
            if (icnt > 0)
                ints->append(itmp);
        }
        fv = intsToType(ints, curtype);
        free(tokens);
        }
        break;
      case TYPE_NODE:
        if (strcmp(value, "NULL") == 0)
            fv = new SFNode();
        if (curtype == MFNODE)
            fv = new MFNode();
        break;
      default:
        assert("internal error: no fmtype");
    }
    return fv;
}

static void parseContainerField(Node *node, const char **attrib, int index)
{
    int field = -1;
    int nodeTop = nodeStack.getTop();
    Node *parentNode = nodeStack.peek(nodeTop - 1);
    if ((nodeTop > 0) && (parentNode != NULL)) {
        field = parentNode->getProto()->lookupField(attrib[index + 1]);
        if (field >= 0) {
            node->setContainerField(field);
        } else
            swDebugf("invalid containerField=%s in line %d\n", 
                     attrib[index + 1], lineno);
    }
}

/*
Concept of operation:
<X3D>
 <Scene>
  <Transform>
   <Shape>
     ...
   </Shape>
   <Transform>
     ...
   </Transform>
  </Transform>
 </Scene>
<X3D>

To gather children [] or other defaultContainer field data nested 
within a <Type> </Type> pair, we provide an empty node list at the end of 
a xml start() and check it for contents at the beginning of the corresponding xml end().
If there's something in the node list, and the parent node takes nodes in children or other field
as indicated by a status flag we go through it item by item finding the best parent field to put it in.
Some xml elements -like X3D, Scene, ProtoInterface, ProtoBody, head- don't correspond to vrml nodes.
we skip adding non-node elements in the end() function by checking status and acting accoringly. 

depth - indent depth for xml 
level - node depth
status     - holds a status flag indicating how to handle the xml element in end()
- STATUS_NONE  - a non-node xml element, such as X3D, Scene, ROUTE, IS - skip in end()
- STATUS_NODE  - a regular node - most go here
- STATUS_PROTOINSTANCE - a proto instance, with fieldValues
- STATUS_PROTO - a proto definition
...
nodeStack  - holds the current node (or NULL at the scene rootnode level)
protoStack - holds the current proto (or NULL in the scene context)
current_node  - is the one that children  nodes and fields are added to
current_proto - is where proto body nodes are added (if null, added to scene)
nodes      - stack of node lists. Each nodelist captures the children or 
             defaultContainer child elements, or SFNode/MFNode field nodes, 
             or protobody nodes, for retrieval in xml end().
*/

static void XMLCALL start(void *data, const char *element, const char **attrib)
{
    //we'll do some unconditional pushes here, and if wrong we'll pop and re-push below
    status.push(STATUS_NONE); //assume a non-node xml element here
    Node *current_node  = nodeStack.peek(); //null is at the scene rootnode level
    nodeStack.push(current_node); //assume a non-node xml element here and re-push parent node as current node to bridge node gap
    Proto *current_proto = protoStack.peek(); //null is in the scene context

    if (strcmp(element, "X3D") == 0) { 
        x3d = true;
        bool setX3dVersion = false;
        for (int i = 0; attrib[i] != NULL; i += 2)
            if (strcmp(attrib[i], "version") == 0) 
                if (strcmp(attrib[i + 1], "3.0") == 0) {
                    setX3dVersion = true;
                    scene->setParsedX3dVersion(0);
                    scene->updateSceneMap();
                }
        if (!setX3dVersion) {
            scene->setParsedX3dVersion(-1);
            scene->updateSceneMap();
        }
    }
    if (strcmp(element, "X3D") && strcmp(element, "Scene") && 
        strcmp(element, "head") && strcmp(element, "component") && 
        strcmp(element, "IMPORT") && strcmp(element, "EXPORT") && 
        strcmp(element, "meta") && strcmp(element, "unit") && 
        strcmp(element, "ROUTE")) {
        bool isUse = false;
        Node *node = NULL;
        /* check for a USE */
        for (int i = 0; attrib[i] != NULL; i += 2) { 
            if (strcmp("USE",attrib[i]) == 0) { //"USE" 
                node = scene->use(checkName(attrib[i + 1]));
                MyString name = "";
                name += attrib[i + 1];
                checkName(name);
                if (node != NULL) {
                    isUse = true;
                    status.pop();
                    status.push(STATUS_USE);
                    nodeStack.pop();
                    nodeStack.push(node);
                    current_node = node;
                    current_node->setContainerField(-1);
                    level++;
                    node->appendTo(nodes.peek().nodeList);
                }
                for (int j = 0; attrib[j] != NULL; j += 2) 
                    if (!strcmp("containerField", attrib[j])) {
                        parseContainerField(current_node, attrib, j);
                    }
                nodes.peek().containerFields->append(
                    current_node->getContainerField());

                break; //if it's a USE we ignor all other attributes except 'containerfield'
            }
        }
        if (!isUse) {
            /* create node */
            bool protoInstance = false;
            if (!strcmp(element, "field")) {
                /* doesn't create a node, adds to it's parents fields */
                const char *name = NULL;
                const char *type = NULL;
                const char *accessType = NULL;
                const char *value = NULL;
                const char *appinfo = NULL;
                const char *documentation = NULL;
                for (int i = 0; attrib[i] != NULL; i += 2) { 
                    if (!strcmp(attrib[i], "name"))
                        name = attrib[i + 1];
                    else if (!strcmp(attrib[i], "type"))
                        type = attrib[i + 1];
                    else if (!strcmp(attrib[i], "accessType"))
                        accessType = attrib[i + 1];
                    else if (!strcmp(attrib[i],"value"))
                        value = attrib[i + 1];
                    else if (!strcmp(attrib[i],"appinfo"))
                        appinfo = attrib[i + 1];
                    else if (!strcmp(attrib[i],"documentation"))
                        documentation = attrib[i + 1];
                    else
                        swDebugf("unrecognized field attribute %s line %d -ignoring\n",
                                 attrib[i], lineno);
                }
                int itype = typeStringToEnum(type); //data type ie MFString, SFVec3f ...
                FieldValue *fv = SillyDefaultValue(itype);
                if (value) {
                    /*do string delimited values here in xml start(), and do contained SFNode/MFNode values in xml end() */
                    fv = parseFieldValue(itype, value);
                }

                bool isExposed = false;
                Element *element = NULL;
                if (!strcmp(accessType, "inputOnly")) {
                    element = new EventIn(itype, MyString(name));
                } else if (!strcmp(accessType, "outputOnly")) {
                    element = new EventOut(itype, MyString(name));
                } else if (!strcmp(accessType, "initializeOnly")) {
                    element = new Field(itype, MyString(name), fv);
                } else if (!strcmp(accessType, "inputOutput")) {
                    element = new ExposedField(itype, MyString(name), fv);
                    isExposed = true;
                }
                element->setAppinfo(MyString(appinfo));
                element->setDocumentation(MyString(documentation));

                int nullStatus = status.pop();
                bool isScript = status.peek() == STATUS_SCRIPT; 
                bool isProto = status.peek() == STATUS_PROTOINTERFACE;
                if (status.peek() == STATUS_PROTO_ELEMENT)
                    isProto = true; 
                if (status.peek() == STATUS_PROTO)
                    isProto = true; 
                                                
                status.push(nullStatus);
                if (isScript) {
                    NodeScript *current_script = (NodeScript *)current_node; 
                    if (current_script != NULL) {
                        if (isExposed) {
                            int iexposedField = addDynamicExposedElement(
                                                    current_script, element);
                            currentFieldIndex.push(iexposedField);
                        } else {
                            int ifield = addDynamicElement(current_script, 
                                                           element);
                            currentFieldIndex.push(ifield);
                        }
                        status.pop();
                        status.push(STATUS_SCRIPT_ELEMENT); 
                    }
                }
                if (isProto) {
                    if (current_proto != NULL) {
                        int ifield;
                        ifield = current_proto->addOrUpdateElement(element);
                        currentFieldIndex.push(ifield);
                        status.pop();
                        status.push(STATUS_PROTO_ELEMENT); 
                    }
                }
                // we need to look for SFNode/MFNode contained field values 
                // when we get to xml end()
            } else if (!strcmp(element, "Script") || 
                       !strcmp(element, "ComposedShader") || 
                       !strcmp(element, "ShaderProgram") || 
                       !strcmp(element, "PackagedShader")) {
                if (!strcmp(element, "Script"))
                    node = new NodeScript(scene);
                else
                    node = newNode(element);
                if (node) {
                    status.pop();
                    status.push(STATUS_SCRIPT);
                    XML_SetCdataSectionHandler(x3dParser[contextIndex], 
                                               startCDATA, endCDATA);
                    XML_SetCharacterDataHandler(x3dParser[contextIndex], 
                                                handleCDATA);
                    gotcdata = 0;
                }
            } else if (!strcmp(element, "ShaderPart")) {
                //seems like if you have a url, the url can be a CDATA child.
                //problem: ComposedShader can have a direct CDATA, 
                // and so can it's child ShaderPart have a CDATA
                // and I'm not assigning the retrieved ComposedShader CDATA to 
                // ComposedShader node
                // until I get to /ComposedShader.
                parentURL = currentURL;
                currentURL = NULL;
                node = newNode(element);
                if (node) {
                    status.pop();
                    status.push(STATUS_SHADERPART);
                } 
            } else if (!strcmp(element, "fieldValue")) {
                /* doesn't create a node, adds to it's parents fields */
                //<fieldValue name='initValuep' value='.15'/>
                const char *name = NULL;
                const char *value = NULL;
                for (int i = 0; attrib[i] != NULL; i += 2) { 
                    if (!strcmp(attrib[i], "name"))
                        name = attrib[i + 1];
                    else if (!strcmp(attrib[i],"value"))
                        value = attrib[i + 1];
                    else
                        swDebugf("unrecognized fieldValue attribute %s line %d -ignoring\n",
                                 attrib[i], lineno);
                }
                int ifield = -1;
                int itype = -1;

                if (current_node != NULL) {
                    Proto *proto = current_node->getProto();
                    int nf = proto->getNumFields();
                    Field *f = NULL;
                    for (int j = 0; j < nf; j++) {
                        f = proto->getField(j);
                        if (f->getName(x3d) == MyString(name)) {
                            ifield = j;
                            itype = f->getType();
                        }
                    }
                    if (ifield > -1) {
                        if (value) {
                            FieldValue *fv = SillyDefaultValue(itype);
                            // do string delimited values here in xml start(), 
                            // and do contained SFNode/MFNode values in xml end()
                            fv = parseFieldValue(itype, value);
                            current_node->setField(ifield, fv);
                        }
                        currentFieldIndex.push(ifield);
                        status.pop();
                        status.push(STATUS_PROTOINSTANCE_FIELDVALUE); 
                    }
                }
            } else if (!strcmp(element, "ProtoDeclare")) {
                const char *name = NULL;
                const char *appinfo = NULL;
                const char *documentation = NULL;

                for (int i = 0; attrib[i] != NULL; i += 2) { 
                    if (!strcmp(attrib[i], "name"))
                        name = attrib[i + 1];
                    else if (!strcmp(attrib[i], "appinfo"))
                        appinfo = attrib[i + 1];
                    else if (!strcmp(attrib[i], "documentation"))
                        documentation = attrib[i + 1];
                    else
                        swDebugf("unrecognized ProtoDeclare attribute %s line %d -ignoring\n",
                                 attrib[i], lineno);
                }
                Proto *proto = scene->getProto(MyString(name));
                if ((proto == NULL) || proto->isScriptedProto()) {
                   proto = new Proto(scene, MyString(name));
                   proto->setAppinfo(appinfo);
                   proto->setDocumentation(documentation);
                } else if (!proto->isExternProto())
                   proto->deleteElements(); 
                protoStack.push(proto); 
                current_proto = proto;
                level++;          
                status.push(STATUS_PROTO);
                if (!scene->addProtoName(MyString(name)))
                    scene->warning(IDS_PROTO_ALREADY_DEFINED);
                startProto();
            } else if (!strcmp(element, "ExternProtoDeclare")) {
                const char *name = NULL;
                const char *appinfo = NULL;
                const char *documentation = NULL;
                const char *url = NULL;

                for (int i = 0; attrib[i] != NULL; i += 2) { 
                    if (!strcmp(attrib[i], "name"))
                        name = attrib[i + 1];
                    else if (!strcmp(attrib[i], "appinfo"))
                        appinfo = attrib[i + 1];
                    else if (!strcmp(attrib[i], "documentation"))
                        documentation = attrib[i + 1];
                    else if (!strcmp(attrib[i], "url"))
                        url = attrib[i + 1];
                    else
                        swDebugf("unrecognized ExternProtoDeclare attribute %s line %d -ignoring\n",
                                 attrib[i], lineno);
                }
                Proto *proto = new Proto(scene, MyString(name));
                proto->setAppinfo(appinfo);
                proto->setDocumentation(documentation);
                if (url) {
                    int itype = typeStringToEnum("MFString"); 
                    FieldValue *fv = SillyDefaultValue(itype);
                    fv = parseFieldValue(itype, url);
                    proto->addURLs(fv);
                } else
                    scene->warning(IDS_EXTERN_PROTO_MISSING_URL_FIELD, name);

                protoStack.push(proto); 
                current_proto = proto;
                level++;          
                status.push(STATUS_PROTO);
                if (!scene->addProtoName(MyString(name))) {
                    swDebugf("warning: extern proto already definied: %s\n",name);
                } 
                startProto();
            } else if (!strcmp(element, "ProtoInterface")) {
                status.pop();
                status.push(STATUS_PROTOINTERFACE);
            } else if (!strcmp(element, "ProtoBody")) {
                status.pop();
                status.push(STATUS_PROTOBODY);
            } else if (!strcmp(element, "IS")) {
            } else if (!strcmp(element, "connect")) {
                const char *nodeField = NULL;
                const char *protoField = NULL;

                for (int i = 0; attrib[i] != NULL; i += 2) { 
                    if (!strcmp(attrib[i], "nodeField"))
                        nodeField = attrib[i + 1];
                    else if (!strcmp(attrib[i],"protoField"))
                        protoField = attrib[i + 1];
                    else
                        swDebugf("unrecognized IS connect attribute %s line %d -ignoring\n",
                                 attrib[i], lineno);
                }
                isField(current_node, MyString(nodeField), MyString(protoField));
            } else if (!strcmp(element, "ProtoInstance")) {
                // <ProtoInstance DEF='SliderPhotoTransparency' containerField='children' name='sliderProto'>
                const char *name = NULL;
                for (int i = 0; attrib[i] != NULL; i += 2) { 
                    if (!strcmp(attrib[i], "name"))
                        name = attrib[i + 1];
                }
                if (name == NULL) 
                    swDebugf("ProtoInstance has no type line %d\n",lineno);
                else {
                    Proto* proto = scene->getProto(MyString(name));
                    if (proto) {
                        protoInstance = true;
                        node = proto->create(scene);
                    }
                    else
                        swDebugf("ProtoInstance - failed to find proto of type %s line %d\n",name,lineno);
                }
                status.pop();
                status.push(STATUS_NODE);
            } else {
                node = newNode(element); // element would be a string like "Transform" 
                if (node) {
                    status.pop();
                    status.push(STATUS_NODE);
                } 
            }
            if (node != NULL) {
                nodeStack.pop();
                nodeStack.push(node);
                current_node = node;
                level++;
                node->appendTo(nodes.peek().nodeList);
                for (int i = 0; attrib[i] != NULL; i += 2) { 
                    if (strcmp("DEF", attrib[i]) == 0)   //"DEF" 
                        scene->def(uniqName(attrib[i + 1]), node);  // "myTrans"
                    else if (!strcmp("containerField", attrib[i])) {
                        parseContainerField(current_node, attrib, i);
                    } else if (!strcmp("name", attrib[i]) && protoInstance) {
                        //ProtoInstance has a name= field processed above, but checkField doesn't handle
                        continue;
                    } else {
                        int iField = checkField(current_node, attrib[i]);
                        if (iField != INVALID_INDEX) {
                            // x3d knows the node type and field type - 
                            // get types before attempting to parse tokens
                            iField = repairField(nodeStack.peek(), 
                                                 iField);
                            int curtype = current_node->getProto()->
                                          getField(iField)->getType();
                            FieldValue *fv;
                            fv = parseFieldValue(curtype, attrib[i + 1]);
                            setField(current_node, iField, fv);
                        }
                    }
                }
                nodes.peek().containerFields->append(
                    current_node->getContainerField());
                protoInstance = false;
            }
        }
    } else {
        if (!strcmp(element, "ROUTE")) {
            MyString srcNode, srcField, dstNode, dstField, tmp;
            for (int i = 0; attrib[i] != NULL; i += 2) { 
                tmp = "";
                tmp += MyString(attrib[i + 1]);
                if ( strcmp("fromNode", attrib[i]) == 0)    
                    srcNode = tmp;  
                else if (strcmp("fromField", attrib[i]) == 0)
                    srcField = tmp;
                else if (strcmp("toNode", attrib[i]) == 0)
                    dstNode = tmp;
                else if (strcmp("toField", attrib[i]) == 0)
                    dstField = tmp;
            }
            if (srcNode && srcField && dstNode && dstField) {
                status.pop();
                status.push(STATUS_ROUTE);
                level++;
                route(srcNode,srcField, dstNode, dstField);
            } else
                swDebugf("ouch - bad route fn=%s ff=%s tn=%s tf=%s lineno=%d\n",
                         srcNode.getData(), srcField.getData(),
                         dstNode.getData(), dstField.getData(), lineno);
        }
        if (!strcmp(element, "head")) {
            status.pop();
            status.push(STATUS_HEAD);
        }
        if (!strcmp(element, "meta")) {
            if (status.peek() == STATUS_HEAD) {
                const char *name = NULL;
                const char *content = NULL;
                for (int i = 0; attrib[i] != NULL; i += 2) { 
                    if (strcmp("name", attrib[i]) == 0)    
                        name = attrib[i + 1];  
                    if (strcmp("content", attrib[i]) == 0)    
                        content = attrib[i + 1];  
                }
                if (name != NULL && content != NULL)
                    scene->addMeta(strdup(name), strdup(content));
            }
        }
        if (!strcmp(element, "unit")) {
            const char *category = NULL;
            const char *name = NULL;
            double conversionFactor = 1;
            for (int i = 0; attrib[i] != NULL; i += 2) { 
                if (strcmp("category", attrib[i]) == 0)    
                    category = attrib[i + 1];  
                if (strcmp("name", attrib[i]) == 0)    
                    name = attrib[i + 1];  
                if (strcmp("conversionFactor", attrib[i]) == 0)    
                    conversionFactor = atof(attrib[i + 1]);  
            }
            if (name != NULL && category != NULL)
                scene->addUnit(strdup(category), strdup(name), 
                               conversionFactor);
        }
        if (!strcmp(element, "IMPORT") || !strcmp(element, "EXPORT")) {
            Node *node = NULL;
            if (!strcmp(element,"IMPORT")) {
                const char *InlineDEF = NULL;
                const char *importedDEF = NULL;
                const char *as = NULL;
                for (int i = 0; attrib[i] != NULL; i += 2) { 
                    if (strcmp("InlineDEF", attrib[i]) == 0)    
                        InlineDEF = attrib[i + 1];  
                    if (strcmp("importedDEF", attrib[i]) == 0)    
                        importedDEF = attrib[i + 1];  
                    if (strcmp("AS", attrib[i]) == 0)    
                        as = attrib[i + 1];  
                }
                if (InlineDEF != NULL && importedDEF != NULL)
                    node = addImport(checkName(InlineDEF),importedDEF, as);
                else
                    swDebugf("IMPORT missing InlineDEF=[%s] or importedDEF=[%s]\n",
                             InlineDEF,importedDEF);

            } else if (!strcmp(element,"EXPORT")){
                const char *localDEF = NULL;
                const char *as = NULL;
                for (int i = 0; attrib[i] != NULL; i += 2) { 
                    if (strcmp("localDEF", attrib[i]) == 0)    
                        localDEF = attrib[i + 1];  
                    if (strcmp("AS", attrib[i]) == 0)    
                        as = attrib[i + 1];  
                 }
                 if (localDEF != NULL)
                     node = addExport(checkName(localDEF), as);
                 else
                     swDebugf("EXPORT missing localDEF\n");

            }
            if (node) {
                status.pop();
                status.push(STATUS_IMPORTEXPORT);
                nodeStack.pop();
                nodeStack.push(node);
                current_node = node;
                level++;
                node->appendTo(nodes.peek().nodeList);
                nodes.peek().containerFields->append(
                    current_node->getContainerField());

            } 
        }
    }
    depth++;
    DataNode dataNode;
    dataNode.nodeList = new NodeList();
    dataNode.containerFields = new IntArray;
    nodes.push(dataNode);
}

static void XMLCALL end(void *data, const char *el) 
{
    NodeList *childList;  
    depth--;
    Array<int> *containerFields = nodes.peek().containerFields;
    childList = nodes.pop().nodeList;
    Node *current_node = nodeStack.pop();
    Proto *current_proto = protoStack.peek(); 
    int istatus = status.pop();

    if (istatus > 0) {
        if (istatus == STATUS_SCRIPT_ELEMENT || istatus == STATUS_PROTO_ELEMENT) {
            /* it was a script field */
            int nc = childList->size();
            int ifield = currentFieldIndex.pop();
            if (nc > 0 && (ifield != -1)) {
                // we have field > SFNode or MFNode value > contained nodes
                if (istatus == STATUS_SCRIPT_ELEMENT) {
                    NodeScript *parentScript = (NodeScript *)current_node;
                    FieldValue *fv = parentScript->getField(ifield);

                    if (fv->getType() == SFNODE) {
                        Node * cc = childList->get(0);
                        FieldValue *newValue = new SFNode(cc) ; 
                        parentScript->setField(ifield,newValue);
                    } else if (fv->getType() == MFNODE){
                        NodeList *list = new NodeList();
                        for (int i = 0; i < nc; i++)
                            list->append(childList->get(i));
                        FieldValue *newValue = new MFNode(list);
                        parentScript->setField(ifield, newValue);
                    }
                } else if (istatus == STATUS_PROTO_ELEMENT) {
                    Field *ff = current_proto->getField(ifield);
                    FieldValue *fv = ff->getDefault(x3d); 
                    if (fv->getType() == SFNODE) {
                        Node * cc = childList->get(0);
                        fv->addNode(cc);
                    } else if (fv->getType() == MFNODE){
                        for (int i = 0; i < nc; i++)
                            fv->addNode(childList->get(i));
                    }
                }
            }
        } else if (istatus == STATUS_PROTOINSTANCE_FIELDVALUE) {
            /* it was a protoinstance fieldvalue */
            int nc = childList->size();
            int ifield = currentFieldIndex.pop();
            if (nc > 0) {
                Node *node = current_node;
                FieldValue *fv = node->getField(ifield);
                if (fv->getType() == SFNODE) {
                    Node * cc = childList->get(0);
                    FieldValue *newValue = new SFNode(cc) ; 
                    node->setField(ifield,newValue);
                } else if (fv->getType() == MFNODE){
                    NodeList *list = new NodeList();
                    for (int i = 0; i < nc; i++)
                        list->append(childList->get(i));
                    FieldValue *newValue = new MFNode(list);
                    node->setField(ifield, newValue);
                }
            }
        } else if (istatus == STATUS_PROTOBODY) {
            int nc = childList->size();
            if (nc > 0) {
                //Q. where do we put the proto body nodes?
                if (current_proto) {
                    Node* first = childList->get(0);
                    childList->remove(0);
                    current_proto->define(first, childList); // define(primary,list)
                    current_proto->setLoaded(true);
                }
            }
        } else if (istatus == STATUS_PROTO) {
            Proto *currentProto = protoStack.peek();
            Proto *newProto = protoStack.pop();
            if (currentProto != NULL) {
                scene->addProto(currentProto->getName(x3d), newProto); 
            }
            stopProto();
            level--;
        } else if (istatus == STATUS_SHADERPART) {
            level--;
            if (gotcdata) {
                MFString *mfs = new MFString(currentURL);
                currentURL = NULL; //ownership transferred to MFString
                int ic = checkField(current_node, "url");
                current_node->setField(ic, mfs);
            }
            currentURL = parentURL;
            if (currentURL)
               gotcdata = strlen(currentURL);
            else
               gotcdata = 0;
        } else if (istatus == STATUS_NODE || 
                   istatus == STATUS_SCRIPT || 
                   istatus == STATUS_USE || 
                   istatus == STATUS_ROUTE || 
                   istatus == STATUS_IMPORTEXPORT) {
            level--;
            if (istatus == STATUS_SCRIPT) {
                /* any script url CDATA section ? */
                XML_SetCdataSectionHandler(x3dParser[contextIndex], NULL, NULL);
                XML_SetDefaultHandler(x3dParser[contextIndex], NULL);
                if (gotcdata) {
                    MFString * mfs = new MFString(MyString(currentURL));
                    currentURL = NULL; //ownership transferred to MFString
                    int ic = checkField(current_node, "url");
                    current_node->setField(ic, mfs);
                }
            }
            int nc = childList->size();
            if (nc > 0) {
                if (istatus == STATUS_NODE || istatus == STATUS_SCRIPT) {
                    /* no chidren field, so we'll hunt down a good field to put the node in */
                    int nc = childList->size();
                    for (int i = 0; i < nc; i++) {
                        Node* child = childList->get(i);
                        int ic = -1;
                        if (i < containerFields->size())
                            ic = (*containerFields)[i];
                        // it has children so we'll put the node in the children field
                        if (ic < 0)
                            ic = current_node->getChildrenField();
                        int childType = child->getType();
                        if (ic < 0)
                            ic = current_node->findFirstValidFieldType(childType);
                        if (ic < 0) {
                            int childClass = child->getNodeClass();
                            ic = current_node->findFirstValidFieldType(childClass);
                        }
                        if (ic > -1) {
                            ic = current_node->repairField(ic);
                            Field *field = current_node->getProto()->getField(ic);
                            int fieldType = field->getType();
                            if (fieldType == SFNODE)
                                current_node->setField(ic, new SFNode(child),
                                                       ic);
                            else if (fieldType == MFNODE) {
                                NodeList *nl = new NodeList(childList->get(i));
                                current_node->addFieldNodeList(ic, nl, ic);
                            }
                        } else {
                            if (childType != VRML_COMMENT)
                                swDebugf("%s %s %d for %s\n",
                                         "internal parse error:",
                                         "no field index ", ic,
                                         (const char *)
                                         current_node->getProto()->getName(
                                             true));
                        }
                    }
                } else if (istatus == STATUS_USE) {
                    swDebugf("USE should not have children - ignoring, lineno=%d\n",
                             lineno);
                } else if (istatus == STATUS_ROUTE) {
                    swDebugf("ROUTE should not have children - ignoring, lineno=%d\n",
                             lineno);
                } else if (istatus == STATUS_IMPORTEXPORT) {
                    swDebugf("Inline, IMPORT and EXPORT should not have children - ignoring, lineno=%d\n",
                             lineno);
                }
            } 
            if (level == 0) {
                //level tracks real nodes not nodeless xml elements such as <X3D> and <Scene> (use depth for all elements)
                //so when we get to level 0, it means there's no more real nodes above us in the nodeListX3d -or scenegraph-
                // and we should take this real node and shove it .. into rootNodes.
                rootNodes->append(current_node);
            }
        } 
    } 
}

const char *parseX3d()
{
    scene->setX3dXml();
    rootNodes = new NodeList();
    done = false;     
    contextIndex++;
    x3dParser[contextIndex] = XML_ParserCreate(NULL);

    XML_SetElementHandler(x3dParser[contextIndex], start, end);
    // expat should support something like XML_SkipQuotPredefinedEntity(true);
    //XML_SetCdataSectionHandler(x3dParser[contextIndex], startCDATA, endCDATA);
    //XML_SetDefaultHandler(x3dParser[contextIndex], handleCDATA);
    //XML_SetUserData(x3dParser[contextIndex], &iparent); // iparent
    XML_SetCommentHandler(x3dParser[contextIndex], handleComment);

    int max_size = 1024; 
    char *buf = (char *) malloc(max_size);
    int count = 0;
    const char *retval;
    bool xmlError = false;
    depth = 0;
    level = 0;
    while (!nodes.empty())
        nodes.pop();
    while (!nodeStack.empty())
        nodeStack.pop();
    while (!nodeStack.empty())
        nodeStack.pop();
    while (!protoStack.empty())
        protoStack.pop();
    do {
#ifdef HAVE_LIBZ
        count = gzread(inputFile, buf, max_size);
#else
        char *more = fgets(buf, max_size, inputFile);
        count = (more == NULL) ? 0 : strlen(buf);
#endif
        if (count > 0) {
            XML_Parser parser = x3dParser[contextIndex];
            if (XML_Parse(parser, buf, count, FALSE) == XML_STATUS_ERROR) {
               swDebugf("Xml error: %s at line %u\n",
                        XML_ErrorString(XML_GetErrorCode(parser)),
                        XML_GetCurrentLineNumber(parser));
                retval = "ouch - couldn't finish x3d parsing";
                xmlError = true;
                break;
            }
            lineno++;
        }
    } while (count > 0);
    bool saveXmlError = xmlError;
    if (x3dParser[contextIndex] != NULL) {
#ifdef HAVE_XML_STOPPARSER
        XML_StopParser(x3dParser[contextIndex], XML_FALSE);
#endif
        XML_ParserFree(x3dParser[contextIndex]);
    }
    x3dParser[contextIndex] = NULL;
    contextIndex--;
    free(buf);
    if (currentURL != NULL) {
        free(currentURL);
        currentURL = NULL;
    }
    if (!saveXmlError) {
        retval = NULL;
        // recursive: addNodes indirectly calls 
        // Scene.cpp L.497 scanForExternProtos() and scanForInlines(nodes)
        // which call parseX3D
        scene->addNodes(targetNode, targetField, rootNodes, scanFor);
    }
    return retval;
}
#endif

