/*
 * NodeMultiPart.h
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

#ifndef _NODE_MULTI_PART_H
#define _NODE_MULTI_PART_H

#ifndef _NODE_H
#include "ExternTheApp.h"
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "NodeGroup.h"

#include "SFMFTypes.h"

class ProtoMultiPart : public ProtoGroup {
public:
                    ProtoMultiPart(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_MULTI_PART; }
    virtual int     getNodeClass() const { return GROUPING_NODE | 
                                                  BOUNDED_OBJECT_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex initialVisibility;
    FieldIndex isPickable;
    FieldIndex load;
    FieldIndex mapDEFToID;
    FieldIndex nameSpaceName;
    FieldIndex render;
    FieldIndex solid;
    FieldIndex sortKey;
    FieldIndex sortType;
    FieldIndex url;
    FieldIndex urlIDMap;
};

class NodeMultiPart : public NodeGroup {
public:
                    NodeMultiPart(Scene *scene, Proto *proto);

    virtual Node   *copy() { return new NodeMultiPart(m_scene, m_proto); }

    virtual int     getX3dVersion(void) const { return -1; }

    virtual void    load();

    fieldMacros(SFString, initialVisibility, ProtoMultiPart)
    fieldMacros(SFBool, isPickable, ProtoMultiPart)
    fieldMacros(SFBool, load, ProtoMultiPart)
    fieldMacros(SFBool, mapDEFToID, ProtoMultiPart)
    fieldMacros(MFString, nameSpaceName, ProtoMultiPart)
    fieldMacros(SFBool, render, ProtoMultiPart)
    fieldMacros(SFBool, solid, ProtoMultiPart)
    fieldMacros(SFInt32, sortKey, ProtoMultiPart)
    fieldMacros(SFString, sortType, ProtoMultiPart)
    fieldMacros(MFString, url, ProtoMultiPart)
    fieldMacros(MFString, urlIDMap, ProtoMultiPart)
};

#endif
