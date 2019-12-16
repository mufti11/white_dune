/*
 * SFDouble.h
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

#ifndef _SFDOUBLE_H
#define _SFDOUBLE_H

#ifndef _FIELDVALUE_H
#include "FieldValue.h"
#endif

class SFString;

class SFDouble : public FieldValue {
public:
                        SFDouble(double value);
                        SFDouble(const char* value) { setValue(value); }
                        SFDouble(SFString *value);
                        SFDouble(void) { m_value = 0.0f; } // silly default

    virtual int         getType() const { return SFDOUBLE; }
    virtual const char *getTypeName() const { return "SFDouble"; }
    virtual MyString    getString(int index, int stride);

    virtual int         writeData(int filedes, int i); 

    virtual const char *getTypeC(int languageFlag) const { return "double"; }
    virtual const char *getDefaultC(int languageFlag) const { return "0"; }

    virtual bool        readLine(int index, char *line);

    virtual int         getNumbersPerType(void) { return 1; }
    virtual bool        needCheckFloat(void) { return true; }

    virtual FieldValue *copy() { return new SFDouble(*this); }

    double              getValue() const { return m_value; }

    virtual bool        equals(const FieldValue *value) const;
    virtual bool        equalsAngle(FieldValue *value, double angleUnit);
    virtual void        clamp(const FieldValue *min, const FieldValue *max);

    MyString            getEcmaScriptComment(MyString name, int flags);

    bool                supportAnimation(bool x3d) { return x3d; }
    void                setValue(const char *value)
                           {
                           sscanf(value, "%lf", &m_value);
                           }

    double              getFixedAngle(double angleUnit) 
                            {
                            if (angleUnit != 0) 
                                return m_value * angleUnit;
                            return m_value;
                            }

    virtual bool        isX3DType() const { return true; }

    FieldValue         *getRandom(Scene *scene, int nodetype) 
                           { return new SFDouble(FLOAT_RAND()); };
private:
    double              m_value;
};

#endif // _SFDOUBLE_H
