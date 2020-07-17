/*
 * NodeViewpoint.h
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

#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "Quaternion.h"
#include "Vec3f.h"
#include "ViewpointNode.h"
#include "swt.h"
#include "SFMFTypes.h"
#include "CoverNode.h"
#include "KambiViewpointCommonFields.h"
#include "DuneApp.h"

class ProtoViewpoint : public ViewpointProto {
public:
                        ProtoViewpoint(Scene *scene);
    virtual Node       *create(Scene *scene);

    virtual int         getType() const { return VRML_VIEWPOINT; }
    virtual int         getNodeClass() const 
                           { return CHILD_NODE | VIEWPOINT_NODE; }

    virtual bool        isDeclaredInRwd_h() { return true; }      

    FieldIndex position;
    FieldIndex type;

    virtual int     writeProto(int filedes) 
                       {
                       return ((Node *)this)->writeProto(filedes, 
                                    "urn:inet:www.hlrs.de:library:COVER",
                                    "coverNodes"
#ifdef HAVE_COVER_NODES_PROTO_URL
                                    , HAVE_COVER_NODES_PROTO_URL
#endif
                                                              );
                       }
    virtual bool    isCoverNode(void) { return true; }

    kambiViewpointCommonFieldIndex()
};


class NodeViewpoint : public ViewpointNode {
public:
                        NodeViewpoint(Scene *scene, Proto *proto);

    virtual int         getX3dVersion(void) const { return 0; }
    virtual Node       *copy() const { return new NodeViewpoint(*this); }

    virtual Vec3d       getPosition() const;
    virtual void        setPosition(const Vec3d &pos);

    virtual void        setField(int index, FieldValue *value, int cf = -1)
                            { Node::setField(index, value, cf); }
    virtual void        flip(int index);
    virtual void        swap(int fromTo);

    fieldMacros(SFVec3f,    position,          ProtoViewpoint)
    fieldMacros(SFString,   type,              ProtoViewpoint)

    kambiViewpointCommonFieldMacros(ProtoViewpoint)
};

