/*
 * SFString.cpp
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

#include "SFString.h"
#include "DuneApp.h"

SFString::SFString(const MyString &value)
  : m_value(value)
{
}

SFString::SFString(const char *str)
  : m_value(str)
{  
}


MyString    
SFString::getString(int index, int stride) const
{    
    MyString ret = "";
    ret += '"';
    ret += m_value; 
    ret += '"';
    return ret;
   
}

bool        
SFString::readLine(int index, char *line)
{
    m_value = "";
    m_value += line;
    return true;
}

bool
SFString::equals(const FieldValue *value) const
{
    return value->getType() == SFSTRING &&
      !strcmp(((SFString *) value)->getValue(), (const char *) m_value);
}

int SFString::writeData(int f, int i) const
{
    RET_ONERROR( mywritestr(f, "\"") )
    RET_ONERROR( mywritestr(f, (const char *)m_value) )
    RET_ONERROR( mywritestr(f, "\"") )
    return(0);
}

int SFString::writeDataXml(int f, int i) const
{
    MyString string = "";
    string += m_value;
    string.gsub("&", "&amp;");
    string.gsub("\\\"", "\\&quot;");
    string.gsub("'", "&apos;");
    string.gsub(">", "&gt;");
    string.gsub("<", "&lt;");
    RET_ONERROR( mywritestr(f, "\"") )
    RET_ONERROR( mywritestr(f, string) )
    RET_ONERROR( mywritestr(f, "\"") )
    return(0);
}

int SFString::writeRaw(int f, int indent) const
{
    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, m_value) )
    RET_ONERROR( mywritestr(f, "\n") )
    TheApp->incSelectionLinenumber();
    return(0);
}

int SFString::write4FieldPipe(int filedes, int indent) const
{
    return ((FieldValue *)this)->writeDequoted(filedes, m_value);
}

const char *
SFString::getTypeC(int languageFlag) const
{ 
    if (languageFlag & JAVA_SOURCE)
        return "String";
    return "const char*";
}

int
SFString::writeCWonderlandArt(int filedes, const char* variableName,
                              int languageFlag) const
{
    RET_ONERROR( mywritestr(filedes, variableName) )
    RET_ONERROR( mywritestr(filedes, " = ") )
    RET_ONERROR( TheApp->writeWonderlandModuleArtPath(filedes, m_value) )
    RET_ONERROR( mywritestr(filedes, ";\n") )
    return 0;
}

MyString
SFString::getEcmaScriptComment(MyString name, int flags) const
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // 'string value' \n";
    }
    if (TheApp->GetEcmaScriptAddBrowserObject()) {
        ret += indent;
        ret += "// related Browser Object functions:\n";
        if (flags != EL_EVENT_IN) {
            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Browser.getName();\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Browser.getVersion();\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Browser.getWorldURL();\n";
        }
        if (flags != EL_EVENT_OUT) {
            ret += indent;
            ret += "   // Browser.setDescription(";
            ret += name;
            ret += ");\n";
        }
    }
    if (TheApp->GetEcmaScriptAddExampleUsage()) {
        if (flags != EL_EVENT_IN) {
            ret += indent;
            ret += "// example usage:\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = 'hello';\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Browser.getName();\n";
        } 
    }
    return ret;
}

FieldValue *
SFString::getRandom(Scene *scene, int nodeType)
{
    MyString newString = "";
    for (int j = 0; j < INT_RAND(); j++) {
        char newChar = (char)('0' + RAND() * (126 - '0'));
        if ((newChar != '\\') && (newChar != '"'))
            newString += newChar;
    }
    return new SFString(newString);
}


