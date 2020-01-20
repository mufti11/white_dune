/*
 * NodeSuperRevolver.h
 *
 * Copyright (C) 1999 Stephen F. White, 2008 J. "MUFTI" Scheurich
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

#include "ProtoMacros.h"
#include "Proto.h"
#include "NodeNurbsCurve.h"
#include "MeshMorphingNode.h"
#include "SFMFTypes.h"

class SuperRevolverData
{
public:
    float a;
    float b;
    float m;
    float n1;
    float n2;
    float n3;
    float border;
    float bottomBorder;
    float *controlPoint;
    float *weight;
    float *knot;
};


class ProtoSuperRevolver : public Proto {
public:
                    ProtoSuperRevolver(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return DUNE_SUPER_REVOLVER; }
    virtual int     getNodeClass() const 
                       { return PARAMETRIC_GEOMETRY_NODE | GEOMETRY_NODE; }

    virtual bool    isScriptedExternProto(void) { return true; }

    virtual bool    isMesh(void) { return true; }

    FieldIndex      a;
    FieldIndex      b;
    FieldIndex      m;
    FieldIndex      n1;
    FieldIndex      n2;
    FieldIndex      n3;
    FieldIndex      border;
    FieldIndex      bottomBorder;
    FieldIndex      superTessellation;
    FieldIndex      nurbsTessellation;
    FieldIndex      controlPoint;
    FieldIndex      weight;
    FieldIndex      knot;
    FieldIndex      order;
    FieldIndex      creaseAngle;
    FieldIndex      ccw;
    FieldIndex      solid;
    FieldIndex      pieceOfCake;
};

class NodeSuperRevolver : public MeshMorphingNode {
public:
                    NodeSuperRevolver(Scene *scene, Proto *proto);
                    ~NodeSuperRevolver();

    virtual int     getProfile(void) const { return PROFILE_IMMERSIVE; }  
    virtual int     getX3dVersion(void) const { return -1; }
    virtual Node   *copy() const { return new NodeSuperRevolver(*this); }

    virtual void    draw() { meshDraw(); }
    virtual void    drawHandles(void);
    virtual void    drawAHandles();

    virtual Vec3f   getHandle(int handle, int *constraint, int *field);
    virtual void    setHandle(int handle, const Vec3f &v);

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual void    flip(int index);
    virtual void    swap(int fromTo);
    virtual bool    canMoveTo(int direction) { return true; }

    virtual int     getSolidField() { return solid_Field(); }
    virtual void    flipSide(void) { ccw(new SFBool(!ccw()->getValue())); }

    virtual bool    maySetDefault(void) { return false; }

    virtual bool    avoidProtoOnPureVrml(void) { return true; }
    virtual int     writeProto(int filedes);

    Node           *degreeElevate(int newDegree);

    virtual Node   *toNurbsCurve(void);
    virtual Node   *toNurbs(int uTess,  int vTess, int uDegree, int vDegree);

    fieldMacros(SFFloat, a,                 ProtoSuperRevolver)
    fieldMacros(SFFloat, b,                 ProtoSuperRevolver)
    fieldMacros(SFFloat, m,                 ProtoSuperRevolver)
    fieldMacros(SFFloat, n1,                ProtoSuperRevolver)
    fieldMacros(SFFloat, n2,                ProtoSuperRevolver)
    fieldMacros(SFFloat, n3,                ProtoSuperRevolver)
    fieldMacros(SFFloat, border,            ProtoSuperRevolver)
    fieldMacros(SFFloat, bottomBorder,      ProtoSuperRevolver)
    fieldMacros(SFInt32, superTessellation, ProtoSuperRevolver)
    fieldMacros(SFInt32, nurbsTessellation, ProtoSuperRevolver)
    fieldMacros(MFVec2f, controlPoint,      ProtoSuperRevolver)
    fieldMacros(MFFloat, weight,            ProtoSuperRevolver)
    fieldMacros(MFFloat, knot,              ProtoSuperRevolver)
    fieldMacros(SFInt32, order,             ProtoSuperRevolver)
    fieldMacros(SFFloat, creaseAngle,       ProtoSuperRevolver)
    fieldMacros(SFBool,  ccw,               ProtoSuperRevolver)
    fieldMacros(SFBool,  solid,             ProtoSuperRevolver)
    fieldMacros(SFBool,  pieceOfCake,       ProtoSuperRevolver)
protected:
    void            createMesh(bool cleanDoubleVertices = true,
                               bool triangulate = true);
    void            createMesh(SuperRevolverData &data);
    void           *initializeData(void);
    void            loadDataFromInterpolators(void *data, Interpolator *inter,
                                              int field, float key);
    void            createMeshFromData(void* data, bool optimize);
    void            finalizeData(void* data);
    void            copyData(SuperRevolverData *data);

protected:
    NodeNurbsCurve    *m_nurbsCurve;
    MyArray<float>     m_chain;
    SuperRevolverData  m_tempStoreData;
};

