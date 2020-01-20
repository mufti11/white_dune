/*
 * NodeSuperEllipsoid.h
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

#pragma once

#include "MeshMorphingNode.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "SFMFTypes.h"

class Mesh;

class SuperEllipsoidData {
public:
   float n1;
   float n2;
   float border;
   float bottomBorder;
   float size[3];
   bool triangulate;
};

class ProtoSuperEllipsoid : public Proto {
public:
                    ProtoSuperEllipsoid(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return DUNE_SUPER_ELLIPSOID; }
    virtual int     getNodeClass() const 
                       { return PARAMETRIC_GEOMETRY_NODE | GEOMETRY_NODE; }

    virtual bool    isScriptedExternProto(void) { return true; }

    virtual bool    isMesh(void) { return true; }

    FieldIndex      n1;
    FieldIndex      n2;
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

class NodeSuperEllipsoid : public MeshMorphingNode {
public:
                    NodeSuperEllipsoid(Scene *scene, Proto *proto);
                    ~NodeSuperEllipsoid();

    virtual int     getProfile(void) const { return PROFILE_IMMERSIVE; }  
    virtual int     getX3dVersion(void) const { return -1; }
    virtual Node   *copy() const { return new NodeSuperEllipsoid(*this); }

    virtual bool    hasNumbers4kids(void) { return true; }
 
    virtual void    draw() { meshDraw(); }
    virtual void    drawHandles(void);
    virtual Vec3f   getHandle(int handle, int *constraint, int *field); 
    virtual void    setHandle(int handle, const Vec3f &v);

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual Node   *toNurbs(int uTess,  int vTess, int uDegree, int vDegree);

    virtual bool    hasTwoSides(void) { return true; }
    virtual bool    isDoubleSided(void) { return !solid()->getValue(); }
    virtual void    toggleDoubleSided(void)
                       { solid(new SFBool(!solid()->getValue())); }
    virtual int     getSolidField() { return solid_Field(); }
    virtual void    flipSide(void) { ccw(new SFBool(!ccw()->getValue())); }

    virtual bool    maySetDefault(void) { return !(TheApp->is4Kids()); }

    virtual bool    avoidProtoOnPureVrml(void) { return true; }
    int             writeProto(int filedes);

    fieldMacros(SFFloat, n1,            ProtoSuperEllipsoid)
    fieldMacros(SFFloat, n2,            ProtoSuperEllipsoid)
    fieldMacros(SFFloat, border,        ProtoSuperEllipsoid)
    fieldMacros(SFBool,  bottom,        ProtoSuperEllipsoid)
    fieldMacros(SFFloat, bottomBorder,  ProtoSuperEllipsoid)
    fieldMacros(SFBool,  ccw,           ProtoSuperEllipsoid)
    fieldMacros(SFFloat, creaseAngle,   ProtoSuperEllipsoid)
    fieldMacros(SFVec3f, size,          ProtoSuperEllipsoid)
    fieldMacros(SFBool,  solid,         ProtoSuperEllipsoid)
    fieldMacros(SFNode,  texCoord,      ProtoSuperEllipsoid)
    fieldMacros(SFBool,  top,           ProtoSuperEllipsoid)
    fieldMacros(SFInt32, uTessellation, ProtoSuperEllipsoid)
    fieldMacros(SFInt32, vTessellation, ProtoSuperEllipsoid)
protected:
    void            createMesh(bool cleanDoubleVertices = true,
                               bool triangulate = true);
    void            createMesh(SuperEllipsoidData *data, bool cleanVertices);
    float           superellipse1xy(float angle1, float n2);
    float           superellipse1z(float angle1, float n2);
    float           superellipse2x(float angle2, float n1);
    float           superellipse2y(float angle2, float n1);
protected:
    virtual void   *initializeData(void);
    virtual void    loadDataFromInterpolators(void *data, Interpolator *inter,
                                              int field, float key);
    virtual void    createMeshFromData(void* data, bool optimize);
    virtual void    finalizeData(void* data);
};


