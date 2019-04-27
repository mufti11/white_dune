/*
 * NodeDISEntityTypeMapping.h
 *
 * Copyright (C) 2009 J. "MUFTI" Scheurich
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

#ifndef _NODE_DIS_ENTITY_TYPE_MAPPING_H
#define _NODE_DIS_ENTITY_TYPE_MAPPING_H

#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"

class ProtoDISEntityTypeMapping : public Proto {
public:
                    ProtoDISEntityTypeMapping(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_DIS_ENTITY_TYPE_MAPPING; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex url;
    FieldIndex category;
    FieldIndex country;
    FieldIndex domain;
    FieldIndex extra;
    FieldIndex kind;
    FieldIndex specific;
    FieldIndex subcategory;
};

class NodeDISEntityTypeMapping : public Node {
public:
                    NodeDISEntityTypeMapping(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const { return "DIS"; }
    virtual int         getComponentLevel(void) const { return 2; }
    virtual int     getX3dVersion(void) const { return 2; }
    virtual Node   *copy() const { return new NodeDISEntityTypeMapping(*this); }

    virtual void    load();

    fieldMacros(MFString, url, ProtoDISEntityTypeMapping);
    fieldMacros(SFInt32, category, ProtoDISEntityTypeMapping);
    fieldMacros(SFInt32, country, ProtoDISEntityTypeMapping);
    fieldMacros(SFInt32, domain, ProtoDISEntityTypeMapping);
    fieldMacros(SFInt32, extra, ProtoDISEntityTypeMapping);
    fieldMacros(SFInt32, kind, ProtoDISEntityTypeMapping);
    fieldMacros(SFInt32, specific, ProtoDISEntityTypeMapping);
    fieldMacros(SFInt32, subcategory, ProtoDISEntityTypeMapping);

protected:
    MyString m_baseURL;
};

#endif
