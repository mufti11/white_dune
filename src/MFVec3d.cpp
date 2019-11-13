/*
 * MFVec3d.cpp
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

#include "MFVec3d.h"
#include "SFVec3d.h"
#include "MFString.h"
#include "MFVec3f.h"
#include "Vec3d.h"
#include "DuneApp.h"

MFVec3d::MFVec3d(MFString *values) : MFDouble(values->getSize(), 3)
{
    for (int i = 0; i < values->getSize(); i++) {
        const char *string = values->getValue(i); 
        sscanf(string, "%lf %lf %lf", &m_value[i * getStride() + 0],
                                      &m_value[i * getStride() + 1],
                                      &m_value[i * getStride() + 2]);
    }        
}

FieldValue *MFVec3d::copy()
{ 
    const double *values = ((MFDouble *)MFDouble::copy())->getValues();
    return new MFVec3d((double *) values, getSize());
}

MFVec3f *MFVec3d::getMFVec3f(void)
{
    MFVec3f *values = new MFVec3f();
    for (long i = 0; i < m_value.size(); i += getStride())
        values->appendSFValue(m_value[i], 
                              m_value[i + 1],
                              m_value[i + 2]);  
    return values;
}


bool        
MFVec3d::readLine(int index, char *line)
{
    if (sscanf(line, "%lf %lf %lf", &m_value[index * getStride() + 0],
                                    &m_value[index * getStride() + 1],
                                    &m_value[index * getStride() + 2]) != 3)
        return false;
    return true;
}

bool
MFVec3d::equals(const FieldValue *value) const
{
    return value->getType() == MFVEC3D && 
           MFDouble::equals((const MFDouble *) value);
}

FieldValue *
MFVec3d::getSFValue(int index) const
{
    return new SFVec3d(getValue(index));
}

void
MFVec3d::setSFValue(int index, FieldValue *value)
{
#ifdef DEBUG
    if (value->getType() != SFVEC3D) {
        assert(0);
        return;
    }
#endif

    setSFValue(index, ((SFVec3d *) value)->getValue());
}

void
MFVec3d::setSFValue(int index, const double *values)
{
    m_value[index * 3    ] = values[0];
    m_value[index * 3 + 1] = values[1];
    m_value[index * 3 + 2] = values[2];
}

void
MFVec3d::setSFValue(int index, const char* values)
{
    sscanf(values, "%lf %lf %lf",
           &m_value[index], &m_value[index + 1], &m_value[index + 2]);
}

void
MFVec3d::setSFValue(int index, double x, double y, double z)
{
    m_value[index * 3    ] = x;
    m_value[index * 3 + 1] = y;
    m_value[index * 3 + 2] = z;
}

void
MFVec3d::setVec(int index, Vec3d v)
{
    m_value[index * 3    ] = v.x;
    m_value[index * 3 + 1] = v.y;   
    m_value[index * 3 + 2] = v.z;   
}

Vec3d
MFVec3d::getVec(int index)
{
    return Vec3d(m_value[index * 3    ],
                 m_value[index * 3 + 1],
                 m_value[index * 3 + 2]);
}

MyString
MFVec3d::getEcmaScriptComment(MyString name, int flags) const
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // array ([0] [1] [2] [3] ...) of 3D Vectors, 3 double floating point numbers\n";
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
            ret += " = new MFVec3d(sfvec3d_v1, sfvec3d_v2, ...);\n";
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
             ret += " = new MFVec3d(SFVec3d(3, 5.2, 1), sfvec3d_v);\n";

             ret += indent;
             ret += "   // ";
             ret += name;
             ret += "[0].z = 0.5;\n";
        } 
        if (flags != EL_EVENT_OUT) {
             ret += indent;
             ret += "   // double_z =";
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
MFVec3d::insertSFValue(int index, FieldValue *value)
{
    insertSFValue(index, ((SFVec3d *)value)->getValue()); 
}

void 
MFVec3d::insertSFValue(int index, const double *values)
{
    for (int i = 0; i < getStride(); i++)
        m_value.insert(values[i], index * getStride() + i);
}

void
MFVec3d::insertSFValue(int index, double x, double y, double z)
{
    m_value.insert(x, index * 3    );
    m_value.insert(y, index * 3 + 1);
    m_value.insert(z, index * 3 + 2);
}

Vec3d
MFVec3d::getMinBoundingBox(void)
{
    Vec3d retMin(0, 0, 0);
    const double *values = getValues();
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

Vec3d
MFVec3d::getMaxBoundingBox(void)
{
    Vec3d retMax(0, 0, 0);
    const double *values = getValues();
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
MFVec3d::flip(int index)
{
    for (int i = 0; i < getSFSize(); i++)
        m_value[i * 3 + index] *= -1.0;
}

void
MFVec3d::swap(int fromTo)
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

FieldValue *
MFVec3d::getRandom(Scene *scene, int nodeType)
{
    int size = INT_RAND() * getStride();
    double *array = new double[size];
    for (int i = 0; i < size; i++)
        array[i] = FLOAT_RAND();
    return new MFVec3d(array, size);
}

