/*
 * NodeDisk2D.h
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

#include "MeshBasedNode.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "Vec3f.h"
#include "SFMFTypes.h"

class ProtoDisk2D : public Proto {
public:
                    ProtoDisk2D(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_DISK_2D; }
    virtual int     getNodeClass() const
                       { return PARAMETRIC_GEOMETRY_NODE | GEOMETRY_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    virtual bool    isMesh(void) { return true; }

    FieldIndex innerRadius;
    FieldIndex outerRadius;
    FieldIndex solid;
    x3domGeometryCommonFieldIndex()
    FieldIndex ccw;
};

class NodeDisk2D : public MeshBasedNode {
public:
                    NodeDisk2D(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeDisk2D(*this); }

    virtual const char* getComponentName(void) const 
                           { return "Geometry2D"; }
    virtual int         getComponentLevel(void) const { return 2; }
    virtual int     getX3dVersion(void) const { return 0; }

    virtual void    draw() { meshDraw(); }

    virtual bool    hasTwoSides(void) { return true; }
    virtual bool    isDoubleSided(void) { return !solid()->getValue(); }
    virtual void    toggleDoubleSided(void)
                       { solid(new SFBool(!solid()->getValue())); }
    virtual int     getSolidField() { return solid_Field(); }

    virtual int     countPrimitives(void) {return 1;}
    virtual int     countPolygons(void) { return 0; }

    virtual int     writeProto(int f) { return writeX3dProto(f); }

    fieldMacros(SFFloat, innerRadius, ProtoDisk2D)
    fieldMacros(SFFloat, outerRadius, ProtoDisk2D)
    fieldMacros(SFBool,  solid,       ProtoDisk2D)
    x3domGeometryCommonFieldMacros(ProtoDisk2D) 
    fieldMacros(SFBool,  ccw,             ProtoDisk2D)

protected:
    void            createMesh(bool cleanDoubleVertices = true,
                               bool triangulate = true);

};

