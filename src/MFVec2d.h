/*
 * MFVec2d.h
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

#include "MFDouble.h"
#include "Vec2d.h"

class MFVec2f;
class MFString;

class MFVec2d : public MFDouble {
public:
                        MFVec2d() : MFDouble(2) {}
                        MFVec2d(int size) : MFDouble(size * 2) {}
                        MFVec2d(const double *values, int len) :
                                MFDouble(values, len) {}
                        MFVec2d(MFString *values);
                        MFVec2d(MFVec2d *values);

    virtual int         getType() const { return MFVEC2D; }
    virtual const char *getTypeName() const { return "MFVec2d"; }
    virtual int         getStride() const { return 2; }
    virtual FieldValue *copy();

    virtual bool        readLine(int index, char *line);

    virtual bool        equals(const FieldValue *value) const;

    virtual FieldValue *getSFValue(int index) const;
    virtual void        setSFValue(int index, FieldValue *value);
    virtual void        setSFValue(int index, const double *values);
    virtual void        setSFValue(int index, double x, double y);
    virtual void        setSFValue(int index, const char* values);

    const double       *getValue(int index) const 
                              { return m_value.getData() + index * 2; } 

    void                setVec(int index, Vec2d v);
    Vec2d               getVec(int index);

    virtual void        insertSFValue(int index, FieldValue *value);
    virtual void        insertSFValue(int index, const double *values);
    virtual void        insertSFValue(int index, double x, double y, double z);

    Vec2d               getMinBoundingBox(void);
    Vec2d               getMaxBoundingBox(void);

    void                flip(int index);
    void                swap(int fromTo);

    MyString            getEcmaScriptComment(MyString name, int flags) const;

    FieldValue         *getRandom(Scene *scene, int nodeType);
};

