/*
 * MFMatrix4f.h
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

#ifndef _MFMATRIX4F_H
#define _MFMATRIX4F_H

#ifndef _MFFLOAT_H
# include "MFFloat.h"
#endif

class MFMatrix4f : public MFFloat {
public:
                        MFMatrix4f() : MFFloat() {}
                        MFMatrix4f(int size) : MFFloat(size * 16) {}
                        MFMatrix4f(const MFMatrix4f *values) :
                              MFFloat(values->getValues(), values->getSize()) {}
                        MFMatrix4f(const float *values, int len) :
                              MFFloat(values, len) {}
                        MFMatrix4f(const double *values, int len) :
                              MFFloat(values, len) {}

    virtual int         getType() const { return MFMATRIX4F; }
    virtual const char *getTypeName() const { return "MFMatrix4f"; }
    virtual int         getStride() const { return 16; }
    virtual FieldValue *copy();

    virtual bool        readLine(int index, char *line);

    virtual bool        equals(const FieldValue *value) const;

    virtual FieldValue *getSFValue(int index) const;
    virtual void        setSFValue(int index, FieldValue *value);
    virtual void        setSFValue(int index, const float *values);

    const float        *getValue(int index) const
                              { return m_value.getData() + index * 16; } 

    virtual void        insertSFValue(int index, FieldValue *value);
    virtual void        insertSFValue(int index, const float *values);

    MyString            getEcmaScriptComment(MyString name, int flags) const;

    bool                supportAnimation(bool x3d) const { return false; }

    randomFunction(MFMatrix4f)                  
};

#endif
