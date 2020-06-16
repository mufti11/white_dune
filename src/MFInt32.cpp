/*
 * MFInt32.cpp
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

#include "MFInt32.h"
#include "SFInt32.h"
#include "DuneApp.h"

MFInt32::MFInt32()
{
    m_value.resize(0);
    m_lastMinus1 = false;
}

MFInt32::MFInt32(int *values, int len)
{
    m_value.setData(values, len);
    m_lastMinus1 = false;
}

MFInt32::MFInt32(MFInt32 *value)
{
    m_value.setData(value->getValues(), value->getSize());
    m_lastMinus1 = false;
}

MFInt32::MFInt32(const int value)
{
    m_value.resize(0);
    m_value[0] = value;
    m_lastMinus1 = false;
}

MFInt32::~MFInt32()
{
    if (m_value.size() > 0)
        m_value.resize(0);
}

MyString    
MFInt32::getString(int index, int stride) const
{
    MyString ret = "";
    char buffer[256];
    mysnprintf(buffer, 255, "%d", m_value[index * getStride() + stride]);
    ret += buffer;
    return ret;
}

FieldValue *
MFInt32::copy()
{
    int *value = new int[m_value.size()];
    for (long i = 0; i < m_value.size(); i++)
        value[i] = m_value[i];
    return new MFInt32(value, m_value.size());
}

int MFInt32::writeData(int f, int i) const
{
    bool isDynamicFieldsNode = TheApp->getDynamicFieldsNode();
    int ret = 0;
    if (i != -1 || (!m_lastMinus1) || isDynamicFieldsNode)
        ret = mywritef(f, "%d", m_value[i]);
    if (i == -1)
        m_lastMinus1 = true;
    return ret;
}

int MFInt32::write(int f, int indent, bool writeBrackets, 
                   bool compactFormat) const
{
    bool isDynamicFieldsNode = TheApp->getDynamicFieldsNode();
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
    /* search for -1 values */
    bool noMinus1=true;
    {
    for (long i = 0; i < m_value.size(); i++)
        if (m_value[i]==-1)
            noMinus1=false;
    }
    /* if compactFormat, write all values in one line till -1 */
    bool indentflag = true;
    for (long i = 0; i < m_value.size(); i++) {
        if (indentflag) 
            RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
        if (m_value[i] != -1 || (!m_lastMinus1) || isDynamicFieldsNode)
            RET_ONERROR( mywritef(f, "%d ", m_value[i]) )
        if (m_value[i] == -1)
            m_lastMinus1 = true;
        else
            m_lastMinus1 = false;
        if (m_value[i] == -1)
            indentflag = noMinus1;
        if (!compactFormat)
            indentflag = true;
        if (m_value[i] == -1) 
            indentflag = true;
        if (indentflag || (i == m_value.size() - 1)) {
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

bool        
MFInt32::readLine(int index, char *line)
{
    if (sscanf(line, "%d", &m_value[index]) != 1)
        return false;
    return true;
}

bool
MFInt32::equals(const FieldValue *value) const
{
    if (value->getType() == MFINT32) {
        MFInt32 *v = (MFInt32 *) value;
        if (v->getSize() != (int)m_value.size()) return false;
        for (long i = 0; i < m_value.size(); i++)
            if (m_value[i] != v->getValue(i))
                return false;
        return true;
    }
    return false;
}

FieldValue *
MFInt32::getSFValue(int index) const
{
    return new SFInt32(m_value[index]);
}

void
MFInt32::setSFValue(int index, FieldValue *value)
{
    m_value[index] = ((SFInt32 *) value)->getValue();
}

void
MFInt32::setSFValue(int index, const int value)
{
    m_value[index] = value;
}


MyString
MFInt32::getEcmaScriptComment(MyString name, int flags) const
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // array ([0] [1] [2] ...) of signed 32 bit integers (... -1, 0, 1, 2 ..)\n";
        ret += indent;
        ret += "   // integer range: about (-2147483647-1) to 2147483647\n";
    }
    if (TheApp->GetEcmaScriptAddAvailableFunctions()) {
        ret += indent;
        ret += "// available functions:\n";
        if (flags != EL_EVENT_IN) {
            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = new MFInt32(int_i1, int_i2, ...);\n";
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
    if (TheApp->GetEcmaScriptAddExampleUsage()) {
        ret += indent;
        ret += "// example usage:\n";
        if (flags != EL_EVENT_IN) {
             ret += indent;
             ret += "   // ";
             ret += name;
             ret += " = new MFInt32(-1, 2, -3, 100);\n";

             ret += indent;
             ret += "   // ";
             ret += name;
             ret += "[0] = 42;\n";
        } 
        if (flags != EL_EVENT_OUT) {
             ret += indent;
             ret += "   // int_i =";
             ret += name;
             ret += "[0]\n";
        }
    }
    return ret;

}

void 
MFInt32::insertSFValue(int index, FieldValue *value)
{
    m_value.insert(((SFInt32 *)value)->getValue(), index);
}

void 
MFInt32::insertSFValue(int index, const int value)
{
    m_value.insert(value, index);
}

void 
MFInt32::appendSFValue(const int value)
{
    m_value.append(value);
}

FieldValue *
MFInt32::getRandom(Scene *scene,int nodeType)
{
    int size = INT_RAND();
    int *array = new int[size];
    for (int i = 0; i < size; i++)
        array[i] = INT_RAND();
    return new MFInt32(array, size);
}

