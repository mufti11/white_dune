/*
 * SFMatrix4f.h
 *
 * Copyright (C) 1999 Stephen F. White, 2007 J. "MUFTI" Scheurich
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

#ifndef _SFMatrix4f_H
#define _SFMatrix4f_H

#include "FieldValue.h"

class SFMatrix4f : public FieldValue {
public:
                        SFMatrix4f(float m00, float m01, float m02, float m03,
                                   float m10, float m11, float m12, float m13,
                                   float m20, float m21, float m22, float m23,
                                   float m30, float m31, float m32, float m33)
                           {
                           m_value[0] = m00; 
                           m_value[1] = m01; 
                           m_value[2] = m02; 
                           m_value[3] = m03; 
                           m_value[4] = m10; 
                           m_value[5] = m11; 
                           m_value[6] = m12; 
                           m_value[7] = m13; 
                           m_value[8] = m20; 
                           m_value[9] = m21; 
                           m_value[10] = m22; 
                           m_value[11] = m23; 
                           m_value[12] = m30; 
                           m_value[13] = m31; 
                           m_value[14] = m32; 
                           m_value[15] = m33; 
                           }
                        SFMatrix4f(const float *v)
                           {
                           for (int i = 0; i < 16; i++)
                               m_value[i] = v[i];   
                           }
                        SFMatrix4f(SFMatrix4f &m)
                           {
                           const float *v = m.getValue();
                           for (int i = 0; i < 16; i++)
                               m_value[i] = v[i];   
                           }
                        SFMatrix4f(void)  // silly default: identity matrix
                           {
                           for (int i = 0; i < 16; i++)
                               m_value[i] = 0;
                           m_value[0] = 1;
                           m_value[5] = 1;
                           m_value[10] = 1;
                           m_value[15] = 1;
                           }

    const float        *getValue() const { return m_value; }
    const float         getValue(int pos) const { return m_value[pos]; }
    void                setValue(int index, float value)
                           {
                           assert(index >= 0 && index < 16);
                           m_value[index] = value;
                           }
    virtual int         getType() const { return SFMATRIX4F; }
    virtual const char *getTypeName() const { return "SFMatrix4f"; }
    virtual int         getStride() const { return 16; }
    virtual MyString    getString(int index, int stride);
    virtual FieldValue *copy() { return new SFMatrix4f(*this); }
    virtual bool        equals(const FieldValue *value) const;
    virtual void        clamp(const FieldValue *min, const FieldValue *max);
    virtual bool        supportAnimation(bool x3d) { return false; }
    virtual bool        supportInteraction(void) { return true; }
    MyString            getEcmaScriptComment(MyString name, int flags);

    virtual int         writeData(int filedes, int i); 
    virtual int         write(int filedes, int indent);

    virtual int         writeC(int filedes, const char* variableName,
                               int languageFlag);
    virtual const char *getTypeC(int languageFlag) { return "float"; }
    virtual bool        isArrayInC(void) { return true; }

    virtual int         writeAc3d(int filedes, int indent);

    virtual bool        readLine(int index, char *line);
    virtual int         getNumbersPerType(void) { return 9; }

    virtual bool        isX3DType() const { return true; }

    FieldValue         *getRandom(Scene *scene, int nodetype);
protected:
    float               m_value[16];
};

#endif
