/*
 * NodeCone.h
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

#ifndef _NODE_CONE_H
#define _NODE_CONE_H

#ifndef _MESH_BASED_NODE_H
#include "MeshBasedNode.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif
#ifndef _VEC3F_H
#include "Vec3f.h"
#endif

#include "SFMFTypes.h"

class ProtoCone : public Proto {
public:
                    ProtoCone(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_CONE; }
    virtual int     getNodeClass() const 
                       { return PRIMITIVE_GEOMETRY_OR_MASS_DENSITY_MODEL_NODE |
                                GEOMETRY_NODE; }

    virtual bool    isMesh(void) { return true; }

    FieldIndex bottom;
    FieldIndex bottomRadius;
    FieldIndex height;
    FieldIndex side;
    FieldIndex solid;
    FieldIndex texCoord;
    x3domGeometryCommonFieldIndex()
    FieldIndex ccw;
    FieldIndex subdivision;
    FieldIndex top;
    FieldIndex topRadius;
};

class NodeCone : public MeshBasedNode {
public:
                    NodeCone(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() { return new NodeCone(m_scene, m_proto); }
    virtual bool    isInvalidChildNode(void) { return true; }
    virtual void    draw() { meshDraw(); }
    virtual void    drawHandles(void);
    virtual Vec3f   getHandle(int handle, int *constraint, int *field);
    virtual void    setHandle(int handle, const Vec3f &v);
    virtual bool    validHandle(int handle);


    virtual Node   *toNurbs(int nshell, int narea, int narcs, int uDegree, int vDegree);

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual int     countPrimitives(void) {return 1;}
    virtual int     countPolygons(void) { return 0; }

    virtual bool    canConvertToIndexedTriangleSet(void) { return hasParent(); }
    virtual Node   *toIndexedTriangleSet(bool hasCcw = true)
                       { 
                       return alreadyTriangulatedToIndexedTriangleSet(hasCcw); 
                       }

    virtual bool    shouldConvertToIndexedFaceSet(void) { return false; }

//    virtual bool    optimizeOnConvertionToIndexedTriangleSet(void) 
//                       { return false; }

    fieldMacros(SFBool,  bottom,       ProtoCone)
    fieldMacros(SFFloat, bottomRadius, ProtoCone)
    fieldMacros(SFFloat, height,       ProtoCone)
    fieldMacros(SFBool,  side,         ProtoCone)
    fieldMacros(SFBool,  solid,        ProtoCone)
    fieldMacros(SFNode,  texCoord,     ProtoCone)
    x3domGeometryCommonFieldMacros(ProtoCone) 
    fieldMacros(SFBool,  ccw,          ProtoCone)
    fieldMacros(SFInt32, subdivision,  ProtoCone)
    fieldMacros(SFBool,  top,          ProtoCone)
    fieldMacros(SFFloat, topRadius,    ProtoCone)

protected:
    void            createMesh(bool cleanDoubleVertices = true,
                               bool triangulate = true);

};

#endif // _NODE_CONE_H
