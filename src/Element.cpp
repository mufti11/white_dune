/*
 * Element.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2009 J. "MUFTI" Scheurich
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

#include "stdafx.h"
#include "Element.h"
#include "Types.h"
#include "x3dFlags.h"
#include "DuneApp.h"

Element::Element(const Element *ptr)
{
    m_type = ptr->m_type;
    m_name = ptr->m_name;
    m_x3dName = ptr->m_x3dName;
    m_flags = ptr->m_flags;
    for (size_t i = 0; i < ptr->m_isArray.size(); i++)
        m_isArray[i] = ptr->m_isArray[i];
    m_appinfo = "";
    m_appinfo += ptr->m_appinfo; 
    m_documentation = "";
    m_documentation += ptr->m_documentation;
}


const MyString &
Element::getName(bool x3d) const 
{
#ifdef HAVE_NULL_COMPARE
    if (this == NULL)
        return *new MyString("");
#endif
    if (x3d && (m_x3dName.length() > 0))
        return m_x3dName;
    else
        return m_name; 
}

int
Element::getType() const 
{ 
#ifdef HAVE_NULL_COMPARE
    if (this == NULL) 
        return -1; 
    else
#endif
    return m_type; 
}

#include <stdio.h>
#include "Proto.h"
void
Element::addIs(Node *node, int field, int elementType, 
               Proto *origProto, int origField, int flags)
{
    m_isArray.append(new IsElement(node, field, elementType,
                                   origProto, origField));
    m_flags |= FF_IS | flags;
}

int Element::writeElementPart(int f, int indent, int flags) const
{
    bool x3d = isX3d(flags);
    const char *appinfo = getAppinfo();
    const char *documentation = getDocumentation();
    if (isX3dXml(flags)) {
        RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
        RET_ONERROR( mywritestr(f, "<field name='") )
        RET_ONERROR( mywritestr(f, (const char *) getName(x3d)) )
        RET_ONERROR( mywritestr(f, "' type='") )
        RET_ONERROR( mywritestr(f, typeEnumToString(m_type)) )
        RET_ONERROR( mywritestr(f, "' accessType='") )
        RET_ONERROR( mywritestr(f, getElementName(x3d)) )
        RET_ONERROR( mywritestr(f, "'") )
        if (strlen(appinfo) > 0) {
            RET_ONERROR( mywritestr(f, " appinfo='") )
            RET_ONERROR( mywritestr(f, appinfo) )
            RET_ONERROR( mywritestr(f, "'") )
        }
        if (strlen(documentation) > 0) {
            RET_ONERROR( mywritestr(f, " documentation='") )
            RET_ONERROR( mywritestr(f, documentation) )
            RET_ONERROR( mywritestr(f, "'") )
        }
    } else {
        if (strlen(appinfo) > 0) {
            RET_ONERROR( indentf(f ,indent) )
            RET_ONERROR( mywritestr(f, "# ") )
            RET_ONERROR( mywritestr(f, getName(x3d)) )
            RET_ONERROR( mywritestr(f, " appinfo='") )
            RET_ONERROR( mywritestr(f, appinfo) )
            RET_ONERROR( mywritestr(f, "'") )
            RET_ONERROR( mywritestr(f, "\n") )
            TheApp->incSelectionLinenumber();
        }
        if (strlen(documentation) > 0) {
            RET_ONERROR( indentf(f ,indent) )
            RET_ONERROR( mywritestr(f, "# ") )
            RET_ONERROR( mywritestr(f, getName(x3d)) )
            RET_ONERROR( mywritestr(f, " documentation='") )
            RET_ONERROR( mywritestr(f, documentation) )
            RET_ONERROR( mywritestr(f, "'") )
            RET_ONERROR( mywritestr(f, "\n") )
            TheApp->incSelectionLinenumber();
        }
        RET_ONERROR( indentf(f ,indent) )
        RET_ONERROR( mywritestr(f, getElementName(x3d)) )
        RET_ONERROR( mywritestr(f, " ") )
        RET_ONERROR( mywritestr(f, typeEnumToString(m_type)) )
        RET_ONERROR( mywritestr(f, " ") )
        RET_ONERROR( mywritestr(f, (const char *) getName(x3d)) )
    }
    return(0);
}

#define MAX_INDENT 800

int indentf(int f, int indent)
{
/*
    int tabs = indent / 8;
    int spaces = indent % 8;
    char *buf = new char[tabs + spaces + 1];
    char *b = buf;
    int ret;

    for (int i = 0; i < tabs; i++)
        *b++ = '\t';

    for (int j = 0; j < spaces; j++)
        *b++ = ' ';

    *b = '\0';
    ret= mywritestr(f , buf);
    delete[] buf;
    return(ret);
*/

    for (int i = 0; i < indent && i < MAX_INDENT; i++)
        if (mywritestr(f , " ") != 0)
            return -1;
    return 0;
}


