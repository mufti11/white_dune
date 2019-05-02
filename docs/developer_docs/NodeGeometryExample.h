/*
 * NodeGeometryExample.h
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
 * not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

// the following is a typical template for a geometric node,
// that is animatable (not rigid during time)

#ifndef _NODE_GEOMETRY_EXAMPLE_H
#define _NODE_GEOMETRY_EXAMPLE_H

#ifndef _MESH_MORPHING_NODE_H
#include "MeshMorphingNode.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"

class Mesh;

class GeometryExampleData
{
public:
    sfsomething field1;
    mfsomething field2;
    float creaseAngle;
 
    bool cleanDoubleVertices;
};

class ProtoGeometryExample : public Proto {
public:
                    ProtoGeometryExample(Scene *scene);
    virtual Node   *create(Scene *scene);
    FieldIndex      field1;
    FieldIndex      field2;

    // creaseAngle and solid are typical fields needed for geometry,
    // see IndexedFaceSet in the VRML97 specification for further description
    FieldIndex      creaseAngle;
    FieldIndex      solid;
};

// for rigid shapes, use "MeshBasedNode" instead of "MeshMorphingNode"
class NodeGeometryExample : public MeshMorphingNode {
public:
                    NodeGeometryExample(Scene *scene, Proto *proto);
                    ~NodeGeometryExample();
    virtual int     getType() const { return DUNE_GEOMETRY_EXAMPLE; }
    virtual int     getNodeClass() const { return PARAMETRIC_GEOMETRY_NODE; }
    virtual int     getProfile(void) const { return PROFILE_IMMERSIVE; }  
    virtual Node   *copy() const { return new NodeGeometryExample(*this); }

    virtual void    draw() { meshDraw(); }
    virtual void    drawHandles();
    virtual Vec3f   getHandle(int handle, int *constraint, int *field); 
    virtual void    setHandle(int handle, const Vec3f &v);

    virtual void    setField(int index, FieldValue *value);

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual int     writeProto(int filedes);
    int             write(int filedes, int indent);

    virtual bool    hasTwoSides(void) { return true; }
    virtual bool    isDoubleSided(void) { return !solid()->getValue(); }
    virtual void    toggleDoubleSided(void)
                       { solid(new SFBool(!solid()->getValue())); }
    virtual void    flipSide(void) { ccw(new SFBool(!ccw()->getValue())); }

    // not needed for rigid shapes
    virtual int     getSolidField() { return solid_Field(); }

    fieldMacros(SFSomething, field1,            ProtoGeometryExample)
    fieldMacros(MFSomething, field2,            ProtoGeometryExample)
    fieldMacros(SFFloat, creaseAngle,   ProtoGeometryExample)
    fieldMacros(SFBool,  solid,         ProtoGeometryExample)
protected:
    void            createMesh(bool cleanDoubleVertices = true);
// all following functions are not needed for rigid shapes
    void            createMesh(struct GeometryExampleData &data);
protected:
    virtual void   *initializeData(void);
    virtual void    loadDataFromInterpolators(void *data, Interpolator *inter,
                                              int field, float key);
    virtual void    createMeshFromData(void* data, bool optimize);
    virtual void    finalizeData(void* data);
};

#endif // _NODE_GEOMETRY_EXAMPLE_H
