/*
 * MFInt32.h
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

#include "Array.h"
#include "FieldValue.h"

class MFInt32 : public MFieldValue {
public:
                        MFInt32();
                        MFInt32(int *values, int len);
                        MFInt32(MFInt32 *value);
                        MFInt32(const int value);
    virtual            ~MFInt32();

    virtual int         getType() const { return MFINT32; }
    virtual const char *getTypeName() const { return "MFInt32"; }
    virtual MyString    getString(int index, int stride) const;

    virtual int         writeData(int filedes, int i) const; 

    virtual int         write(int f, int indent) const
                           { 
                           return write(f, indent, writeBrackets(), true); 
                           }
    virtual int         write4FieldPipe(int f, int indent) const
                           { return write(f, indent, false, false); }
    int                 write(int f, int indent, bool writeBrackets, 
                              bool compactFormat) const;

    virtual const char *getTypeC(int languageFlag) const { return "int"; }

    virtual bool        readLine(int index, char *line);
    virtual int         getNumbersPerType(void) const { return getStride(); }

    virtual bool        equals(const FieldValue *value) const;
    virtual int         getSFSize() const { return m_value.size(); }
    virtual FieldValue *getSFValue(int index) const;
    virtual void        setSFValue(int index, FieldValue *value);
    virtual void        setSFValue(int index, const int value);
    virtual FieldValue *copy();

    int                 getValue(int index) const 
                           { 
                           if (index > -1)
                               return m_value[index]; 
                           else
                               return -1;
                           }
    const int          *getValues() const { return m_value.getData(); }
    int                 getSize() const { return m_value.size(); }

    virtual void        insertSFValue(int index, FieldValue *value);
    virtual void        insertSFValue(int index, const int value);
    virtual void        appendSFValue(const int value);
    virtual void        removeSFValue(int index) { m_value.remove(index); }

    MyString            getEcmaScriptComment(MyString name, int flags) const;

    FieldValue         *getRandom(Scene *scene, int nodeType);

    int                 find(int i) { return m_value.find(i); }
protected:
    MyArray<int>        m_value;
    mutable bool       m_lastMinus1;
};

