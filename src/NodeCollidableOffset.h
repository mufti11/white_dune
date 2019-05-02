/*
 * NodeCollidableOffset.h
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

#ifndef _NODE_COLLIDABLE_OFFSET_H
#define _NODE_COLLIDABLE_OFFSET_H

#include "X3DNBodyCollidableNode.h"

#include "SFMFTypes.h"

class ProtoCollidableOffset : public X3DNBodyCollidableProto {
public:
                    ProtoCollidableOffset(Scene *scene);

    virtual int     getType() const { return X3D_COLLIDABLE_OFFSET; }

    virtual bool    isX3dInternalProto(void) { return true; }

    virtual Node   *create(Scene *scene);

    FieldIndex      collidable;
};

class NodeCollidableOffset : public X3DNBodyCollidableNode {
public:
                    NodeCollidableOffset(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return 2; }
    virtual Node   *copy() const { return new NodeCollidableOffset(*this); }

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

    fieldMacros(SFNode, collidable, ProtoCollidableOffset)
};

#endif
