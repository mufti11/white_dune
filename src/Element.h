/*
 * Element.h
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

#include "MyString.h"
#include "Array.h"
#include "x3dFlags.h"

enum {
    EL_UNKNOWN,

    EL_EVENT_IN,
    EL_EVENT_OUT,
    EL_FIELD,
    EL_EXPOSED_FIELD,

    EL_INITIALIZE,
    EL_EVENTS_PROCESSED,
    EL_SHUTDOWN
};

enum {
    FF_HIDDEN                = 1<<0,
    FF_FIXEDSTRINGS          = 1<<1,
    FF_URL                   = 1<<2,
    FF_WONDERLAND_ART        = 1<<3,
    FF_DOUBLE                = 1<<4,
    FF_3_DOUBLE              = 1<<5,
    FF_DELETED               = 1<<6,
    FF_STATIC                = 1<<7,
    FF_DEPRECATED            = 1<<8,
    FF_IN_SCRIPT             = 1<<9,
    FF_SCRIPT_URL            = 1<<10,
    FF_COVER_ONLY            = 1<<11,
    FF_X3D_ONLY              = 1<<12,
    FF_VRML_ONLY             = 1<<13,
    FF_ALWAYS                = 1<<14,
    FF_NEVER                 = 1<<15,
    FF_KAMBI_ONLY            = 1<<16,
    FF_KAMBI_DEPRECATED      = 1<<17,
    FF_4KIDS                 = 1<<18,
    FF_X3DOM_ONLY            = 1<<19,
    FF_ROOT_ONLY             = 1<<20,
    FF_UPDATE                = 1<<21,
    FF_IS_NAME               = 1<<22,
    FF_IS                    = 1<<23,
    EIF_IS                   = 1<<24,
    EOF_IS                   = 1<<25,
    EIF_RECOMMENDED          = 1<<26,
    EOF_RECOMMENDED          = 1<<27,
    EIF_IS_HIDDEN            = 1<<28,
    EOF_IS_HIDDEN            = 1<<29,
    EIF_WONDERLAND_SUPPORTED = 1<<30,
    EOF_WONDERLAND_SUPPORTED = 1<<31
};

class Proto;
class Element;

#include "Node.h"

class IsElement {
public:
          IsElement() 
             { 
             m_nodeIndex = -1; 
             m_node = NULL; 
             m_field = -1;
             m_elementType = EL_UNKNOWN;
             m_originalProto = NULL;
             m_originalField = -1;
             }
          IsElement(Node *node, int field, int elementType, 
                    Proto *origProto, int origField) 
             { 
             m_nodeIndex = -1; 
             m_node = node; 
             m_field = field; 
             m_elementType = elementType;
             m_originalProto = origProto;
             m_originalField = origField;
             }
    Node* getNode(void)  { return m_node; }
    int   getField(void) { return m_field; }
    int   getElementType(void) { return m_elementType; }
    int   getNodeIndex(void) { return m_nodeIndex; }
    void  setNodeIndex(int nodeIndex) { m_nodeIndex = nodeIndex; } 
protected:
    int    m_nodeIndex;
    Node  *m_node;
    int    m_field;
    Proto *m_originalProto;
    int    m_originalField;
    int    m_elementType;
};

class Element {
public:
                        Element() {}
                        Element(const Element *ptr);
    virtual            ~Element() {}
    virtual int         getElementType() const = 0;
    virtual const char *getElementName(bool x3d) const = 0;
    const MyString     &getName(bool x3d) const;
    int                 getType() const;
    int                 getFlags() const { return m_flags; }
    void                setFlags(int flags) { m_flags = flags; }
    void                addFlags(int flags) { m_flags |= flags; }
    void                removeFlags(int flags) 
                           { m_flags = m_flags & !(flags & 0xffff); }

    virtual int         write(int filedes, int indent, int flag = 0) 
                              const = 0;
    virtual int         writeElementPart(int filedes, int indent, int flag) 
                                         const;

    void                addIs(Node *node, int field, int elementType,
                              Proto *origProto, int origField, int flags = 0);
    int                 getNumIs(void) { return m_isArray.size(); }
    Node               *getIsNode(int i) 
                            { 
                                if (m_isArray[i])
                                    return m_isArray[i]->getNode();
                                return NULL;
                            }
    int                 getIsField(int i) 
                            { 
                            int ret = -1;
                            if (m_isArray[i] != NULL)
                                ret = m_isArray[i]->getField();
                            return ret;
                            } 
    int                 getIsElementType(int i) 
                            { return m_isArray[i]->getElementType(); }
    void                removeIs(int i) { m_isArray.remove(i); }

    int                 getIsNodeIndex(int i) 
                           { return m_isArray[i]->getNodeIndex(); }
    void                setIsNodeIndex(int i, int nodeIndex) 
                           { m_isArray[i]->setNodeIndex(nodeIndex); }

    void                setAppinfo(const MyString& appinfo) 
                           { m_appinfo = appinfo; } 
    MyString            getAppinfo() const 
                           { return m_appinfo; } 
    void                setDocumentation(const MyString& documentation) 
                           { m_documentation = documentation; } 
    MyString            getDocumentation() const 
                           { return m_documentation; } 
protected:
    int                 m_type;
    MyString            m_name;
    MyString            m_x3dName;
    int                 m_flags;
    MyArray<IsElement *> m_isArray;

    MyString            m_appinfo;
    MyString            m_documentation;
};

int indentf(int filedes, int indent);


