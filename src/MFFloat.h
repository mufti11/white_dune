/*
 * MFFloat.h
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

#ifndef _MFFLOAT_H
#define _MFFLOAT_H

#ifndef _ARRAY_H
#include "Array.h"
#endif
#ifndef _FIELD_VALUE_H
#include "FieldValue.h"
#endif
#ifndef _SFFLOAT_H
#include "SFFloat.h"
#endif

class MFFloat : public MFieldValue {
public:
                        MFFloat();
                        MFFloat(int size);
                        MFFloat(const MFFloat &value);
                        MFFloat(const MFFloat *values);
                        MFFloat(const float *values, int len, int stride = 1);
                        MFFloat(const double *values, int len, int stride = 1);
                        MFFloat(const float value);
    virtual            ~MFFloat();

    virtual int         getType() const { return MFFLOAT; }
    virtual int         getStride() const { return 1; }
    virtual MyString    getString(int index, int stride) const; 
    virtual const char *getTypeName() const { return "MFFloat"; }

    virtual int         writeData(int filedes, int i) const; 
    virtual int         writeDataC(int filedes, int i, int languageFlag) const;

    virtual const char *getTypeC(int languageFlag) const { return "float"; }

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
    void                setSFValue(int index, const float value);

    const float        *getValues() const { return m_value.getData(); }
    float               getValue(int i) const { return m_value[i]; }
    void                setValue(int index, float value) 
                           { m_value[index] = value; }
    int                 getSize() const { return m_value.size(); }

    virtual void        insertSFValue(int index, FieldValue *value);
    void                insertSFValue(int index, const float value);
    virtual void        appendSFValue(const float value);
    virtual void        removeSFValue(int index);

    float               getMaxValue();
    float               getMinValue();

    MyString            getEcmaScriptComment(MyString name, int flags) const;

    float              *getRandomFloats(int sfsize);
    FieldValue         *getRandom(Scene *scene, int nodeType);

#define randomFunction(Type)                     \
FieldValue *getRandom(Scene *scene, int nodeType) \
{                                                  \
    int size = INT_RAND() * getStride();            \
    return new Type(getRandomFloats(size), size);    \
}
 
protected:
    bool                equals(const MFFloat *value) const;

protected:
    Array<float>        m_value;
};

#endif // _MFFLOAT_H
