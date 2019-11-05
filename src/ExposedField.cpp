/*
 * ExposedField.cpp
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

#include <stdio.h>
#include "stdafx.h"

#include "ExposedField.h"
#include "Scene.h"
#include "FieldValue.h"
#include "Field.h"   // for FF_HIDDEN

ExposedField::ExposedField(int type, const MyString &name, FieldValue *value, 
                           FieldValue *min, FieldValue *max,
                           int nodeType, int flags, const char **strings,
                           const MyString &x3dName)
{
    m_type = type;
    m_name = name;
    m_x3dName = x3dName;
    m_value = value;
    if (m_value) m_value->ref();
    m_field = NULL;
    m_min = min;
    if (m_min) m_min->ref();
    m_max = max;
    if (m_max) m_max->ref();
    m_nodeType = nodeType;
    m_flags = flags;
    m_strings = strings;
}

ExposedField::ExposedField(ExposedField *ptr) : Element(ptr)
{ 
    m_type = ptr->m_type;
    m_name = ptr->m_name;
    m_x3dName = ptr->m_x3dName;
    m_value = ptr->m_value;
    m_field = ptr->m_field;
    if (m_value) m_value->ref();
    m_min = ptr->m_min;
    if (m_min) m_min->ref();
    m_max = ptr->m_max;
    if (m_max) m_max->ref();
    m_nodeType = ptr->m_nodeType;
    m_flags = ptr->m_flags;
    m_strings = ptr->m_strings;
}

ExposedField::~ExposedField()
{
    if (m_value) m_value->unref();
    if (m_min) m_min->unref();
    if (m_max) m_max->unref();
}

const char *
ExposedField::getElementName(bool x3d) const
{
    if (x3d)
        return "inputOutput";
    else
        return "exposedField";
}

int 
ExposedField::write(int f, int indent, int flags) const
{
    if (m_flags & (FF_STATIC | FF_HIDDEN)) return(0);

    if (m_value->isNullNode())
        flags |= NULL_VALUE;
    RET_ONERROR( writeElementPart(f, indent, flags) )
    if (isX3dXml(flags)) {
        if (flags & NULL_VALUE)
            if (!(flags & WITHOUT_VALUE))
                RET_ONERROR( mywritestr(f, " value='NULL'") )
        bool nodeField = (m_type == SFNODE) || (m_type == MFNODE);
        if ((flags & (WITHOUT_VALUE | NULL_VALUE)) && (!m_value->isUseNode())) {
            RET_ONERROR( mywritestr(f, " ></field>\n") )
            TheApp->incSelectionLinenumber();
        } else if (nodeField) {
            RET_ONERROR( mywritestr(f, ">\n") )
            TheApp->incSelectionLinenumber();
            if (m_value) 
                RET_ONERROR( m_value->writeXml(f, indent + TheApp->GetIndent()) )
            RET_ONERROR( indentf(f, indent) )
            RET_ONERROR( mywritestr(f, "</field>\n") )
            TheApp->incSelectionLinenumber();
        } else {
            if (m_value) {
                RET_ONERROR( mywritestr(f, " value=") )
                RET_ONERROR( m_value->writeXml(f, 0) )
            }
            RET_ONERROR( mywritestr(f, " ></field>\n") )
            TheApp->incSelectionLinenumber();
        }
    } else {
        if (!(flags & WITHOUT_VALUE)) {
            if (m_value) {
                RET_ONERROR( mywritestr(f, " ") )
                RET_ONERROR( m_value->write(f, indent) )
            }
        } else {
            RET_ONERROR( mywritestr(f, "\n") )
            TheApp->incSelectionLinenumber();
        }
    }
    return 0;
}

FieldValue *
ExposedField::getValue() const 
{
    if (m_field == NULL)
        return m_value;
    return m_field->getDefault(true); 
}

