/*
 * NodeSphere.h
 *
 * Copyright (C) 1999 Stephen F. White
 *               2003 Th. Rothermel
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

class ProtoSphere : public Proto {
public:
                    ProtoSphere(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_SPHERE; }
    virtual int     getNodeClass() const 
                       { return PRIMITIVE_GEOMETRY_OR_MASS_DENSITY_MODEL_NODE |
                                GEOMETRY_NODE; }
    
    virtual bool    isMesh(void) { return true; }

    FieldIndex radius;
    FieldIndex solid;
    FieldIndex texCoord;
    x3domGeometryCommonFieldIndex()
    FieldIndex ccw;
    FieldIndex subdivision;
};

class NodeSphere : public MeshBasedNode {
public:
                    NodeSphere(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual int     getX3dVersion(void) const { return 0; } 
    virtual Node   *copy() const { return new NodeSphere(*this); }

    virtual bool    isInvalidChildNode(void) { return true; }

    virtual void    draw() { meshDraw(); }
    virtual void    drawHandles(void);
    virtual Vec3f   getHandle(int handle, int *constraint, int *field); 
    virtual void    setHandle(int handle, const Vec3f &v);
    virtual bool    validHandle(int handle);

    virtual Node   *toNurbs(int narcslong,  int narcslat, int uDegree, int vDegree);

    virtual int     countPrimitives(void) {return 1;}
    virtual int     countPolygons(void) { return 0; }

    virtual bool    shouldConvertToIndexedFaceSet(void) { return false; }

    fieldMacros(SFFloat, radius,      ProtoSphere)
    fieldMacros(SFBool,  solid,       ProtoSphere)
    fieldMacros(SFNode,  texCoord,    ProtoSphere)
    x3domGeometryCommonFieldMacros(ProtoSphere) 
    fieldMacros(SFBool,  ccw,         ProtoSphere)
    fieldMacros(SFVec2f, subdivision, ProtoSphere)

protected:
    void            createMesh(bool cleanDoubleVertices = true,
                               bool triangulate = true);

};

