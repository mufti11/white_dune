/*
 * MFBool.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2008 J. "MUFTI" Scheurich
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

#include "MFBool.h"
#include "SFBool.h"
#include "DuneApp.h"

MFBool::MFBool()
{
    m_value.resize(0);
}

MFBool::MFBool(bool *values, int len)
{
    m_value.setData(values, len);
}

MFBool::MFBool(MFBool *value)
{
    m_value.setData(value->getValues(), value->getSize());
}

MFBool::MFBool(const bool value)
{
    m_value.resize(0);
    m_value[0] = value;
}

MFBool::~MFBool()
{
    if (m_value.size() > 0)
        m_value.resize(0);
}

MyString    
MFBool::getString(int index, int stride) const
{
    MyString ret = "";
    ret +=  m_value[index * getStride() + stride] ? "true" : "false";
    return ret;
}

FieldValue *
MFBool::copy()
{
    bool *value = new bool[m_value.size()];
    for (int i = 0;i < m_value.size(); i++)
        value[i] = m_value[i];
    return new MFBool(value, m_value.size());
}

int MFBool::writeData(int f, int i) const
{
    return mywritestr(f, m_value[i] ? "TRUE" : "FALSE");
}

int MFBool::writeXml(int f, int indent) const
{
    RET_ONERROR( mywritestr(f, "'") )
    for (int i = 0; i < getSFSize(); i++) { 
        RET_ONERROR( mywritestr(f, m_value[i] ? "true" : "false") )
        if (i != (getSFSize() - 1))
            RET_ONERROR( mywritestr(f, "  ") )
    }
    RET_ONERROR( mywritestr(f, "'") )
    return(0);
}

int MFBool::writeDataC(int f, int i, int languageFlag) const
{
    if (languageFlag & C_SOURCE)
        RET_ONERROR( mywritestr(f, m_value[i] ? "-1" : "0") )
    else    
        RET_ONERROR( mywritestr(f, m_value[i] ? "true" : "false") )
   return 0;
}

const char *
MFBool::getTypeC(int languageFlag) const 
{ 
    if (languageFlag & C_SOURCE)
        return "short";
    if (languageFlag & JAVA_SOURCE)
        return "boolean";
    return "bool"; 
}

bool        
MFBool::readLine(int index, char *line)
{
    if (strncmp(line, "TRUE", strlen("TRUE")) == 0)
        m_value[index] = true;
    else
        m_value[index] = false;
    return true;
}

bool
MFBool::equals(const FieldValue *value) const
{
    if (value->getType() == MFBOOL) {
        MFBool *v = (MFBool *) value;
        if (v->getSize() != m_value.size()) return false;
        for (int i = 0; i < m_value.size(); i++)
            if (m_value[i] != v->getValue(i))
                return false;
        return true;
    }
    return false;
}

FieldValue *
MFBool::getSFValue(int index) const
{
    return new SFBool(m_value[index]);
}

void
MFBool::setSFValue(int index, FieldValue *value)
{
    m_value[index] = ((SFBool *) value)->getValue();
}

void
MFBool::setSFValue(int index, const int value)
{
    m_value[index] = value;
}


MyString
MFBool::getEcmaScriptComment(MyString name, int flags) const
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // array ([0] [1] [2] ...) of boolean values (true/false)";
    }
    if (TheApp->GetEcmaScriptAddAvailableFunctions()) {
        ret += indent;
        ret += "// available functions:\n";
        if (flags != EL_EVENT_IN) {
            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = new MFBool(bool_b1, bool_b2, ...);\n";
        }
        if (flags != EL_EVENT_OUT) {
            ret += indent;
            ret += "   // bool_b = ";
            ret += name;
            ret += ".length();\n";

            ret += indent;
            ret += "   // string_str = ";
            ret += name;
            ret += ".toString();\n";
       }
    }
    if (TheApp->GetEcmaScriptAddExampleUsage()) {
        ret += indent;
        ret += "// example usage:\n";
        if (flags != EL_EVENT_IN) {
             ret += indent;
             ret += "   // ";
             ret += name;
             ret += " = new MFBool(true, false, true, true);\n";

             ret += indent;
             ret += "   // ";
             ret += name;
             ret += "[0] = false;\n";
        } 
        if (flags != EL_EVENT_OUT) {
             ret += indent;
             ret += "   // bool_b =";
             ret += name;
             ret += "[0]\n";
        }
    }
    return ret;
}

void 
MFBool::insertSFValue(int index, FieldValue *value)
{
    m_value.insert(((SFBool *)value)->getValue(), index);
}

void 
MFBool::insertSFValue(int index, const bool value)
{
    m_value.insert(value, index);
}

FieldValue *
MFBool::getRandom(Scene *scene, int nodeType)
{
    int size = INT_RAND();
    bool *array = new bool[size];
    for (int i = 0; i < size; i++)
        array[i] = RAND() > 0.5;
    return new MFBool(array, size);
}

