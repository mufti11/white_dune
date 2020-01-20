/*
 * NodePolypoint2D.h
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

#include "GeometryNode.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "SFMFTypes.h"

class NodePointSet;

class ProtoPolypoint2D : public GeometryProto {
public:
                    ProtoPolypoint2D(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_POLYPOINT_2D; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex point;
};

class NodePolypoint2D : public GeometryNode {
public:
                    NodePolypoint2D(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual int     getX3dVersion(void) const { return 0; } 
    virtual Node   *copy() const { return new NodePolypoint2D(*this); }

    virtual void    draw();

    virtual const char* getComponentName(void) const 
                           { return "Geometry2D"; }
    virtual int         getComponentLevel(void) const { return 1; }

    virtual bool    isInvalidChildNode(void) { return true; }
    virtual bool    hasBoundingBox(void)     { return true; }

    virtual int     countPrimitives(void) {return 1;}
    virtual int     countPolygons(void) { return 0; }
    
    virtual Vec3f   getMinBoundingBox(void);
    virtual Vec3f   getMaxBoundingBox(void);
    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual bool    maySetDefault(void) { return false; }

    virtual int     writeProto(int f) { return writeX3dProto(f); }

    fieldMacros(MFVec2f, point, ProtoPolypoint2D)

    void            pointDraw();
    NodePointSet   *convertToPointSet(void);
};

