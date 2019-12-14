/*
 * NodeGroup.h
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

#ifndef _NODE_GROUP_H
#define _NODE_GROUP_H

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

#include "SFMFTypes.h"
#include "StaticGroupNode.h"

class NodeNurbsGroup;
class NodeNurbsSet;
class NodeAnchor;

class ProtoGroup : public StaticGroupProto {
public:
                    ProtoGroup(Scene *scene);
                    ProtoGroup(Scene *scene, const char *name);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_GROUP; }

    virtual bool    isDeclaredInRwd_h() { return true; }

    void            addElements(void);

    FieldIndex render;
    FieldIndex doPickPass;
    FieldIndex pickMode;
    FieldIndex shadowObjectIdMapping;
};

class NodeGroup : public StaticGroupNode {
public:
                    NodeGroup(Scene *scene, Proto *proto);
                    NodeGroup(NodeNurbsGroup *nurbsGroup);
                    NodeGroup(NodeNurbsSet *nurbsSet);
                    NodeGroup(NodeAnchor *anchor);

    virtual int     getProfile(void) const;
    virtual Node   *copy() { return new NodeGroup(m_scene, m_proto); }

    virtual bool    hasX3domOnclick(void) { return true; }

    NodeList       *buildVrml97Children(MFNode *children);

    virtual void    setBoundingBox(void);

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    fieldMacros(SFBool,   render, ProtoGroup)
    fieldMacros(SFBool,   doPickPass, ProtoGroup)
    fieldMacros(SFString, pickMode, ProtoGroup)
    fieldMacros(SFString, shadowObjectIdMapping, ProtoGroup)
};

#endif // _NODE_GROUP_H
