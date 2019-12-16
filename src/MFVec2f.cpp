/*
 * MFVec2f.cpp
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

#include "MFVec2f.h"
#include "SFVec2f.h"
#include "Vec2f.h"
#include "DuneApp.h"
#include "ExternTheApp.h"

MFVec2f::MFVec2f() : MFFloat()
{
}

FieldValue *MFVec2f::copy()
{ 
    const float *values = ((MFFloat *)MFFloat::copy())->getValues();
    return new MFVec2f((float *) values, getSize());
}

bool        
MFVec2f::readLine(int index, char *line)
{
    if (sscanf(line, "%f %f", &m_value[index * getStride() + 0],
                              &m_value[index * getStride() + 1]) != 2)
        return false;
    return true;
}

bool
MFVec2f::equals(const FieldValue *value) const
{
    return value->getType() == MFVEC2F && equals((MFFloat *)value);
}

FieldValue *
MFVec2f::getSFValue(int index)
{
    return new SFVec2f(getValue(index));
}

void
MFVec2f::setSFValue(int index, FieldValue *value)
{
#ifdef DEBUG
    if (value->getType() != SFVEC2F) {
        assert(0);
        return;
    }
#endif
    setSFValue(index, (float *)((SFVec2f *)value)->getValue());
}

void
MFVec2f::setSFValue(int index, float *values)
{
    m_value[index * 2   ] = values[0];
    m_value[index * 2 + 1] = values[1];
}

void
MFVec2f::setSFValue(int index, float x, float y)
{
    m_value[index * 2   ] = x;
    m_value[index * 2 + 1] = y;
}

void
MFVec2f::setVec(int index, Vec2f v)
{
    m_value[index * 2    ] = v.x;
    m_value[index * 2 + 1] = v.y;   
}
 
Vec2f
MFVec2f::getVec(int index)
{
    return Vec2f(m_value[index * 2], m_value[index * 2 + 1]);
}


MyString
MFVec2f::getEcmaScriptComment(MyString name, int flags)
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // array ([0] [1] [2] ...) of 2D Vectors, 2 floating point numbers\n";
    }
    if (TheApp->GetEcmaScriptAddAllowedComponents()) {
        ret += indent;
        ret += "// allowed components:\n";

        ret += indent;
        ret += "   // x: ";
        ret += name;
        ret += "[???].x or ";
        ret += name;
        ret += "[???][0]\n";

        ret += indent;
        ret += "   // y: ";
        ret += name;
        ret += "[???].y or ";
        ret += name;
        ret += "[???][1]\n";
    }
    if (TheApp->GetEcmaScriptAddAvailableFunctions()) {
        ret += indent;
        ret += "// available functions:\n";
        if (flags != EL_EVENT_IN) {
            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = new MFVec2f(sfvec2f_v1, sfvec2f_v2, ...);\n";
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
             ret += " = new MFVec2f(new SFVec2f(3, 5.2), sfvec2f_v1, sfvec2f_v2);\n";

             ret += indent;
             ret += "   // ";
             ret += name;
             ret += "[0].y = 0.5;\n";
        } 
        if (flags != EL_EVENT_OUT) {
             ret += indent;
             ret += "   // float_x =";
             ret += name;
             ret += "[0].x;\n";

             ret += indent;
             ret += "   // float_l =";
             ret += name;
             ret += "[0].normalize();\n";
        }
        if (flags == EL_FIELD) {
             ret += indent;
             ret += "   // ";
             ret += name;
             ret += "[0] = ";
             ret += name;
             ret += "[1].multiply(5.2);\n";
        } 
    }
    return ret;
}

void 
MFVec2f::insertSFValue(int index, FieldValue *value)
{
    insertSFValue(index, (SFVec2f *)value); 
}

void 
MFVec2f::insertSFValue(int index, float *values)
{
    for (int i = 0; i < getStride(); i++)
        m_value.insert(values[i], index * getStride() + i);
}

void
MFVec2f::insertSFValue(int index, float x, float y)
{
    m_value.insert(x, index * 2    );
    m_value.insert(y, index * 2 + 1);
}

void
MFVec2f::flip(int index)
{
    if (index < 2)
        for (int i = 0; i < getSFSize(); i++)
             m_value[i * 2 + index] *= -1.0;
}

void
MFVec2f::swap(int fromTo)
{
    if (fromTo == SWAP_XY)
        for (int i = 0; i < getSFSize(); i++)
            ::myswap(m_value[i * 2 + 0], m_value[i * 2 + 1]);
}



