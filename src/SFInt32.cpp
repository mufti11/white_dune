/*
 * SFInt32.cpp
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

#include "SFInt32.h"

SFInt32::SFInt32(int value)
{
    m_value = value;
}

MyString    
SFInt32::getString(int index, int stride)
{
    MyString ret = "";
    char buffer[256];
    mysnprintf(buffer, 255, "%d", m_value);
    ret += buffer;
    return ret;
}

int SFInt32::writeData(int f, int i)
{
    return mywritef(f, "%d", m_value);
}

bool        
SFInt32::readLine(int index, char *line)
{
    if (sscanf(line, "%d", &m_value) != 1)
        return false;
    return true;
}

bool
SFInt32::equals(const FieldValue *value) const
{
    return value->getType() == SFINT32
           && ((SFInt32 *) value)->getValue() == m_value;
}

void SFInt32::clamp(const FieldValue *min, const FieldValue *max)
{
    if (min) {
        int imin = ((SFInt32 *) min)->getValue();
        if (m_value < imin) m_value = imin;
    }

    if (max) {
        int imax = ((SFInt32 *) max)->getValue();
        if (m_value > imax) m_value = imax;
    }
}

MyString
SFInt32::getEcmaScriptComment(MyString name, int flags)
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // a signed 32 bit integer (... -1, 0, 1, 2 ..) number\n";
        ret += indent;
        ret += "   // range: about (-2147483647-1) to 2147483647\n";
    }
    if (TheApp->GetEcmaScriptAddExampleUsage()) {
        ret += indent;
        ret += "// example usage:\n";
        if (flags != EL_EVENT_IN) {
             ret += indent;
             ret += "   // ";
             ret += name;
             ret += " = 42;\n";
        } 
        if (flags != EL_EVENT_OUT) {
             ret += indent;
             ret += "   // int_i =";
             ret += name;
             ret += "\n";
        }
    }
    return ret;
}
