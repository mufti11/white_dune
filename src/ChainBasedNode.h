/*
 * ChainBasedNode.h
 *
 * Copyright (C) 2003 Th. Rothermel, 2007 J. "MUFTI" Scheurich
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

#include "GeometryNode.h"
#include "Vec3f.h"

class Scene;
class MFVec3f;
class MFInt32;
class NodePositionInterpolator;
class NodeOrientationInterpolator;

typedef Vec3f (*getNormalCallback)(int index, int indices, void *data);

class ChainBasedNode : public Node {
public:
                    ChainBasedNode(Scene *scene, Proto *proto);
    virtual        ~ChainBasedNode();

    virtual void    update(void) { m_chainDirty = true; }
    virtual void    reInit(void);

    virtual void    createChain(void) = 0;

    virtual bool    isChainBasedNode(void) { return true; }

    virtual void    draw(void);

    virtual int     write(int f, int indent);

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual bool    canConvertToIndexedLineSet(void) { return hasParent(); }
    virtual Node   *toIndexedLineSet(void);

    virtual bool    canConvertToPositionInterpolator(void) 
                       { return hasParent(); }
    virtual Node   *toPositionInterpolator(void);
    Node           *toPositionInterpolator(NodePositionInterpolator* node);

    virtual bool    canConvertToOrientationInterpolator(void)
                        { return hasParent(); }
    virtual Node   *toOrientationInterpolator(Direction direction);
    void            toOrientationInterpolator(NodeOrientationInterpolator*
                                              rotationInterpolator,
                                              const float *rotationAxis);

    virtual int     writeLdrawDat(int f, int indent);

    virtual Vec3f   getMinBoundingBox(void);
    virtual Vec3f   getMaxBoundingBox(void);

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    const Vec3f    *getChain();
    int             getChainLength();

protected:
    Vec3f           accountProjection(Vec3f vector, Vec3f normal);
    float           accountAngle(Vec3f vec1, Vec3f vec2);
    Quaternion      accountQuat(Vec3f vec1, Vec3f vec2);

protected:
    MyArray<Vec3f>  m_chain;
    bool            m_chainDirty;
};

class ChainBasedGeometryNode : public ChainBasedNode {
public:
                    ChainBasedGeometryNode(Scene *scene, Proto *proto);
    virtual        ~ChainBasedGeometryNode();

    GeometryNodeDeclarationMacros()

};


