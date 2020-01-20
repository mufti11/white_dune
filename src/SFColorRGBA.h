/*
 * SFColorRGBA.h
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

#include "FieldValue.h"

class SFColorRGBA : public FieldValue {
public:
                        SFColorRGBA(float r, float g, float b);
                        SFColorRGBA(float r, float g, float b, float a);
                        SFColorRGBA(const float *value);
                        SFColorRGBA(void); // silly default

    virtual int         getType() const { return SFCOLORRGBA; }
    virtual const char *getTypeName() const { return "SFColorRGBA"; }
    virtual int         getStride() const { return 3; }
    virtual MyString    getString(int index, int stride) const;

    virtual int         writeData(int filedes, int i) const; 

    virtual int         writeC(int filedes, const char* variableName,
                               int languageFlag) const;
    virtual const char *getTypeC(int languageFlag) const { return "float"; }
    virtual bool        isArrayInC(void) const { return true; }

    virtual bool        readLine(int index, char *line);

    virtual int         getNumbersPerType(void) const { return 4; }
    virtual bool        needCheckFloat(void) const { return true; }

    virtual bool        equals(const FieldValue *value) const;
    virtual void        clamp(const FieldValue *min, const FieldValue *max);
    virtual FieldValue *copy() { return new SFColorRGBA(*this); }

    const float        *getValue() const { return m_value; }
    float               getValue(int index) const { return m_value[index]; }
    void                setValue(int index, float value)
                           { m_value[index] = value; }
    void                setValue(float v1, float v2, float v3);
    void                setValue(float v1, float v2, float v3, float v4);

    MyString            getEcmaScriptComment(MyString name, int flags) const;

    bool                supportAnimation(bool x3d) const { return false; }

    FieldValue         *getRandom(Scene *scene, int nodetype);
protected:
    bool                equals(const SFColorRGBA *value) const;


protected:
    // m_value[3] = -1 means no transparency information
    float               m_value[4];
};
