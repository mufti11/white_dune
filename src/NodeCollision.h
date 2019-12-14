/*
 * NodeCollision.h
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

#ifndef _NODE_COLLISION_H
#define _NODE_COLLISION_H

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

class ProtoCollision : public WonderlandExportProto {
public:
                    ProtoCollision(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_COLLISION; }
    virtual int     getNodeClass() const { return GROUPING_NODE | CHILD_NODE; }

    virtual bool    isDeclaredInRwd_h() { return true; }      

    virtual bool    isCRouteSource(void) { return true; }       

    FieldIndex children;
    FieldIndex collide;    // x3dname: enabled
    FieldIndex bboxCenter;
    FieldIndex bboxSize;
    FieldIndex proxy;
    FieldIndex render;
};

class NodeCollision : public Node {
public:
                    NodeCollision(Scene *scene, Proto *proto);
    virtual int     getProfile(void) const { return PROFILE_IMMERSIVE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() { return new NodeCollision(m_scene, m_proto); }

    virtual void    preDraw() { children()->preDraw(); }
    virtual void    draw(int pass) { children()->draw(pass, children_Field()); }

    virtual bool    hasNumbers4kids(void) { return true; } 
 
    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual int     countPolygons(void)
                       { return children()->countPolygons(); } 
    virtual int     countPrimitives(void) 
                       { return children()->countPrimitives(); }

    virtual int     countPolygons1Sided(void)
                       { return children()->countPolygons1Sided(); } 
    virtual int     countPolygons2Sided(void)
                       { return children()->countPolygons2Sided(); } 

    virtual bool    showFields() { return true; }

    fieldMacros(MFNode,  children,   ProtoCollision)
    fieldMacros(SFBool,  collide,    ProtoCollision)
    fieldMacros(SFVec3f, bboxCenter, ProtoCollision)
    fieldMacros(SFVec3f, bboxSize,   ProtoCollision)
    fieldMacros(SFNode,  proxy,      ProtoCollision)
    fieldMacros(SFBool,  render,     ProtoCollision)
};

#endif // _NODE_COLLISION_H
