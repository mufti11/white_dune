/*
 * SFFloat.cpp
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

#include "SFFloat.h"

SFFloat::SFFloat(float value)
{
    m_value = value;
}

MyString    
SFFloat::getString(int index, int stride) const
{
    MyString ret = "";
    char buffer[256];
    mysnprintf(buffer, 255, "%g", m_value);
    ret += buffer;
    return ret;
}

int SFFloat::writeData(int f, int i) const
{
    return mywritef(f, "%g", m_value);
}

int SFFloat::writeDataC(int f, int i, int languageFlag) const
{
   if (languageFlag & JAVA_SOURCE)
       return mywritef(f, "%gf", m_value);
   return writeData(f, i);
}

bool        
SFFloat::readLine(int index, char *line)
{
    if (sscanf(line, "%f", &m_value) != 1)
        return false;
    return true;
}

bool
SFFloat::equals(const FieldValue *value) const
{
    return value->getType() == SFFLOAT
            && ((SFFloat *) value)->getValue() == m_value;
}

bool
SFFloat::equalsAngle(const FieldValue *value, double angle) const
{
    return value->getType() == SFFLOAT
            && (float)(((SFFloat *) value)->getValue() * angle == m_value);
}

void
SFFloat::clamp(const FieldValue *min, const FieldValue *max)
{
    if (min) {
        float fmin = ((SFFloat *) min)->getValue();
        if (m_value < fmin) m_value = fmin;
    }

    if (max) {
        float fmax = ((SFFloat *) max)->getValue();
        if (m_value > fmax) m_value = fmax;
    }
}

MyString
SFFloat::getEcmaScriptComment(MyString name, int flags) const
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // single-precision floating point number\n";

        if (TheApp->GetEcmaScriptAddMathObject()) {
            ret += indent;
            ret += "// related Math Object constants:\n";

            ret += indent;
            ret += "   // Math.PI Math.SQRT1_2 Math.SQRT_2 Math.E\n";
            ret += indent;
            ret += "   // Math.LN2 Math.LN10 Math.LOG2E Math.LOG10E\n";
        }
    }
    if (TheApp->GetEcmaScriptAddMathObject()) {
        if (flags != EL_EVENT_IN) {
            ret += indent;
            ret += "// related Math Object functions:\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.random();               // returns (quasi) random number\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.abs(float_f);           // absolut value (forget sign)\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.round(float_f);         // round\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.floor(float_f);         // round down\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.ceil(float_f);          // round up\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.max(float_a, float_b);  // maximal value\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.min(float_a, float_b);  // minimal value\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.sqrt(float_f);          // square root function\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.pow(float_a, float_b);  // power function\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.exp(float_f);           // exponential function\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.log(float_f);           // natural logarithmic function\n";


            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.sin(float_f);           // sinus function\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.cos(float_f);           // cosinus function\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.tan(float_f);           // tangens function\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.acos(float_f);          // arcus cosinus function\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.asin(float_f);          // arcus sinus function\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.atan(float_f);          // arcus tangens function\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.atan2(float_x,float_y); // arcus tangens function\n";
        }
    }
    if (TheApp->GetEcmaScriptAddBrowserObject()) {
        if (flags != EL_EVENT_IN) {
            ret += indent;
            ret += "// related Browser Object functions:\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Browser.getCurrentSpeed();\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Browser.getCurrentFrameRate();\n";
        }
    }
    if (TheApp->GetEcmaScriptAddExampleUsage()) {
        ret += indent;
        ret += "// example usage:\n";
        if (flags != EL_EVENT_IN) {
             ret += indent;
             ret += "   // ";
             ret += name;
             ret += " = 1 + Math.cos(Math.PI * 2) - Math.LN2;\n";
        } 
        if (flags != EL_EVENT_OUT) {
             ret += indent;
             ret += "   // float_number = ";
             ret += name;
             ret += ";\n";

             ret += indent;
             ret += "   // float_number = Math.abs(";
             ret += name;
             ret += ");\n";
        }
        if (flags == EL_FIELD) {
             ret += indent;
             ret += "   // ";
             ret += name;
             ret += " = Math.floor(";
             ret += name;
             ret += ");\n";
        } 
    }
    return ret;
}
