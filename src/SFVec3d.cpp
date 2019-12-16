/*
 * SFVec3d.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2006 J. "MUFTI" Scheurich
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

#include "SFVec3d.h"
#include "SFDouble.h"

SFVec3d::SFVec3d(SFString *value)
{
    const char *string = value->getValue(); 
    sscanf(string, "%lf %lf %lf", &m_value[0], &m_value[1], &m_value[2]);
}

MyString    
SFVec3d::getString(int index, int stride)
{
    MyString ret = "";
    char buffer[256];
    mysnprintf(buffer, 255, "%g", m_value[stride]);
    ret += buffer;
    return ret;
}


void
SFVec3d::clamp(const FieldValue *min, const FieldValue *max)
{
    if (min) {
        double fmin = ((SFDouble *) min)->getValue();
        for (int i = 0; i < 3; i++) {
            if (m_value[i] < fmin) m_value[i] = fmin;
        }
    }

    if (max) {
        double fmax = ((SFDouble *) max)->getValue();
        for (int i = 0; i < 3; i++) {
            if (m_value[i] > fmax) m_value[i] = fmax;
        }
    }
}

bool
SFVec3d::equals(const FieldValue *value) const
{
    if (value->getType() == SFVEC3D) {
        SFVec3d *v = (SFVec3d *) value;
        return v->getValue()[0] == m_value[0]
            && v->getValue()[1] == m_value[1]
            && v->getValue()[2] == m_value[2];
    } else {
        return false;
    }
}

bool        
SFVec3d::readLine(int index, char *line)
{
    if (sscanf(line, "%lf %lf %lf", m_value + 0, m_value + 1, m_value + 2) != 3)
        return false;
    return true;
}

int 
SFVec3d::writeData(int f, int i)
{
    return mywritef(f, "%lg %lg %lg", m_value[0], m_value[1], m_value[2]);
}

int
SFVec3d::writeC(int filedes, const char* variableName, int languageFlag)
{
    RET_ONERROR( mywritestr(filedes, "m_") )
    RET_ONERROR( mywritestr(filedes, variableName) )
    RET_ONERROR( mywritestr(filedes, "[") )
    if (languageFlag & (C_SOURCE | CC_SOURCE))
        RET_ONERROR( mywritestr(filedes, "3") )
    RET_ONERROR( mywritestr(filedes, "] = { ") )
    RET_ONERROR( mywritef(filedes, "%lg, %lg, %lg", m_value[0], m_value[1], 
                                                    m_value[2]))
    RET_ONERROR( mywritestr(filedes, " };\n") )
    TheApp->incSelectionLinenumber();
    return 0;
}

MyString
SFVec3d::getEcmaScriptComment(MyString name, int flags)
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // 3D Vector, 3 double size floating point numbers\n";
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

        ret += indent;
        ret += "   // z: ";
        ret += name;
        ret += ".z or ";
        ret += name;
        ret += "[2]\n";
    }
    if (TheApp->GetEcmaScriptAddAvailableFunctions()) {
        ret += indent;
        ret += "// available functions:\n";
        if (flags != EL_EVENT_IN) {
            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = new SFVec3d(double_x, double_y, double_z);\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfvec3d_vec1.negate();\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfvec3d_vec1.normalize();\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfvec3d_vec1.add(sfvec3d_vec2);\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfvec3d_vec1.subtract(sfvec3d_vec2);\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfvec3d_vec1.cross(sfvec3d_vec2);\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfvec3d_vec.multiply(double_f);\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfvec3d_vec.divide(double_f);\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfrotaton_rot.getAxis();\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfrotaton_rot.multVec(sfvec3d_vec);\n";
        }
        if (flags != EL_EVENT_OUT) {
            ret += indent;
            ret += "   // double_f = ";
            ret += name;
            ret += ".length();\n";

            ret += indent;
            ret += "   // double_f = ";
            ret += name;
            ret += ".dot(sfvec3d_vec);\n";

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
             ret += " = new SFVec3d(3, 5.2, 1);\n";

             ret += indent;
             ret += "   // ";
             ret += name;
             ret += ".z = 0.5;\n";
        } 
        if (flags != EL_EVENT_OUT) {
             ret += indent;
             ret += "   // double_z =";
             ret += name;
             ret += ".z;\n";
        }
        if (flags == EL_FIELD) {
             ret += indent;
             ret += "   // ";
             ret += name;
             ret += " = ";
             ret += name;
             ret += ".normalize();\n";
        } 
    }
    return ret;
}

