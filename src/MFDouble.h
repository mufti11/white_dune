/*
 * MFDouble.h
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

#ifndef _MFDOUBLE_H
#define _MFDOUBLE_H

#ifndef _ARRAY_H
#include "Array.h"
#endif
#ifndef _FIELD_VALUE_H
#include "FieldValue.h"
#endif
#ifndef _SFDOUBLE_H
#include "SFDouble.h"
#endif

class MFDouble : public MFieldValue {
public:
                        MFDouble(int stride = 1);
                        MFDouble(int len, int stride);
                        MFDouble(const MFDouble &value);
                        MFDouble(const double *values, int len, int stride = 1);
                        MFDouble(const float *values, int len, int stride = 1);
                        MFDouble(const double value);
    virtual            ~MFDouble();

    virtual int         getType() const { return MFDOUBLE; }
    virtual int         getStride() const { return 1; }
    virtual MyString    getString(int index, int stride) const;
    virtual const char *getTypeName() const { return "MFDouble"; }

    virtual int         writeData(int filedes, int i) const; 
    virtual int         writeDataC(int filedes, int i, int languageFlag) const;

    virtual const char *getTypeC(int languageFlag) const { return "double"; }

    virtual bool        readLine(int index, char *line);

    virtual int         getNumbersPerType(void) const { return getStride(); }
    virtual bool        needCheckFloat(void) const { return true; }

    virtual bool        equals(const FieldValue *value) const;
    virtual void        clamp(const FieldValue *min, const FieldValue *max);
    virtual FieldValue *copy(); 

    virtual int         getSFSize() const
                           { return m_value.size() / getStride(); }
    virtual FieldValue *getSFValue(int index) const; 
    virtual void        setSFValue(int index, FieldValue *value);
    void                setSFValue(int index, const double value);

    const double       *getValues() const { return m_value.getData(); }
    double              getValue(int i) const { return m_value[i]; }
    int                 getSize() const { return m_value.size(); }
    void                setValue(int index, double value) 
                           { m_value[index] = value; }

    virtual void        insertSFValue(int index, FieldValue *value);
    void                insertSFValue(int index, const double value);

    virtual void        removeSFValue(int index);

    double              getMaxValue();
    double              getMinValue();

    MyString            getEcmaScriptComment(MyString name, int flags) const;

    virtual bool        isX3DType() { return true; }

    FieldValue         *getRandom(Scene *scene, int nodeType);
protected:
    bool                equals(const MFDouble *value) const;

protected:
    MyArray<double>     m_value;
};

#endif // _MFDOUBLE_H
