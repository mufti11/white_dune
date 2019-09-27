/*
 * NodePolyline2D.h
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

#ifndef _NODE_POLYLINE_2D_H
#define _NODE_POLYLINE_2D_H

#ifndef _CHAIN_BASED_NODE_H
#include "ChainBasedNode.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif
#ifndef _SCENE_H
#include "Scene.h"
#endif

#include "SFMFTypes.h"

class ProtoPolyline2D : public Proto {
public:
                    ProtoPolyline2D(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_POLYLINE_2D; }
    virtual int     getNodeClass() const;

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex point;
    x3domGeometryCommonFieldIndex()
    FieldIndex ccw;
    FieldIndex solid;
};

class NodePolyline2D : public ChainBasedNode {
public:
                    NodePolyline2D(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_IMMERSIVE; }
    virtual const char* getComponentName(void) const 
                           { return "Geometry2D"; }
    virtual int         getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 0; } 
    virtual Node   *copy() const { return new NodePolyline2D(*this); }

    virtual bool    isInvalidChildNode(void) { return !m_scene->isX3d(); }
    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual int     countPrimitives(void) {return 1;}
    virtual int     countPolygons(void) { return 0; }
    
    virtual bool    maySetDefault(void) { return false; }

    virtual bool    avoidProtoOnPureVrml(void) { return true; }

    int             write(int filedes, int indent, bool avoidUse = false);

    int             writeProto(int f); 

    fieldMacros(MFVec2f, point, ProtoPolyline2D)
    x3domGeometryCommonFieldMacros(ProtoPolyline2D) 
    fieldMacros(SFBool,  ccw,   ProtoPolyline2D)
    fieldMacros(SFBool,  solid, ProtoPolyline2D)

protected:
    void            createChain(void);
};

#endif // _NODE_POLYLINE_2D_H
