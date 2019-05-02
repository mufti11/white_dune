/*
 * NodeViewpointGroup.h
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

#ifndef _NODE_VIEWPOINT_GROUP_H
#define _NODE_VIEWPOINT_GROUP_H

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

class ProtoViewpointGroup : public Proto {
public:
                    ProtoViewpointGroup(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_VIEWPOINT_GROUP; }
    virtual int     getNodeClass() const 
                       { return CHILD_NODE | VIEWPOINT_GROUP_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex center;
    FieldIndex children;
    FieldIndex description;
    FieldIndex displayed;
    FieldIndex retainUserOffsets;
    FieldIndex size;
};

class NodeViewpointGroup : public Node {
public:
                    NodeViewpointGroup(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const { return "Navigation"; }
    virtual int         getComponentLevel(void) const { return 2; }
    virtual int     getX3dVersion(void) const { return 2; }    
    virtual Node   *copy() const { return new NodeViewpointGroup(*this); }

    fieldMacros(SFVec3f,  center,            ProtoViewpointGroup);
    fieldMacros(MFNode,   children,          ProtoViewpointGroup);
    fieldMacros(SFString, description,       ProtoViewpointGroup);
    fieldMacros(SFBool,   displayed,         ProtoViewpointGroup);
    fieldMacros(SFBool,   retainUserOffsets, ProtoViewpointGroup);
    fieldMacros(SFVec3f,  size,              ProtoViewpointGroup);
};

#endif
