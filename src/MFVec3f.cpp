/*
 * MFVec3f.cpp
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

#include "MFVec3f.h"
#include "SFVec3f.h"
#include "MFVec2f.h"
#include "MFVec3d.h"
#include "Vec3f.h"
#include "DuneApp.h"

MFVec3f::MFVec3f(MFVec2f *mfVec2f)
{
    int size = mfVec2f->getSFSize();
    const float *vec2f = mfVec2f->getValues();
    m_value.resize(size * 3);
    for (int i = 0; i < size; i++) {
        m_value[i * 3 + 0] = vec2f[i * 2 + 0];
        m_value[i * 3 + 1] = vec2f[i * 2 + 1];
        m_value[i * 3 + 2] = 0.0f;
    }
}

FieldValue *MFVec3f::copy()
{ 
    const float *values = ((MFFloat *)MFFloat::copy())->getValues();
    return new MFVec3f((float *) values, getSize());
}

MFVec3d *MFVec3f::getMFVec3d(void)
{
    MFVec3d *values = new MFVec3d();
    for (int i = 0; i < m_value.size(); i += getStride())
        values->appendSFValue(m_value[i], 
                              m_value[i + 1],
                              m_value[i + 2]);  
    return values;
}

bool        
MFVec3f::readLine(int index, char *line)
{
    if (sscanf(line, "%f %f %f", &m_value[index * getStride() + 0],
                                 &m_value[index * getStride() + 1],
                                 &m_value[index * getStride() + 2]) != 3)
        return false;
    return true;
}

bool
MFVec3f::equals(const FieldValue *value) const
{
    return value->getType() == MFVEC3F && 
           MFFloat::equals((const MFFloat *) value);
}

FieldValue *
MFVec3f::getSFValue(int index) const
{
    return new SFVec3f(getValue(index));
}

void
MFVec3f::setSFValue(int index, FieldValue *value)
{
#ifdef DEBUG
    if (value->getType() != SFVEC3F) {
        assert(0);
        return;
    }
#endif

    setSFValue(index, ((SFVec3f *) value)->getValue());
}

void
MFVec3f::setSFValue(int index, const float *values)
{
    m_value[index * 3    ] = values[0];
    m_value[index * 3 + 1] = values[1];
    m_value[index * 3 + 2] = values[2];
}

void
MFVec3f::setSFValue(int index, float x, float y, float z)
{
    m_value[index * 3    ] = x;
    m_value[index * 3 + 1] = y;
    m_value[index * 3 + 2] = z;
}

void
MFVec3f::setVec(int index, Vec3f v)
{
    m_value[index * 3    ] = v.x;
    m_value[index * 3 + 1] = v.y;   
    m_value[index * 3 + 2] = v.z;   
}

Vec3f
MFVec3f::getVec(int index)
{
    return Vec3f(m_value[index * 3    ],
                 m_value[index * 3 + 1],
                 m_value[index * 3 + 2]);
}

MyString
MFVec3f::getEcmaScriptComment(MyString name, int flags) const
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
            ret += " = new MFVec3f(sfvec3f_v1, sfvec3f_v2, ...);\n";
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
             ret += " = new MFVec3f(SFVec3f(3, 5.2, 1), sfvec3f_v);\n";

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
MFVec3f::insertSFValue(int index, FieldValue *value)
{
    insertSFValue(index, ((SFVec3f *)value)->getValue()); 
}

void 
MFVec3f::insertSFValue(int index, const float *values)
{
    for (int i = 0; i < getStride(); i++)
        m_value.insert(values[i], index * getStride() + i);
}

void
MFVec3f::insertSFValue(int index, float x, float y, float z)
{
    m_value.insert(x, index * 3    );
    m_value.insert(y, index * 3 + 1);
    m_value.insert(z, index * 3 + 2);
}

Vec3f
MFVec3f::getMinBoundingBox(void)
{
    Vec3f retMin(0, 0, 0);
    const float *values = getValues();
    if (getSFSize() > 0) {
       retMin[0] = values[0];
       retMin[1] = values[1];
       retMin[2] = values[2];
       for (int j = 1; j < getSFSize(); j++)
           for (int i = 0; i < 3; i++)
               if (values[j * 3 + i] < retMin[i])
                   retMin[i] = values[j * 3 + i];
    }
    return retMin;
}

Vec3f
MFVec3f::getMaxBoundingBox(void)
{
    Vec3f retMax(0, 0, 0);
    const float *values = getValues();
    if (getSFSize() > 0) {
       retMax[0] = values[0];
       retMax[1] = values[1];
       retMax[2] = values[2];
       for (int j = 1; j < getSFSize(); j++)
           for (int i = 0; i < 3; i++)
               if (values[j * 3 + i] > retMax[i])
                   retMax[i] = values[j * 3 + i];
    }
    return retMax;
}

void
MFVec3f::flip(int index)
{
    for (int i = 0; i < getSFSize(); i++)
        m_value[i * 3 + index] *= -1.0;
}

void
MFVec3f::swap(int fromTo)
{
    for (int i = 0; i < getSFSize(); i++) {
        switch(fromTo) {
          case SWAP_XY:
             ::myswap(m_value[i * 3 + 0], m_value[i * 3 + 1]);
             break;
          case SWAP_XZ:
             ::myswap(m_value[i * 3 + 0], m_value[i * 3 + 2]);
             break;
          case SWAP_YZ:
             ::myswap(m_value[i * 3 + 1], m_value[i * 3 + 2]);
             break;
        }
    }
}

