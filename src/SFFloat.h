/*
 * SFFloat.h
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

#ifndef _SFFLOAT_H
#define _SFFLOAT_H

#ifndef _FIELDVALUE_H
#include "FieldValue.h"
#endif

class SFFloat : public FieldValue {
public:
                        SFFloat(float value);
                        SFFloat(void) { m_value = 0.0; } // silly default

    virtual int         getType() const { return SFFLOAT; }
    virtual const char *getTypeName() { return "SFFloat"; }
    virtual MyString    getString(int index, int stride);

    virtual int         writeData(int filedes, int i); 
    virtual int         writeDataC(int filedes, int i, int languageFlag); 

    virtual const char *getTypeC(int languageFlag) { return "float"; }
    virtual const char *getDefaultC(int languageFlag) { return "0.0f"; }

    virtual bool        readLine(int index, char *line);

    virtual int         getNumbersPerType(void) { return 1; }
    virtual bool        needCheckFloat(void) { return true; }

    virtual FieldValue *copy() { return new SFFloat(*this); }

    float               getValue() 
                           {
#ifdef HAVE_NULL_COMPARE
                           if (this == NULL)
                               return 0;
#endif 
                           return m_value; 
                           }

    virtual bool        equals(FieldValue *value);
    virtual bool        equalsAngle(const FieldValue *value, double angleUnit);
    virtual void        clamp(const FieldValue *min, const FieldValue *max);

    double              getFixedAngle(double angleUnit) 
                            {
#ifdef HAVE_NULL_COMPARE
                            if (this == NULL)
                                return 0;
#endif
                            if (angleUnit != 0) 
                                return m_value * angleUnit;
                            return m_value;
                            }

    MyString            getEcmaScriptComment(MyString name, int flags);

    bool                supportAnimation(bool x3d) { return true; }

    FieldValue         *getRandom(Scene *scene, int nodetype) 
                           { return new SFFloat(FLOAT_RAND()); }
private:
    float               m_value;
};

#endif // _SFFLOAT_H
