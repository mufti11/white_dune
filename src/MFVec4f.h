/*
 * MFVec4f.h
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

#pragma once

#include "MFFloat.h"
#include "Vec4f.h"

class MFVec2f;

class MFVec4f : public MFFloat {
public:
                        MFVec4f() : MFFloat() {}
                        MFVec4f(int size) : MFFloat(size * 4) {}
                        MFVec4f(const MFVec4f *values) :
                              MFFloat(values->getValues(), values->getSize()) {}
                        MFVec4f(const float *values, int len) :
                              MFFloat(values, len) {}
                        MFVec4f(const double *values, int len) :
                              MFFloat(values, len) {}

                        MFVec4f(MFVec2f *mfVec2f);

    virtual int         getType() const { return MFVEC4F; }
    virtual const char *getTypeName() const { return "MFVec4f"; }
    virtual int         getStride() const { return 4; }
    virtual FieldValue *copy();

    virtual bool        readLine(int index, char *line);

    virtual bool        equals(const FieldValue *value) const;

    virtual FieldValue *getSFValue(int index) const;
    virtual void        setSFValue(int index, FieldValue *value);
    virtual void        setSFValue(int index, const float *values);
    virtual void        setSFValue(int index, float x, float y, float z, 
                                              float w);

    const float        *getValue(int index) const
                              { return m_value.getData() + index * 4; } 

    void                setVec(int index, Vec4f v);
    Vec4f               getVec(int index);

    virtual void        insertSFValue(int index, FieldValue *value);
    virtual void        insertSFValue(int index, const float *values);
    virtual void        insertSFValue(int index, float x, float y, float z, 
                                                 float w);

    virtual void        appendSFValue(float x, float y, float z, float w) 
                           { insertSFValue(getSFSize(), x, y, z, w); }
                        
    Vec4f               getMinBoundingBox(void);
    Vec4f               getMaxBoundingBox(void);

    void                flip(int index);
    void                swap(int fromTo);

    MyString            getEcmaScriptComment(MyString name, int flags) const;

    bool                supportAnimation(bool x3d) const { return true; }

    randomFunction(MFVec4f)                  
};
