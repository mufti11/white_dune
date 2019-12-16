/*
 * SFString.h
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

#ifndef _SFSTRING_H
#define _SFSTRING_H

#ifndef _FIELDVALUE_H
#include "FieldValue.h"
#endif
#ifndef _DUNE_STRING_H
#include "MyString.h"
#endif

class SFString : public FieldValue {
public:
                        SFString(const MyString &value);
                        SFString(const char *str);
                        SFString(void) { m_value = ""; } // silly default

    const char         *getValue() const
                           {
#ifdef HAVE_NULL_COMPARE
                           if (this == NULL)
                               return "";
#endif 
                           return m_value; 
                           }
    virtual int         getType() const { return SFSTRING; }
    virtual const char *getTypeName() const { return "SFString"; }
    virtual MyString    getString(int index, int stride);
                        
    virtual int         write4FieldPipe(int filedes, int indent); 

    virtual int         writeCWonderlandArt(int filedes, 
                                            const char* variableName,
                                            int languageFlag);

    virtual int         writeRaw(int filedes, int indent);

    virtual int         writeData(int filedes, int i); 
    virtual int         writeDataXml(int filedes, int i); 

    virtual const char *getTypeC(int languageFlag) const;

    virtual bool        readLine(int index, char *line);

    virtual int         getNumbersPerType(void) const { return 0; }

    virtual bool        equals(const FieldValue *value) const;
    virtual FieldValue *copy() { return new SFString(*this); }

    MyString            getEcmaScriptComment(MyString name, int flags);

    virtual bool        supportAnimation(bool x3d) { return false; }

    FieldValue         *getRandom(Scene *scene, int nodeType);
private:
    MyString            m_value;
};

#endif // _SFSTRING_H
