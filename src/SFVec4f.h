/*
 * SFVec4f.h
 *
 * Copyright (C) 1999 Stephen F. White, 2009 J. "MUFTI" Scheurich
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

#ifndef _SFVEC4F_H
#define _SFVEC4F_H

#ifndef _VEC4F_H
# include "Vec4f.h"
#endif
#include "FieldValue.h"

class SFVec4f : public FieldValue {
public:
                        SFVec4f(const Vec4f &v)
                          {
                          m_value[0] = v.x; 
                          m_value[1] = v.y; 
                          m_value[2] = v.z;
                          m_value[3] = v.w;
                          }
                        SFVec4f(float x, float y, float z, float w)
                           {
                           m_value[0] = x; 
                           m_value[1] = y; 
                           m_value[2] = z;
                           m_value[3] = w;
                           }
                        SFVec4f(const float* value)
                           {
                           m_value[0] = value[0]; 
                           m_value[1] = value[1]; 
                           m_value[2] = value[2];
                           m_value[3] = value[3];
                           }
                        SFVec4f(void)  // silly default
                           {
                           m_value[0] = m_value[1] = m_value[2] = m_value[3] = 0.0; 
                           }

    virtual int         getType() const { return SFVEC4F; }
    virtual const char *getTypeName() const { return "SFVec4f"; }
    virtual MyString    getString(int index, int stride) const;
    virtual FieldValue *copy() { return new SFVec4f(*this); }
    virtual bool        equals(const FieldValue *value) const;
    virtual void        clamp(const FieldValue *min, const FieldValue *max);
    virtual bool        supportAnimation(bool x3d) const { return true; }
    virtual bool        supportInteraction(void) const { return true; }
    MyString            getEcmaScriptComment(MyString name, int flags) const;

    virtual int         writeData(int filedes, int i) const; 

    virtual int         writeC(int filedes, const char* variableName,
                               int languageFlag) const;
    virtual const char *getTypeC(int languageFlag) const { return "float"; }
    virtual bool        isArrayInC(void) const { return true; }

    virtual int         writeAc3d(int filedes, int indent) const;

    virtual bool        readLine(int index, char *line);

    virtual int         getNumbersPerType(void) const { return 4; }
    virtual bool        needCheckFloat(void) const { return true; }
    
    const float        *getValue() const { return m_value; }
    float               getValue(int pos) const { return m_value[pos]; }
    void                setValue(int index, float value)
                           {
                           assert(index >= 0 && index < 4);
                           m_value[index] = value;
                           }
    void                setValue(float v1, float v2, float v3, float v4)
                           {
                           m_value[0] = v1;
                           m_value[1] = v2;
                           m_value[2] = v3;
                           m_value[3] = v4;
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

    FieldValue         *getRandom(Scene *scene, int nodeType) 
                           { return new SFVec4f(FLOAT_RAND(), FLOAT_RAND(), 
                                                FLOAT_RAND(), FLOAT_RAND()); }
protected:
    float               m_value[4];
};

#endif // _SFVEC4F_H
