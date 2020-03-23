/*
 * SFBool.cpp
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
#include "DuneApp.h"

#include "SFBool.h"

SFBool::SFBool(bool value)
{
    m_value = value;
}

MyString    
SFBool::getString(int index, int stride) const
{
    MyString ret = "";
    ret += m_value ? "true" : "false";
    return ret;
}

int SFBool::writeData(int f, int i)
{
    return mywritestr(f, m_value ? "TRUE" : "FALSE");
}

int SFBool::writeXml(int f, int i, int containerField, bool avoidUse) const
{
    return mywritestr(f, m_value ? "'true'" : "'false'");
}

int SFBool::writeC(int f, const char* variableName, int languageFlag) const
{
    RET_ONERROR( mywritestr(f, variableName) );
    RET_ONERROR( mywritestr(f, " = ") );
    if (languageFlag & C_SOURCE)
        RET_ONERROR( mywritestr(f, m_value ? "-1" : "0") )
    else    
        RET_ONERROR( mywritestr(f, m_value ? "true" : "false") )
    RET_ONERROR( mywritestr(f, ";\n") );
    return 0;
}

const char *
SFBool::getTypeC(int languageFlag) const 
{ 
    if (languageFlag & C_SOURCE)
        return "short";
    if (languageFlag & JAVA_SOURCE)
        return "boolean";
    return "bool"; 
}

const char *
SFBool::getDefaultC(int languageFlag) const 
{ 
    if (languageFlag & C_SOURCE)
        return "0";
    return "false"; 
}

/* not implemented yet
int SFBool::writeDune(int f, char* variableName) const
{
    RET_ONERROR( mywritestr(f, "SFBool sfBool_") )
    RET_ONERROR( mywritestr(f, variableName) )
    RET_ONERROR( mywritestr(f, " = new SFBool(") )
    RET_ONERROR( mywritestr(f, variableName) )
    RET_ONERROR( mywritestr(f, ");\n") )
    return 0;
}
*/

bool 
SFBool::checkInput(char *line)
{
    if (strcmp(line, "TRUE") == 0)
        return true;
    if (strcmp(line, "FALSE") == 0)
        return true;
    return false;
}

bool        
SFBool::readLine(int index, char *line)
{
    if (strcmp(line, "TRUE") == 0) {
        m_value = true;
        return true;
    }
    if (strcmp(line, "FALSE") == 0) {
        m_value = false;
        return true;
    }
    return false;
}

bool
SFBool::equals(const FieldValue *value) const
{
    return value->getType() == SFBOOL 
        && ((SFBool *) value)->getValue() == m_value;
}

MyString
SFBool::getEcmaScriptComment(MyString name, int flags) const
{    
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // true false\n";
    }
    if (TheApp->GetEcmaScriptAddExampleUsage()) {
        ret += indent;
        ret += "// example usage:\n";
        if (flags != EL_EVENT_IN) {
            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = true;\n";
        }
        if (flags != EL_EVENT_OUT) {
            ret += indent;
            ret += "   // if (";
            ret += name;
            ret += ")\n";

            ret += indent;
            ret += "   //    {\n";

            ret += indent;
            ret += "   //    }\n";
        }
        if (flags == EL_FIELD) {
             ret += indent;
             ret += "   // ";
             ret += name;
             ret += " = !";
             ret += name;
             ret += "; // logical NOT operation\n";
        } 
    }
    return ret;
}

FieldValue *
SFBool::getRandom(Scene *scene, int nodeType)
{
    return new SFBool(RAND() > 0.5);
}


