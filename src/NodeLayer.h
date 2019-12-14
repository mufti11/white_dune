/*
 * NodeLayer.h
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

#ifndef _NODE_LAYER_H
#define _NODE_LAYER_H

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

class ProtoLayer : public GroupProto {
public:
                    ProtoLayer(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_LAYER; }
    virtual int     getNodeClass() const { return LAYER_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex isPickable;
    FieldIndex viewport;
};

class NodeLayer : public GroupNode {
public:
                        NodeLayer(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const { return "Layering"; }
    virtual int         getComponentLevel(void) const { return 1; }
    virtual int         getX3dVersion(void) const { return 2; }
    virtual Node       *copy() { return new NodeLayer(m_scene, m_proto); }

    virtual bool        showFields() { return true; }

    virtual int         getChildrenField(void) { return -1; }

    fieldMacros(SFBool, isPickable, ProtoLayer);
    fieldMacros(SFNode, viewport,   ProtoLayer);
};

#endif
