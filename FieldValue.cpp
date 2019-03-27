/*
 * FieldValue.cpp
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

#include "FieldValue.h"
#include "SFMFTypes.h"
#include "Scene.h"
#include "Element.h"
#include "DuneApp.h"

FieldValue::FieldValue() {
    m_refs = 0;
    m_isDefaultValue = false;
}

int
FieldValue::write(int filedes, int indent) const
{ 
    RET_ONERROR( writeData(filedes, indent) )
    RET_ONERROR( mywritestr(filedes, "\n") )
    TheApp->incSelectionLinenumber();
    return 0;
}

int
FieldValue::writeXml(int filedes, int indent) const
{ 
    RET_ONERROR( mywritestr(filedes, "'") )
    RET_ONERROR( writeDataXml(filedes, 0) )
    RET_ONERROR( mywritestr(filedes, "'") )
    return 0;
}

int
FieldValue::writeDequoted(int filedes, const char *string)
{
    // remove if and last quotes 
    char *copy = (char *)malloc(strlen(string) + 1);
    char *copyptr = copy;
    for (unsigned int i = 0; i < strlen(string); i++) {
        bool copyChar = true;
        if (string[i] == '"') {
            if (i == 0)           
                copyChar = false;
            if (i == (strlen(string) - 1))
                copyChar = false;
        } 
        if (copyChar) {
           *copyptr = string[i];
           copyptr++;
        }    
    }
    *copyptr = (char)0;
    RET_ONERROR( mywritestr(filedes, copy) )
    free(copy);
    return 0;
}

int
FieldValue::writeC(int filedes, const char* variableName, 
                   int languageFlag) const
{
    RET_ONERROR( mywritestr(filedes, variableName) )
    RET_ONERROR( mywritestr(filedes, " = ") )
    RET_ONERROR( writeDataC(filedes, 0, languageFlag) )
    RET_ONERROR( mywritestr(filedes, ";\n") )
    return 0;
}

int         
FieldValue::writeCSendEventFunction(int filedes, int languageFlag)
{
    if (getType() == SFNODE) {
        SFNode *sfnode = (SFNode *)this;
        Node *node = sfnode->getValue();
        if (node != NULL)
            return 0;
    }
    if (isArrayInC()) {
        if (languageFlag & (JAVA_SOURCE))
            RET_ONERROR( mywritestr(filedes, "    ") )
        RET_ONERROR( mywritestr(filedes, "void ") )
        RET_ONERROR( mywritestr(filedes, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(filedes, getTypeName()) )
        RET_ONERROR( mywritestr(filedes, "SendEvent(") )
        RET_ONERROR( mywritestr(filedes, getTypeC(languageFlag)) )
        if (languageFlag & JAVA_SOURCE)
            RET_ONERROR( mywritestr(filedes, "[]") )
        else
            RET_ONERROR( mywritestr(filedes, "*") )
        RET_ONERROR( mywritestr(filedes, " target, ") )
        RET_ONERROR( mywritestr(filedes, getTypeC(languageFlag)) )
        if (languageFlag & JAVA_SOURCE)
            RET_ONERROR( mywritestr(filedes, "[]") )
        else
            RET_ONERROR( mywritestr(filedes, "*") )
        RET_ONERROR( mywritestr(filedes, " source) {\n") )
        if (languageFlag & JAVA_SOURCE)
            RET_ONERROR( mywritestr(filedes, "    ") )
        RET_ONERROR( mywritestr(filedes, "    int i;\n") )

        if (languageFlag & JAVA_SOURCE)
            RET_ONERROR( mywritestr(filedes, "    ") )
        RET_ONERROR( mywritestr(filedes, "    if ((target == ") )
        if (languageFlag & (JAVA_SOURCE))
            RET_ONERROR( mywritestr(filedes, "null") )
        else
            RET_ONERROR( mywritestr(filedes, "NULL") )
        RET_ONERROR( mywritestr(filedes, ") || (source == ") )
        if (languageFlag & (JAVA_SOURCE))
            RET_ONERROR( mywritestr(filedes, "null") )
        else
            RET_ONERROR( mywritestr(filedes, "NULL") )
        RET_ONERROR( mywritestr(filedes, ")) return;\n") )
   
        if (languageFlag & JAVA_SOURCE)
            RET_ONERROR( mywritestr(filedes, "    ") )
        RET_ONERROR( mywritestr(filedes, "    for (i = 0; i < ") )
        RET_ONERROR( mywritef(filedes, "%d", getNumbersPerType()) )
        RET_ONERROR( mywritestr(filedes, "; i++)\n") )
        if (languageFlag & JAVA_SOURCE)
            RET_ONERROR( mywritestr(filedes, "    ") )
        RET_ONERROR( mywritestr(filedes, "        target[i] = source[i];\n") )
        if (languageFlag & JAVA_SOURCE)
            RET_ONERROR( mywritestr(filedes, "    ") )
        RET_ONERROR( mywritestr(filedes, "}\n") )
    } else {
        if (languageFlag & JAVA_SOURCE)
            RET_ONERROR( mywritestr(filedes, "    ") )
        if (languageFlag & JAVA_SOURCE) {
            RET_ONERROR( mywritestr(filedes, getTypeC(languageFlag)) )
            RET_ONERROR( mywritestr(filedes, " ") )
        } else
            RET_ONERROR( mywritestr(filedes, "void ") )
        RET_ONERROR( mywritestr(filedes, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(filedes, getTypeName()) )
        RET_ONERROR( mywritestr(filedes, "SendEvent(") )
        if (languageFlag & (C_SOURCE | CC_SOURCE)) {    
            RET_ONERROR( mywritestr(filedes, getTypeC(languageFlag)) )
            RET_ONERROR( mywritestr(filedes, "*") )
            RET_ONERROR( mywritestr(filedes, " target, ") )
        }
        RET_ONERROR( mywritestr(filedes, getTypeC(languageFlag)) )
        RET_ONERROR( mywritestr(filedes, " source) {\n") )
        if (languageFlag & JAVA_SOURCE)
            RET_ONERROR( mywritestr(filedes, "    ") )
        RET_ONERROR( mywritestr(filedes, "    ") )
        if (languageFlag & JAVA_SOURCE)
            RET_ONERROR( mywritestr(filedes, "return source;\n") )
        else {
            RET_ONERROR( mywritestr(filedes, "*") )
            RET_ONERROR( mywritestr(filedes, "target = ") )
            RET_ONERROR( mywritestr(filedes, "source;\n") )
        }
        if (languageFlag & JAVA_SOURCE)
            RET_ONERROR( mywritestr(filedes, "    ") )
        RET_ONERROR( mywritestr(filedes, "}\n") )
    }
    return 0;
}

const char *
FieldValue::getDefaultC(int languageFlag) const
{ 
    return "NULL"; 
}

FieldValue *
FieldValue::addNode(Node * /* node */, int /* index */) const
{
    assert(0);
    return NULL;
}

FieldValue *
FieldValue::removeNode(Node * /* node */) const
{
    assert(0);
    return NULL;
}

const char *
FieldValue::getEcmaScriptIndent(int flags)
{
    if (flags == EL_EVENT_IN) {
        static const char* readIndent  = "      ";
        return readIndent;
    } else {
        static const char* writeIndent = "   ";
        return writeIndent;
    }
}

bool        
FieldValue::checkInput(char *line)
{
    if (getNumbersPerType() == 0)
        return true;

    int numbersPerType = 0;

    bool hasSign = false;
    bool hasNumber = false;
    bool hasComma = false;
    bool hasExponent = false;
    bool hasExponentSign = false;
    bool hasExponentComma = false;

    bool whitespace = true;
    for (unsigned int i = 0; i < strlen(line); i++) {
        if ((line[i] == ' ') || (line[i] == '\t'))
            whitespace = true;
        else {
            // check valid characters 
            // integer: [+-][0-9]* 
            // float:   [+-][0-9]*\.[0-9]*[eE][+-][0-9]*\.[0-9]*
            if (whitespace == true) {
                whitespace = false;
                numbersPerType++;
                hasNumber = false;
                hasSign = false;
                hasComma = false;
                hasExponent = false;
                hasExponentSign = false;
                hasExponentComma = false;
            }
            bool validNumber = false;
            if ((line[i] >= '0') && (line[i] <= 9)) {
                validNumber = true;
                hasNumber = true;
            }
            if ((line[i] == '+') && (line[i] == '-')) {
                validNumber = true;
                if (needCheckFloat() && hasExponent)
                    if (hasExponentSign)
                        return false;
                    else
                        hasExponentSign = true;
                else
                    if (hasSign)
                        return false;
                    else
                        hasSign = true;
            }
            if (needCheckFloat() && (line[i] == '.')) {
                validNumber = true;
                if (needCheckFloat() && hasExponent)
                    if (hasExponentComma)
                        return false;
                    else
                        hasExponentComma = true;
                else
                    if (hasComma)
                        return false;
                    else
                        hasComma = true;
                if ((line[i] == 'e') && (line[i] == 'E')) {
                    if (!needCheckFloat())
                        return false;
                    validNumber = true;
                    if (!hasNumber)
                        return false;
                    if (hasExponent)
                        return false;
                    else
                        hasExponent = true;
                }
            if (!validNumber)
                return false; 
            }
        }
    }
    if (numbersPerType != getNumbersPerType())
        return false;
    return true;   
}

FieldValue *
rewriteField(FieldValue *value, const char *oldBase, const char *newBase,
             int writeFlags)
{
    MyString r;

    switch (value->getType()) {
      case SFSTRING:
        r = rewriteURL(((SFString *) value)->getValue(), oldBase, newBase);
#ifdef _WIN32
        // handle invalid paths like c:/something
        if (r != NULL)
            r.gsubOnce("|",":"); 
#endif
        return new SFString(r);
      case MFSTRING:
      {
        StringArray *a = new StringArray();
        int n = ((MFString *) value)->getSize();
        for (int i = 0; i < n; i++) {
            bool flag = false;
            flag = !isSortOfEcmascript(((MFString *) value)->getValue(i));
            const char* url = (const char*) ((MFString *) value)->getValue(i);
            if (notURN(url) && flag) {
               (*a)[i] = rewriteURL(url, oldBase, newBase);
#ifdef _WIN32
               // handle invalid paths like c:/something
               if ((*a)[i] != NULL)
                   ((*a)[i]).gsubOnce("|",":"); 
#endif
               }
            else 
               (*a)[i] = url;
           if (notURN(url) && (writeFlags & CONVERT_FROM_XML) && !flag)
               ((*a)[i]).gsub("\"","\\\""); 
           else if (notURN(url) && (writeFlags & CONVERT_TO_XML) && !flag)
               ((*a)[i]).gsub("\\\"","\""); 
        }
        return new MFString(a);
      }
      default:
        assert(0);
        return NULL;
    }
}

int MFieldValue::write(int f, int indent, bool writeBrackets) const
{ 
    // SFImage is not a real MFType, it may not use brackets
    if (writeBrackets) {
        if (!TheApp->GetkrFormating()) {
            RET_ONERROR( mywritestr(f, "\n") )
            TheApp->incSelectionLinenumber();
            RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
        }
        RET_ONERROR( mywritestr(f, "[\n") )
        TheApp->incSelectionLinenumber();
    } else if (isMFNode()) {
        RET_ONERROR( mywritestr(f, "\n") )
        TheApp->incSelectionLinenumber();
    }
    if (getSFSize() == 1) {
        RET_ONERROR( writeData(f, 0) )
        RET_ONERROR( mywritestr(f, "\n") )
        TheApp->incSelectionLinenumber();
    } else {
        for (int i = 0; i < getSFSize(); i++) {
            RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
            RET_ONERROR( writeData(f, i) )
            RET_ONERROR( mywritestr(f, "\n") )
            TheApp->incSelectionLinenumber();
        }
    }
    // SFImage is not a real MFType, it may not use brackets
    if (writeBrackets) {
        if (!TheApp->GetkrFormating())
            RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
        else
            RET_ONERROR( indentf(f, indent) )
        RET_ONERROR( mywritestr(f, "]\n") )
        TheApp->incSelectionLinenumber();
    }
    return(0);
}

int MFieldValue::writeXml(int f, int indent) const
{
    RET_ONERROR( mywritestr(f, "'") )
    for (int i = 0; i < getSFSize(); i++) { 
        writeDataXml(f, i);
        if (i != (getSFSize() - 1))
            RET_ONERROR( mywritestr(f, "  ") )
    }
    RET_ONERROR( mywritestr(f, "'") )
    return(0);
}

bool                
FieldValue::writeType(int languageFlag) const
{
    if (languageFlag & JAVA_SOURCE)
        if (isMFieldValue() && (!isNode()))
            return false;
    return true;
}

int
MFieldValue::writeC(int filedes, const char* variableName, 
                    int languageFlag) const
{
    RET_ONERROR( mywritestr(filedes, "m_") );
    RET_ONERROR( mywritestr(filedes, variableName) );
    if (getSFSize() == 0) {
        RET_ONERROR( mywritestr(filedes, "[1];\n") );
        return 0;
    }
    RET_ONERROR( mywritestr(filedes, "[] = { ") );
    for (int i = 0; i < getSFSize(); i++) {
        writeDataC(filedes, i, languageFlag);
        if (i < (getSFSize() - 1))
            RET_ONERROR( mywritestr(filedes, ", ") );
    }
    RET_ONERROR( mywritestr(filedes, " };\n") );
    return 0;
}

/// ugly construct to fight against java's "code too large" problem 8-(
int
MFieldValue::writeJavaLongArray(int filedes, int languageFlag,
                                const char* variableName, 
                                int offset, int length, bool wonderlandArt,
                                const char *sceneUrl) 
                                const
{
    RET_ONERROR( mywritestr(filedes, "        ") )
    if (languageFlag & MANY_JAVA_CLASSES)
        RET_ONERROR( mywritestr(filedes, "    ") )
    const char *indexName = "i";
    if (strcmp(variableName, "i") == 0)
        indexName = "m_i";
    RET_ONERROR( mywritef(filedes, "int %s = %d;\n", indexName,
                                                     offset * getStride()) )
    for (int i = 0; ((i + offset) < getSFSize() && (i < length)); i++) {
        for (int j = 0; j < getStride(); j++) {
             RET_ONERROR( mywritestr(filedes, "        ") )
             if (languageFlag & MANY_JAVA_CLASSES)
                 RET_ONERROR( mywritestr(filedes, "    ") )
             RET_ONERROR( mywritestr(filedes, "v.") )
             RET_ONERROR( mywritestr(filedes, variableName) )
       
             RET_ONERROR( mywritef(filedes, "[%s++] = ", indexName) )
             MyString string = getString(i + offset, j);
             if (wonderlandArt) {
                 // remove quotes
                 char *unquoted_string = strdup(string);
                 char *ptr = unquoted_string + 1;
                 ptr[strlen(ptr) - 1] = 0;
                 URL url(sceneUrl, ptr);
                 const char *path = url.GetPath();
                 if (TheApp->writeWonderlandModuleArtPath(filedes, path) != 0) {
                     // try next url ?
                     if (i >= length - 1)
                         if (j >= (getStride() -1))
                             return -1; // end of loop
                 }
                 free(unquoted_string);
             } else
                 RET_ONERROR( mywritef(filedes, string) )
             RET_ONERROR( mywritestr(filedes, ";\n") )
        }
    }
    return 0;
}

int         
MFieldValue::writeCSendEventFunction(int filedes, int languageFlag)
{
    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritestr(filedes, "    ") )
    RET_ONERROR( mywritestr(filedes, "void ") )
    RET_ONERROR( mywritestr(filedes, TheApp->getCPrefix()) )
    RET_ONERROR( mywritestr(filedes, getTypeName()) )
    RET_ONERROR( mywritestr(filedes, "SendEvent(") )
    RET_ONERROR( mywritestr(filedes, getTypeC(languageFlag)) )
    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritestr(filedes, "[]") )
    else
        RET_ONERROR( mywritestr(filedes, "*") )
    RET_ONERROR( mywritestr(filedes, " target, ") )
    RET_ONERROR( mywritestr(filedes, getTypeC(languageFlag)) )
    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritestr(filedes, "[]") )
    else
        RET_ONERROR( mywritestr(filedes, "*") )
    RET_ONERROR( mywritestr(filedes, " source, int size) {\n") )
    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritestr(filedes, "    ") )
    RET_ONERROR( mywritestr(filedes, "    int i;\n") )

    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritestr(filedes, "    ") )
    RET_ONERROR( mywritestr(filedes, "    if ((target == ") )
    if (languageFlag & (JAVA_SOURCE))
        RET_ONERROR( mywritestr(filedes, "null") )
    else
        RET_ONERROR( mywritestr(filedes, "NULL") )
    RET_ONERROR( mywritestr(filedes, ") || (source == ") )
    if (languageFlag & (JAVA_SOURCE))
        RET_ONERROR( mywritestr(filedes, "null") )
    else
        RET_ONERROR( mywritestr(filedes, "NULL") )
    RET_ONERROR( mywritestr(filedes, ")) return;\n") )

    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritestr(filedes, "    ") )
    RET_ONERROR( mywritestr(filedes, "    for (i = 0; i < size; i++)\n") )
    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritestr(filedes, "    ") )
    RET_ONERROR( mywritestr(filedes, "        target[i] = source[i];\n") )
    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritestr(filedes, "    ") )
    RET_ONERROR( mywritestr(filedes, "}\n") )
    return 0;
}

void
MFieldValue::getDiff(IntArray *newIndices, IntArray *deleteIndices,
                     MFieldValue *old)
{
    int lastIndex = 0;
    for (int i = 0; i < getSFSize(); i++) {
        bool found = false;
        // continue search of next item after result of last search 
        for (int j = lastIndex + 1; j < old->getSFSize(); j++)
            if (getSFValue(i)->equals(old->getSFValue(j))) {
                found = true;
                lastIndex = j;
                break;
            }
        if (found == false)
            for (int j = 0; (j < (lastIndex + 1)) && (j < old->getSFSize()); j++)
                if (getSFValue(i)->equals(old->getSFValue(j))) {
                    found = true;
                    lastIndex = j;
                    break;
                }
        if (found == false)
            newIndices->append(i);
    }
    lastIndex = 0;
    for (int i = 0; i < old->getSFSize(); i++) {
        bool found = false;
        // continue search of next item after result of last search 
        for (int j = lastIndex + 1; j < getSFSize(); j++)
            if (old->getSFValue(i)->equals(getSFValue(j))) {
                found = true;
                lastIndex = j;
                break;
            }
        if (found == false)
            for (int j = 0; (j < (lastIndex + 1)) && (j < getSFSize()); j++)
                if (old->getSFValue(i)->equals(getSFValue(j))) {
                    found = true;
                    lastIndex = j;
                    break;
                }
        if (found == false)
            deleteIndices->append(i);
    }
}

int
FieldValue::writeAc3d(int filedes, int indent) const
{
    int appIndent = TheApp->GetIndent();
    TheApp->SetIndent(0);
    RET_ONERROR( write(filedes, indent) )
    TheApp->SetIndent(appIndent);
    return 0;
}

int
FieldValue::writeCattGeo(int filedes, int indent) const
{
    int appIndent = TheApp->GetIndent();
    TheApp->SetIndent(0);
    RET_ONERROR( write(filedes, indent) )
    TheApp->SetIndent(appIndent);
    return 0;
}




