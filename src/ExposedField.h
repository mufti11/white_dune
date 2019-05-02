/*
 * ExposedField.h
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

#ifndef _EXPOSED_FIELD_H
#define _EXPOSED_FIELD_H

#ifndef _ELEMENT_H
# include "Element.h"
#endif
#ifndef _NODE_H   
# include "Node.h"   
#endif

class Field;
class FieldValue;

class ExposedField : public Element {
public:
                        ExposedField(int type, 
                                     const MyString &name, 
                                     FieldValue *value, 
                                     FieldValue *min = NULL, 
                                     FieldValue *max = NULL,
                                     int nodeType = ANY_NODE,
                                     int flags = 0, 
                                     const char **strings = NULL,
                                     const MyString &x3dName = "");
                        ExposedField(ExposedField *ptr); 
    virtual            ~ExposedField();
    virtual int         getElementType() const { return EL_EXPOSED_FIELD; }
    virtual const char *getElementName(bool x3d) const;
    virtual int         write(int filedes, int indent, int flags = 0) const;
    FieldValue         *getValue() const;
    FieldValue         *getMin() const { return m_min; }
    FieldValue         *getMax() const { return m_max; }
    int                 getNodeType() const { return m_nodeType; }
    void                addToNodeType(int nodeType) { m_nodeType += nodeType; }
    const char        **getStrings() const { return m_strings; }

    void                setField(Field *field) { m_field = field; }

    int                 getFieldIndex() const { return m_fieldIndex; }
    int                 getEventIn() const { return m_eventIn; }
    int                 getEventOut() const { return m_eventOut; }

    void                setFieldIndex(int field) { m_fieldIndex = field; }
    void                setEventIn(int eventIn) { m_eventIn = eventIn; }
    void                setEventOut(int eventOut) { m_eventOut = eventOut; }


private:
    FieldValue     *m_value;
    Field          *m_field;
    int             m_fieldIndex;
    int             m_eventIn;
    int             m_eventOut;
    FieldValue     *m_min;
    FieldValue     *m_max;
    int             m_nodeType;
    const char    **m_strings;
};

#endif // _EXPOSED_FIELD_H
