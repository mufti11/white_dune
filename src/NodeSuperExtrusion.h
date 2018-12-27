/*
 * NodeSuperExtrusion.h
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

#ifndef _NODE_SUPER_EXTRUSION_H
#define _NODE_SUPER_EXTRUSION_H

#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif
#include "NodeExtrusion.h"
#include "NodeNurbsCurve.h"
#include "MeshMorphingNode.h"

#include "SFMFTypes.h"

class Mesh;

class SuperExtrusionData
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


class ProtoSuperExtrusion : public Proto {
public:
                    ProtoSuperExtrusion(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return DUNE_SUPER_EXTRUSION; }
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
    FieldIndex      spineTessellation;
    FieldIndex      controlPoint;
    FieldIndex      weight;
    FieldIndex      knot;
    FieldIndex      order;
    FieldIndex      creaseAngle;
    FieldIndex      beginCap;
    FieldIndex      endCap;
    FieldIndex      solid;
    FieldIndex      scale;        
};

class NodeSuperExtrusion : public MeshMorphingNode {
public:
                    NodeSuperExtrusion(Scene *scene, Proto *proto);
                    ~NodeSuperExtrusion();

    virtual int     getProfile(void) const { return PROFILE_IMMERSIVE; }  
    virtual int     getX3dVersion(void) const { return -1; }
    virtual Node   *copy() const { return new NodeSuperExtrusion(*this); }

    virtual void    draw();
    virtual void    drawHandles(void);
    virtual void    drawAHandles();
    virtual void    drawScaleHandles();

    virtual Vec3f   getHandle(int handle, int *constraint, int *field);
    virtual void    setHandle(int handle, const Vec3f &v);

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual bool    hasBoundingBox(void) { return true; }
    virtual Vec3f   getMinBoundingBox(void);
    virtual Vec3f   getMaxBoundingBox(void);

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual int     getSolidField() { return solid_Field(); }

    virtual bool    maySetDefault(void) { return false; }

    virtual bool    avoidProtoOnPureVrml(void) { return true; }
    virtual int     writeProto(int filedes);

    void            update(void) { m_extrusionDirty = true; }
    void            reInit(void);

    virtual int     countPolygons(void);

    virtual Node   *toExtrusion(void);
    virtual bool    canConvertToExtrusion(void) { return hasParent(); }

    virtual Node   *toNurbsCurve(void);

    virtual Node   *toNurbs(int uTess,  int vTess, int uDegree, int vDegree);

    virtual int     writeRib(int filedes, int indent);

    Node           *degreeElevate(int newDegree);

    fieldMacros(SFFloat, a,                 ProtoSuperExtrusion)
    fieldMacros(SFFloat, b,                 ProtoSuperExtrusion)
    fieldMacros(SFFloat, m,                 ProtoSuperExtrusion)
    fieldMacros(SFFloat, n1,                ProtoSuperExtrusion)
    fieldMacros(SFFloat, n2,                ProtoSuperExtrusion)
    fieldMacros(SFFloat, n3,                ProtoSuperExtrusion)
    fieldMacros(SFFloat, border,            ProtoSuperExtrusion)
    fieldMacros(SFFloat, bottomBorder,      ProtoSuperExtrusion)
    fieldMacros(SFInt32, superTessellation, ProtoSuperExtrusion)
    fieldMacros(SFInt32, spineTessellation, ProtoSuperExtrusion)
    fieldMacros(MFVec3f, controlPoint,      ProtoSuperExtrusion)
    fieldMacros(MFFloat, weight,            ProtoSuperExtrusion)
    fieldMacros(MFFloat, knot,              ProtoSuperExtrusion)
    fieldMacros(SFInt32, order,             ProtoSuperExtrusion)
    fieldMacros(SFFloat, creaseAngle,       ProtoSuperExtrusion)
    fieldMacros(SFBool,  beginCap,          ProtoSuperExtrusion)
    fieldMacros(SFBool,  endCap,            ProtoSuperExtrusion)
    fieldMacros(SFBool,  solid,             ProtoSuperExtrusion)
    fieldMacros(MFVec2f, scale,             ProtoSuperExtrusion)
protected:
    void            createExtrusion();
    void            createMesh(bool cleanDoubleVertices = true,
                               bool triangulate = true);
    void            createMesh(SuperExtrusionData &data);
    void           *initializeData(void);
    void            loadDataFromInterpolators(void *data, Interpolator *inter,
                                              int field, float key);
    void            createMeshFromData(void* data, bool optimize);
    void            finalizeData(void* data);
    void            copyData(SuperExtrusionData *data);

protected:
    NodeExtrusion     *m_extrusion;
    NodeNurbsCurve    *m_nurbsCurve;
    bool               m_extrusionDirty;
    Array<float>       m_chain;
    SuperExtrusionData m_tempStoreData;
};
#endif // _NODE_SUPER_EXTRUSION_H
