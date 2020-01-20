/*
 * NodeCollidableShape.h
 *
 * Copyright (C) 1999 Stephen F. White, 2007 J. "MUFTI" Scheurich
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

#include "X3DNBodyCollidableNode.h"
#include "SFMFTypes.h"

class ProtoCollidableShape : public X3DNBodyCollidableProto {
public:
                    ProtoCollidableShape(Scene *scene);

    virtual int     getType() const { return X3D_COLLIDABLE_SHAPE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    virtual Node   *create(Scene *scene);

    FieldIndex      shape;
    FieldIndex      render;
    FieldIndex      transform2;
};

class NodeCollidableShape : public X3DNBodyCollidableNode {
public:
                    NodeCollidableShape(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return 2; }
    virtual Node   *copy() const { return new NodeCollidableShape(*this); }

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual void    preDraw();
    virtual void    draw(int pass);

    virtual int     countPolygons(void);
    virtual int     countPrimitives(void);

    virtual int     countPolygons1Sided(void);
    virtual int     countPolygons2Sided(void);

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual float   getTransparency(void);

    virtual Node   *convert2X3d(void);
    virtual Node   *convert2Vrml(void);

    virtual bool    canWriteAc3d();
    virtual bool    canWriteCattGeo();

    virtual void    transformBranch();
    void            RigidBodyTransform();

    virtual void    transform() 
                       { 
                       RigidBodyTransform();
                       X3DNBodyCollidableNode::transform();
                       }
    virtual void    transformForHandle(int handle) 
                       { 
                       RigidBodyTransform();
                       X3DNBodyCollidableNode::transformForHandle(handle);
                       }                       

    fieldMacros(SFNode, shape, ProtoCollidableShape)
    fieldMacros(SFBool, render, ProtoCollidableShape)
    fieldMacros(SFNode, transform2, ProtoCollidableShape)

protected:
    bool            m_inSetup;
};

