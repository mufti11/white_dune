/*
 * SfVec3x.h
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

#pragma once

#include "FieldValue.h"
#include "Vec3f.h"
#include "Quaternion.h"

template<class X> class SfVec3x : public FieldValue {
public:
    virtual int         getType() const=0;
    virtual const char *getTypeName() const=0;

    virtual int         writeData(int filedes, int i) = 0; 

    virtual bool        readLine(int index, char *line)=0;

    virtual bool        equals(const FieldValue *value) const=0;

    virtual void        clamp(const FieldValue *min, const FieldValue *max)=0;

    virtual MyString    getEcmaScriptComment(MyString name, int flags) const=0;

    virtual int         getNumbersPerType(void) const { return 3; }
    virtual bool        needCheckFloat(void) const { return true; }

    virtual FieldValue *copy()=0;
    
    const X            *getValue() const { return m_value; }
    X                   getValue(int pos) const { return m_value[pos]; }
    void                setValue(int index, X value)
                           {
                           assert(index >= 0 && index < 3);
                           m_value[index] = value;
                           }
    void                setValue(X v1, X v2, X v3)
                           {
                           m_value[0] = v1;
                           m_value[1] = v2;
                           m_value[2] = v3;
                           }

    void                flip(int index) { m_value[index] *= -1.0; }
    void                swap(int fromTo)
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
                           }
    void                scale(X scale1, X scale2, X scale3)
                           {
                           m_value[0] *= scale1;
                           m_value[1] *= scale2;
                           m_value[2] *= scale3;
                           }
    void                scale(X* mult)
                           {
                           m_value[0] *= mult[0];
                           m_value[1] *= mult[1];
                           m_value[2] *= mult[2];
                           }
    void                translate(X* v)
                           {
                           m_value[0] += v[0];
                           m_value[1] += v[1];
                           m_value[2] += v[2];
                           }
    void                rotate(X* rot)
                           {
                           Vec3f rotationAxis(rot[0], rot[1], rot[2]);
                           Quaternion quat(rotationAxis, rot[3]);
                           quat.normalize();
                           Vec3f vector(m_value[0], m_value[1], m_value[2]);
                           Vec3f rotatedVector = quat*vector;
                           m_value[0] = rotatedVector.x;
                           m_value[1] = rotatedVector.y;
                           m_value[2] = rotatedVector.z;
                           }

protected:
    X                   m_value[3];
};

