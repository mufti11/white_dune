/*
 * MFTime.h
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

#ifndef _MFTIME_H
#define _MFTIME_H

#ifndef _FIELDVALUE_H
#include "FieldValue.h"
#endif
#ifndef _ARRAY_H
#include "Array.h"
#endif
#ifndef _SFTIME_H
#include "SFTime.h"
#endif

class MFTime : public MFieldValue {
public:
                        MFTime();
                        MFTime(const MFTime &value);
                        MFTime(double *values, int len);
                        MFTime(const double *values, int len);
                        MFTime(double value);

    virtual int         getType() const             { return MFTIME; }
    virtual MyString    getString(int index, int stride) const;
    virtual const char *getTypeName() const         { return "MFTime"; }
    virtual int         getSFSize() const           { return m_value.size(); }

    virtual int         writeData(int filedes, int i) const; 

    virtual const char *getTypeC(int languageFlag) const { return "double"; }

    virtual bool        readLine(int index, char *line);

    virtual int         getNumbersPerType(void) const { return 1; }
    virtual bool        needCheckFloat(void) const { return true; }

    virtual void        clamp(const FieldValue *min, const FieldValue *max);
    virtual FieldValue *copy();

    int                 getSize() const { return m_value.size(); }
    double              getValue(int i) const { return m_value[i]; }
    virtual void        setValue(int index, double value);
    const double       *getValues() const { return m_value.getData(); }
    virtual FieldValue *getSFValue(int index) const   
                           { return new SFTime(m_value[index]); }
    virtual void        setSFValue(int index, FieldValue *value);

    virtual void        insertSFValue(int index, FieldValue *value);
    virtual void        insertSFValue(int index, const double value);
    virtual void        removeSFValue(int index) { m_value.remove(index); }

    virtual bool        equals(const FieldValue *value) const;
    bool                equals(const MFTime *value) const;

    MyString            getEcmaScriptComment(MyString name, int flags) const;

    FieldValue         *getRandom(Scene *scene, int nodeType);
private:
    Array<double>       m_value;
};

#endif // _MFTIME_H
