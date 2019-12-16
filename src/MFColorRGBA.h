/*
 * MFColorRGBA.h
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

class MFColorRGBA : public MFFloat {
public:
                        MFColorRGBA();
                        MFColorRGBA(int size) : MFFloat(size * 4) {}
                        MFColorRGBA(MFColorRGBA *values) :
                              MFFloat(values->getValues(), values->getSize()) {}
                        MFColorRGBA(float *values, int len) :
                                MFFloat(values, len) {}
                        MFColorRGBA(double* values, int len) : 
                              MFFloat(values, len) {}

    virtual int         getType() const { return MFCOLORRGBA; }
    virtual const char *getTypeName() const { return "MFColorRGBA"; }
    virtual int         getStride() const { return 4; }
    virtual FieldValue *copy();

    virtual bool        readLine(int index, char *line);

    virtual bool        equals(const FieldValue *value) const;

    float              *getValue(int index) const
                           { return m_value.getData() + index * 3; }

    virtual FieldValue *getSFValue(int index);
    virtual void        setSFValue(int index, FieldValue *value);
    virtual void        setSFValue(int index, float r, float g, float b, float a);
    virtual void        setSFValue(int index, float* values);
    virtual void        clamp(const FieldValue *min, const FieldValue *max);

    virtual void        insertSFValue(int index, FieldValue *value);
    virtual void        insertSFValue(int index, float r, float g, float b, float a);
    virtual void        insertSFValue(int index, float *values);

    virtual void        appendSFValue(float r, float g, float b, float a)
                           { insertSFValue(getSFSize(), r, g, b, a); }

    MyString            getEcmaScriptComment(MyString name, int flags);

    randomFunction(MFColorRGBA)                  
};

