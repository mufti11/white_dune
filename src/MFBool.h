/*
 * MFBool.h
 *
 * Copyright (C) 1999 Stephen F. White, 2008 J. "MUFTI" Scheurich
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

#include "Array.h"
#include "FieldValue.h"

class MFBool : public MFieldValue {
public:
                        MFBool();
                        MFBool(bool *values, int len);
                        MFBool(MFBool *value);
                        MFBool(const bool value);
    virtual            ~MFBool();

    virtual int         getType() const { return MFBOOL; }
    virtual const char *getTypeName() const { return "MFBool"; }
    virtual MyString    getString(int index, int stride);

    virtual int         writeData(int filedes, int i); 
    virtual int         writeXml(int filedes, int indent, int containerField,
                                 bool avoidUse);

    virtual int         writeDataC(int filedes, int i, int languageFlag);
    virtual const char *getTypeC(int languageFlag) const;

    virtual bool        readLine(int index, char *line);
    virtual int         getNumbersPerType(void) const
                            { return ((MFBool *)this)->getStride(); }

    virtual bool        equals(const FieldValue *value) const;
    virtual int         getSFSize() const { return m_value.size(); }
    virtual FieldValue *getSFValue(int index);
    virtual void        setSFValue(int index, FieldValue *value);
    virtual void        setSFValue(int index, const int value);
    virtual FieldValue *copy();

    bool                getValue(int index) 
                            { return m_value[index]; }
    const bool         *getValues() const { return m_value.getData(); }
    int                 getSize() const { return m_value.size(); }

    virtual void        insertSFValue(int index, FieldValue *value);
    virtual void        insertSFValue(int index, const bool value);
    virtual void        removeSFValue(int index) { m_value.remove(index); }

    MyString            getEcmaScriptComment(MyString name, int flags);

    virtual bool        isX3DType() const { return true; }

    FieldValue         *getRandom(Scene *scene, int nodeType);
protected:
    MyArray<bool>       m_value;
};

