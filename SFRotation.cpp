/*
 * SFRotation.cpp
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
#include <math.h>
#include "stdafx.h"
#include "swt.h"
#include "DuneApp.h"

#include "Vec4f.h"
#include "SFRotation.h"
#include "EulerAngles.h"

MyString    
SFRotation::getString(int index, int stride) const
{
    MyString ret = "";
    char buffer[256];
    mysnprintf(buffer, 255, "%g", m_value[stride]);
    ret += buffer;
    return ret;
}

void SFRotation::generateQuaternion()
{ 
    m_quaternion=Quaternion(Vec3f(m_value[0], m_value[1], m_value[2]), m_value[3]);
}

SFRotation::SFRotation(float x, float y, float z, float w)
{
    m_value[0] = x; m_value[1] = y; m_value[2] = z; m_value[3] = w;
    this->normalize();
    generateQuaternion();
    m_eulerDirty = true;
}

SFRotation::SFRotation(const float *value)
{
    m_value[0] = value[0];
    m_value[1] = value[1];
    m_value[2] = value[2];
    m_value[3] = value[3];
    this->normalize();
    generateQuaternion();
    m_eulerDirty = true;
}

SFRotation::SFRotation(Vec3f angles, int mode)
{
    setEulerAngles(angles, mode);
}

SFRotation::SFRotation(void)
{
    m_value[0] = 0.0f;
    m_value[1] = 0.0f;
    m_value[2] = 1.0f;
    m_value[3] = 0.0f;
    this->normalize();
    generateQuaternion();
    m_eulerDirty = true;
}

int SFRotation::writeData(int f, int i) const
{
    return mywritef(f, "%g %g %g %g", 
                    m_value[0], m_value[1], m_value[2], m_value[3]);
}

int
SFRotation::writeAc3d(int filedes, int indent) const
{
    Matrix m;
    m[0] = m[5] = m[10] = 1.0f;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glRotatef(RAD2DEG(m_value[3]), m_value[0], m_value[1], m_value[2]);
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    glPopMatrix();

    int glError = glGetError();
    if (glError != GL_NO_ERROR)
        swDebugf("%s", gluErrorString(glError));

    RET_ONERROR( mywritef(filedes, "%f %f %f %f %f %f %f %f %f\n", 
                          m[0], m[1], m[2],
                          m[4], m[5], m[6], 
                          m[8], m[9], m[10]) )
    return 0;
}

int
SFRotation::writeC(int filedes, const char* variableName, 
                   int languageFlag) const
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

bool        
SFRotation::readLine(int index, char *line)
{
    if (sscanf(line, "%f %f %f %f", m_value + 0, m_value + 1, m_value + 2, 
                                    m_value + 3) != 4)
        return false;
    this->normalize();
    return true;
}

void
SFRotation::angleMult(float mult)
{
    generateQuaternion();
    m_quaternion.w *= mult;
    SFRotation rot(m_quaternion);
    const float *value = rot.getValue();
    m_value[0] = value[0];
    m_value[1] = value[1];
    m_value[2] = value[2];
    m_value[3] = value[3];
    generateQuaternion();
}

void SFRotation::setValue(float v1, float v2, float v3, float v4)
{
    m_value[0] = v1;
    m_value[1] = v2;
    m_value[2] = v3;
    m_value[3] = v4;
    generateQuaternion();
}

void SFRotation::setValue(int index, float value)
{
    assert(index >= 0 && index < 4);

    m_value[index] = value;
    generateQuaternion();
}

void SFRotation::normalize()
{
    float rlen = (float) sqrt(m_value[0] * m_value[0] + 
                              m_value[1] * m_value[1] +  
                              m_value[2] * m_value[2] );  
    if (rlen > 0.0f) { 
        rlen = 1.0f / rlen; 

        m_value[0] *= rlen;
        m_value[1] *= rlen;
        m_value[2] *= rlen;
    } else {
        // according to ISO 14997 5.8:
        // SFRotation: The first three values specify a normalized rotation 
        // axis vector about which the rotation takes place. 

        // 0 0 0 ? is not normalized, therefore it would be a invalid SFRotation
        m_value[0] = 0;
        m_value[1] = 0;
        m_value[2] = 1;
        m_value[3] = 0;
    }
}

SFRotation::SFRotation(const Quaternion &q)
{
    Quaternion q2=q;
    if (fabs(q2.w) > 1)
       q2.normalize();
    m_value[0] = q2.x;
    m_value[1] = q2.y;
    m_value[2] = q2.z;
    m_value[3] = 2.0f * (float) acos(q2.w);
    this->normalize();
    generateQuaternion();
    m_eulerDirty = true;
}

SFRotation::SFRotation(const Matrix &mat)
{
    HMatrix hmat;
    mat.getValue(hmat);
    EulerAngles a = Eul_FromHMatrix(hmat, EulOrdXYZs);

    setEulerAngles(Vec3f(a.x, a.y, a.z), EulOrdXYZs);
}

bool
SFRotation::equals(const FieldValue *value) const
{
    if (value->getType() == SFROTATION) {
        for (int i = 0; i < 4; i++)
            if (((SFRotation *) value)->getValue()[i] != m_value[i])
                return false;
        return true;
    }
    return false;
}

Vec3f SFRotation::getEulerAngles(int order)
{
    if (m_eulerDirty || m_eulerOrder != order) {
        Vec3f v(m_value[0], m_value[1], m_value[2]);
        Quaternion q(v, m_value[3]);
        EulerAngles a;
        a = Eul_FromQuat(q, order);
        m_euler = Vec3f(RAD2DEG(a.x), RAD2DEG(a.y), RAD2DEG(a.z));
        m_eulerOrder = order;
        m_eulerDirty = false;
    }
    return m_euler;
}

const Quaternion &
SFRotation::getQuat() const
{
    return m_quaternion;
}

void
SFRotation::setEulerAngles(Vec3f angles, int order)
{
    Quat q;
    EulerAngles a;

    a.x = DEG2RAD(angles.x);
    a.y = DEG2RAD(angles.y);
    a.z = DEG2RAD(angles.z);
    a.w = (float) order;

    q = Eul_ToQuat(a);
    m_value[0] = q.x;
    m_value[1] = q.y;
    m_value[2] = q.z;
    m_value[3] = 2.0f * (float) acos(q.w);
    m_quaternion = Quaternion(q.x, q.y, q.z, q.w);
    this->normalize();
    m_euler = angles;
    m_eulerOrder = order;
    m_eulerDirty = false;
}

Vec3f
SFRotation::operator *(const Vec3f &v) const
{
    return m_quaternion * v;
}

MyString
SFRotation::getEcmaScriptComment(MyString name, int flags) const
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // 4 floating point numbers\n";
        ret += indent;
        ret += "   // 3 numbers: normalized axis vector";
        ret += " + 1 number: angle (0 to 2 * pi)\n";
    }
    if (TheApp->GetEcmaScriptAddAllowedComponents()) {
        ret += indent;
        ret += "// allowed components:\n";

        ret += indent;
        ret += "   // x:     ";
        ret += name;
        ret += ".x or ";
        ret += name;
        ret += "[0]\n";

        ret += indent;
        ret += "   // y:     ";
        ret += name;
        ret += ".y or ";
        ret += name;
        ret += "[1]\n";

        ret += indent;
        ret += "   // z:     ";
        ret += name;
        ret += ".z or ";
        ret += name;
        ret += "[2]\n";

        ret += indent;
        ret += "   // angle: ";
        ret += name;
        ret += ".a or ";
        ret += name;
        ret += "[3]\n";
    }
    if (TheApp->GetEcmaScriptAddAvailableFunctions()) {
        ret += indent;
        ret += "// available functions:\n";
        if (flags != EL_EVENT_IN) {
            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = new SFRotation(float_x, float_y, float_z, float_a);\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = new SFRotation(sfvec3f_axis, float_angle);\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = new SFRotation(sfvec3f_fromVector, sfvec3f_toVector);\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfrotation_rot.inverse();\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfrotation_rot.multiply(sfrotation_rot2);\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = sfrotation_rot.slerp(sfrotation_dest,float_t);\n";
       }
       if (flags != EL_EVENT_OUT) {
            ret += indent;
            ret += "   // sfvec3f_axis = ";
            ret += name;
            ret += ".getAxis();\n";

            ret += indent;
            ret += "   // sfvec3f_vec = ";
            ret += name;
            ret += ".multVec(sfvec3f_vec);\n";

            ret += indent;
            ret += "   // ";
            ret += name;
            ret += ".setAxis(sfvec3f_vec);\n";

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
             ret += " = new SFRotation(0, 0, 1, Math.PI / 4.0);\n";

             ret += indent;
             ret += "   // ";
             ret += name;
             ret += ".a = Math.PI / 2.0;\n";
        } 
        if (flags != EL_EVENT_OUT) {
             ret += indent;
             ret += "   // float_x = ";
             ret += name;
             ret += ".x;\n";
        }
        if (flags == EL_FIELD) {
             ret += indent;
             ret += "   // ";
             ret += name;
             ret += " = ";
             ret += name;
             ret += ".inverse();\n";
        }
    }
    return ret;
}

void
SFRotation::flip(int index) 
{
    m_value[index] *= -1.0; 
    m_value[3] *= -1.0; 
}

void
SFRotation::swap(int fromTo)
{
    switch(fromTo) {
      case SWAP_XY:
        ::myswap(m_value[0], m_value[1]);
        break;
     case SWAP_XZ:
        ::myswap(m_value[0], m_value[2]);
        break;
     case SWAP_YZ:
        ::myswap(m_value[1], m_value[2]);
        break;
    }
    m_value[3] *= -1.0; 
}

FieldValue *
SFRotation::getRandom(Scene *scene, int nodeType)
{
    Vec4f vec(RAND(), RAND(), RAND(), FLOAT_RAND());
    vec.normalize();
    return new SFRotation(vec.x, vec.y, vec.z, vec.w);
}

