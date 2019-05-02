/*
 * NodeSuperShape.h
 *
 * Copyright (C) 1999 Stephen F. White, 2004 J. "MUFTI" Scheurich
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

#ifndef _NODE_SUPER_SHAPE_H
#define _NODE_SUPER_SHAPE_H

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

class SuperShapeData
{
public:
    float ua;
    float ub;
    float um;
    float un1;
    float un2;
    float un3;
    float va;
    float vb;
    float vm;
    float vn1;
    float vn2;
    float vn3;
    float border;
    float bottomBorder;
    float creaseAngle;
    float size[3];
    Node *texCoord;
    int uTessellation;
    int vTessellation;
 
    bool cleanDoubleVertices;
};

class ProtoSuperShape : public Proto {
public:
                    ProtoSuperShape(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return DUNE_SUPER_SHAPE; }
    virtual int     getNodeClass() const 
                       { return PARAMETRIC_GEOMETRY_NODE | GEOMETRY_NODE; }

    virtual bool    isScriptedExternProto(void) { return true; }

    virtual bool    isMesh(void) { return true; }

    FieldIndex      ua;
    FieldIndex      ub;
    FieldIndex      um;
    FieldIndex      un1;
    FieldIndex      un2;
    FieldIndex      un3;
    FieldIndex      va;
    FieldIndex      vb;
    FieldIndex      vm;
    FieldIndex      vn1;
    FieldIndex      vn2;
    FieldIndex      vn3;
    FieldIndex      border;
    FieldIndex      bottom;
    FieldIndex      bottomBorder;
    FieldIndex      ccw;
    FieldIndex      creaseAngle;
    FieldIndex      size;
    FieldIndex      solid;
    FieldIndex      texCoord;
    FieldIndex      top;
    FieldIndex      uTessellation;
    FieldIndex      vTessellation;    
};

class NodeSuperShape : public MeshMorphingNode {
public:
                    NodeSuperShape(Scene *scene, Proto *proto);
                    ~NodeSuperShape();

    virtual int     getProfile(void) const { return PROFILE_IMMERSIVE; }  
    virtual int     getX3dVersion(void) const { return -1; }
    virtual Node   *copy() const { return new NodeSuperShape(*this); }

    virtual bool    hasNumbers4kids(void) { return true; } 

    virtual void    draw() { meshDraw(); }
    virtual void    drawHandles(void);
    virtual Vec3f   getHandle(int handle, int *constraint, int *field); 
    virtual void    setHandle(int handle, const Vec3f &v);

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual void    flip(int index);

    virtual bool    avoidProtoOnPureVrml(void) { return true; }
    virtual int     writeProto(int filedes);

    virtual Node   *toNurbs(int uTess,  int vTess, int uDegree, int vDegree);

    virtual bool    hasTwoSides(void) { return true; }
    virtual bool    isDoubleSided(void) { return !solid()->getValue(); }
    virtual void    toggleDoubleSided(void)
                       { solid(new SFBool(!solid()->getValue())); }
    virtual int     getSolidField() { return solid_Field(); }
    virtual void    flipSide(void) { ccw(new SFBool(!ccw()->getValue())); }

    virtual bool    maySetDefault(void) { return !(TheApp->is4Kids()); }

    fieldMacros(SFFloat, ua,            ProtoSuperShape)
    fieldMacros(SFFloat, ub,            ProtoSuperShape)
    fieldMacros(SFFloat, um,            ProtoSuperShape)
    fieldMacros(SFFloat, un1,           ProtoSuperShape)
    fieldMacros(SFFloat, un2,           ProtoSuperShape)
    fieldMacros(SFFloat, un3,           ProtoSuperShape)
    fieldMacros(SFFloat, va,            ProtoSuperShape)
    fieldMacros(SFFloat, vb,            ProtoSuperShape)
    fieldMacros(SFFloat, vm,            ProtoSuperShape)
    fieldMacros(SFFloat, vn1,           ProtoSuperShape)
    fieldMacros(SFFloat, vn2,           ProtoSuperShape)
    fieldMacros(SFFloat, vn3,           ProtoSuperShape)
    fieldMacros(SFFloat, border,        ProtoSuperShape)
    fieldMacros(SFBool,  bottom,        ProtoSuperShape)
    fieldMacros(SFFloat, bottomBorder,  ProtoSuperShape)
    fieldMacros(SFBool,  ccw,           ProtoSuperShape)
    fieldMacros(SFFloat, creaseAngle,   ProtoSuperShape)
    fieldMacros(SFVec3f, size,          ProtoSuperShape)
    fieldMacros(SFBool,  solid,         ProtoSuperShape)
    fieldMacros(SFNode,  texCoord,      ProtoSuperShape)
    fieldMacros(SFBool,  top,           ProtoSuperShape)
    fieldMacros(SFInt32, uTessellation, ProtoSuperShape)
    fieldMacros(SFInt32, vTessellation, ProtoSuperShape)
protected:
    void            createMesh(bool cleanDoubleVertices = true,
                               bool triangulate = true);
    void            createMesh(struct SuperShapeData &data);
    float           superellipse1xy(float angle1);
    float           superellipse1z(float angle1);
    float           superellipse2x(float angle2);
    float           superellipse2y(float angle2);
protected:
    virtual void   *initializeData(void);
    virtual void    loadDataFromInterpolators(void *data, Interpolator *inter,
                                              int field, float key);
    virtual void    createMeshFromData(void* data, bool optimize);
    virtual void    finalizeData(void* data);
};

#endif // _NODE_SUPER_SHAPE_H
