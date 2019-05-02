/*
 * MFColor.h
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

#ifndef _MFCOLOR_H
#define _MFCOLOR_H

#ifndef _MFFLOAT_H
#include "MFFloat.h"
#endif

class MFColor : public MFFloat {
public:
                        MFColor();
                        MFColor(int size) : MFFloat(size * 3) {}
                        MFColor(const MFColor *values) :
                              MFFloat(values->getValues(), values->getSize()) {}
                        MFColor(float *values, int len) :
                                MFFloat(values, len) {}
                        MFColor(double* values, int len) : 
                              MFFloat(values, len) {}

    virtual int         getType() const { return MFCOLOR; }
    virtual const char *getTypeName() const { return "MFColor"; }
    virtual int         getStride() const { return 3; }
    virtual FieldValue *copy();

    virtual bool        readLine(int index, char *line);

    virtual bool        equals(const FieldValue *value) const;

    const float        *getValue(int index) const 
                           { return m_value.getData() + index * 3; }

    virtual FieldValue *getSFValue(int index) const;
    virtual void        setSFValue(int index, FieldValue *value);
    virtual void        setSFValue(int index, float r, float g, float b);
    virtual void        setSFValue(int index, const float* values);
    virtual void        clamp(const FieldValue *min, const FieldValue *max);

    virtual void        insertSFValue(int index, FieldValue *value);
    virtual void        insertSFValue(int index, float r, float g, float b);
    virtual void        insertSFValue(int index, const float *values);

    virtual void        appendSFValue(float r, float g, float b)
                           { insertSFValue(getSFSize(), r, g, b); }

    MyString            getEcmaScriptComment(MyString name, int flags) const;

    randomFunction(MFColor)                  
};

#endif // _MFCOLOR_H
