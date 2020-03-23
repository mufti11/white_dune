/*
 * SFDouble.cpp
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

#include "SFDouble.h"
#include "SFString.h"

SFDouble::SFDouble(double value)
{
    m_value = value;
}

SFDouble::SFDouble(SFString *value)
{
    const char *string = value->getValue(); 
    sscanf(string, "%lf", &m_value);
}

MyString    
SFDouble::getString(int index, int stride) const
{
    MyString ret = "";
    char buffer[256];
    mysnprintf(buffer, 255, "%g", m_value);
    ret += buffer;
    return ret;
}

int SFDouble::writeData(int f, int i)
{
    return mywritef(f, "%g", m_value);
}

bool        
SFDouble::readLine(int index, char *line)
{
    if (sscanf(line, "%lf", &m_value) != 1)
        return false;
    return true;
}

bool
SFDouble::equals(const FieldValue *value) const
{
    return value->getType() == SFDOUBLE
            && ((SFDouble *) value)->getValue() == m_value;
}

bool
SFDouble::equalsAngle(const FieldValue *value, double angle) const
{
    return value->getType() == SFFLOAT
            && ((SFDouble *) value)->getValue() * angle == m_value;
}

void
SFDouble::clamp(const FieldValue *min, const FieldValue *max)
{
    if (min) {
        double fmin = ((SFDouble *) min)->getValue();
        if (m_value < fmin) m_value = fmin;
    }

    if (max) {
        double fmax = ((SFDouble *) max)->getValue();
        if (m_value > fmax) m_value = fmax;
    }
}

MyString
SFDouble::getEcmaScriptComment(MyString name, int flags) const
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // double-precision floating point number\n";

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
            ret += " = Math.abs(double_f);           // absolut value (forget sign)\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.round(double_f);         // round\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.floor(double_f);         // round down\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.ceil(double_f);          // round up\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.max(double_a, double_b);  // maximal value\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.min(double_a, double_b);  // minimal value\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.sqrt(double_f);          // square root function\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.pow(double_a, double_b);  // power function\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.exp(double_f);           // exponential function\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.log(double_f);           // natural logarithmic function\n";


            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.sin(double_f);           // sinus function\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.cos(double_f);           // cosinus function\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.tan(double_f);           // tangens function\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.acos(double_f);          // arcus cosinus function\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.asin(double_f);          // arcus sinus function\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.atan(double_f);          // arcus tangens function\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Math.atan2(double_x,double_y); // arcus tangens function\n";
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
             ret += "   // double_number = ";
             ret += name;
             ret += ";\n";

             ret += indent;
             ret += "   // double_number = Math.abs(";
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
