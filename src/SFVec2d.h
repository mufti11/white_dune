/*
 * SFVec2d.h
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

#pragma once

#include "Vec2d.h"
#include "FieldValue.h"

class SFVec2d : public FieldValue {
public:
                        SFVec2d(double x, double y);
                        SFVec2d(double *values);
                        SFVec2d(void); // silly default

    virtual int         getType() const { return SFVEC2D; }
    virtual int         getStride() { return 2; }
    virtual const char *getTypeName() { return "SFVec2d"; }
    virtual MyString    getString(int index, int stride);

    virtual int         writeData(int filedes, int i); 

    virtual int         writeC(int filedes, const char* variableName,
                               int languageFlag);
    virtual const char *getTypeC(int languageFlag) { return "double"; }
    virtual bool        isArrayInC(void) { return true; }

    virtual bool        readLine(int index, char *line);

    virtual int         getNumbersPerType(void) { return 2; }
    virtual bool        needCheckFloat(void) { return true; }

    virtual bool        equals(FieldValue *value);
    virtual void        clamp(const FieldValue *min, const FieldValue *max);
    virtual FieldValue *copy() { return new SFVec2d(*this); }

    double              getValue(int index) { return m_value[index]; }
    double             *getValue() { return m_value; }
    void                setValue(int pos, double value)
                           { m_value[pos] = value; }
    void                setValue(double v1, double v2);
    void                setValue(int index, float value)
                           {
                           assert(index >= 0 && index < 4);
                           m_value[index] = value;
                           }
    void                setValue(float v1, float v2)
                           {
                           m_value[0] = v1;
                           m_value[1] = v2;
                           }
    void                setSFValue(FieldValue *value) 
                           {
                           m_value[0] = ((SFVec2d *)value)->getValue()[0];
                           m_value[1] = ((SFVec2d *)value)->getValue()[1];
                           }
    Vec2d              getSFValue(int index)
                           {
                           static Vec2d vec;
                           vec.x = getValue(index * 2);
                           vec.y = getValue(index * 2 + 1);
                           return vec;
                           }


    MyString            getEcmaScriptComment(MyString name, int flags);

    bool                supportAnimation(bool x3d) { return x3d; }

    FieldValue         *getRandom(Scene *scene, int nodeType) 
                           { return new SFVec2d(FLOAT_RAND(), FLOAT_RAND()); }
private:
    double               m_value[2];
};
