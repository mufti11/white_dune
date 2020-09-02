/*
 * MFString.cpp
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

#include "MFString.h"
#include "SFString.h"
#include "DuneApp.h"
#include "swt.h"

MFString::MFString()
{
}

MFString::MFString(StringArray *values)
{
    m_value.setData(values->getData(), values->size());
}

MFString::MFString(MyString value)
{
    m_value.resize(0);
    m_value.append(value);
}

MFString::MFString(const MFString &string)
{
    m_value.setData(string.getValues(), string.getSize());
}

MFString::~MFString()
{
    if (m_value.size() > 0)
        m_value.resize(0);
}

MyString    
MFString::getString(int index, int stride) const
{    
    MyString ret = "";
    ret += '"';
    ret += m_value[index]; 
    ret += '"';
    ret.gsub("\n", "\\n");
    ret.gsub("\r", "\\r");
    return ret;
   
}

FieldValue *
MFString::copy()
{
    StringArray *value= new StringArray();
    for (long i = 0;i < m_value.size(); i++)
        value->append(*new MyString(m_value[i]));
    return new MFString(value);
}

int MFString::writeDataC(int f, int i, int languageFlag) const
{
    MyString value = "";
    value += m_value[i];
    bool hasEndN = false;
    if (value.length() > 0  && value[value.length() - 1] == '\n')
        hasEndN = true;
    value.gsub("\n", " \\");
//    if (hasEndN)
//        value.setChar(value.length() - 1, '\n');
    value.gsub("\\", " \\\n");
    value.gsub("\r", "\\r");
    RET_ONERROR( mywritestr(f, "\"") )
    RET_ONERROR( mywritestr(f, value) )
    RET_ONERROR( mywritestr(f, "\"") )
    return(0);
}

int MFString::writeData(int f, int i) const
{
    RET_ONERROR( mywritestr(f, "\"") )
    RET_ONERROR( mywritestr(f, m_value[i]) )
    RET_ONERROR( mywritestr(f, "\"") )
    return(0);
}

int MFString::writeDataXml(int f, int i) const
{
    MyString string = "";
    string += m_value[i];
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

int MFString::write4FieldPipe(int filedes, int indent) const
{
    for (int i = 0; i < getSFSize(); i++) {
        RET_ONERROR( ((FieldValue *)this)->writeDequoted(filedes, m_value[i]) )
        RET_ONERROR( mywritestr(filedes, "\n") )
    }
    return(0);
}

int MFString::writeRaw(int f, int indent) const
{
    for (int i = 0; i < getSFSize(); i++) {
        RET_ONERROR( indentf(f, indent) )
        RET_ONERROR( mywritestr(f, m_value[i]) )
        if (i != 0 && i != getSFSize() - 1) {
            RET_ONERROR( mywritestr(f, "\"\n\"") )
            TheApp->incSelectionLinenumber();
        }
    }
    return(0);
}

int
MFString::writeCWonderlandArt(int filedes, const char* variableName,
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
        RET_ONERROR( TheApp->writeWonderlandModuleArtPath(filedes, m_value[i]) )
        if (i < (getSFSize() - 1))
            RET_ONERROR( mywritestr(filedes, ", ") );
    }
    RET_ONERROR( mywritestr(filedes, " };\n") );
    return 0;
}

const char *
MFString::getTypeC(int languageFlag) const
{ 
    if (languageFlag & JAVA_SOURCE)
        return "String";
    return "const char*";
}

bool        
MFString::readLine(int index, char *line)
{
    m_value.set(index, line);    
    return true;
}

bool
MFString::equals(const FieldValue *value) const
{
    if (value->getType() == MFSTRING) {
        MFString *v = (MFString *) value;
        if (v->getSize() != (int)m_value.size()) return false;
        for (long i = 0; i < m_value.size(); i++)
            if (v->getValue(i) != m_value[i]) return false;
        return true;
    }
    return false;
}

FieldValue *
MFString::getSFValue(int index) const
{ 
    return new SFString(m_value[index]);
}

void
MFString::setSFValue(int index, FieldValue *value)
{
    m_value.set(index, ((SFString *) value)->getValue());
}

void
MFString::setSFValue(int index, const char* value)
{
    m_value.set(index, value);
}


MyString
MFString::getEcmaScriptComment(MyString name, int flags) const
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // array ([0] [1] [2] ...) of 'string value's \n";
    }
    if (TheApp->GetEcmaScriptAddAvailableFunctions()) {
        ret += indent;
        ret += "// available functions:\n";
        if (flags != EL_EVENT_IN) {
            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = new MFString(string_s1, string_s2, ...);\n";
        }
        if (flags != EL_EVENT_OUT) {
            ret += indent;
            ret += "   // int_i = ";
            ret += name;
            ret += ".length();\n";

            ret += indent;
            ret += "   // string_str = ";
            ret += name;
            ret += ".toString();\n";
       }
    }
    if (TheApp->GetEcmaScriptAddBrowserObject()) {
        if (flags != EL_EVENT_OUT) {
            ret += indent;
            ret += "// related Browser Object functions:\n";

            ret += indent;
            ret += "   // Browser.createVrmlFromURL(";
            ret += name;
            ret += ", sfnode_n, string_event);\n";

            ret += indent;
            ret += "   // Browser.loadURL(";
            ret += name;
            ret += ", mfstring_parameter);\n";
        }
    }
    if (TheApp->GetEcmaScriptAddExampleUsage()) {
        ret += indent;
        ret += "// example usage:\n";
        if (flags != EL_EVENT_IN) {
            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = new MFString('hello','world');\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += "[0] = ' hello ';\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += "[1] = Browser.getName();\n";

        } 
        if (flags != EL_EVENT_OUT) {
            ret += indent;
            ret += "   // Browser.loadURL(";
            ret += name;
            ret += ", new MFString());\n";
        }
    }
    return ret;
}

void 
MFString::insertSFValue(int index, FieldValue *value)
{
    if (value->getType() != SFSTRING)
        assert(0);
    m_value.insert(((SFString *)value)->getValue(), index);
}

void 
MFString::insertSFValue(int index, const char* value)
{
    m_value.insert(value, index);
}

FieldValue *
MFString::getRandom(Scene *scene, int nodeType)
{
    StringArray *values = new StringArray();
    for (int i = 0; i < INT_RAND(); i++) {
        MyString newString = "";
        for (int j = 0; j < INT_RAND(); j++) {
            char newChar = (char)('0' + RAND() * (126 - '0'));
            if ((newChar != '\\') && (newChar != '"'))
                newString += newChar;
        }
        values->append(newString);
    }
    return new MFString(values);
}
