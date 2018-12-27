/*
 * NodeNurbsSweptSurface.h
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

#ifndef _NODE_NURBS_SWEPT_SURFACE_H
#define _NODE_NURBS_SWEPT_SURFACE_H

#ifndef _MESH_MORPHING_NODE_H
# include "MeshMorphingNode.h"
#endif
#ifndef _PROTO_MACROS_H
# include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
# include "Proto.h"
#endif
#ifndef _VEC3F
# include "Vec3f.h"
#endif
#ifndef _SFMFTYPES_H
# include "SFMFTypes.h"
#endif
#include "NodeExtrusion.h"
#include "NodeNurbsCurve.h"

class Mesh;
class NodeNurbsGroup;
class NodeNurbsCurve;

class Mesh;

class NurbsSweptSurfaceData
{
public:
    MFVec3f *controlPoint;
    MFVec2d *controlPoint2D;
};

class ProtoNurbsSweptSurface : public GeometryProto {
public:
                    ProtoNurbsSweptSurface(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_NURBS_SWEPT_SURFACE; }
    virtual int     getNodeClass() const
                       { return PARAMETRIC_GEOMETRY_NODE | GEOMETRY_NODE; }

    virtual bool    isMesh(void) { return true; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex crossSectionCurve;
    FieldIndex trajectoryCurve;
    FieldIndex ccw;
    FieldIndex solid;
};

class NodeNurbsSweptSurface : public MeshMorphingNode {
public:
                    NodeNurbsSweptSurface(Scene *scene, Proto *proto);
protected:
    virtual        ~NodeNurbsSweptSurface();

public:
    virtual const char* getComponentName(void) const { return "NURBS"; }
    virtual int     getComponentLevel(void) const { return 3; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeNurbsSweptSurface(*this); }

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual void    draw();

    virtual void    update();

    virtual bool    hasBoundingBox(void) { return true; }
    virtual Vec3f   getMinBoundingBox(void);
    virtual Vec3f   getMaxBoundingBox(void);

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual bool    hasTwoSides(void) { return true; }
    virtual bool    isDoubleSided(void) { return !solid()->getValue(); }
    virtual void    toggleDoubleSided(void) 
                       { solid(new SFBool(!solid()->getValue())); }
    virtual int     getSolidField() { return solid_Field(); }
    virtual void    flipSide(void) { ccw(new SFBool(!ccw()->getValue())); }

    virtual bool    showFields() { return true; }

    virtual Node   *toExtrusion(void);
    virtual bool    canConvertToExtrusion(void) { return hasParent(); }

    virtual int     writeRib(int filedes, int indent);

    Node           *convert2X3d(void);
    Node           *convert2Vrml(void);

    fieldMacros(SFNode, crossSectionCurve, ProtoNurbsSweptSurface)
    fieldMacros(SFNode, trajectoryCurve,   ProtoNurbsSweptSurface)
    fieldMacros(SFBool, ccw,               ProtoNurbsSweptSurface)
    fieldMacros(SFBool, solid,             ProtoNurbsSweptSurface)

protected:
    void            createExtrusion();
    void            createMesh(bool cleanDoubleVertices = true,
                               bool triangulate = true);
    void            createMesh(NurbsSweptSurfaceData &data);
    virtual void    findInterpolators(InterpolatorInfo& info);
    void           *initializeData(void);
    void            loadDataFromInterpolators(void *data, Interpolator *inter,
                                              int field, float key);
    void            createMeshFromData(void* data, bool optimize);
    void            finalizeData(void* data);
    void            copyData(NurbsSweptSurfaceData *data);

protected:
    NodeExtrusion        *m_extrusion;
    bool                  m_extrusionDirty;
    bool                  m_extrusionNew;
    NodeNurbsCurve       *m_crossSectionCurve;
    Array<float>          m_chain;
    NurbsSweptSurfaceData m_tempStoreData;
};

#endif
