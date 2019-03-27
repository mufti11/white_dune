/*
 * lexer.l
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

 /* string parsing rule from gengtype-lex.l/gcc  
     Copyright (C) 2002, 2003 Free Software Foundation, Inc.
 */

%{
#include <stdlib.h>
#include <string.h>
#include "stdafx.h"
#include "swDebugf.h"

#include "Element.h"
#include "EventIn.h"
#include "EventOut.h"
#include "ExposedField.h"
#include "Field.h"
#include "FieldValue.h"
#include "Node.h"
#include "Scene.h"

#include "SFBool.h"
#include "SFNode.h"
#include "MFNode.h"
#include "SFString.h"

#include "parser.h"
#include "y.tab.h"

#define YY_NEVER_INTERACTIVE 1

#ifdef HAVE_LIBZ
# define YY_INPUT(buf, result, max_size) \
            { result = gzread( inputFile , buf, max_size ); }
#else
# define YY_INPUT(buf, result, max_size) \
            { result = fread( buf, 1, max_size, inputFile ); }
#endif

#if defined(__sgi) && !defined(__GNUC__)
#pragma set woff 1110
#endif

long hexint;
bool protoFlag=false;
int protoNested=0;
bool hasX3DFlag=false;
bool hasProfileFlag=false;

void startProto(void)
{
    protoFlag=true;
    if (protoNested > 0)
        scene->setNestedProto();
    scene->addProtoDefinition();       
    if (protoNested > 0)
        scene->setNestedProto();
    protoNested++;
}
void toProto(char* string)
{
    if (protoFlag)
        scene->addToProtoDefinition(string);
}
void stopProto(void)
{
    if (protoNested--==1)
        protoFlag=false;
}
void toComment(char* string)
{
    if (!protoFlag)
        addToCurrentComment(string);
}
#define YY_NO_UNISTD_H 1
%}

IDFIRSTCHAR     [^ \r\n\t'\"#,\.\[\]\\\{\}0-9\+\-]
IDRESTCHARS     [^ \r\n\t'\"#,\.\[\]\\\{\}]

X3D_IDFIRSTCHAR     [^ \r\n\t'\"#,\.\[\]\\\{\}0-9\+\-\:]
X3D_IDRESTCHARS     [^ \r\n\t'\"#,\.\[\]\\\{\}\:]

EXPONENT        ([Ee]("-"|"+")?[0-9]+)

%%

\{                              {
                                toProto(yytext); 
                                nodeComment();
                                return WING_BRACKET_ON;
                                }

\}                              {
                                toProto(yytext); 
                                insideNodeComment();
                                return WING_BRACKET_OFF;
                                }

\[                              {
                                toProto(yytext); 
                                MFComment();
                                return BRACKET_ON;
                                }

\]                              {
                                toProto(yytext); 
                                insideMFComment();
                                return BRACKET_OFF;
                                }

("-"|"+")?([0-9]+"."[0-9]*|[0-9]*"."[0-9]+){EXPONENT}? {
                                                       toProto(yytext); 
                                                       yylval.sffloat = 
                                                          (float) atof(yytext); 
                                                       return FLOAT_NUM; 
                                                       }

("-"|"+")?[0-9]+{EXPONENT}      {
                                toProto(yytext); 
                                yylval.sffloat = (float) atof(yytext); 
                                return FLOAT_NUM; 
                                }

("-"|"+")?[0-9]+                {
                                toProto(yytext); 
                                yylval.int32 = atoi(yytext); 
                                return INT_NUM; 
                                }

("-"|"+")?0x[0-9a-fA-F]+        { 
                                toProto(yytext); 
                                sscanf(yytext, "%lx", &hexint);
                                yylval.int32 = (int) hexint;
                                return INT_NUM; 
                                }

Script          { toProto(yytext); return SCRIPT; }

ComposedShader  { 
                toProto(yytext); 
                if (hasX3DFlag)       
                    return COMPOSED_SHADER; 
                yylval.id = scene->addSymbol(yytext); 
                return ID;
                }

ShaderProgram   { 
                toProto(yytext); 
                if (hasX3DFlag)       
                    return SHADER_PROGRAM; 
                yylval.id = scene->addSymbol(yytext); 
                return ID;
                }

PackagedShader  { 
                toProto(yytext); 
                if (hasX3DFlag)       
                    return PACKAGED_SHADER; 
                yylval.id = scene->addSymbol(yytext); 
                return ID;
                }

EXPORT          { 
                toProto(yytext); 
                if (hasX3DFlag)       
                    return EXPORT; 
                yylval.id = scene->addSymbol(yytext); 
                return ID;
                }


IMPORT          { 
                toProto(yytext); 
                if (hasX3DFlag)       
                    return IMPORT; 
                yylval.id = scene->addSymbol(yytext); 
                return ID;
                }

AS              { 
                toProto(yytext); 
                if (hasX3DFlag)       
                    return AS; 
                yylval.id = scene->addSymbol(yytext); 
                return ID;
                }

DEF             { toProto(yytext); return DEF; }

EXTERNPROTO     { startProto(); toProto(yytext); return EXTERNPROTO; }

FALSE           { toProto(yytext); return FALSE_TOK; }

IS              { toProto(yytext); return IS; }

NULL            { toProto(yytext); return NULL_TOK; }

PROTO           { startProto(); toProto(yytext); return PROTO; }

ROUTE           { toProto(yytext); return ROUTE; }

TO              { toProto(yytext); return TO; }

TRUE            { toProto(yytext); return TRUE_TOK; }

USE             { toProto(yytext); return USE; }

eventIn         { toProto(yytext); return EVENT_IN; }

eventOut        { toProto(yytext); return EVENT_OUT; }

exposedField    { toProto(yytext); return EXPOSED_FIELD; }

field           { toProto(yytext); return FIELD; }

inputOnly       { 
                toProto(yytext); 
                if (hasX3DFlag)       
                    return EVENT_IN;
                yylval.id = scene->addSymbol(yytext); 
                return ID;
                }

outputOnly      { 
                toProto(yytext); 
                if (hasX3DFlag)       
                    return EVENT_OUT;
                yylval.id = scene->addSymbol(yytext); 
                return ID;
                }

inputOutput     { 
                toProto(yytext); 
                if (hasX3DFlag)       
                    return INPUT_OUTPUT;
                yylval.id = scene->addSymbol(yytext); 
                return ID;
                }

initializeOnly  { 
                toProto(yytext); 
                if (hasX3DFlag)       
                    return FIELD;
                yylval.id = scene->addSymbol(yytext); 
                return ID;
                }

PROFILE         { 
                if (hasX3DFlag) {
                    hasProfileFlag = true;
                    return PROFILE;
                } 
                toProto(yytext); 
                yylval.id = scene->addSymbol(yytext); 
                return ID;
                }

COMPONENT       {
                if (hasProfileFlag)
                    return COMPONENT;
                toProto(yytext); 
                yylval.id = scene->addSymbol(yytext); 
                return ID;
                }

META            { 
                if (hasProfileFlag)
                    return META; 
                toProto(yytext); 
                yylval.id = scene->addSymbol(yytext); 
                return ID;
                }

UNIT            { 
                if (hasProfileFlag)
                    return UNIT; 
                toProto(yytext); 
                yylval.id = scene->addSymbol(yytext); 
                return ID;
                }

^"#X3D V3.0 utf8"       {
                        if (lineno == 1) {
                            lineno++;
                            hasX3DFlag = true;
                            setX3d();
                            scene->setParsedX3dVersion(0);
                            scene->updateSceneMap();
                            return X3D;
                        }
                        toProto(yytext); 
                        toComment(yytext);
                        /* eat up comments */
                        }

^"#X3D V3."[1-3]" utf8"       {
                        if (lineno == 1) {
                            lineno++;
                            hasX3DFlag = true;
                            setX3d();
                            scene->setParsedX3dVersion(-1);
                            scene->updateSceneMap();
                            return X3D;
                        }
                        toProto(yytext); 
                        toComment(yytext);
                        /* eat up comments */
                        }

^"#X3D V3."[4-9]*" utf8" {
                        swDebugf("Warning: about \"%s\": only ",
                                 yytext);
                        swDebugf("ISO/IEC 19775-1:2013");
                        swDebugf(" is currently supported\n"); 
                        if (lineno == 1) {
                            lineno++;
                            hasX3DFlag = true;
                            setX3d();
                            scene->setParsedX3dVersion(-1);
                            scene->updateSceneMap();
                            return X3D;
                        }
                        toProto(yytext); 
                        toComment(yytext);
                        /* eat up comments */
                        }

^"#VRML V1.0"[^\n\r]*   {
                        if (lineno == 1) {
                            scene->errorf("VRML Version 1 not supported\n"); 
                            TheApp->setVrml1Error();
                            return(VRML1);
                        }
                        toProto(yytext); 
                        toComment(yytext);
                        /* eat up comments */
                        }


^\xef\xbb\xbf           {
                        /* 
                           UTF8 Byte Order Mark (BOM)
                           (used to mark utf8 documents at the beginning) 
                           ignored at the begin of file
                        */
                        if (lineno != 1)
                            return yytext[0];
                        } 


{X3D_IDFIRSTCHAR}{X3D_IDRESTCHARS}* {
                                    toProto(yytext); 
                                    yylval.id = scene->addSymbol(yytext); 
                                    return ID_X3D;
                                    }

{IDFIRSTCHAR}{IDRESTCHARS}*     {
                                toProto(yytext); 
                                yylval.id = scene->addSymbol(yytext); 
                                return ID;
                                }

\"([^"\\]|\\.|\\\n)*\"  {
                        toProto(yytext); 
                        for (const char *s = yytext; *s; s++) {
                           if (*s == '\n') lineno++;
                        }
                        yytext[strlen(yytext)-1] = '\0';
                        yylval.id = scene->addSymbol(yytext+1);
                        return STRING;
                        }

\n"#"[^\n\r]*           {
                        toProto(yytext); 
                        toComment(yytext);
                        lineno++; 
                        /* eat up comments */
                        }

"#"[^\n\r]*             {
                        toProto(yytext); 
                        toComment(yytext);
                        /* eat up comments */
                        }

[ \t\r,]+               {
                        toProto(yytext); 
                        /* eat up whitespace and commas */
                        }

\n|\n\r|\r              { 
                        toProto(yytext); 
                        lineno++; 
                        }

.                       {
                        toProto(yytext);  
                        return yytext[0]; 
                        }

%%
