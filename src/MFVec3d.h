/*
 * MFVec3d.h
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
#include "Vec3d.h"

class MFString;
class MFVec3f;

class MFVec3d : public MFDouble {
public:
                        MFVec3d() : MFDouble(3) {}
                        MFVec3d(int size) : MFDouble(size * 3) {}
                        MFVec3d(const double *values, int len) :
                              MFDouble(values, len) {}
                        MFVec3d(const float *values, int len) :
                              MFDouble(values, len) {}
                        MFVec3d(MFString *values);

    MFVec3f            *getMFVec3f(void);

    virtual int         getType() const { return MFVEC3D; }
    virtual const char *getTypeName() const { return "MFVec3d"; }
    virtual int         getStride() const { return 3; }
    virtual FieldValue *copy();

    virtual bool        readLine(int index, char *line);

    virtual bool        equals(const FieldValue *value) const;

    virtual FieldValue *getSFValue(int index) const;
    virtual void        setSFValue(int index, FieldValue *value);
    virtual void        setSFValue(int index, const double *values);
    virtual void        setSFValue(int index, double x, double y, double z);
    virtual void        setSFValue(int index, const char* values);

    const double       *getValue(int index) const 
                              { return m_value.getData() + index * 3; } 

    void                setVec(int index, Vec3d v);
    Vec3d               getVec(int index);

    virtual void        insertSFValue(int index, FieldValue *value);
    virtual void        insertSFValue(int index, const double *values);
    virtual void        insertSFValue(int index, double x, double y, double z);

    virtual void        appendSFValue(double x, double y, double z) 
                           { insertSFValue(getSFSize(), x, y, z); }
    virtual void        appendSFValue(const double *values) 
                           { insertSFValue(getSFSize(), values); }
                        
    Vec3d               getMinBoundingBox(void);
    Vec3d               getMaxBoundingBox(void);

    void                flip(int index);
    void                swap(int fromTo);

    MyString            getEcmaScriptComment(MyString name, int flags) const;

    FieldValue         *getRandom(Scene *scene, int nodeType);
};
