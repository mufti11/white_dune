/*
 * Field.h
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

#ifndef _FIELD_H
#define _FIELD_H

#ifndef _ELEMENT_H
# include "Element.h"
#endif
#ifndef _NODE_H   
# include "Node.h"   
#endif

#include "config.h"

class FieldValue;
class ExposedField;

class Field : public Element {
public:
                    Field(int type, 
                          const MyString &name, FieldValue *value,
                          ExposedField *exposedField = NULL,
                          FieldValue *min = NULL, FieldValue *max = NULL,
                          int nodeType = ANY_NODE, int flags = 0, 
                          const char **strings = NULL,
                          const MyString &x3dName = "");
                    Field(const Field *ptr);
    virtual        ~Field();
    virtual int     getElementType() const { return EL_FIELD; }
    virtual const char *getElementName(bool x3d) const;
    virtual int     write(int filedes, int indent, int flags = 0) const;
    FieldValue     *getDefault(bool x3d) const 
                       { 
#ifdef HAVE_NULL_COMPARE
                       if (this == NULL)
                           return NULL;
#endif
                       if (x3d && (m_x3dValue != NULL))
                           return m_x3dValue; 
                       return m_value; 
                       }
    void            setDefault(FieldValue *value);
    ExposedField   *getExposedField() const { return m_exposedField; }
    FieldValue     *getMin() const { return m_min; }
    FieldValue     *getMax() const { return m_max; }
    int             getNodeType() const { return m_nodeType; }
    void            addToNodeType(int nodeType);
    const char    **getStrings() const { return m_strings; }
    int             getEventIn() { return m_eventIn; }
    void            setEventIn(int eventIn) { m_eventIn = eventIn; } 
    void            addX3dDefault(FieldValue *x3dValue) 
                       { m_x3dValue = x3dValue; }
private:
    FieldValue     *m_value;
    FieldValue     *m_x3dValue;
    ExposedField   *m_exposedField;
    int             m_eventIn;
    FieldValue     *m_min;
    FieldValue     *m_max;
    int             m_nodeType;
    const char    **m_strings;
};
#endif // _FIELD_H
