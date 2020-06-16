/*
 * SFVec4f.cpp
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

#include "swt.h"
#include "SFVec4f.h"
#include "SFFloat.h"

MyString    
SFVec4f::getString(int index, int stride) const
{
    MyString ret = "";
    char buffer[256];
    mysnprintf(buffer, 255, "%g", m_value[stride]);
    ret += buffer;
    return ret;
}

bool
SFVec4f::equals(const FieldValue *value) const
{
    if (value->getType() == SFVEC4F) {
        SFVec4f *v = (SFVec4f *) value;
        return v->getValue()[0] == m_value[0]
            && v->getValue()[1] == m_value[1]
            && v->getValue()[2] == m_value[2]
            && v->getValue()[3] == m_value[3];
    } else {
        return false;
    }
}

void
SFVec4f::clamp(const FieldValue *min, const FieldValue *max)
{
    if (min) {
        float fmin = ((SFFloat *) min)->getValue();
        for (int i = 0; i < 4; i++) {
            if (m_value[i] < fmin) m_value[i] = fmin;
        }
    }

    if (max) {
        float fmax = ((SFFloat *) max)->getValue();
        for (int i = 0; i < 4; i++) {
            if (m_value[i] > fmax) m_value[i] = fmax;
        }
    }
}

bool        
SFVec4f::readLine(int index, char *line)
{
    if (sscanf(line, "%f %f %f %f", m_value + 0, m_value + 1, m_value + 2,
               m_value + 3) != 4)
        return false;
    return true;
}

int 
SFVec4f::writeData(int f, int i) const
{
    return mywritef(f, "%g %g %g %g", m_value[0], m_value[1], m_value[2], 
                    m_value[3]);
}

int
SFVec4f::writeC(int filedes, const char* variableName, int languageFlag) const
{
    RET_ONERROR( mywritestr(filedes, "m_") )
    RET_ONERROR( mywritestr(filedes, variableName) )
    RET_ONERROR( mywritestr(filedes, "[") )
    if (languageFlag & C_SOURCE)
        RET_ONERROR( mywritestr(filedes, "4") )
    RET_ONERROR( mywritestr(filedes, "] = { ") )
    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritef(filedes, "%gf, %gf, %gf, %gf", 
                              m_value[0], m_value[1], m_value[2], m_value[3]))
    else
        RET_ONERROR( mywritef(filedes, "%g, %g, %g, %g", 
                              m_value[0], m_value[1], m_value[2], m_value[3]))
    RET_ONERROR( mywritestr(filedes, " };\n") )
    TheApp->incSelectionLinenumber();
    return 0;
}

int
SFVec4f::writeAc3d(int filedes, int indent) const
{
    RET_ONERROR( mywritef(filedes, "%f %f %f %f", 
                          m_value[0], m_value[1], m_value[2], m_value[3]) )
    RET_ONERROR( mywritestr(filedes, "\n") )
    return 0;
}

MyString
SFVec4f::getEcmaScriptComment(MyString name, int flags) const
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // 3D Vector and plane distance, 4 floating point numbers\n";
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

        ret += indent;
        ret += "   // ?(unknown): ";
        ret += name;
        ret += ".?(unknown) or ";
        ret += name;
        ret += "[4]\n";
    }
    if (TheApp->GetEcmaScriptAddAvailableFunctions()) {
        ret += indent;
        ret += "// available functions:\n";
        if (flags != EL_EVENT_IN) {
            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = new SFVec4f(float_x, float_y, float_z, float_);\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfvec4f_vec1.negate();\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfvec4f_vec1.normalize();\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfvec4f_vec1.add(sfvec4f_vec2);\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfvec4f_vec1.subtract(sfvec4f_vec2);\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfvec4f_vec1.cross(sfvec4f_vec2);\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfvec4f_vec.multiply(float_f);\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfvec4f_vec.divide(float_f);\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfrotaton_rot.getAxis();\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfrotaton_rot.multVec(sfvec4f_vec);\n";
        }
        if (flags != EL_EVENT_OUT) {
            ret += indent;
            ret += "   // float_f = ";
            ret += name;
            ret += ".length();\n";

            ret += indent;
            ret += "   // float_f = ";
            ret += name;
            ret += ".dot(sfvec4f_vec);\n";

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
             ret += " = new SFVec4f(3, 5.2, 1, 0);\n";

             ret += indent;
             ret += "   // ";
             ret += name;
             ret += ".z = 0.5;\n";
        } 
        if (flags != EL_EVENT_OUT) {
             ret += indent;
             ret += "   // float_z =";
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
