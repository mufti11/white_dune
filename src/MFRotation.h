/*
 * MFRotation.h
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

#include "MFFloat.h"

class MFRotation : public MFFloat {
public:
                        MFRotation();
                        MFRotation(float *values, int len) :
                              MFFloat(values, len) {}
                        MFRotation(double *values, int len) :
                              MFFloat(values, len) {}

    virtual int         getType() const { return MFROTATION; }
    virtual const char *getTypeName() { return "MFRotation"; }
    virtual int         getStride() { return 4; }
    virtual FieldValue *copy();

    virtual bool        readLine(int index, char *line);

    virtual bool        equals(FieldValue *value);
    virtual void        fixAngle(double angleUnit); 

    float              *getValue(int index)
                           { return m_value.getData() + index * 4; }
    virtual FieldValue *getSFValue(int index);
    virtual void        setSFValue(int index, FieldValue *value);
    virtual void        setSFValue(int index, const float *values);

    virtual void        insertSFValue(int index, FieldValue *value);
    virtual void        insertSFValue(int index, const float *values);
    virtual void        removeSFValue(int index) { m_value.remove(index); }

    void                flip(int index);
    void                swap(int fromTo);

    MyString            getEcmaScriptComment(MyString name, int flags);

    randomFunction(MFRotation)                  
};
