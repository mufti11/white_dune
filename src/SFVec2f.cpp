/*
 * SFVec2f.cpp
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
#include "DuneApp.h"
#include "ExternTheApp.h"
#include "SFDouble.h"

#include "SFVec2f.h"

SFVec2f::SFVec2f(float x, float y)
{
    m_value[0] = x; m_value[1] = y;
}

SFVec2f::SFVec2f(const float *values)
{
    m_value[0] = values[0]; m_value[1] = values[1];
}

SFVec2f::SFVec2f(void)
{
    m_value[0] = 0.0f; m_value[1] = 0.0f;
}

MyString    
SFVec2f::getString(int index, int stride)
{
    MyString ret = "";
    char buffer[256];
    mysnprintf(buffer, 255, "%g", m_value[stride]);
    ret += buffer;
    return ret;
}

bool        
SFVec2f::readLine(int index, char *line)
{
    if (sscanf(line, "%f %f", m_value + 0, m_value + 1) != 2)
        return false;
    return true;
}

void 
SFVec2f::setValue(float v1, float v2)
{
    m_value[0] = v1;
    m_value[1] = v2;
}


bool
SFVec2f::equals(const FieldValue *value) const
{
    return value->getType() == SFVEC2F
        && ((SFVec2f *) value)->getValue(0) == m_value[0]
        && ((SFVec2f *) value)->getValue(1) == m_value[1];
}

void
SFVec2f::clamp(const FieldValue *min, const FieldValue *max)
{
    if (min) {
        double fmin = ((SFDouble *) min)->getValue();
        for (int i = 0; i < 2; i++) {
            if (m_value[i] < fmin) m_value[i] = fmin;
        }
    }

    if (max) {
        double fmax = ((SFDouble *) max)->getValue();
        for (int i = 0; i < 2; i++) {
            if (m_value[i] > fmax) m_value[i] = fmax;
        }
    }
}

int 
SFVec2f::writeData(int f, int i)
{
    return mywritef(f, "%g %g", m_value[0], m_value[1]);
}

int
SFVec2f::writeC(int filedes, const char* variableName, int languageFlag)
{
    RET_ONERROR( mywritestr(filedes, "m_") )
    RET_ONERROR( mywritestr(filedes, variableName) )
    RET_ONERROR( mywritestr(filedes, "[") )
    if (languageFlag & (C_SOURCE | CC_SOURCE))
        RET_ONERROR( mywritestr(filedes, "2") )
    RET_ONERROR( mywritestr(filedes, "] = { ") )
    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritef(filedes, "%gf, %gf", m_value[0], m_value[1]))
    else
        RET_ONERROR( mywritef(filedes, "%g, %g", m_value[0], m_value[1]))
    RET_ONERROR( mywritestr(filedes, " };\n") )
    return 0;
}

MyString
SFVec2f::getEcmaScriptComment(MyString name, int flags)
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // 2D Vector, 2 floating point numbers\n";
    }
    if (TheApp->GetEcmaScriptAddAllowedComponents()) {
        ret += indent;
        ret += "// allowed components:\n";

        ret += indent;
        ret += "   // x: ";
        ret += name;
        ret += ".x or ";
        ret += name;
        ret += "[0]\n";

        ret += indent;
        ret += "   // y: ";
        ret += name;
        ret += ".y or ";
        ret += name;
        ret += "[1]\n";
    }
    if (TheApp->GetEcmaScriptAddAvailableFunctions()) {
        ret += indent;
        ret += "// available functions:\n";
        if (flags != EL_EVENT_IN) {
            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = new SFVec2f(float_x, float_y);\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfvec2f_vec1.add(sfvec2f_vec2);\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfvec2f_vec1.subtract(sfvec2f_vec2);\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfvec2f_vec.multiply(float_n);\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfvec2f_vec.divide(float_n);\n";
        }
        if (flags != EL_EVENT_OUT) {
            ret += indent;
            ret += "   // float_n = ";
            ret += name;
            ret += ".dot(sfvec2f_vec);\n";

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
             ret += " = new SFVec2f(3, 5.2);\n";

             ret += indent;
             ret += "   // ";
             ret += name;
             ret += ".y = 0.5;\n";
        } 
        if (flags != EL_EVENT_OUT) {
             ret += indent;
             ret += "   // float_x =";
             ret += name;
             ret += ".x;\n";

             ret += indent;
             ret += "   // float_l =";
             ret += name;
             ret += ".normalize();\n";
        }
        if (flags == EL_FIELD) {
             ret += indent;
             ret += "   // ";
             ret += name;
             ret += " = ";
             ret += name;
             ret += ".multiply(5.2);\n";
        } 
    }
    return ret;
}

Vec2f        
SFVec2f::getVec(void) const
{
    Vec2f v(m_value[0], m_value[1]);
    return v;
}

