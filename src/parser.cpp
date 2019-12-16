/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.3.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 24 "parser.y" /* yacc.c:337  */

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
#include "NodeVrmlCut.h"
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
#include "NodeVrmlCut.h"

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

static MyStack<Node *> nodeStack;
static MyStack<Proto *> protoStack;
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
static void test(Node *node);
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

MyArray<nameTranslation*> NameTranslation;

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

#line 216 "y.tab.c" /* yacc.c:337  */
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
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
/* Tokens.  */
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
#line 170 "parser.y" /* yacc.c:352  */

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

#line 353 "y.tab.c" /* yacc.c:352  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */



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
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
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
#  define YYSIZE_T unsigned
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

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
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
#define YYLAST   373

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  61
/* YYNRULES -- Number of rules.  */
#define YYNRULES  135
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  226

#define YYUNDEFTOK  2
#define YYMAXUTOK   295

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    41,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   216,   216,   222,   228,   229,   232,   237,   240,   241,
     242,   243,   244,   247,   253,   253,   259,   262,   263,   266,
     267,   270,   270,   291,   296,   299,   306,   309,   309,   313,
     313,   317,   318,   319,   322,   328,   329,   329,   334,   333,
     359,   366,   369,   370,   371,   375,   379,   385,   392,   394,
     398,   400,   404,   406,   409,   409,   427,   427,   445,   446,
     449,   450,   453,   459,   460,   461,   465,   466,   469,   470,
     471,   478,   486,   493,   502,   506,   509,   510,   514,   519,
     522,   525,   529,   533,   537,   540,   542,   548,   549,   557,
     558,   559,   560,   561,   562,   563,   567,   571,   578,   579,
     582,   583,   586,   587,   588,   591,   594,   595,   599,   601,
     604,   610,   616,   617,   626,   627,   628,   629,   630,   631,
     633,   634,   635,   636,   639,   642,   645,   646,   649,   651,
     652,   655,   658,   659,   662,   665
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "BRACKET_ON", "BRACKET_OFF",
  "WING_BRACKET_ON", "WING_BRACKET_OFF", "SCRIPT", "COMPOSED_SHADER",
  "PACKAGED_SHADER", "SHADER_PROGRAM", "VRML_CUT", "EXPORT", "IMPORT",
  "AS", "ID", "ID_X3D", "STRING", "INT_NUM", "FLOAT_NUM", "DEF",
  "EXTERNPROTO", "FALSE_TOK", "IS", "NULL_TOK", "PROTO", "ROUTE", "TO",
  "TRUE_TOK", "USE", "EVENT_IN", "EVENT_OUT", "EXPOSED_FIELD",
  "INPUT_OUTPUT", "FIELD", "X3D", "PROFILE", "COMPONENT", "UNIT", "META",
  "VRML1", "'.'", "$accept", "vrmlScene", "statements", "statement",
  "nodeStatement", "$@1", "protoStatement", "protoStatements", "proto",
  "$@2", "protoBody", "interfaceDeclarations", "fieldDeclaration", "$@3",
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
     295,    46
};
# endif

#define YYPACT_NINF -148

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-148)))

#define YYTABLE_NINF -54

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     195,  -148,  -148,  -148,    11,   321,  -148,  -148,   -18,  -148,
    -148,  -148,  -148,  -148,  -148,  -148,  -148,     6,     6,  -148,
    -148,     6,     6,     6,     6,     6,  -148,  -148,  -148,  -148,
    -148,  -148,  -148,  -148,  -148,    31,  -148,    46,  -148,   321,
       6,    65,    45,  -148,    43,  -148,  -148,  -148,    50,  -148,
    -148,  -148,  -148,  -148,    32,     6,    58,     0,    15,    55,
      44,    44,    55,     6,     6,   218,   102,   109,     6,  -148,
    -148,  -148,  -148,     6,   120,  -148,    44,  -148,  -148,  -148,
    -148,  -148,  -148,   125,  -148,  -148,  -148,   114,  -148,   152,
    -148,    29,    59,  -148,  -148,     6,    33,  -148,    38,  -148,
       6,  -148,     6,     6,     6,     6,  -148,   229,   252,  -148,
    -148,  -148,  -148,   275,   119,  -148,  -148,  -148,  -148,  -148,
       2,     6,     6,     6,     6,     6,  -148,   148,     6,     6,
       6,   275,   275,  -148,  -148,   115,     6,  -148,     6,  -148,
    -148,   298,  -148,  -148,  -148,  -148,     6,  -148,  -148,  -148,
    -148,    51,  -148,    98,   100,     6,  -148,  -148,     6,   142,
    -148,  -148,  -148,     6,     6,     6,     6,     6,  -148,     6,
       6,  -148,     6,   149,   151,     6,     6,  -148,  -148,  -148,
       4,    42,    82,    24,   118,  -148,  -148,  -148,  -148,  -148,
    -148,  -148,  -148,    26,  -148,  -148,  -148,  -148,  -148,   344,
     169,   170,  -148,  -148,     6,  -148,     6,     6,  -148,  -148,
    -148,  -148,  -148,  -148,  -148,  -148,  -148,  -148,  -148,  -148,
    -148,   275,  -148,  -148,   321,  -148
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    53,   124,    53,     0,     2,     7,    53,   114,     5,
       4,     1,    60,    63,    65,    64,    62,     0,     0,   113,
     112,     0,     0,     0,     0,     0,     6,     8,     9,    17,
      18,    10,    12,    11,    13,     0,    61,     0,    80,     3,
       0,   115,    48,    79,     0,    14,    38,    21,     0,    16,
      56,    54,   125,    82,     0,     0,     0,   116,   126,   123,
     129,   120,   132,     0,     0,     0,     0,     0,     0,    53,
      53,    83,   128,     0,     0,   117,   118,   127,   122,   130,
     121,   133,    49,    50,    15,    53,    53,     0,    67,     0,
      59,     0,     0,   131,   119,     0,     0,    41,     0,    26,
       0,    57,     0,     0,     0,     0,    77,     0,     0,    69,
      76,    66,    68,     0,    78,    55,    58,   135,   134,    51,
       0,     0,     0,     0,     0,     0,    40,     0,     0,     0,
       0,     0,     0,    35,    25,     0,     0,    81,     0,    29,
      27,     0,    84,   101,   105,    99,     0,    87,    98,    86,
      33,    91,    97,    89,    90,     0,    34,    74,     0,     0,
     106,    39,    52,     0,     0,     0,     0,     0,    53,     0,
       0,    36,     0,    31,    32,     0,     0,    95,   108,   110,
       0,     0,     0,     0,     0,    72,    96,   100,   104,   103,
     102,    73,    75,     0,    42,    43,    45,    46,    44,     0,
       0,    20,    31,    32,     0,    47,     0,     0,    30,    28,
      94,    92,    93,    85,   109,    88,   111,   107,    53,    19,
      22,     0,    70,    71,    23,    37
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -148,  -148,    -6,  -148,    -5,  -148,   -75,  -148,  -148,  -148,
    -148,  -148,    83,  -148,    86,  -148,    89,  -148,  -148,  -148,
    -148,  -148,  -148,   -62,  -148,  -148,  -148,    12,   123,  -148,
    -148,  -148,  -148,  -148,  -148,  -148,    99,  -148,    -1,    34,
      41,  -148,  -148,  -101,    52,  -147,    56,    57,  -148,    40,
    -148,   -15,  -148,  -148,  -148,   143,  -148,    35,  -148,    73,
    -148
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,     5,    26,   149,    65,    28,   199,    29,    67,
     200,    98,   107,   176,   108,   175,   109,   134,   204,    30,
      66,    96,   126,    31,    32,    33,   161,     6,    34,    70,
      69,    91,    35,    36,    89,   111,   112,   113,    42,    37,
     136,    52,    72,   150,   151,   152,   153,   154,   162,   183,
     184,    38,     7,     8,    41,    57,    58,    59,    60,    61,
      62
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      27,    39,    43,    43,   186,   159,    43,   156,   210,    43,
      43,    11,   157,     9,   106,    10,   106,    44,    40,   160,
      45,    19,    20,    48,    49,    53,   145,   110,   213,   110,
     217,   156,   148,   186,    27,   115,    50,   120,    55,    56,
      73,   214,   127,   214,    19,    20,   211,    71,    43,    83,
      22,    51,    54,    87,    23,    24,    46,    47,    92,    63,
     187,   188,    82,   121,   122,   123,   124,   125,   128,   129,
     130,   104,   105,   145,   114,    74,   114,   117,   118,   148,
      43,    88,    90,    56,    64,    43,   212,   137,   137,   137,
     137,    68,    75,    55,   119,    79,    80,    97,    99,   135,
     189,   190,    54,    55,    56,    85,   137,   137,   137,   137,
     137,    94,    86,   137,   137,   137,   187,   188,   189,   190,
     225,   173,   215,   174,   219,    12,    13,    14,    15,    16,
      76,   185,    78,    19,    20,    81,   179,    93,    21,    95,
     191,   100,   158,   192,   138,   139,   140,    25,   194,   195,
     196,   197,   198,   168,   202,   203,   172,   205,   101,   178,
     208,   209,   163,   164,   165,   166,   167,    19,    20,   169,
     170,   171,   206,    22,   207,   220,   -24,    23,    24,   216,
     201,   131,   102,   103,   132,   104,   105,   133,    84,   221,
     116,   222,   223,   180,   218,   -53,     1,   181,   182,   193,
       0,    77,   -53,   -53,   -53,   -53,   -53,   -53,   -53,     0,
     -53,   -53,   224,     0,     0,   -53,   -53,     0,     0,    27,
     -53,   -53,     0,     0,   -53,    12,    13,    14,    15,    16,
       2,     0,   141,    19,    20,     3,    12,    13,    14,    15,
      16,     0,     0,     0,    19,    20,   142,   143,   144,    21,
       0,   145,   146,   147,     0,   141,     0,   148,    25,    12,
      13,    14,    15,    16,     0,     0,     0,    19,    20,   142,
     143,   144,    21,     0,   145,   155,   147,     0,   141,     0,
     148,    25,    12,    13,    14,    15,    16,     0,     0,     0,
      19,    20,   142,   143,   144,    21,     0,   145,     0,   147,
       0,     0,   177,   148,    25,    12,    13,    14,    15,    16,
       0,     0,     0,    19,    20,   178,   143,   144,    21,     0,
     145,     0,     0,     0,     0,     0,   148,    25,    12,    13,
      14,    15,    16,    17,    18,     0,    19,    20,     0,     0,
       0,    21,    22,     0,     0,     0,    23,    24,     0,     0,
      25,    12,    13,    14,    15,    16,     0,     0,     0,    19,
      20,     0,     0,     0,    21,    22,     0,     0,     0,    23,
       0,     0,     0,    25
};

static const yytype_int16 yycheck[] =
{
       5,     7,    17,    18,   151,     3,    21,   108,     4,    24,
      25,     0,   113,     1,    89,     3,    91,    18,    36,    17,
      21,    15,    16,    24,    25,    40,    22,    89,     4,    91,
       4,   132,    28,   180,    39,     6,     5,     4,    38,    39,
      55,    17,     4,    17,    15,    16,     4,    15,    63,    64,
      21,     5,    37,    68,    25,    26,    22,    23,    73,    14,
      18,    19,    63,    30,    31,    32,    33,    34,    30,    31,
      32,    33,    34,    22,    89,    17,    91,    18,    19,    28,
      95,    69,    70,    39,    41,   100,     4,   102,   103,   104,
     105,    41,    57,    38,    95,    60,    61,    85,    86,   100,
      18,    19,    37,    38,    39,     3,   121,   122,   123,   124,
     125,    76,     3,   128,   129,   130,    18,    19,    18,    19,
     221,   136,     4,   138,   199,     7,     8,     9,    10,    11,
      57,   146,    59,    15,    16,    62,   141,    17,    20,    14,
     155,    27,    23,   158,   103,   104,   105,    29,   163,   164,
     165,   166,   167,     5,   169,   170,    41,   172,     6,    17,
     175,   176,   121,   122,   123,   124,   125,    15,    16,   128,
     129,   130,    23,    21,    23,     6,     6,    25,    26,   184,
     168,    98,    30,    31,    98,    33,    34,    98,    65,   204,
      91,   206,   207,   141,   199,     0,     1,   141,   141,   159,
      -1,    58,     7,     8,     9,    10,    11,    12,    13,    -1,
      15,    16,   218,    -1,    -1,    20,    21,    -1,    -1,   224,
      25,    26,    -1,    -1,    29,     7,     8,     9,    10,    11,
      35,    -1,     3,    15,    16,    40,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    16,    17,    18,    19,    20,
      -1,    22,    23,    24,    -1,     3,    -1,    28,    29,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    16,    17,
      18,    19,    20,    -1,    22,    23,    24,    -1,     3,    -1,
      28,    29,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    16,    17,    18,    19,    20,    -1,    22,    -1,    24,
      -1,    -1,     4,    28,    29,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    16,    17,    18,    19,    20,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    28,    29,     7,     8,
       9,    10,    11,    12,    13,    -1,    15,    16,    -1,    -1,
      -1,    20,    21,    -1,    -1,    -1,    25,    26,    -1,    -1,
      29,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      16,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    29
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,    35,    40,    43,    44,    69,    94,    95,    69,
      69,     0,     7,     8,     9,    10,    11,    12,    13,    15,
      16,    20,    21,    25,    26,    29,    45,    46,    48,    50,
      61,    65,    66,    67,    70,    74,    75,    81,    93,    44,
      36,    96,    80,    93,    80,    80,    81,    81,    80,    80,
       5,     5,    83,    93,    37,    38,    39,    97,    98,    99,
     100,   101,   102,    14,    41,    47,    62,    51,    41,    72,
      71,    15,    84,    93,    17,    99,   101,    97,   101,    99,
      99,   101,    80,    93,    70,     3,     3,    93,    69,    76,
      69,    73,    93,    17,    99,    14,    63,    69,    53,    69,
      27,     6,    30,    31,    33,    34,    48,    54,    56,    58,
      65,    77,    78,    79,    93,     6,    78,    18,    19,    80,
       4,    30,    31,    32,    33,    34,    64,     4,    30,    31,
      32,    54,    56,    58,    59,    80,    82,    93,    82,    82,
      82,     3,    17,    18,    19,    22,    23,    24,    28,    46,
      85,    86,    87,    88,    89,    23,    85,    85,    23,     3,
      17,    68,    90,    82,    82,    82,    82,    82,     5,    82,
      82,    82,    41,    93,    93,    57,    55,     4,    17,    46,
      86,    88,    89,    91,    92,    93,    87,    18,    19,    18,
      19,    93,    93,    91,    93,    93,    93,    93,    93,    49,
      52,    69,    93,    93,    60,    93,    23,    23,    93,    93,
       4,     4,     4,     4,    17,     4,    46,     4,    46,    48,
       6,    93,    93,    93,    44,    85
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    42,    43,    43,    43,    43,    44,    44,    45,    45,
      45,    45,    45,    46,    47,    46,    46,    48,    48,    49,
      49,    51,    50,    52,    52,    53,    53,    55,    54,    57,
      56,    58,    58,    58,    58,    59,    60,    59,    62,    61,
      63,    63,    64,    64,    64,    64,    64,    65,    66,    66,
      67,    67,    68,    69,    71,    70,    72,    70,    73,    73,
      74,    74,    74,    75,    75,    75,    76,    76,    77,    77,
      77,    77,    77,    77,    78,    78,    78,    78,    79,    80,
      81,    82,    83,    84,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    86,    86,    87,    87,
      88,    88,    89,    89,    89,    89,    90,    90,    91,    91,
      92,    92,    93,    93,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    95,    96,    97,    97,    98,    99,
      99,   100,   101,   101,   102,   102
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
       1,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       5,     5,     3,     3,     2,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     3,     1,
       1,     1,     3,     3,     3,     2,     2,     1,     1,     1,
       2,     1,     2,     2,     2,     1,     1,     3,     1,     2,
       1,     2,     1,     1,     1,     2,     3,     4,     4,     5,
       3,     4,     4,     3,     1,     2,     1,     2,     2,     1,
       2,     3,     1,     2,     4,     4
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
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
  unsigned long yylno = yyrline[yyrule];
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
                       &yyvsp[(yyi + 1) - (yynrhs)]
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
            else
              goto append;

          append:
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

  return (YYSIZE_T) (yystpcpy (yyres, yystr) - yyres);
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
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
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
    default: /* Avoid compiler warnings. */
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
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
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
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yynewstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  *yyssp = (yytype_int16) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1);

# if defined yyoverflow
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
# else /* defined YYSTACK_RELOCATE */
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
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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
| yyreduce -- do a reduction.  |
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
#line 216 "parser.y" /* yacc.c:1652  */
    { 
                                  nodeComment();
                                  addCommentsToNodeList((yyvsp[0].nodeList));
                                  scene->addNodes(targetNode, targetField, (yyvsp[0].nodeList),
                                                  scanFor); 
                                 }
#line 1657 "y.tab.c" /* yacc.c:1652  */
    break;

  case 3:
#line 222 "parser.y" /* yacc.c:1652  */
    { 
                                  nodeComment();
                                  addCommentsToNodeList((yyvsp[0].nodeList));
                                  scene->addNodes(targetNode, targetField, (yyvsp[0].nodeList),
                                                  scanFor); 
                                }
#line 1668 "y.tab.c" /* yacc.c:1652  */
    break;

  case 6:
#line 232 "parser.y" /* yacc.c:1652  */
    {
                                  if ((yyvsp[0].node) != NULL) 
                                      (yyvsp[0].node)->appendTo((yyvsp[-1].nodeList)); 
                                  (yyval.nodeList) = (yyvsp[-1].nodeList); 
                                }
#line 1678 "y.tab.c" /* yacc.c:1652  */
    break;

  case 7:
#line 237 "parser.y" /* yacc.c:1652  */
    { (yyval.nodeList) = new NodeList(); }
#line 1684 "y.tab.c" /* yacc.c:1652  */
    break;

  case 8:
#line 240 "parser.y" /* yacc.c:1652  */
    { (yyval.node) = (yyvsp[0].node); }
#line 1690 "y.tab.c" /* yacc.c:1652  */
    break;

  case 9:
#line 241 "parser.y" /* yacc.c:1652  */
    { (yyval.node) = NULL; }
#line 1696 "y.tab.c" /* yacc.c:1652  */
    break;

  case 10:
#line 242 "parser.y" /* yacc.c:1652  */
    { (yyval.node) = NULL; }
#line 1702 "y.tab.c" /* yacc.c:1652  */
    break;

  case 11:
#line 243 "parser.y" /* yacc.c:1652  */
    { (yyval.node) = (yyvsp[0].node); }
#line 1708 "y.tab.c" /* yacc.c:1652  */
    break;

  case 12:
#line 244 "parser.y" /* yacc.c:1652  */
    { (yyval.node) = (yyvsp[0].node); }
#line 1714 "y.tab.c" /* yacc.c:1652  */
    break;

  case 13:
#line 247 "parser.y" /* yacc.c:1652  */
    {
                         if ((yyvsp[0].node)) 
                             (yyval.node) = (yyvsp[0].node);
                         else
                             (yyval.node) = NULL;     
                       }
#line 1725 "y.tab.c" /* yacc.c:1652  */
    break;

  case 14:
#line 253 "parser.y" /* yacc.c:1652  */
    { defName = (yyvsp[0].id); }
#line 1731 "y.tab.c" /* yacc.c:1652  */
    break;

  case 15:
#line 253 "parser.y" /* yacc.c:1652  */
    { 
                                                if ((yyvsp[0].node))
                                                    (yyval.node) = (yyvsp[0].node); 
                                                else
                                                    (yyval.node) = NULL;
                                              }
#line 1742 "y.tab.c" /* yacc.c:1652  */
    break;

  case 16:
#line 259 "parser.y" /* yacc.c:1652  */
    { (yyval.node) = scene->use(checkName(SYMB((yyvsp[0].id)))); }
#line 1748 "y.tab.c" /* yacc.c:1652  */
    break;

  case 21:
#line 270 "parser.y" /* yacc.c:1652  */
    { 
                                   Proto *proto = scene->getProto(SYMB((yyvsp[0].id)));
                                   if ((proto == NULL) || 
                                       proto->isScriptedProto()) {
                                       proto = new Proto(scene, SYMB((yyvsp[0].id)));
                                   }
                                   protoStack.push(proto);
                                   if (!scene->addProtoName(SYMB((yyvsp[0].id))))
                                      scene->warning(IDS_PROTO_ALREADY_DEFINED,
                                                     (const char*)SYMB((yyvsp[0].id)));
                                   if (TheApp->isImportingProtoLibrary())
                                       proto->setFromProtoLibrary();
                                 }
#line 1766 "y.tab.c" /* yacc.c:1652  */
    break;

  case 22:
#line 285 "parser.y" /* yacc.c:1652  */
    { 
                                   scene->addProto(SYMB((yyvsp[-7].id)), protoStack.pop());
                                   stopProto();
                                 }
#line 1775 "y.tab.c" /* yacc.c:1652  */
    break;

  case 23:
#line 292 "parser.y" /* yacc.c:1652  */
    { 
                                 protoStack.peek()->define((yyvsp[-1].node), (yyvsp[0].nodeList)); 
                                 protoStack.peek()->setLoaded(true);
                                 }
#line 1784 "y.tab.c" /* yacc.c:1652  */
    break;

  case 25:
#line 300 "parser.y" /* yacc.c:1652  */
    {
            if (protoStack.empty()) 
                yyerror("syntax error");
            else 
                protoStack.peek()->addOrUpdateElement((yyvsp[0].element));
          }
#line 1795 "y.tab.c" /* yacc.c:1652  */
    break;

  case 27:
#line 309 "parser.y" /* yacc.c:1652  */
    { currentType = (yyvsp[0].int32); }
#line 1801 "y.tab.c" /* yacc.c:1652  */
    break;

  case 28:
#line 310 "parser.y" /* yacc.c:1652  */
    { (yyval.element) = createField((yyvsp[-2].int32), SYMB((yyvsp[0].id))); }
#line 1807 "y.tab.c" /* yacc.c:1652  */
    break;

  case 29:
#line 313 "parser.y" /* yacc.c:1652  */
    { currentType = (yyvsp[0].int32); }
#line 1813 "y.tab.c" /* yacc.c:1652  */
    break;

  case 30:
#line 314 "parser.y" /* yacc.c:1652  */
    { (yyval.element) = createField((yyvsp[-2].int32), SYMB((yyvsp[0].id))); }
#line 1819 "y.tab.c" /* yacc.c:1652  */
    break;

  case 31:
#line 317 "parser.y" /* yacc.c:1652  */
    { (yyval.element) = new EventIn((yyvsp[-1].int32), SYMB((yyvsp[0].id))); }
#line 1825 "y.tab.c" /* yacc.c:1652  */
    break;

  case 32:
#line 318 "parser.y" /* yacc.c:1652  */
    { (yyval.element) = new EventOut((yyvsp[-1].int32), SYMB((yyvsp[0].id))); }
#line 1831 "y.tab.c" /* yacc.c:1652  */
    break;

  case 33:
#line 319 "parser.y" /* yacc.c:1652  */
    { (yyval.element) = new Field((yyvsp[-1].element)->getType(), 
                                                         (yyvsp[-1].element)->getName(x3d), (yyvsp[0].value)); 
                                        }
#line 1839 "y.tab.c" /* yacc.c:1652  */
    break;

  case 34:
#line 323 "parser.y" /* yacc.c:1652  */
    { (yyval.element) = new ExposedField((yyvsp[-1].element)->getType(), 
                                                     (yyvsp[-1].element)->getName(x3d), (yyvsp[0].value)); 
                                        }
#line 1847 "y.tab.c" /* yacc.c:1652  */
    break;

  case 36:
#line 329 "parser.y" /* yacc.c:1652  */
    { currentType = (yyvsp[0].int32); }
#line 1853 "y.tab.c" /* yacc.c:1652  */
    break;

  case 37:
#line 330 "parser.y" /* yacc.c:1652  */
    { (yyval.element) = new ExposedField((yyvsp[-3].int32), SYMB((yyvsp[-1].id)), (yyvsp[0].value)); }
#line 1859 "y.tab.c" /* yacc.c:1652  */
    break;

  case 38:
#line 334 "parser.y" /* yacc.c:1652  */
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
#line 1876 "y.tab.c" /* yacc.c:1652  */
    break;

  case 39:
#line 347 "parser.y" /* yacc.c:1652  */
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
#line 1891 "y.tab.c" /* yacc.c:1652  */
    break;

  case 40:
#line 360 "parser.y" /* yacc.c:1652  */
    {
            if (protoStack.empty()) 
                yyerror("syntax error");
            else 
                protoStack.peek()->addElement((yyvsp[0].element));
          }
#line 1902 "y.tab.c" /* yacc.c:1652  */
    break;

  case 42:
#line 369 "parser.y" /* yacc.c:1652  */
    { (yyval.element) = new EventIn((yyvsp[-1].int32), SYMB((yyvsp[0].id))); }
#line 1908 "y.tab.c" /* yacc.c:1652  */
    break;

  case 43:
#line 370 "parser.y" /* yacc.c:1652  */
    { (yyval.element) = new EventOut((yyvsp[-1].int32), SYMB((yyvsp[0].id))); }
#line 1914 "y.tab.c" /* yacc.c:1652  */
    break;

  case 44:
#line 371 "parser.y" /* yacc.c:1652  */
    { 
                                          (yyval.element) = new Field((yyvsp[-1].int32), SYMB((yyvsp[0].id)),
                                                         SillyDefaultValue((yyvsp[-1].int32)));
                                        }
#line 1923 "y.tab.c" /* yacc.c:1652  */
    break;

  case 45:
#line 375 "parser.y" /* yacc.c:1652  */
    {
                                          (yyval.element) = new ExposedField((yyvsp[-1].int32), SYMB((yyvsp[0].id)),
                                                SillyDefaultValue((yyvsp[-1].int32)));
                                        }
#line 1932 "y.tab.c" /* yacc.c:1652  */
    break;

  case 46:
#line 379 "parser.y" /* yacc.c:1652  */
    {
                                          (yyval.element) = new ExposedField((yyvsp[-1].int32), SYMB((yyvsp[0].id)),
                                                SillyDefaultValue((yyvsp[-1].int32)));
                                        }
#line 1941 "y.tab.c" /* yacc.c:1652  */
    break;

  case 47:
#line 386 "parser.y" /* yacc.c:1652  */
    { 
               route(checkName(SYMB((yyvsp[-6].id))), SYMB((yyvsp[-4].id)), 
                     checkName(SYMB((yyvsp[-2].id))), SYMB((yyvsp[0].id))); 
             }
#line 1950 "y.tab.c" /* yacc.c:1652  */
    break;

  case 48:
#line 393 "parser.y" /* yacc.c:1652  */
    { (yyval.node) = addExport(checkName(SYMB((yyvsp[0].id))), ""); }
#line 1956 "y.tab.c" /* yacc.c:1652  */
    break;

  case 49:
#line 395 "parser.y" /* yacc.c:1652  */
    { (yyval.node) = addExport(checkName(SYMB((yyvsp[-2].id))), SYMB((yyvsp[0].id))); }
#line 1962 "y.tab.c" /* yacc.c:1652  */
    break;

  case 50:
#line 399 "parser.y" /* yacc.c:1652  */
    { (yyval.node) = addImport(checkName(SYMB((yyvsp[-2].id))), SYMB((yyvsp[0].id)), ""); }
#line 1968 "y.tab.c" /* yacc.c:1652  */
    break;

  case 51:
#line 401 "parser.y" /* yacc.c:1652  */
    { (yyval.node) = addImport(checkName(SYMB((yyvsp[-4].id))), SYMB((yyvsp[-2].id)), SYMB((yyvsp[0].id))); }
#line 1974 "y.tab.c" /* yacc.c:1652  */
    break;

  case 54:
#line 409 "parser.y" /* yacc.c:1652  */
    { 
                                        MyString newName = SYMB((yyvsp[-1].id));
                                        if (newName.length() > 0) {
                                            (yyval.node) = newNode(newName);
                                            Node *node = (yyval.node);
                                            if (node != NULL) {
                                                addCommentsToNode((yyval.node));
                                                nodeStack.push((yyval.node));
                                                if (defName != -1) { 
                                                    scene->def(uniqName(SYMB(
                                                        defName)), (yyval.node));
                                                   defName = -1;
                                                }
                                            } else
                                                nodeStack.push(NULL);
                                        }
                                      }
#line 1996 "y.tab.c" /* yacc.c:1652  */
    break;

  case 55:
#line 426 "parser.y" /* yacc.c:1652  */
    { (yyval.node) = nodeStack.pop(); }
#line 2002 "y.tab.c" /* yacc.c:1652  */
    break;

  case 56:
#line 427 "parser.y" /* yacc.c:1652  */
    { 
                                        (yyval.node) = (yyvsp[-1].node);
Node *node = (yyval.node);
                                        addCommentsToNode((yyval.node)); 
                                        nodeStack.push((yyval.node));
                                        if (defName != -1) { 
                                            scene->def(uniqName(SYMB(defName)), 
                                                       (yyval.node));
                                            defName = -1;
                                        }
                                      }
#line 2018 "y.tab.c" /* yacc.c:1652  */
    break;

  case 57:
#line 438 "parser.y" /* yacc.c:1652  */
    { 
                                        (yyval.node) = nodeStack.pop(); 
                                        ((NodeScript *) (yyval.node))->update(); 
                                      }
#line 2027 "y.tab.c" /* yacc.c:1652  */
    break;

  case 60:
#line 449 "parser.y" /* yacc.c:1652  */
    { (yyval.node) = new NodeScript(scene); }
#line 2033 "y.tab.c" /* yacc.c:1652  */
    break;

  case 61:
#line 450 "parser.y" /* yacc.c:1652  */
    { (yyval.node) = (yyvsp[0].node); }
#line 2039 "y.tab.c" /* yacc.c:1652  */
    break;

  case 62:
#line 453 "parser.y" /* yacc.c:1652  */
    { 
                                        (yyval.node) = scene->createDynamicFieldsNode(
                                            "VrmlCut"); 
                                        }
#line 2048 "y.tab.c" /* yacc.c:1652  */
    break;

  case 63:
#line 459 "parser.y" /* yacc.c:1652  */
    { (yyval.node) = newNode("ComposedShader"); }
#line 2054 "y.tab.c" /* yacc.c:1652  */
    break;

  case 64:
#line 460 "parser.y" /* yacc.c:1652  */
    { (yyval.node) = newNode("ShaderProgram"); }
#line 2060 "y.tab.c" /* yacc.c:1652  */
    break;

  case 65:
#line 461 "parser.y" /* yacc.c:1652  */
    { (yyval.node) = newNode("PackagedShader"); }
#line 2066 "y.tab.c" /* yacc.c:1652  */
    break;

  case 68:
#line 469 "parser.y" /* yacc.c:1652  */
    { (yyval.element) = NULL; }
#line 2072 "y.tab.c" /* yacc.c:1652  */
    break;

  case 69:
#line 470 "parser.y" /* yacc.c:1652  */
    { addDynamicElement((yyvsp[0].element)); }
#line 2078 "y.tab.c" /* yacc.c:1652  */
    break;

  case 70:
#line 471 "parser.y" /* yacc.c:1652  */
    { if (protoStack.empty()) 
                                              yyerror(IS_OUTSIDE_PROTO); 
                                          (yyval.element) = new EventIn((yyvsp[-3].int32), SYMB((yyvsp[-2].id)));
                                          addDynamicElement((yyval.element));
                                          isField(nodeStack.peek(),
                                                  SYMB((yyvsp[-2].id)), SYMB((yyvsp[0].id))); 
                                        }
#line 2090 "y.tab.c" /* yacc.c:1652  */
    break;

  case 71:
#line 478 "parser.y" /* yacc.c:1652  */
    { if (protoStack.empty()) 
                                              yyerror(IS_OUTSIDE_PROTO); 
                                          (yyval.element) = new EventOut((yyvsp[-3].int32), SYMB((yyvsp[-2].id))); 
                                          addDynamicElement((yyval.element));
                                          isField(nodeStack.peek(),
                                                  SYMB((yyvsp[-2].id)), SYMB((yyvsp[0].id))); 
                                        }
#line 2102 "y.tab.c" /* yacc.c:1652  */
    break;

  case 72:
#line 486 "parser.y" /* yacc.c:1652  */
    { if (protoStack.empty())
                                              yyerror(IS_OUTSIDE_PROTO);
                                          (yyval.element) = (yyvsp[-2].element);
                                          addDynamicElement((yyval.element));
                                          isField(nodeStack.peek(), 
                                                  (yyvsp[-2].element)->getName(x3d), SYMB((yyvsp[0].id)));
                                        }
#line 2114 "y.tab.c" /* yacc.c:1652  */
    break;

  case 73:
#line 493 "parser.y" /* yacc.c:1652  */
    { if (protoStack.empty()) 
                                              yyerror(IS_OUTSIDE_PROTO); 
                                          (yyval.element) = (yyvsp[-2].element);
                                          addDynamicExposedElement((yyval.element));
                                          isField(nodeStack.peek(),
                                                  (yyvsp[-2].element)->getName(x3d), SYMB((yyvsp[0].id))); 
                                        }
#line 2126 "y.tab.c" /* yacc.c:1652  */
    break;

  case 74:
#line 502 "parser.y" /* yacc.c:1652  */
    { 
                                          setField(nodeStack.peek(),
                                                   (yyvsp[-1].int32), (yyvsp[0].value)); 
                                        }
#line 2135 "y.tab.c" /* yacc.c:1652  */
    break;

  case 75:
#line 506 "parser.y" /* yacc.c:1652  */
    { isField(nodeStack.peek(),
                                                  SYMB((yyvsp[-2].id)), SYMB((yyvsp[0].id))); 
                                        }
#line 2143 "y.tab.c" /* yacc.c:1652  */
    break;

  case 78:
#line 514 "parser.y" /* yacc.c:1652  */
    { (yyval.int32) = checkField(nodeStack.peek(),
                                                          SYMB((yyvsp[0].id))); 
                                        }
#line 2151 "y.tab.c" /* yacc.c:1652  */
    break;

  case 81:
#line 525 "parser.y" /* yacc.c:1652  */
    { (yyval.int32) = fieldTypeToEnum(SYMB((yyvsp[0].id))); }
#line 2157 "y.tab.c" /* yacc.c:1652  */
    break;

  case 84:
#line 537 "parser.y" /* yacc.c:1652  */
    { (yyval.value) = stringToType(SYMB((yyvsp[0].id)),
                                                            currentType); 
                                        }
#line 2165 "y.tab.c" /* yacc.c:1652  */
    break;

  case 85:
#line 541 "parser.y" /* yacc.c:1652  */
    { (yyval.value) = new MFString((yyvsp[-1].stringArray)); }
#line 2171 "y.tab.c" /* yacc.c:1652  */
    break;

  case 86:
#line 542 "parser.y" /* yacc.c:1652  */
    { 
                                          if ((yyvsp[0].node)->isValid())
                                              (yyval.value) = new SFNode((yyvsp[0].node)); 
                                          else
                                              (yyval.value) = NULL;
                                        }
#line 2182 "y.tab.c" /* yacc.c:1652  */
    break;

  case 87:
#line 548 "parser.y" /* yacc.c:1652  */
    { (yyval.value) = emptyMFNodeOrNULL(currentType); }
#line 2188 "y.tab.c" /* yacc.c:1652  */
    break;

  case 88:
#line 550 "parser.y" /* yacc.c:1652  */
    {
                                           NodeList *nodeList = (yyvsp[-1].nodeList);
                                           if (nodeList->isValid()) 
                                               (yyval.value) = new MFNode(nodeList);
                                           else
                                               (yyval.value) = NULL;
                                        }
#line 2200 "y.tab.c" /* yacc.c:1652  */
    break;

  case 89:
#line 557 "parser.y" /* yacc.c:1652  */
    { (yyval.value) = intsToType((yyvsp[0].intArray), currentType); }
#line 2206 "y.tab.c" /* yacc.c:1652  */
    break;

  case 90:
#line 558 "parser.y" /* yacc.c:1652  */
    { (yyval.value) = floatsToType((yyvsp[0].doubleArray), currentType); }
#line 2212 "y.tab.c" /* yacc.c:1652  */
    break;

  case 91:
#line 559 "parser.y" /* yacc.c:1652  */
    { (yyval.value) = boolsToType((yyvsp[0].boolArray), currentType); }
#line 2218 "y.tab.c" /* yacc.c:1652  */
    break;

  case 92:
#line 560 "parser.y" /* yacc.c:1652  */
    { (yyval.value) = intsToType((yyvsp[-1].intArray), currentType); }
#line 2224 "y.tab.c" /* yacc.c:1652  */
    break;

  case 93:
#line 561 "parser.y" /* yacc.c:1652  */
    { (yyval.value) = floatsToType((yyvsp[-1].doubleArray), currentType); }
#line 2230 "y.tab.c" /* yacc.c:1652  */
    break;

  case 94:
#line 562 "parser.y" /* yacc.c:1652  */
    { (yyval.value) = boolsToType((yyvsp[-1].boolArray), currentType); }
#line 2236 "y.tab.c" /* yacc.c:1652  */
    break;

  case 95:
#line 563 "parser.y" /* yacc.c:1652  */
    { (yyval.value) = emptyMF(currentType); }
#line 2242 "y.tab.c" /* yacc.c:1652  */
    break;

  case 96:
#line 567 "parser.y" /* yacc.c:1652  */
    { 
                                (yyvsp[-1].boolArray)->append(((SFBool *)(yyvsp[0].value))->getValue()); 
                                (yyval.boolArray) = (yyvsp[-1].boolArray); 
                                }
#line 2251 "y.tab.c" /* yacc.c:1652  */
    break;

  case 97:
#line 571 "parser.y" /* yacc.c:1652  */
    { 
                                (yyval.boolArray) = new BoolArray(); 
                                (yyval.boolArray)->append(((SFBool *)(yyvsp[0].value))->getValue()); 
                                }
#line 2260 "y.tab.c" /* yacc.c:1652  */
    break;

  case 98:
#line 578 "parser.y" /* yacc.c:1652  */
    { (yyval.value) = new SFBool(true); }
#line 2266 "y.tab.c" /* yacc.c:1652  */
    break;

  case 99:
#line 579 "parser.y" /* yacc.c:1652  */
    { (yyval.value) = new SFBool(false); }
#line 2272 "y.tab.c" /* yacc.c:1652  */
    break;

  case 100:
#line 582 "parser.y" /* yacc.c:1652  */
    { (yyvsp[-1].intArray)->append((yyvsp[0].int32)); (yyval.intArray) = (yyvsp[-1].intArray); }
#line 2278 "y.tab.c" /* yacc.c:1652  */
    break;

  case 101:
#line 583 "parser.y" /* yacc.c:1652  */
    { (yyval.intArray) = new IntArray(); (yyval.intArray)->append((yyvsp[0].int32)); }
#line 2284 "y.tab.c" /* yacc.c:1652  */
    break;

  case 102:
#line 586 "parser.y" /* yacc.c:1652  */
    { (yyvsp[-1].doubleArray)->append((yyvsp[0].sffloat)); (yyval.doubleArray) = (yyvsp[-1].doubleArray); }
#line 2290 "y.tab.c" /* yacc.c:1652  */
    break;

  case 103:
#line 587 "parser.y" /* yacc.c:1652  */
    { (yyvsp[-1].doubleArray)->append((float) (yyvsp[0].int32)); (yyval.doubleArray) = (yyvsp[-1].doubleArray); }
#line 2296 "y.tab.c" /* yacc.c:1652  */
    break;

  case 104:
#line 588 "parser.y" /* yacc.c:1652  */
    { (yyval.doubleArray) = intsToFloats((yyvsp[-1].intArray)); (yyval.doubleArray)->append((yyvsp[0].sffloat)); 
                                  delete (yyvsp[-1].intArray); 
                                }
#line 2304 "y.tab.c" /* yacc.c:1652  */
    break;

  case 105:
#line 591 "parser.y" /* yacc.c:1652  */
    { (yyval.doubleArray) = new DoubleArray(); (yyval.doubleArray)->append((yyvsp[0].sffloat)); }
#line 2310 "y.tab.c" /* yacc.c:1652  */
    break;

  case 106:
#line 594 "parser.y" /* yacc.c:1652  */
    { (yyval.value) = new MFString(SYMB((yyvsp[0].id))); }
#line 2316 "y.tab.c" /* yacc.c:1652  */
    break;

  case 107:
#line 596 "parser.y" /* yacc.c:1652  */
    { (yyval.value) = new MFString((yyvsp[-1].stringArray)); }
#line 2322 "y.tab.c" /* yacc.c:1652  */
    break;

  case 108:
#line 599 "parser.y" /* yacc.c:1652  */
    { (yyval.stringArray) = new StringArray();
                                  (yyval.stringArray)->append(SYMB((yyvsp[0].id))); }
#line 2329 "y.tab.c" /* yacc.c:1652  */
    break;

  case 109:
#line 601 "parser.y" /* yacc.c:1652  */
    { (yyvsp[-1].stringArray)->append(SYMB((yyvsp[0].id))); (yyval.stringArray) = (yyvsp[-1].stringArray); }
#line 2335 "y.tab.c" /* yacc.c:1652  */
    break;

  case 110:
#line 604 "parser.y" /* yacc.c:1652  */
    {

                                          (yyval.nodeList) = new NodeList();
                                          if ((yyvsp[0].node)) 
                                              (yyvsp[0].node)->appendTo((yyval.nodeList));
                                        }
#line 2346 "y.tab.c" /* yacc.c:1652  */
    break;

  case 111:
#line 610 "parser.y" /* yacc.c:1652  */
    { 
                                          if ((yyvsp[-1].nodeList)) 
                                              (yyvsp[0].node)->appendTo((yyvsp[-1].nodeList)); 
                                          (yyval.nodeList) = (yyvsp[-1].nodeList);
                                        }
#line 2356 "y.tab.c" /* yacc.c:1652  */
    break;

  case 112:
#line 616 "parser.y" /* yacc.c:1652  */
    { (yyval.id) = (yyvsp[0].id); }
#line 2362 "y.tab.c" /* yacc.c:1652  */
    break;

  case 113:
#line 617 "parser.y" /* yacc.c:1652  */
    {  
                                          if (x3d)
                                              scene->warning(
                                                    IDS_ILLEGAL_X3D_CHARACTER,
                                                    (const char *)SYMB((yyvsp[0].id)));
                                          (yyval.id) = (yyvsp[0].id); 
                                        }
#line 2374 "y.tab.c" /* yacc.c:1652  */
    break;

  case 114:
#line 626 "parser.y" /* yacc.c:1652  */
    { illegalX3DV(); }
#line 2380 "y.tab.c" /* yacc.c:1652  */
    break;

  case 131:
#line 655 "parser.y" /* yacc.c:1652  */
    { scene->addMeta(SYMB((yyvsp[-1].id)), SYMB((yyvsp[0].id))); }
#line 2386 "y.tab.c" /* yacc.c:1652  */
    break;

  case 134:
#line 662 "parser.y" /* yacc.c:1652  */
    { scene->addUnit(SYMB((yyvsp[-2].id)), SYMB((yyvsp[-1].id)), 
                                                         (yyvsp[0].sffloat)); 
                                        }
#line 2394 "y.tab.c" /* yacc.c:1652  */
    break;

  case 135:
#line 665 "parser.y" /* yacc.c:1652  */
    { scene->addUnit(SYMB((yyvsp[-2].id)), SYMB((yyvsp[-1].id)), 
                                                         (yyvsp[0].int32)); 
                                        }
#line 2402 "y.tab.c" /* yacc.c:1652  */
    break;


#line 2406 "y.tab.c" /* yacc.c:1652  */
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
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

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


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
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
#line 669 "parser.y" /* yacc.c:1918  */


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
        for (size_t i = 0; i < commentNodeList.size(); i++)
            node->appendComment(commentNodeList[i]);
        commentNodeList.resize(0);
    }
}

static void 
addCommentsToNodeList(NodeList *nodelist)
{
    if (commentNodeList.size() > 0) {
        for (size_t i = 0; i < commentNodeList.size(); i++)
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
                for (size_t i = 0; i < array.size(); i++)
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
#define STATUS_HTML_BODY 14

static MyStack<DataNode> nodes;

static bool insideX3D = false;
static bool canStoreHTML = false;
static bool htmlFirstPart = true;

static MyStack<int> status;
static MyStack<int> currentFieldIndex;
static bool done;
static NodeList *rootNodes;
char *currentURL = NULL;
char *parentURL = NULL;
static MyArray<XML_Parser> x3dParser;
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

static const char *htmlData = "";

static void XMLCALL handleHtmlData (void *userData, const char *string, int len) 
{
    if (len == 1)
        if (string[0] == '\r')
            return;

    char *temp = (char *)malloc(len + 2);
    strncpy(temp, string, len);
    temp[len] = '\0';
    temp[len + 1] = '\0';
    if (canStoreHTML) {
        // store HTML part to scene
        scene->storeHtmlData(temp);
    }
}

static void XMLCALL handleCDATA (void *userData, const char *string, int len) 
{
    htmlData = "";
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

static void mfstrsplit(MyArray<const char *> *toksArray, char *string)
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
            for (unsigned int k = 0; k < strlen(wseps); k++) 
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
        MyArray<const char *> tokens;
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

bool strCompare(const char *element, const char *str, Scene *scene)
{
     return (!strcmp(element, str)) || (scene->getStoreAsHtml() && 
                                        (!strcasecmp(element, str)));
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
        insideX3D = true;
        canStoreHTML = false;
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
            } else if ((insideX3D && 
                        strCompare(element, "Script", scene)) || 
                       strCompare(element, "ComposedShader", scene) || 
                       strCompare(element, "ShaderProgram", scene) || 
                       strCompare(element, "PackagedShader", scene)) {
                if (insideX3D && strCompare(element, "Script", scene))
                    node = new NodeScript(scene);
                else if (insideX3D)
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
            } else if (strCompare(element, "ShaderPart", scene)) {
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
            } else if (strCompare(element, "fieldValue", scene)) {
                /* doesn't create a node, adds to it's parents fields */
                // <fieldValue name='initValue' value='.15'/>
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
            } else if (strCompare(element, "ProtoDeclare", scene)) {
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
                    scene->warning(IDS_PROTO_ALREADY_DEFINED, name);
                startProto();
            } else if (strCompare(element, "ExternProtoDeclare", scene)) {
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
            } else if (strCompare(element, "ProtoInterface", scene)) {
                status.pop();
                status.push(STATUS_PROTOINTERFACE);
            } else if (strCompare(element, "ProtoBody", scene)) {
                status.pop();
                status.push(STATUS_PROTOBODY);
            } else if (strCompare(element, "IS", scene)) {
            } else if (strCompare(element, "connect", scene)) {
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
            } else if (strCompare(element, "ProtoInstance", scene)) {
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
                    } else
                        swDebugf("ProtoInstance - failed to find proto of type %s line %d\n",name,lineno);
                }
                status.pop();
                status.push(STATUS_NODE);
            } else if (strcmp(element, "html") == 0 ||
                       strcmp(element, "title") == 0 ||
                       strcmp(element, "link") == 0 ||
                       ((!canStoreHTML) && strcmp(element, "script") == 0)) {
                // parse X3DDOM header content into void
                node = NULL;
            } else if (strcmp(element, "body") == 0) {
                // parse HTML content
                status.pop();
                status.push(STATUS_HTML_BODY);
                canStoreHTML = true;
                node = NULL;
            } else if (strcmp(element, "x3d") == 0) {
                insideX3D = true;
                canStoreHTML = false;
                node = NULL;
            } else if (insideX3D) {
                node = newNode(element); // element would be a string like "Transform" 
                if (node) {
                    status.pop();
                    status.push(STATUS_NODE);
                } 
            } else {
                int numAttrib = 0;
                for (int i = 0; attrib[i] != NULL; i++)
                    numAttrib = i + 1;
                scene->storeHtmlElementAndAttributes(element, attrib, 
                                                     numAttrib, htmlFirstPart);
                node = NULL;
            }
            if (node != NULL) {
                nodeStack.pop();
                nodeStack.push(node);
                current_node = node;
                level++;
                node->appendTo(nodes.peek().nodeList);
                for (int i = 0; attrib[i] != NULL; i += 2) { 
                    if (strcmp("DEF", attrib[i]) == 0) {
                        //"DEF" 
                        scene->def(uniqName(attrib[i + 1]), node);
                    } else if (strcmp("id", attrib[i]) == 0) {
                        // special X3DOM attribute "id"
                        node->setX3domId(attrib[i + 1]);
                    } else if (strcasecmp("OnOutputChange", attrib[i]) == 0) {
                        // special X3DOM attribute "OnOutputChange"
                        node->setX3domOnOutputChange(attrib[i + 1]);
                    } else if (strcasecmp("OnClick", attrib[i]) == 0) {
                        // special X3DOM attribute "OnOutputChange"
                        node->setX3domOnClick(attrib[i + 1]);
                    } else if (!strcmp("containerField", attrib[i])) {
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
        if (strCompare(element, "ROUTE", scene)) {
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
    MyArray<int> *containerFields = nodes.peek().containerFields;
    childList = nodes.pop().nodeList;
    Node *current_node = nodeStack.pop();
    Proto *current_proto = protoStack.peek(); 
    int istatus = status.pop();

    if (strcasecmp(el, "X3D") == 0) {
        insideX3D = false;        
        canStoreHTML = true;
        htmlFirstPart = false;
        // empty HTML attribute second
        scene->storeHtmlElementAndAttributes("", NULL, 0, htmlFirstPart);
    }

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
                        if (i < (int)containerFields->size())
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
    insideX3D = false;
    canStoreHTML = false;
    htmlFirstPart = true;

    // empty HTML attribute first
    scene->storeHtmlElementAndAttributes("", NULL, 0, htmlFirstPart);

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
    XML_SetCharacterDataHandler(x3dParser[contextIndex], handleHtmlData);
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

