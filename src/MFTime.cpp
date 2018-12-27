/*
 * MFTime.cpp
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

#include "MFTime.h"
#include "SFTime.h"
#include "DuneApp.h"

MFTime::MFTime()
{
    m_value.resize(0);
}

MFTime::MFTime(const MFTime &value)
{
    m_value.setData(value.getValues(), value.getSize());
}

MFTime::MFTime(double *values, int len)
{
    m_value.setData(values, len);
}

MFTime::MFTime(double value)
{
    m_value.resize(0);
    m_value[0] = value;
}

MFTime::MFTime(const double *value, int len)
{
    m_value.resize(0);    
    for (int i = 0;i < len; i++)
       m_value.append(value[i]);
}

MyString    
MFTime::getString(int index, int stride) const
{
    MyString ret = "";
    char buffer[256];
    mysnprintf(buffer, 255, "%g", m_value[index]);
    ret += buffer;
    return ret;
}

FieldValue *
MFTime::copy() 
{ 
    double *value = new double[m_value.size()];
    for (int i = 0;i < m_value.size(); i++)
        value[i] = m_value[i];
    return new MFTime(value, m_value.size());
}

bool        
MFTime::readLine(int index, char *line)
{
    if (sscanf(line, "%lf", &m_value[index]) != 1)
        return false;
    return true;
}

bool
MFTime::equals(const FieldValue *value) const
{
    return value->getType() == MFTIME && equals((const MFTime *) value);
}

bool
MFTime::equals(const MFTime *value) const
{
    if (m_value.size() == value->getSize()) {
        for (int i = 0; i < m_value.size(); i++)
            if (m_value[i] != value->getValue(i))
                return false;
        return true;
    }
    return false;
}

int MFTime::writeData(int f, int i) const
{
    return mywritef(f, "%g", m_value[i]);
}

void MFTime::clamp(const FieldValue *min, const FieldValue *max)
{
    if (min) {
        double dmin = ((SFTime *) min)->getValue();
        for (int i = 0; i < m_value.size(); i++) {
            if (m_value[i] < dmin) m_value[i] = dmin;
        }
    }
    if (max) {
        double dmax = ((SFTime *) max)->getValue();
        for (int i = 0; i < m_value.size(); i++) {
            if (m_value[i] > dmax) m_value[i] = dmax;
        }
    }
}

void MFTime::setSFValue(int index, FieldValue *value)
{
    m_value[index] = ((SFTime *) value)->getValue();
}

void  MFTime::setValue(int index, double value)
{
    m_value[index] = value;
}

MyString
MFTime::getEcmaScriptComment(MyString name, int flags) const
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   //  array ([0] [1] [2] ...) of double-precision floating numbers\n";
        ret += indent;
        ret += "   //  time since Jan 1, 1970, 00:00:00 GMT in seconds\n";
    }
    if (TheApp->GetEcmaScriptAddAvailableFunctions()) {
        ret += indent;
        ret += "// available functions:\n";
        if (flags != EL_EVENT_IN) {
            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = new MFTime(sftime_t1, sftime_t2, ...);\n";
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
             ret += " = new MFTime(0.2, 3.7, 8, -123.34);\n";

             ret += indent;
             ret += "   // ";
             ret += name;
             ret += "[0] = 0.0;\n";
        } 
        if (flags != EL_EVENT_OUT) {
             ret += indent;
             ret += "   // number = ";
             ret += name;
             ret += "[0];\n";
        }
    }
    return ret;
}

void 
MFTime::insertSFValue(int index, FieldValue *value)
{
    m_value.insert(((SFTime *)value)->getValue(), index);
}

void 
MFTime::insertSFValue(int index, const double value)
{
    m_value.insert(value, index);
}

FieldValue *
MFTime::getRandom(Scene *scene, int nodeType)
{
    int size = INT_RAND();
    double *array = new double[size];
    for (int i = 0; i < size; i++)
        array[i] = FLOAT_RAND();
    return new MFTime(array, size);
}

