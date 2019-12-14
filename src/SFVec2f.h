/*
 * SFVec2f.h
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

#ifndef _SFVEC2F_H
#define _SFVEC2F_H

#ifndef _FIELDVALUE_H
#include "FieldValue.h"
#endif
#ifndef _VEC2F_H
# include "Vec2f.h"
#endif

class SFVec2f : public FieldValue {
public:
                        SFVec2f(float x, float y);
                        SFVec2f(const float *values);
                        SFVec2f(void); // silly default
                        SFVec2f(const Vec2f &v)
                            {
                            m_value[0] = v.x; 
                            m_value[1] = v.y; 
                            }

    virtual int         getType() const { return SFVEC2F; }
    virtual const char *getTypeName() { return "SFVec2f"; }
    virtual int         getStride() { return 2; }
    virtual MyString    getString(int index, int stride);

    virtual int         writeData(int filedes, int i); 

    virtual int         writeC(int filedes, const char* variableName,
                               int languageFlag);
    virtual const char *getTypeC(int languageFlag) { return "float"; }
    virtual bool        isArrayInC(void) { return true; }

    virtual bool        readLine(int index, char *line);

    virtual int         getNumbersPerType(void) { return 2; }
    virtual bool        needCheckFloat(void) { return true; }

    virtual bool        equals(FieldValue *value);
    virtual void        clamp(const FieldValue *min, const FieldValue *max);
    virtual FieldValue *copy() { return new SFVec2f(*this); }

    float               getValue(int index) { return m_value[index]; }
    float              *getValue() { return m_value; }
    void                setValue(int pos, float value)
                           { m_value[pos] = value; }
    void                setValue(float v1, float v2);
    void                setSFValue(FieldValue *value)
                            {
                            m_value[0] = ((SFVec2f *)value)->getValue()[0];
                            m_value[1] = ((SFVec2f *)value)->getValue()[1];
                            }

    MyString            getEcmaScriptComment(MyString name, int flags);

    bool                supportAnimation(bool x3d) { return x3d; }

    Vec2f               getVec(void);

    FieldValue         *getRandom(Scene *scene, int nodeType) 
                           { return new SFVec2f(FLOAT_RAND(), FLOAT_RAND()); }
private:
    float               m_value[2];
};

#endif // _SFVEC2F_H
