/*
 * SFTime.cpp
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

#include "SFTime.h"

SFTime::SFTime(double value)
{
    m_value = value;
}

MyString    
SFTime::getString(int index, int stride)
{
    MyString ret = "";
    char buffer[256];
    mysnprintf(buffer, 255, "%g", m_value);
    ret += buffer;
    return ret;
}

int SFTime::writeData(int f, int i)
{
    return mywritef(f, "%g", m_value);
}

bool        
SFTime::readLine(int index, char *line)
{
    if (sscanf(line, "%lf", &m_value) != 1)
        return false;
    return true;
}

bool
SFTime::equals(FieldValue *value)
{
    return value->getType() == SFTIME
        && ((SFTime *) value)->getValue() == m_value;
}

void
SFTime::clamp(const FieldValue *min, const FieldValue *max)
{
    if (min) {
        double fmin = ((SFTime *) min)->getValue();
        if (m_value < fmin) m_value = fmin;
    }

    if (max) {
        double fmax = ((SFTime *) max)->getValue();
        if (m_value > fmax) m_value = fmax;
    }
}

MyString
SFTime::getEcmaScriptComment(MyString name, int flags)
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   //  double-precision floating number\n";
        ret += indent;
        ret += "   //  time since Jan 1, 1970, 00:00:00 GMT in seconds\n";
    }
    if (TheApp->GetEcmaScriptAddExampleUsage()) {
        ret += indent;
        ret += "// example usage:\n";
        if (flags != EL_EVENT_IN) {
             ret += indent;
             ret += "   // ";
             ret += name;
             ret += " = 0.0;\n";
        } 
        if (flags != EL_EVENT_OUT) {
             ret += indent;
             ret += "   // number = ";
             ret += name;
             ret += ";\n";
        }
    }
    return ret;
}
