/*
 * MFVec4f.cpp
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

#include "MFVec4f.h"
#include "SFVec4f.h"
#include "MFVec2f.h"
#include "Vec4f.h"
#include "DuneApp.h"
#include "ExternTheApp.h"

MFVec4f::MFVec4f(MFVec2f *mfVec2f)
{
    int size = mfVec2f->getSFSize();
    const float *vec2f = mfVec2f->getValues();
    m_value.resize(size * 4);
    for (int i = 0; i < size; i++) {
        m_value[i * 4 + 0] = vec2f[i * 2 + 0];
        m_value[i * 4 + 1] = vec2f[i * 2 + 1];
        m_value[i * 4 + 2] = 0.0f;
        m_value[i * 4 + 4] = 0.0f;
    }
}

FieldValue *MFVec4f::copy()
{ 
    const float *values = ((MFFloat *)MFFloat::copy())->getValues();
    return new MFVec4f((float *) values, getSize());
}

bool        
MFVec4f::readLine(int index, char *line)
{
    if (sscanf(line, "%f %f %f %f", &m_value[index * getStride() + 0],
                                    &m_value[index * getStride() + 1],
                                    &m_value[index * getStride() + 2],
                                    &m_value[index * getStride() + 3]) != 4)
        return false;
    return true;
}

bool
MFVec4f::equals(FieldValue *value)
{
    return value->getType() == MFVEC4F && equals((MFFloat *)value);
}

FieldValue *
MFVec4f::getSFValue(int index)
{
    return new SFVec4f(getValue(index));
}

void
MFVec4f::setSFValue(int index, FieldValue *value)
{
#ifdef DEBUG
    if (value->getType() != SFVEC4F) {
        assert(0);
        return;
    }
#endif

    setSFValue(index, ((SFVec4f *) value)->getValue());
}

void
MFVec4f::setSFValue(int index, float *values)
{
    m_value[index * 4    ] = values[0];
    m_value[index * 4 + 1] = values[1];
    m_value[index * 4 + 2] = values[2];
    m_value[index * 4 + 3] = values[3];
}

void
MFVec4f::setSFValue(int index, float x, float y, float z, float w)
{
    m_value[index * 4    ] = x;
    m_value[index * 4 + 1] = y;
    m_value[index * 4 + 2] = z;
    m_value[index * 4 + 3] = w;
}

void
MFVec4f::setVec(int index, Vec4f v)
{
    m_value[index * 4   ] = v.x;
    m_value[index * 4 + 1] = v.y;   
    m_value[index * 4 + 2] = v.z;   
    m_value[index * 4 + 3] = v.w;   
}

Vec4f
MFVec4f::getVec(int index)
{
    return Vec4f(m_value[index * 4   ],
                 m_value[index * 4 + 1],
                 m_value[index * 4 + 2],
                 m_value[index * 4 + 3]);
}

MyString
MFVec4f::getEcmaScriptComment(MyString name, int flags)
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // array ([0] [1] [2] [3] ...) of 3D Vectors, 3 floating point numbers\n";
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

        ret += indent;
        ret += "   // z: ";
        ret += name;
        ret += "[???].z or ";
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
            ret += " = new MFVec4f(sfvec4f_v1, sfvec4f_v2, ...);\n";
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
             ret += " = new MFVec4f(SFVec4f(3, 5.2, 1), sfvec4f_v);\n";

             ret += indent;
             ret += "   // ";
             ret += name;
             ret += "[0].z = 0.5;\n";
        } 
        if (flags != EL_EVENT_OUT) {
             ret += indent;
             ret += "   // float_z =";
             ret += name;
             ret += "[0].z;\n";
        }
        if (flags == EL_FIELD) {
             ret += indent;
             ret += "   // ";
             ret += name;
             ret += "[0] = ";
             ret += name;
             ret += "[1].normalize();\n";
        } 
    }
    return ret;
}

void 
MFVec4f::insertSFValue(int index, FieldValue *value)
{
    insertSFValue(index, ((SFVec4f *)value)->getValue()); 
}

void 
MFVec4f::insertSFValue(int index, float *values)
{
    for (int i = 0; i < getStride(); i++)
        m_value.insert(values[i], index * getStride() + i);
}

void
MFVec4f::insertSFValue(int index, float x, float y, float z, float w)
{
    m_value.insert(x, index * 4    );
    m_value.insert(y, index * 4 + 1);
    m_value.insert(z, index * 4 + 2);
    m_value.insert(w, index * 4 + 2);
}

Vec4f
MFVec4f::getMinBoundingBox(void)
{
    Vec4f retMin(0, 0, 0, 0);
    const float *values = getValues();
    if (getSFSize() > 0) {
       retMin[0] = values[0];
       retMin[1] = values[1];
       retMin[2] = values[2];
       retMin[3] = values[3];
       for (int j = 1; j < getSFSize(); j++)
           for (int i = 0; i < 4; i++)
               if (values[j * 4 + i] < retMin[i])
                   retMin[i] = values[j * 4 + i];
    }
    return retMin;
}

Vec4f
MFVec4f::getMaxBoundingBox(void)
{
    Vec4f retMax(0, 0, 0, 0);
    const float *values = getValues();
    if (getSFSize() > 0) {
       retMax[0] = values[0];
       retMax[1] = values[1];
       retMax[2] = values[2];
       retMax[3] = values[3];
       for (int j = 1; j < getSFSize(); j++)
           for (int i = 0; i < 4; i++)
               if (values[j * 4 + i] > retMax[i])
                   retMax[i] = values[j * 4 + i];
    }
    return retMax;
}

void
MFVec4f::flip(int index)
{
    for (int i = 0; i < getSFSize(); i++)
        m_value[i * 4 + index] *= -1.0;
}

void
MFVec4f::swap(int fromTo)
{
    for (int i = 0; i < getSFSize(); i++) {
        switch(fromTo) {
          case SWAP_XY:
             ::myswap(m_value[i * 4 + 0], m_value[i * 4 + 1]);
             break;
          case SWAP_XZ:
             ::myswap(m_value[i * 4 + 0], m_value[i * 4 + 2]);
             break;
          case SWAP_YZ:
             ::myswap(m_value[i * 4 + 1], m_value[i * 4 + 2]);
             break;
        }
    }
}

