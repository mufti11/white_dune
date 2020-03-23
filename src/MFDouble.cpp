/*
 * MFDouble.cpp
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

#include "MFDouble.h"
#include "DuneApp.h"

MFDouble::MFDouble(int len, int stride)
{
    m_value.resize(0);
    for (int i = 0; i < (len * stride); i++)
        m_value.append(0.0);
}

MFDouble::MFDouble(const double *values, int len, int /* stride */)
{
    m_value.setData(values, len);
}

MFDouble::MFDouble(const float *values, int len, int /* stride */)
{
    m_value.resize(0);
    for (int i = 0; i < len; i++)
        m_value.append((double)values[i]);
}

MFDouble::MFDouble(int stride)
{
    m_value.resize(0);
}

MFDouble::MFDouble(const MFDouble &value)
{
    m_value.setData(value.getValues(), value.getSize());
}

MFDouble::MFDouble(const double value)
{
    m_value.resize(0);
    m_value[0] = value;
}

MFDouble::~MFDouble()
{
    if (m_value.size() > 0)
        m_value.resize(0);
}

MyString    
MFDouble::getString(int index, int stride) const
{
    MyString ret = "";
    char buffer[256];
    mysnprintf(buffer, 255, "%g", m_value[index * getStride() + stride]);
    ret += buffer;
    return ret;
}

FieldValue *
MFDouble::copy()
{
    double *value = new double[m_value.size()];
    for (long i = 0; i < m_value.size(); i++)
        value[i] = m_value[i];
    return new MFDouble(value, m_value.size());
}

bool        
MFDouble::readLine(int index, char *line)
{
    if (sscanf(line, "%lf", &m_value[index]) != 1)
        return false;
    return true;
}

bool
MFDouble::equals(const FieldValue *value) const
{
    return value->getType() == MFDOUBLE && equals((const MFDouble *) value);
}

bool
MFDouble::equals(const MFDouble *value) const
{
    if ((int)m_value.size() == value->getSize()) {
        for (long i = 0; i < m_value.size(); i++)
            if (m_value[i] != value->getValue(i))
                return false;
        return true;
    }
    return false;
}

int MFDouble::writeData(int f, int i)
{
   RET_ONERROR( mywritef(f, "%g", m_value[i * getStride()]) )
   if (getStride() > 1)   
       for (int j = 1; j < getStride(); j++)
           RET_ONERROR( mywritef(f, " %g", m_value[i * getStride() + j]) )
   return 0;
}

int MFDouble::writeDataC(int f, int i, int languageFlag)
{
   RET_ONERROR( mywritef(f, "%f", m_value[i * getStride()]) )
   if (getStride() > 1)   
       for (int j = 1; j < getStride(); j++)
           RET_ONERROR( mywritef(f, ", %f", m_value[i * getStride() + j]) )
   return 0;
}

void MFDouble::clamp(const FieldValue *min, const FieldValue *max)
{
    if (min) {
        double fmin = ((SFDouble *) min)->getValue();
        for (long i = 0; i < m_value.size(); i ++) {
            if (m_value[i] < fmin) m_value[i] = fmin;
        }
    }
    if (max) {
        double fmax = ((SFDouble *) max)->getValue();
        for (long i = 0; i < m_value.size(); i ++) {
            if (m_value[i] > fmax) m_value[i] = fmax;
        }
    }
}

void 
MFDouble::setSFValue(int index, FieldValue *value)
{
    m_value[index] = ((SFDouble *) value)->getValue();
}

void 
MFDouble::setSFValue(int index, const double value)
{
    assert(index<getSFSize());
    m_value[index] = value;
}

FieldValue *
MFDouble::getSFValue(int index) const
{
    assert(index<getSFSize());
    return new SFDouble(m_value[index]); 
}

MyString
MFDouble::getEcmaScriptComment(MyString name, int flags) const
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // array ([0] [1] [2] ...) of double-precision floating point numbers\n";
    }
    if (TheApp->GetEcmaScriptAddAvailableFunctions()) {
        ret += indent;
        ret += "// available functions:\n";
        if (flags != EL_EVENT_IN) {
            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = new MFDouble(double_f1, double_f2, ...);\n";
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
             ret += " = new MFDouble(0.2, 3.7, 8, -123.34);\n";

             ret += indent;
             ret += "   // ";
             ret += name;
             ret += "[0] = 1 + Math.cos(Math.PI * 2) - Math.LN2;\n";
        } 
        if (flags != EL_EVENT_OUT) {
             ret += indent;
             ret += "   // double_number = ";
             ret += name;
             ret += "[0];\n";

             ret += indent;
             ret += "   // double_number = Math.abs(";
             ret += name;
             ret += "[3]);\n";
        }
        if (flags == EL_FIELD) {
             ret += indent;
             ret += "   // ";
             ret += name;
             ret += "[0] = Math.floor(";
             ret += name;
             ret += "[1]);\n";
        } 
    }
    return ret;
}

void 
MFDouble::insertSFValue(int index, FieldValue *value)
{
    m_value.insert(((SFDouble *)value)->getValue(), index);
}

void 
MFDouble::insertSFValue(int index, const double value)
{
    m_value.insert(value, index);
}

void        
MFDouble::removeSFValue(int index) 
{ 
    int stride = getStride();
    for (int i = 0; i < stride; i++)
        m_value.remove(index * stride + i); 
}

// search for biggest value
double
MFDouble::getMaxValue(void)
{
    double retMax = 0;
    const double *values = getValues();
    if (getSize() > 0) {
       retMax = values[0];
       for (int i = 1; i < getSize(); i++)
           if (values[i] > retMax)
               retMax = values[i];
    }
    return retMax;
}

// search for smallest value
double
MFDouble::getMinValue(void)
{
    double retMin = 0;
    const double *values = getValues();
    if (getSize() > 0) {
       retMin = values[0];
       for (int i = 1; i < getSize(); i++)
           if (values[i] < retMin)
               retMin = values[i];
    }
    return retMin;
}

FieldValue *
MFDouble::getRandom(Scene *scene, int nodeType)
{
    int size = INT_RAND() * getStride();
    double *array = new double[size];
    for (int i = 0; i < size; i++)
        array[i] = FLOAT_RAND();
    return new MFDouble(array, size, getStride());
}

