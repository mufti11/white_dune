/*
 * NodeScript.cpp
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

#include <stdio.h>
#include "stdafx.h"
#include <ctype.h>

#include "NodeScript.h"
#include "Scene.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFBool.h"
#include "ExposedField.h"
#include "Field.h"
#include "EventIn.h"
#include "EventOut.h"
#include "DuneApp.h"
#include "RouteCommand.h"
#include "UnRouteCommand.h"
#include "URL.h"

ProtoScript::ProtoScript(Scene *scene)
  : WonderlandExportProto(scene, "Script")
{
    url.set(
          addExposedField(MFSTRING, "url", new MFString(), 
                          FF_STATIC | FF_URL, NULL));
    directOutput.set(
          addField(SFBOOL, "directOutput", new SFBool(false), FF_STATIC, NULL));
    mustEvaluate.set(
          addField(SFBOOL, "mustEvaluate", new SFBool(false), FF_STATIC, NULL));
}

Node *
ProtoScript::create(Scene *scene)
{ 
    return new NodeScript(scene); 
}

NodeScript::NodeScript(Scene *scene)
  : DynamicFieldsNode(scene, new ProtoScript(scene))
{
    m_wantInitialize = false;
    m_wantEventsProcessed = false;
    m_wantShutdown = false;
}

NodeScript::~NodeScript()
{
}
 
// since scripts can have dynamically-added fields, we need to update
// the node's fields after the script is defined.
// Same problem for the scriptinterfacebuilder (ScriptDialog) (add or delete)
//
// It is important, that only one item (field, eventIn or eventOut) can
// be added or deleted during this call of update.

void
NodeScript::update()
{
    updateDynamicFields();
}

// strstr like function, that do not care about whitespace characters

bool 
NodeScript::strwhitespacestr(const char* string, const char* what)
{
    char *whatPtr = mystrdup(what);
    int   whatLen = strlen(whatPtr);

    // split a whitespace seperated whatPtr into zero seperated substrings
    for (int i = 0; i < whatLen; i++)
        if (isspace(whatPtr[i]))
            whatPtr[i] = (char)0;

    const char *stringPtr = string;
    int         stringLen = strlen(stringPtr);
    char       *stringEnd = (char *)stringPtr + stringLen;

    while (stringPtr < stringEnd) {
        int whatStart = 0;
        // skip first whitespaces of "what"
        while ((whatStart < whatLen) && (whatPtr[whatStart]==0)) 
            whatStart++;
        // search for first "what" substring in rest of string
        if ((stringPtr = strstr(stringPtr, whatPtr + whatStart))) {
            int stringStart = 0;
            stringPtr += strlen(whatPtr+whatStart);
            whatStart += strlen(whatPtr+whatStart);
            while (whatStart <= whatLen) {
                // find next "what" substring
                while ((whatStart < whatLen) && (whatPtr[whatStart]==0)) 
                    whatStart++;
                // "what" string complete ?
                if (whatStart >= whatLen) {
                    free(whatPtr);
                    return true;
                }
                // skip whitespaces in string
                while (isspace(stringPtr[stringStart])) 
                    stringStart++;
                // compare next substring
                if (stringncmp(stringPtr+stringStart, whatPtr+whatStart)==0) {
                    stringStart += strlen(whatPtr+whatStart);
                    whatStart   += strlen(whatPtr+whatStart);
                } else
                   break;
            }
        } else {
            break;
        }
    } 
    free(whatPtr);
    return false;
}

bool
NodeScript::hasInitialize(char *string)
{
    const char *urlString = NULL;
    if (string == NULL) {
        urlString = getEcmaScriptUrl();
        if (urlString == NULL)
            return false;
    } else
        urlString = string;
    MyString cmptext = " function initialize (";
    return strwhitespacestr(urlString, cmptext);
}

bool
NodeScript::hasShutdown(char *string)
{
    const char *urlString = NULL;
    if (string == NULL) {
        urlString =  getEcmaScriptUrl();
        if (urlString == NULL)
            return false;
    } else
        urlString = string;
    MyString cmptext = " function shutdown (";
    return strwhitespacestr(urlString, cmptext);
}

bool           
NodeScript::hasEventsProcessed(char* string)
{
    const char *urlString = NULL;
    if (string == NULL) {
        urlString =  getEcmaScriptUrl();
        if (urlString == NULL)
            return false;
    } else
        urlString = string;
    MyString cmptext = " function eventsProcessed (";
    return strwhitespacestr(urlString, cmptext);
}

const char *
NodeScript::getEcmaScriptUrl(void) 
{
    MFString *mfurl = url();
    bool hasUrl = true;
    if (mfurl == NULL)
        hasUrl = false;
    if (mfurl->getSize() == 0)
        hasUrl = false;
    if (hasUrl) {
        for (int i = 0; i < mfurl->getSize(); i++)
             if (isSortOfEcmascript(mfurl->getValue(i)))
                 return mfurl->getValue(i);
    } 
    return NULL;
}

const char *
NodeScript::getUrlWithoutHeader(const char *header)
{
    MFString *mfurl = (MFString *) url();
    bool hasUrl = true;  
    if (mfurl == NULL)
        hasUrl = false;
    if (mfurl->getSize() == 0)
        hasUrl = false;
    if (hasUrl) {
        for (int i = 0; i < mfurl->getSize(); i++)   
             if (strncasecmp(mfurl->getValue(i),header,strlen(header))==0)
                 return mfurl->getValue(i)+strlen(header);
    }
    return NULL;
}

void
NodeScript::buildInterfaceData(void) 
{     
    getProto()->buildInterfaceData(getScene()->isX3d());
    if (hasInitialize() || m_wantInitialize)
        getProto()->getInterfaceData()->append(new InterfaceData(EL_INITIALIZE, 
                                                                 -1));
    if (hasEventsProcessed() || m_wantEventsProcessed)
        getProto()->getInterfaceData()->append(
            new InterfaceData(EL_EVENTS_PROCESSED, -1));
    if (hasShutdown() || m_wantShutdown)
        getProto()->getInterfaceData()->append(new InterfaceData(EL_SHUTDOWN, 
                                                                 -1));
}

// in case the c/c++/java export of a Script node, the following headers
// are searched in the Script node and the matching URL is written unchanged
// into the ProcessEventCallback function
//
// "c: /* c code */"
// "c++: /* c++ code */"
// "java: /* java code */"
//
// if no matching URL is found, the Script data is written as usual field data

int             
NodeScript::writeCDynamicNodeCallback(int f, int languageFlag)
{
    const char *source = NULL;
    if (languageFlag & C_SOURCE) {
        source = getUrlWithoutHeader(TheApp->getScriptHeaderC());
        RET_ONERROR( mywritestr(f, "int ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, m_proto->getCName(true)) )
        RET_ONERROR( mywritestr(f, "SendEventsCallback(") )
        RET_ONERROR( mywritef(f, "%sNode *node, const char *event,", 
                                 TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "void* data)\n") )
        RET_ONERROR( mywritestr(f, "{\n") )
        if (source) {
            MyString string = "";
            string += source;
            string.gsub("\\\"","\""); 
            RET_ONERROR( mywritestr(f, (const char *)string) )
        }
        RET_ONERROR( mywritestr(f, "    return 1;\n") )
        RET_ONERROR( mywritestr(f, "}\n\n") )
    } else if (languageFlag & CC_SOURCE) {
        source = getUrlWithoutHeader(TheApp->getScriptHeaderCpp());
        RET_ONERROR( mywritestr(f, "bool ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, m_proto->getCName(true)) )
        RET_ONERROR( mywritestr(f, "SendEventsCallback(") )
        RET_ONERROR( mywritef(f, "%sNode *node, const char *event,",
                                 TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "void* data)\n") )
        RET_ONERROR( mywritestr(f, "{\n") )
        if (source) {
            MyString string = "";
            string += source;
            string.gsub("\\\"","\""); 
            RET_ONERROR( mywritestr(f, (const char *)string) )
        }
        RET_ONERROR( mywritestr(f, "    return true;\n") )
        RET_ONERROR( mywritestr(f, "}\n\n") )
    }
    if (languageFlag & JAVA_SOURCE) {
        source = getUrlWithoutHeader(TheApp->getScriptHeaderJava());
        RET_ONERROR( mywritestr(f, "class ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, m_proto->getCName(true)) )
        RET_ONERROR( mywritestr(f, "SendEventsCallback extends ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, m_proto->getCName(true)) )
        RET_ONERROR( mywritestr(f, "ProcessEventCallback {\n") )
        RET_ONERROR( mywritestr(f, "    public boolean processEvent(") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "Node node, String event) {\n") )
        if (source) {
            MyString string = "";
            string += source;
            string.gsub("\\\"","\""); 
            RET_ONERROR( mywritestr(f, (const char *)string) )
        }
        RET_ONERROR( mywritestr(f, "    return true;\n") )
        RET_ONERROR( mywritestr(f, "    }\n\n") )
        RET_ONERROR( mywritestr(f, "}\n\n") )
    }
    return 0;
}

   
int
NodeScript::writeXml(int filedes, int indent, int containerField)
{
     if (m_scene->getWriteFlags() & X3DOM) {
         for (int i = 0; i < url()->getSize(); i++)
             if (isX3domscript(url()->getValue(i))) {
                 RET_ONERROR( mywritestr(filedes, "<script>") )
                 const char *string = url()->getValue(i);
                 while (isspace(string[0]))
                     string++;
                 string +=strlen("x3domscript:");
                 RET_ONERROR( mywritestr(filedes, string) )
                 RET_ONERROR( mywritestr(filedes, "</script>\n") )
             }
     } else
        return DynamicFieldsNode::writeXml(filedes, indent, containerField);
     return 0;
}

