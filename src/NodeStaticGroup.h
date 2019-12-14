/*
 * NodeStaticGroup.h
 *
 * Copyright (C) 1999 Stephen F. White, 2008 J. "MUFTI" Scheurich
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

#ifndef _NODE_STATIC_GROUP_H
#define _NODE_STATIC_GROUP_H

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

#include "GroupNode.h"

#include "SFMFTypes.h"

class ProtoStaticGroup : public GroupProto {
public:
                    ProtoStaticGroup(Scene *scene);
                    ProtoStaticGroup(Scene *scene, const char *name, 
                                     int extraChrildrenNodeClass = 0);
    virtual Node   *create(Scene *scene);

    void            addElements(void);

    virtual int     getType() const { return X3D_STATIC_GROUP; }
    virtual int     getNodeClass() const { return GROUPING_NODE | 
                                                  CHILD_NODE |
                                                  BOUNDED_OBJECT_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex bboxCenter;
    FieldIndex bboxSize;
    FieldIndex bvhType;
    FieldIndex debug;
    FieldIndex maxDepth;
    FieldIndex maxObjectsPerNode;
    FieldIndex minRelativeBBoxSize;
    FieldIndex render;
    FieldIndex showDebugBoxVolumes;
};

class NodeStaticGroup : public GroupNode {
public:
                    NodeStaticGroup(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const;
    virtual int         getComponentLevel(void) const;
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() { return new NodeStaticGroup(m_scene, m_proto); }

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual void    setBoundingBox(void);

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    fieldMacros(SFVec3f, bboxCenter, ProtoStaticGroup)
    fieldMacros(SFVec3f, bboxSize, ProtoStaticGroup)
    fieldMacros(SFString, bvhType, ProtoStaticGroup)
    fieldMacros(SFBool, debug, ProtoStaticGroup)
    fieldMacros(SFInt32, maxDepth, ProtoStaticGroup)
    fieldMacros(SFInt32, maxObjectsPerNode, ProtoStaticGroup)
    fieldMacros(SFFloat, minRelativeBBoxSize, ProtoStaticGroup)
    fieldMacros(SFBool, render, ProtoStaticGroup)
    fieldMacros(SFBool, showDebugBoxVolumes, ProtoStaticGroup)
};

#endif
