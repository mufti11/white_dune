/*
 * MFFloat.cpp
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

#include "MFFloat.h"
#include "DuneApp.h"

MFFloat::MFFloat()
{
    m_value.resize(0);
}

MFFloat::MFFloat(int size)
{
    m_value.resize(0);
    for (int i = 0; i < size; i++)
        m_value.append(0);

}

MFFloat::MFFloat(const float *values, int len, int /* stride */)
{
    m_value.setData(values, len);
}

MFFloat::MFFloat(const double *values, int len, int /* stride */)
{
    float* floats = new float[len];
    for (int i = 0; i < len; i++)
         floats[i] = values[i];
    m_value.setData(floats, len);    
}

MFFloat::MFFloat(const MFFloat *value)
{
    m_value.setData(value->getValues(), value->getSize());
}

MFFloat::MFFloat(const MFFloat &value)
{
    m_value.setData(value.getValues(), value.getSize());
}

MFFloat::MFFloat(const float value)
{
    m_value.resize(0);
    m_value[0] = value;
}

MFFloat::~MFFloat()
{
    if (m_value.size() > 0)
        m_value.resize(0);
}

MyString    
MFFloat::getString(int index, int stride) const
{
    MyString ret = "";
    char buffer[256];
    mysnprintf(buffer, 255, "%gf", m_value[index * getStride() + stride]);
    ret += buffer;
    return ret;
}

FieldValue *
MFFloat::copy()
{
    float *value = new float[m_value.size()];
    for (long i = 0; i < m_value.size(); i++)
        value[i] = m_value[i];
    return new MFFloat(value, m_value.size());
}

bool        
MFFloat::readLine(int index, char *line)
{
    if (sscanf(line, "%f", &m_value[index]) != 1)
        return false;
    return true;
}

bool
MFFloat::equals(const FieldValue *value) const
{
    return value->getType() == MFFLOAT && equals((const MFFloat *) value);
}

bool
MFFloat::equals(const MFFloat *value) const
{
    if ((int)m_value.size() == value->getSize()) {
        for (long i = 0; i < m_value.size(); i++)
            if (m_value[i] != value->getValue(i))
                return false;
        return true;
    }
    return false;
}

int MFFloat::writeData(int f, int i) const
{
   RET_ONERROR( mywritef(f, "%f", m_value[i * getStride()]) )
   if (getStride() > 1)   
       for (int j = 1; j < getStride(); j++)
           RET_ONERROR( mywritef(f, " %f", m_value[i * getStride() + j]) )
   return 0;
}

int MFFloat::writeDataC(int f, int i, int languageFlag) const
{
   bool java = (languageFlag & JAVA_SOURCE);
   RET_ONERROR( mywritef(f, java ? "%ff" : "%f", m_value[i * getStride()]) )
   if (getStride() > 1)   
       for (int j = 1; j < getStride(); j++)
           RET_ONERROR( mywritef(f, java ? " , %ff": ", %f", 
                                 m_value[i * getStride() + j]) )
   return 0;
}

void MFFloat::clamp(const FieldValue *min, const FieldValue *max)
{
    if (min) {
        float fmin = ((SFFloat *) min)->getValue();
        for (long i = 0; i < m_value.size(); i ++) {
            if (m_value[i] < fmin) m_value[i] = fmin;
        }
    }
    if (max) {
        float fmax = ((SFFloat *) max)->getValue();
        for (long i = 0; i < m_value.size(); i ++) {
            if (m_value[i] > fmax) m_value[i] = fmax;
        }
    }
}

void 
MFFloat::setSFValue(int index, FieldValue *value)
{
    m_value[index] = ((SFFloat *) value)->getValue();
}

void 
MFFloat::setSFValue(int index, const float value)
{
    assert(index<getSFSize());
    m_value[index] = value;
}

FieldValue *
MFFloat::getSFValue(int index) const
{
    assert(index<getSFSize());
    return new SFFloat(m_value[index]); 
}

void        
MFFloat::removeSFValue(long index) 
{ 
    int stride = getStride();
    for (int i = 0; i < stride; i++)
        m_value.remove(index * stride); 
}


void        
MFFloat::removeMFFloatSFValue(int index) 
{ 
    m_value.remove(index); 
}

MyString
MFFloat::getEcmaScriptComment(MyString name, int flags) const
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // array ([0] [1] [2] ...) of single-precision floating point numbers\n";
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
             ret += " = new MFFloat(0.2, 3.7, 8, -123.34);\n";

             ret += indent;
             ret += "   // ";
             ret += name;
             ret += "[0] = 1 + Math.cos(Math.PI * 2) - Math.LN2;\n";
        } 
        if (flags != EL_EVENT_OUT) {
             ret += indent;
             ret += "   // float_number = ";
             ret += name;
             ret += "[0];\n";

             ret += indent;
             ret += "   // float_number = Math.abs(";
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
MFFloat::insertSFValue(int index, FieldValue *value)
{
    m_value.insert(((SFFloat *)value)->getValue(), index);
}

void 
MFFloat::insertSFValue(int index, const float value)
{
    m_value.insert(value, index);
}

void 
MFFloat::appendSFValue(const float value)
{
    m_value.append(value);
}


// search for biggest value
float
MFFloat::getMaxValue(void)
{
    float retMax = 0;
    const float *values = getValues();
    if (getSize() > 0) {
       retMax = values[0];
       for (int i = 1; i < getSize(); i++)
           if (values[i] > retMax)
               retMax = values[i];
    }
    return retMax;
}

// search for smallest value
float
MFFloat::getMinValue(void)
{
    float retMin = 0;
    const float *values = getValues();
    if (getSize() > 0) {
       retMin = values[0];
       for (int i = 1; i < getSize(); i++)
           if (values[i] < retMin)
               retMin = values[i];
    }
    return retMin;
}

float *
MFFloat::getRandomFloats(int size)
{
    float *array = new float[size];
    for (int i = 0; i < size; i++)
        array[i] = FLOAT_RAND();
    return array;
}

FieldValue *
MFFloat::getRandom(Scene *scene, int nodeType)
{
    int size = INT_RAND() * getStride();
    return new MFFloat(getRandomFloats(size), size, getStride());
}

