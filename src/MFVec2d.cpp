/*
 * MFVec2d.cpp
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
#include "MFVec2d.h"
#include "MFFloat.h"
#include "SFVec2d.h"
#include "Vec2d.h"
#include "DuneApp.h"
#include "ExternTheApp.h"

MFVec2d::MFVec2d(MFString *values) : MFDouble(values->getSize(), 2)
{
    for (int i = 0; i < values->getSize(); i++) {
        const char *string = (const char *)values->getValue(i); 
        sscanf(string, "%lf %lf", &m_value[i * getStride() + 0],
                                  &m_value[i * getStride() + 1]);
    }        
}

MFVec2d::MFVec2d(MFVec2d *values) : MFDouble(values->getSize(), 2)
{
}

FieldValue *MFVec2d::copy()
{ 
    const double *values = ((MFDouble *)MFDouble::copy())->getValues();
    return new MFVec2d((double *) values, getSize());
}

bool        
MFVec2d::readLine(int index, char *line)
{
    if (sscanf(line, "%lf %lf", &m_value[index * getStride() + 0],
                                &m_value[index * getStride() + 1]) != 2)
        return false;
    return true;
}

bool
MFVec2d::equals(const FieldValue *value) const
{
    return value->getType() == MFVEC2D && equals((MFFloat *)value);
}

FieldValue *
MFVec2d::getSFValue(int index)
{
    return new SFVec2d(getValue(index));
}

void
MFVec2d::setSFValue(int index, FieldValue *value)
{
#ifdef DEBUG
    if (value->getType() != SFVEC2D) {
        assert(0);
        return;
    }
#endif
    setVec(index, ((SFVec2d *)value)->getSFValue(index));
}

void
MFVec2d::setSFValue(int index, double *values)
{
    m_value[2 * index    ] = values[0];
    m_value[2 * index + 1] = values[1];
}

void
MFVec2d::setSFValue(int index, const char* values)
{
    sscanf(values, "%lf %lf", &m_value[2 * index], &m_value[2 * index+ 1]);
}

void
MFVec2d::setSFValue(int index, double x, double y)
{
    m_value[2 * index   ] = x;
    m_value[2 * index + 1] = y;
}

void
MFVec2d::setVec(int index, Vec2d v)
{
    m_value[index * 2    ] = v.x;
    m_value[index * 2 + 1] = v.y;   
}

Vec2d
MFVec2d::getVec(int index)
{
    return Vec2d(m_value[index * 2    ],
                 m_value[index * 2 + 1]);
}

MyString
MFVec2d::getEcmaScriptComment(MyString name, int flags)
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // array ([0] [1] ...) of 2d Vectors, 2 double floating point numbers\n";
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
            ret += " = new MFVec2d(sfvec2d_v1, sfvec2d_v2, ...);\n";
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
             ret += " = new MFVec2d(SFVec2d(3, 5.2), sfvec2d_v);\n";

             ret += indent;
             ret += "   // ";
             ret += name;
             ret += "[0].y = 0.5;\n";
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
MFVec2d::insertSFValue(int index, FieldValue *value)
{
    insertSFValue(index, (double *)((SFVec2d *)value)->getValue()); 
}

void 
MFVec2d::insertSFValue(int index, double *values)
{
    for (int i = 0; i < getStride(); i++)
        m_value.insert(values[i], index * getStride() + i);
}

void
MFVec2d::insertSFValue(int index, double x, double y, double z)
{
    m_value.insert(x, index * 2    );
    m_value.insert(y, index * 2 + 1);
}

FieldValue *
MFVec2d::getRandom(Scene *scene, int nodeType)
{
    int size = INT_RAND() * getStride();
    double *array = new double[size];
    for (int i = 0; i < size; i++)
        array[i] = FLOAT_RAND();
    return new MFVec2d(array, size);
}

void
MFVec2d::flip(int index)
{
    if (index < 2)
        for (int i = 0; i < getSFSize(); i++)
             m_value[i * 2 + index] *= -1.0;
}

void
MFVec2d::swap(int fromTo)
{
    if (fromTo == SWAP_XY)
        for (int i = 0; i < getSFSize(); i++)
            ::myswap(m_value[i * 2 + 0], m_value[i * 2 + 1]);
}

