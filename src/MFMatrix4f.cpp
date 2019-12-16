/*
 * MFMatrix4f.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2009 J. "MUFTI" Scheurich
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

#include "MFMatrix4f.h"
#include "SFMatrix4f.h"
#include "MFVec2f.h"
#include "DuneApp.h"
#include "ExternTheApp.h"

FieldValue *MFMatrix4f::copy()
{ 
    const float *values = ((MFFloat *)MFFloat::copy())->getValues();
    return new MFMatrix4f((float *) values, getSize());
}

bool        
MFMatrix4f::readLine(int index, char *line)
{
    if (sscanf(line, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", 
                     &m_value[index * getStride() + 0],
                     &m_value[index * getStride() + 1],
                     &m_value[index * getStride() + 2],
                     &m_value[index * getStride() + 3],
                     &m_value[index * getStride() + 4],
                     &m_value[index * getStride() + 5],
                     &m_value[index * getStride() + 6],
                     &m_value[index * getStride() + 7],
                     &m_value[index * getStride() + 8],
                     &m_value[index * getStride() + 9],
                     &m_value[index * getStride() + 10],
                     &m_value[index * getStride() + 11],
                     &m_value[index * getStride() + 12],
                     &m_value[index * getStride() + 13],
                     &m_value[index * getStride() + 14],
                     &m_value[index * getStride() + 15]) != 16)
        return false;
    return true;
}

bool
MFMatrix4f::equals(const FieldValue *value) const
{
    return value->getType() == MFMATRIX4F && equals(value);
}

FieldValue *
MFMatrix4f::getSFValue(int index)
{
    return new SFMatrix4f(getValue(index));
}

void
MFMatrix4f::setSFValue(int index, FieldValue *value)
{
#ifdef DEBUG
    if (value->getType() != SFMATRIX4F) {
        assert(0);
        return;
    }
#endif

    setSFValue(index, ((SFMatrix4f *) value)->getValue());
}

void
MFMatrix4f::setSFValue(int index, const float *values)
{
    for (int i = 0; i < 16; i++)
        m_value[index * 16 + i] = values[i];
}

MyString
MFMatrix4f::getEcmaScriptComment(MyString name, int flags)
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // array ([0] [1] [2] [3] ... ) of 4x4 Matices, each 16 floating point numbers\n";
    }
    if (TheApp->GetEcmaScriptAddAllowedComponents()) {
        ret += indent;
        ret += "// allowed components:\n";

        ret += indent;
        ret += "   // ";
        ret += name;
        ret += "[???][0]\n";

        ret += indent;
        ret += "   // ";
        ret += name;
        ret += "[???][1]\n";

        ret += indent;
        ret += "   // ";
        ret += name;
        ret += "[???][2]\n";

        ret += indent;
        ret += "   // ...\n";

        ret += indent;
        ret += "   // ";
        ret += name;
        ret += "[???][16]\n";

    }
    if (TheApp->GetEcmaScriptAddAvailableFunctions()) {
        ret += indent;
        ret += "// available functions:\n";
        if (flags != EL_EVENT_OUT) {
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
             ret += "[0][3] = 0.5;\n";
        } 
        if (flags != EL_EVENT_OUT) {
             ret += indent;
             ret += "   // float_f =";
             ret += name;
             ret += "[0].[15];\n";
        }
    }
    return ret;
}

void 
MFMatrix4f::insertSFValue(int index, FieldValue *value)
{
    insertSFValue(index, ((SFMatrix4f *)value)->getValue()); 
}

void 
MFMatrix4f::insertSFValue(int index, const float *values)
{
    for (int i = 0; i < getStride(); i++)
        m_value.insert(values[i], index * getStride() + i);
}


