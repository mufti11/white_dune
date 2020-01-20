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

#ifndef _SFVEC2D_H
#define _SFVEC2D_H

#ifndef _FIELDVALUE_H
#include "FieldValue.h"
#endif

class SFVec2d : public FieldValue {
public:
                        SFVec2d(double x, double y);
                        SFVec2d(const double *values);
                        SFVec2d(void); // silly default

    virtual int         getType() const { return SFVEC2D; }
    virtual int         getStride() const { return 2; }
    virtual const char *getTypeName() const { return "SFVec2d"; }
    virtual MyString    getString(int index, int stride) const;

    virtual int         writeData(int filedes, int i) const; 

    virtual int         writeC(int filedes, const char* variableName,
                               int languageFlag) const;
    virtual const char *getTypeC(int languageFlag) const { return "double"; }
    virtual bool        isArrayInC(void) const { return true; }

    virtual bool        readLine(int index, char *line);

    virtual int         getNumbersPerType(void) const { return 2; }
    virtual bool        needCheckFloat(void) const { return true; }

    virtual bool        equals(const FieldValue *value) const;
    virtual FieldValue *copy() { return new SFVec2d(*this); }

    double               getValue(int index) const { return m_value[index]; }
    const double        *getValue() const { return m_value; }
    void                setValue(int pos, double value)
                           { m_value[pos] = value; }
    void                setValue(double v1, double v2);

    MyString            getEcmaScriptComment(MyString name, int flags) const;

    bool                supportAnimation(bool x3d) const { return x3d; }

    FieldValue         *getRandom(Scene *scene, int nodeType) 
                           { return new SFVec2d(FLOAT_RAND(), FLOAT_RAND()); }
private:
    double               m_value[2];
};

#endif // _SFVEC2D_H
