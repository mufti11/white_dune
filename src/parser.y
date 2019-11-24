/*
 * parser.y - a YACC grammar for parsing VRML 2.0 files
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

/* set "int yydebug = 1;" in the parser.cpp file, to get debugging with bison */

%{
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
%}

%union {
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
};

%token BRACKET_ON BRACKET_OFF WING_BRACKET_ON WING_BRACKET_OFF
%token SCRIPT COMPOSED_SHADER PACKAGED_SHADER SHADER_PROGRAM VRML_CUT 
%token EXPORT IMPORT AS 
%token <id> ID ID_X3D STRING
%token <int32> INT_NUM
%token <sffloat> FLOAT_NUM
%token DEF EXTERNPROTO FALSE_TOK IS NULL_TOK PROTO ROUTE TO TRUE_TOK USE
%token EVENT_IN EVENT_OUT EXPOSED_FIELD INPUT_OUTPUT FIELD 
%token <id> X3D PROFILE COMPONENT UNIT META 
%token VRML1

%type <id> id
%type <node> node dynamicNode shaderNode nodeStatement 
//%type <dynamicNode> VRML_CUT
%type <node> importStatement exportStatement statement 
%type <nodeList> nodeStatements statements
%type <element> externInterfaceDeclaration restrictedInterfaceDeclaration
%type <element> scriptBodyElement interfaceDeclaration 
%type <element> fieldDeclaration exposedFieldDeclaration
%type <id> nodeName nodeType 
%type <int32> fieldName fieldType
%type <value> fieldValue sfboolValue mfstringValue URLList
%type <stringArray> strings
%type <intArray> ints
%type <boolArray> bools
%type <doubleArray> floats
%type <id> componentId profileId 
%type <id> x3dHeaderStatement headerStatement 
%type <id> componentStatement metaStatement unitStatement profileStatement 
%type <id> componentStatements metaStatements unitStatements 
%%
vrmlScene:
          statements             { 
                                  nodeComment();
                                  addCommentsToNodeList($1);
                                  scene->addNodes(targetNode, targetField, $1,
                                                  scanFor); 
                                 }
          | x3dHeaderStatement statements { 
                                  nodeComment();
                                  addCommentsToNodeList($2);
                                  scene->addNodes(targetNode, targetField, $2,
                                                  scanFor); 
                                }
          | VRML1 empty         /* on error, skip until file is read */
          | error empty         /* on error, skip until file is read */
        ;
statements:
          statements statement  {
                                  if ($2 != NULL) 
                                      $2->appendTo($1); 
                                  $$ = $1; 
                                }
        | empty                 { $$ = new NodeList(); }
        ;
statement:
          nodeStatement         { $$ = $1; }
        | protoStatement        { $$ = NULL; }
        | routeStatement        { $$ = NULL; }
        | importStatement       { $$ = $1; }
        | exportStatement       { $$ = $1; }
        ;
nodeStatement:
          node         { $$ = $1; }
        | DEF nodeName { defName = $2; } node { $$ = $4; }
        | USE nodeName { $$ = scene->use(checkName(SYMB($2))); }
        ;
protoStatement:
          proto
        | externproto
        ;
protoStatements:
          protoStatements protoStatement
        | empty
        ;
proto:
          PROTO nodeType         { 
                                   Proto *proto = scene->getProto(SYMB($2));
                                   if ((proto == NULL) || 
                                       proto->isScriptedProto()) {
                                       proto = new Proto(scene, SYMB($2));
                                   }
                                   protoStack.push(proto);
                                   if (!scene->addProtoName(SYMB($2)))
                                      scene->warning(IDS_PROTO_ALREADY_DEFINED,
                                                     (const char*)SYMB($2));
                                   if (TheApp->isImportingProtoLibrary())
                                       proto->setFromProtoLibrary();
                                 }
          BRACKET_ON interfaceDeclarations BRACKET_OFF 
          WING_BRACKET_ON protoBody WING_BRACKET_OFF
                                 { 
                                   scene->addProto(SYMB($2), protoStack.pop());
                                   stopProto();
                                 }
        ;
protoBody:
          protoStatements nodeStatement statements
                                 { 
                                 protoStack.peek()->define($2, $3); 
                                 protoStack.peek()->setLoaded(true);
                                 }
        | empty
        ;
interfaceDeclarations:
          interfaceDeclarations interfaceDeclaration
          {
            if (protoStack.empty()) 
                yyerror("syntax error");
            else 
                protoStack.peek()->addOrUpdateElement($2);
          }
        | empty
        ;
fieldDeclaration:
        FIELD fieldType { currentType = $2; } 
             id { $$ = createField($2, SYMB($4)); }
        ;
exposedFieldDeclaration:
        INPUT_OUTPUT fieldType { currentType = $2; } 
             id { $$ = createField($2, SYMB($4)); }
        ;
restrictedInterfaceDeclaration:
          EVENT_IN fieldType id         { $$ = new EventIn($2, SYMB($3)); }
        | EVENT_OUT fieldType id        { $$ = new EventOut($2, SYMB($3)); }
        | fieldDeclaration fieldValue   { $$ = new Field($1->getType(), 
                                                         $1->getName(x3d), $2); 
                                        }
        | exposedFieldDeclaration fieldValue 
                                        { $$ = new ExposedField($1->getType(), 
                                                     $1->getName(x3d), $2); 
                                        }
        ;
interfaceDeclaration:
          restrictedInterfaceDeclaration
        | EXPOSED_FIELD fieldType { currentType = $2; } id fieldValue
                { $$ = new ExposedField($2, SYMB($4), $5); }
        ;
externproto:
          EXTERNPROTO nodeType
                                { 
                                  Proto *proto = new Proto(scene, SYMB($2));
                                  protoStack.push(proto);
                                  if (!scene->belongsToNodeWithExternProto(
                                          SYMB($2))) {
                                      if (!scene->addProtoName(SYMB($2)))
                                          scene->warning(IDS_PROTO_ALREADY_DEFINED,
                                                         (const char*)SYMB($2));
                                      if (TheApp->isImportingProtoLibrary())
                                          proto->setFromProtoLibrary();
                                  }
                                }
          BRACKET_ON externInterfaceDeclarations BRACKET_OFF URLList
                                {                       
                                  if (!scene->hasProtoName(SYMB($2)) &&
                                      (!scene->belongsToNodeWithExternProto(
                                          SYMB($2)))) {
                                      scene->addProto(SYMB($2), protoStack.pop()); 
                                      scene->getProto(SYMB($2))->addURLs($7);
                                  } 
                                  if (!scene->hasProtoName(SYMB($2)))
                                      stopProto();
                                }
        ;
externInterfaceDeclarations:
          externInterfaceDeclarations externInterfaceDeclaration
          {
            if (protoStack.empty()) 
                yyerror("syntax error");
            else 
                protoStack.peek()->addElement($2);
          }
        | empty
        ;
externInterfaceDeclaration:
          EVENT_IN fieldType id         { $$ = new EventIn($2, SYMB($3)); }
        | EVENT_OUT fieldType id        { $$ = new EventOut($2, SYMB($3)); }
        | FIELD fieldType id            { 
                                          $$ = new Field($2, SYMB($3),
                                                         SillyDefaultValue($2));
                                        }
        | EXPOSED_FIELD fieldType id    {
                                          $$ = new ExposedField($2, SYMB($3),
                                                SillyDefaultValue($2));
                                        }
        | INPUT_OUTPUT fieldType id     {
                                          $$ = new ExposedField($2, SYMB($3),
                                                SillyDefaultValue($2));
                                        }
        ;
routeStatement:
          ROUTE nodeName '.' id TO nodeName '.' id  
             { 
               route(checkName(SYMB($2)), SYMB($4), 
                     checkName(SYMB($6)), SYMB($8)); 
             }
        ;
exportStatement:
          EXPORT nodeName 
             { $$ = addExport(checkName(SYMB($2)), ""); }
        | EXPORT nodeName AS nodeName  
             { $$ = addExport(checkName(SYMB($2)), SYMB($4)); }
        ;
importStatement:
          IMPORT nodeName '.' id  
             { $$ = addImport(checkName(SYMB($2)), SYMB($4), ""); }
        | IMPORT nodeName '.' id AS nodeName  
             { $$ = addImport(checkName(SYMB($2)), SYMB($4), SYMB($6)); }
        ;
URLList:
          mfstringValue
        ;
empty:
        ;
node:
          nodeType WING_BRACKET_ON    { 
                                        $<node>$ = newNode(SYMB($1));
                                        Node *node = $<node>$;
                                        if (node != NULL) {
                                            addCommentsToNode($<node>$);
                                            nodeStack.push($<node>$);
                                            if (defName != -1) { 
                                                scene->def(uniqName(SYMB(
                                                       defName)), $<node>$);
                                                defName = -1;
                                            }
                                        }
                                      }
          nodeBody WING_BRACKET_OFF   { $$ = nodeStack.pop(); }
        | dynamicNode WING_BRACKET_ON { 
                                        $<node>$ = $1;
                                        addCommentsToNode($<node>$); 
                                        nodeStack.push($<node>$);
                                        if (defName != -1) { 
                                            scene->def(uniqName(SYMB(defName)), 
                                                       $<node>$);
                                            defName = -1;
                                        }
                                      }
          scriptBody WING_BRACKET_OFF { 
                                        $$ = nodeStack.pop(); 
                                        ((NodeScript *) $$)->update(); 
                                      }
        ;

nodeBody:
          nodeBody nodeBodyElement
        | empty
        ;

dynamicNode: SCRIPT                     { $$ = new NodeScript(scene); }
        | shaderNode                    { $$ = $1; }
        ;

dynamicNode: VRML_CUT                   { 
                                        $$ = scene->createDynamicFieldsNode(
                                            "VrmlCut"); 
                                        }
        ;

shaderNode: COMPOSED_SHADER             { $$ = newNode("ComposedShader"); }
        |   SHADER_PROGRAM              { $$ = newNode("ShaderProgram"); }
        |   PACKAGED_SHADER             { $$ = newNode("PackagedShader"); }
        ;

scriptBody:
          scriptBody scriptBodyElement 
        | empty
        ;
scriptBodyElement:
          nodeBodyElement               { $$ = NULL; }
        | restrictedInterfaceDeclaration { addDynamicElement($1); } 
        | EVENT_IN fieldType id IS id   { if (protoStack.empty()) 
                                              yyerror(IS_OUTSIDE_PROTO); 
                                          $$ = new EventIn($2, SYMB($3));
                                          addDynamicElement($$);
                                          isField(nodeStack.peek(),
                                                  SYMB($3), SYMB($5)); 
                                        }
        | EVENT_OUT fieldType id IS id  { if (protoStack.empty()) 
                                              yyerror(IS_OUTSIDE_PROTO); 
                                          $$ = new EventOut($2, SYMB($3)); 
                                          addDynamicElement($$);
                                          isField(nodeStack.peek(),
                                                  SYMB($3), SYMB($5)); 
                                        }

        | fieldDeclaration IS id        { if (protoStack.empty())
                                              yyerror(IS_OUTSIDE_PROTO);
                                          $$ = $1;
                                          addDynamicElement($$);
                                          isField(nodeStack.peek(), 
                                                  $1->getName(x3d), SYMB($3));
                                        }
        | exposedFieldDeclaration IS id { if (protoStack.empty()) 
                                              yyerror(IS_OUTSIDE_PROTO); 
                                          $$ = $1;
                                          addDynamicExposedElement($$);
                                          isField(nodeStack.peek(),
                                                  $1->getName(x3d), SYMB($3)); 
                                        }
        ;
nodeBodyElement:
          fieldName fieldValue          { setField(nodeStack.peek(),
                                                   $1, $2); 
                                        }
        | id IS id                      { isField(nodeStack.peek(),
                                                  SYMB($1), SYMB($3)); 
                                        }
        | routeStatement
        | protoStatement
        ;

fieldName:
        id                              { $$ = checkField(nodeStack.peek(),
                                                          SYMB($1)); 
                                        }
        ;
nodeName:
        id
        ;
nodeType:
        id
        ;
fieldType:
        id                              { $$ = fieldTypeToEnum(SYMB($1)); }
        ;

profileId:
        id
        ;

componentId:
        ID
        ;

fieldValue:
        STRING                          { $$ = stringToType(SYMB($1),
                                                            currentType); 
                                        }
        | BRACKET_ON strings BRACKET_OFF
                                        { $$ = new MFString($2); }
        | nodeStatement                 { $$ = new SFNode($1); }
        | NULL_TOK                      { $$ = emptyMFNodeOrNULL(currentType); }
        | BRACKET_ON nodeStatements BRACKET_OFF
                                        { $$ = new MFNode($2); }
        | ints                          { $$ = intsToType($1, currentType); }
        | floats                        { $$ = floatsToType($1, currentType); }
        | bools                         { $$ = boolsToType($1, currentType); }
        | BRACKET_ON ints BRACKET_OFF   { $$ = intsToType($2, currentType); }
        | BRACKET_ON floats BRACKET_OFF { $$ = floatsToType($2, currentType); }
        | BRACKET_ON bools BRACKET_OFF  { $$ = boolsToType($2, currentType); }
        | BRACKET_ON BRACKET_OFF        { $$ = emptyMF(currentType); }
        ;

bools:
          bools sfboolValue     { 
                                $1->append(((SFBool *)$2)->getValue()); 
                                $$ = $1; 
                                }
        | sfboolValue           { 
                                $$ = new BoolArray(); 
                                $$->append(((SFBool *)$1)->getValue()); 
                                }
          ;

sfboolValue:
          TRUE_TOK              { $$ = new SFBool(true); }
        | FALSE_TOK             { $$ = new SFBool(false); }
        ;
ints:
          ints INT_NUM          { $1->append($2); $$ = $1; }
        | INT_NUM               { $$ = new IntArray(); $$->append($1); }
        ;
floats:
          floats FLOAT_NUM      { $1->append($2); $$ = $1; }
        | floats INT_NUM        { $1->append((float) $2); $$ = $1; }
        | ints FLOAT_NUM        { $$ = intsToFloats($1); $$->append($2); 
                                  delete $1; 
                                }
        | FLOAT_NUM             { $$ = new DoubleArray(); $$->append($1); }
        ;
mfstringValue:
          STRING                { $$ = new MFString(SYMB($1)); }
        | BRACKET_ON strings BRACKET_OFF
                                { $$ = new MFString($2); }
        ;
strings:
          STRING                { $$ = new StringArray();
                                  $$->append(SYMB($1)); }
        | strings STRING        { $1->append(SYMB($2)); $$ = $1; }
        ;
nodeStatements:
          nodeStatement                 {
                                          $$ = new NodeList();
                                          if ($1) 
                                              $1->appendTo($$);
                                        }
        | nodeStatements nodeStatement  { 
                                          if ($2) 
                                              $2->appendTo($1); 
                                          $$ = $1;
                                        }
        ;
id:     ID_X3D                          { $$ = $1; }
        | ID                            {  
                                          if (x3d)
                                              scene->warning(
                                                    IDS_ILLEGAL_X3D_CHARACTER,
                                                    (const char *)SYMB($1));
                                          $$ = $1; 
                                        }
        ;
x3dHeaderStatement:
          headerStatement               { illegalX3DV(); }
        | headerStatement profileStatement 
        | headerStatement profileStatement componentStatements
        | headerStatement profileStatement componentStatements metaStatements
        | headerStatement profileStatement componentStatements unitStatements
        | headerStatement profileStatement componentStatements unitStatements
                                           metaStatements
        | headerStatement profileStatement unitStatements
        | headerStatement profileStatement unitStatements metaStatements 
        | headerStatement profileStatement metaStatements unitStatements 
        | headerStatement profileStatement metaStatements
        ;
headerStatement:
        X3D 
        ;
profileStatement:
        PROFILE profileId
        ;
componentStatements:
        componentStatement 
        | componentStatement componentStatements 
        ;
componentStatement:
        COMPONENT componentId;
metaStatements:
          metaStatement 
        | metaStatement metaStatements 
        ;
metaStatement:
        META STRING STRING              { scene->addMeta(SYMB($2), SYMB($3)); }
        ;
unitStatements:
          unitStatement 
        | unitStatement unitStatements 
        ;
unitStatement:
        UNIT id id FLOAT_NUM            { scene->addUnit(SYMB($2), SYMB($3), 
                                                         $4); 
                                        }
        | UNIT id id INT_NUM            { scene->addUnit(SYMB($2), SYMB($3), 
                                                         $4); 
                                        }
        ;
%%

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

