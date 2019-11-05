/*
 * Field.cpp
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

#include "Field.h"
#include "ExposedField.h"
#include "Scene.h"
#include "FieldValue.h"

Field::Field(int type, const MyString &name, FieldValue *value,
             ExposedField *exposedField, 
             FieldValue *min, FieldValue *max, int nodeType, int flags, 
             const char **strings, const MyString &x3dName)
{
    m_name = name;
    m_x3dName = x3dName;
    m_type = type;
    m_value = value;
    m_x3dValue = NULL;
    if (m_value) m_value->ref();
    m_exposedField = exposedField;
    m_min = min;
    if (m_min) m_min->ref();
    m_max = max;
    if (m_max) m_max->ref();
    m_nodeType = nodeType;
    m_flags = flags;
    m_strings = strings;
    m_eventIn = -1;
}

Field::Field(const Field *ptr) : Element(ptr)
{
    m_name = ptr->m_name;  
    m_x3dName = ptr->m_x3dName;
    m_type = ptr->m_type; 
    m_value = ptr->m_value;
    m_x3dValue = ptr->m_x3dValue;
    if (m_value) m_value->ref();
    m_exposedField = ptr->m_exposedField;
    m_min = ptr->m_min;
    if (m_min) m_min->ref();
    m_max = ptr->m_max;
    if (m_max) m_max->ref();
    m_nodeType = ptr->m_nodeType;
    m_flags = ptr->m_flags;
    m_strings = ptr->m_strings;
    m_eventIn = ptr->m_eventIn;    
}

Field::~Field()
{
    bool delValue = false;
    if (m_value && m_value->getRefs() == 1)
        delValue = true;
    if (m_value) m_value->unref();
    if (delValue) m_value = NULL;

    bool delMin = false;
    if (m_min && m_min->getRefs() == 1)
        delMin = true;
    if (m_min) m_min->unref();
    if (delMin) m_min = NULL;

    bool delMax = false;
    if (m_max && m_max->getRefs() == 1)
        delMax = true;
    if (m_max) m_max->unref();
    if (delMax) m_max = NULL;
}

const char * 
Field::getElementName(bool x3d) const
{
    if (x3d)
        return "initializeOnly";
    else
        return "field";
}

void
Field::setDefault(FieldValue *value)
{ 
    m_value->unref();
    m_value = value;
    m_value->ref();
    m_x3dValue = NULL;
}

int Field::write(int f, int indent, int flags) const
{
    if (m_flags & (FF_HIDDEN | FF_STATIC)) return 0;

    if (m_value->isNullNode())
        flags |= NULL_VALUE;
    RET_ONERROR( writeElementPart(f, indent, flags) )
    if (isX3dXml(flags)) {
        if (flags & NULL_VALUE)
            if (!(flags & WITHOUT_VALUE))
                RET_ONERROR( mywritestr(f, " value='NULL'") )
        bool nodeField = (m_type == SFNODE) || (m_type == MFNODE);
        if ((flags & (WITHOUT_VALUE | NULL_VALUE)) && (!m_value->isUseNode())) {
            RET_ONERROR( mywritestr(f, "></field>\n") )
            TheApp->incSelectionLinenumber();
        } else if (nodeField) {
            RET_ONERROR( mywritestr(f, ">\n") )
            TheApp->incSelectionLinenumber();
            if (m_x3dValue) 
                RET_ONERROR( m_x3dValue->writeXml(f, indent + TheApp->GetIndent()) )
            else if (m_value) 
                RET_ONERROR( m_value->writeXml(f, indent + TheApp->GetIndent()) )
            RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
            RET_ONERROR( mywritestr(f, "</field>\n") )
            TheApp->incSelectionLinenumber();
        } else {
            if (m_x3dValue) {
                RET_ONERROR( mywritestr(f, " value=") )
                RET_ONERROR( m_x3dValue->writeXml(f, 0) )
            } else if (m_value) {
                RET_ONERROR( mywritestr(f, " value=") )
                RET_ONERROR( m_value->writeXml(f, 0) )
            }
            RET_ONERROR( mywritestr(f, " ></field>\n") )
            TheApp->incSelectionLinenumber();
        }
    } else {
        if (!(flags & WITHOUT_VALUE)) {
            if ((isX3d(flags)) && m_x3dValue) {
                RET_ONERROR( mywritestr(f, " ") )
                RET_ONERROR( m_x3dValue->write(f, 0) )
            } else if (m_value) {
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

void Field::addToNodeType(int nodeType)
{
    m_nodeType += nodeType;
    ExposedField *exposedField = getExposedField();
    if (exposedField) 
        exposedField->addToNodeType(nodeType);
}
