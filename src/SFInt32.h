/*
 * SFInt32.h
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

class SFInt32 : public FieldValue {
public:
                        SFInt32(int value);
                        SFInt32(void) { m_value = 0; } // silly default

    virtual int         getType() const { return SFINT32; }
    virtual const char *getTypeName() const { return "SFInt32"; }
    virtual MyString    getString(int index, int stride) const;

    virtual int         writeData(int filedes, int i) const; 

    virtual const char *getTypeC(int languageFlag) const { return "int"; }
    virtual const char *getDefaultC(int languageFlag) const { return "0"; }

    virtual bool        readLine(int index, char *line);

    virtual int         getNumbersPerType(void) const { return 1; }

    virtual bool        equals(const FieldValue *value) const;
    virtual void        clamp(const FieldValue *min, const FieldValue *max);
    virtual FieldValue *copy() { return new SFInt32(*this); }

    int                 getValue() const 
                           {
#ifdef HAVE_NULL_COMPARE
                           if (this == NULL)
                               return 0;
#endif 
                           return m_value; 
                           }

    MyString            getEcmaScriptComment(MyString name, int flags) const;

    // VRML 200x IntegerSequencer not implemented yet
    bool                supportAnimation(bool x3d) const { return false; }

    FieldValue         *getRandom(Scene *scene, int nodetype) 
                           { return new SFInt32(INT_RAND()); }

private:
    int                 m_value;
};

