/*
 * SFBool.h
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

#ifndef _SFBOOL_H
#define _SFBOOL_H

#ifndef _FIELDVALUE_H
#include "FieldValue.h"
#endif

class SFBool : public FieldValue {
public:
                        SFBool(bool value);
                        SFBool(void) { m_value = false; } // silly default

    virtual int         getType() const { return SFBOOL; }
    virtual const char *getTypeName() const { return "SFBool"; }
    virtual MyString    getString(int index, int stride) const;

    virtual int         writeData(int filedes, int i) const; 
    virtual int         writeXml(int filedes, int indent) const;

    virtual int         writeC(int filedes, const char* variableName, 
                               int languageFlag) const;
    virtual const char *getTypeC(int languageFlag) const;
    virtual const char *getDefaultC(int languageFlag) const;

    virtual bool        readLine(int index, char *line);

    virtual int         getNumbersPerType(void) const { return 0; }
    virtual bool        checkInput(char *line);

    virtual bool        equals(const FieldValue *value) const;
    virtual FieldValue *copy() { return new SFBool(*this); }

    bool                getValue() const 
                           {
#ifdef HAVE_NULL_COMPARE
                           if (this == NULL)
                               return false;
#endif 
                           return m_value; 
                           }

    MyString            getEcmaScriptComment(MyString name, int flags) const;

    // VRML 200x BooleanSequencer not implemented yet
    virtual bool        supportAnimation(bool x3d) const { return false; }
    virtual bool        supportInteraction(void) const { return true; }

    virtual bool        isText() { return false; }      

    FieldValue         *getRandom(Scene *scene, int nodetype);
private:
    bool                m_value;
};

#endif // _SFBOOL_H
