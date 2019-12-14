/*
 * MFRotation.cpp
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

#include "MFRotation.h"
#include "SFRotation.h"
#include "DuneApp.h"
#include "ExternTheApp.h"

MFRotation::MFRotation() : MFFloat()
{
}

FieldValue *MFRotation::copy()
{ 
    const float *values = ((MFFloat *)MFFloat::copy())->getValues();
    return new MFRotation((float *) values, getSize());
}

bool        
MFRotation::readLine(int index, char *line)
{
    if (sscanf(line, "%f %f %f %f", &m_value[index * getStride() + 0],
                                    &m_value[index * getStride() + 1],
                                    &m_value[index * getStride() + 2],
                                    &m_value[index * getStride() + 3]) != 4)
        return false;

    ((SFRotation *)getSFValue(index))->normalize();
    return true;
}

bool
MFRotation::equals(FieldValue *value)
{
    return value->getType() == MFROTATION && MFFloat::equals((MFFloat *) value);
}

void
MFRotation::fixAngle(double angleUnit)
{
    if (angleUnit != 0)
        for (int i = 0; i < getSFSize(); i++) {
            m_value[i * 4 + 3] /= angleUnit;
        }
}

FieldValue *
MFRotation::getSFValue(int index)
{
    return new SFRotation(getValue(index));
}

void
MFRotation::setSFValue(int index, FieldValue *value)
{
    setSFValue(index, ((SFRotation *) value)->getValue());
}

void
MFRotation::setSFValue(int index, const float *values)
{
    for (int i = 0; i < getStride(); i++)
        m_value[index * getStride() + i] = values[i];
}

MyString
MFRotation::getEcmaScriptComment(MyString name, int flags)
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // array ([0] [1] [2] [3] [4] ...) of 4 floating point numbers\n";
        ret += indent;
        ret += "   // 3 numbers: normalized axis vector";
        ret += " + 1 number: angle (0 to 2 * pi)\n";
    }
    if (TheApp->GetEcmaScriptAddAllowedComponents()) {
        ret += indent;
        ret += "// allowed components:\n";

        ret += indent;
        ret += "   // x:     ";
        ret += name;
        ret += "[???].x or ";
        ret += name;
        ret += "[???][0]\n";

        ret += indent;
        ret += "   // y:     ";
        ret += name;
        ret += "[???].y or ";
        ret += name;
        ret += "[???][1]\n";

        ret += indent;
        ret += "   // z:     ";
        ret += name;
        ret += "[???].z or ";
        ret += name;
        ret += "[???][2]\n";

        ret += indent;
        ret += "   // angle: ";
        ret += name;
        ret += "[???].a or ";
        ret += name;
        ret += "[???][3]\n";
    }
    if (TheApp->GetEcmaScriptAddAvailableFunctions()) {
        ret += indent;
        ret += "// available functions:\n";
        if (flags != EL_EVENT_IN) {
            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = new MFFloat(float_f1, float_f2, ...);\n";
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
             ret += " = new MFRotation(sfrotation_rot1, sfrotation_rot2);\n";

             ret += indent;
             ret += "   // ";
             ret += name;
             ret += "[0].a = Math.PI / 2.0;\n";
        } 
        if (flags != EL_EVENT_OUT) {
             ret += indent;
             ret += "   // float_x = ";
             ret += name;
             ret += "[0].x;\n";
        }
        if (flags == EL_FIELD) {
             ret += indent;
             ret += "   // ";
             ret += name;
             ret += "[0] = ";
             ret += name;
             ret += "[1].inverse();\n";
        }
    }
    return ret;
}

void 
MFRotation::insertSFValue(int index, FieldValue *value)
{
    insertSFValue(index, ((SFRotation *)value)->getValue());
}

void 
MFRotation::insertSFValue(int index, const float *values)
{
    for (int i = 0; i < getStride(); i++)
        m_value.insert(values[i], index * getStride() + i);
}

void
MFRotation::flip(int index)
{
    for (int i = 0; i < getSFSize(); i++) {
        m_value[i * 4 + index] *= -1.0;
        m_value[i * 4 + 3] *= -1.0;
    }
}

void
MFRotation::swap(int fromTo)
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
        m_value[i * 4 + 3] *= -1.0;
    }
}

