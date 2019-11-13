/*
 * MFColor.cpp
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

#include "MFColor.h"
#include "SFColor.h"
#include "DuneApp.h"

MFColor::MFColor() : MFFloat()
{
}

FieldValue *MFColor::copy()
{ 
    const float *values = ((MFFloat *)MFFloat::copy())->getValues();
    return new MFColor((float *) values, getSize());
}

bool        
MFColor::readLine(int index, char *line)
{
    if (sscanf(line, "%f %f %f", &m_value[index * getStride() + 0],
                                 &m_value[index * getStride() + 1],
                                 &m_value[index * getStride() + 2]) != 3)
        return false;
    for (int i = 0; i < getStride(); i++)
        if ((m_value[index * getStride() + i] < 0.0) ||
            (m_value[index * getStride() + i] > 1.0))
            return false;
    return true;
}

bool
MFColor::equals(const FieldValue *value) const
{
    return value->getType() == MFCOLOR && MFFloat::equals((const MFFloat *) value);
}

FieldValue *
MFColor::getSFValue(int index) const
{
    return new SFColor(m_value.getData() + index * 3);
} 

void
MFColor::setSFValue(int index, FieldValue *value)
{
    setSFValue(index, ((SFColor *) value)->getValue());
}

void
MFColor::setSFValue(int index, const float *values)
{
    for (int i = 0; i < getStride(); i++)
        m_value[index * getStride() + i] = values[i];
}


void
MFColor::setSFValue(int index, float r, float g, float b)
{
    m_value[index * 3    ] = r;
    m_value[index * 3 + 1] = g;
    m_value[index * 3 + 2] = b;
}

void 
MFColor::clamp(const FieldValue *min, const FieldValue *max)
{
    for (long i = 0; i < m_value.size(); i++) {
        if (m_value[i] < 0.0f) m_value[i] = 0.0f;
        if (m_value[i] > 1.0f) m_value[i] = 1.0f;
    }
}

MyString
MFColor::getEcmaScriptComment(MyString name, int flags) const
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // array ([0] [1] [2] [3] ...) of 3 floating point numbers, range: 0.0 to 1.0\n";
    }
    if (TheApp->GetEcmaScriptAddAllowedComponents()) {
        ret += indent;
        ret += "// allowed components:\n";

        ret += indent;
        ret += "   // red:   ";
        ret += name;
        ret += "[???].r or ";
        ret += name;
        ret += "[???][0]\n   ";

        ret += indent;
        ret += "// green: ";
        ret += name;
        ret += "[???].g or ";
        ret += name;
        ret += "[???][1]\n   ";

        ret += indent;
        ret += "// blue:  ";
        ret += name;
        ret += "[???].b or ";
        ret += name;
        ret += "[???][2]\n";
    }
    if (TheApp->GetEcmaScriptAddAvailableFunctions()) {
        ret += indent;
        ret += "// available functions:\n";
        if (flags != EL_EVENT_IN) {
            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = new MFColor(sfcolor_c1, sfcolor_c2, ...);\n";
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
             ret += " = new MFColor(new SFColor(0, 0.2, 1), new SFColor(0.4, 0.3, 0));\n";

             ret += indent;
             ret += "   // ";
             ret += name;
             ret += "[0].r = 0.5;\n";
        } 
        if (flags != EL_EVENT_OUT) {
             ret += indent;
             ret += "   // float_r = ";
             ret += name;
             ret += "[0].r;\n";
        }
    }
    return ret;
}

void 
MFColor::insertSFValue(int index, FieldValue *value)
{
    insertSFValue(index, ((SFColor *)value)->getValue()); 
}

void 
MFColor::insertSFValue(int index, const float *values)
{
    for (int i = 0; i < getStride(); i++)
        m_value.insert(values[i], index * getStride() + i);
}

void
MFColor::insertSFValue(int index, float r, float g, float b)
{
        m_value.insert(r, index * getStride());
        m_value.insert(g, index * getStride() + 1);
        m_value.insert(b, index * getStride() + 2);
}

