/*
 * NodeNurbsSwungSurface.h
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

#include "MeshMorphingNode.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "Vec3f.h"
#include "SFMFTypes.h"

class Mesh;
class NodeNurbsGroup;
class NodeExtrusion;
class NodeNurbsCurve;

class NurbsSwungSurfaceData
{
public:
    MFVec2d *controlPoint1;
    MFVec2d *controlPoint2;
};

class ProtoNurbsSwungSurface : public GeometryProto {
public:
                    ProtoNurbsSwungSurface(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_NURBS_SWUNG_SURFACE; }
    virtual int     getNodeClass() const
                       { return PARAMETRIC_GEOMETRY_NODE | GEOMETRY_NODE; }

    virtual bool    isMesh(void) { return true; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex profileCurve;
    FieldIndex trajectoryCurve;
    FieldIndex ccw;
    FieldIndex solid;
};

class NodeNurbsSwungSurface : public MeshMorphingNode {
public:
                    NodeNurbsSwungSurface(Scene *scene, Proto *proto);
protected:
    virtual        ~NodeNurbsSwungSurface();

public:
    virtual const char* getComponentName(void) const { return "NURBS"; }
    virtual int     getComponentLevel(void) const { return 3; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeNurbsSwungSurface(*this); }

    virtual void    draw();

    virtual void    update();

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual bool    hasTwoSides(void) { return true; }
    virtual bool    isDoubleSided(void) { return !solid()->getValue(); }
    virtual void    toggleDoubleSided(void) 
                       { solid(new SFBool(!solid()->getValue())); }
    virtual int     getSolidField() { return solid_Field(); }
    virtual void    flipSide(void) { ccw(new SFBool(!ccw()->getValue())); }

    virtual Node   *toExtrusion(void);
    virtual bool    canConvertToExtrusion(void) { return hasParent(); }

    virtual int     writeRib(int filedes, int indent);

    virtual bool    showFields() { return true; }

    fieldMacros(SFNode, profileCurve,      ProtoNurbsSwungSurface)
    fieldMacros(SFNode, trajectoryCurve,   ProtoNurbsSwungSurface)
    fieldMacros(SFBool, ccw,               ProtoNurbsSwungSurface)
    fieldMacros(SFBool, solid,             ProtoNurbsSwungSurface)
protected:
    void            createExtrusion();
    void            createMesh(bool cleanDoubleVertices = true,
                               bool triangulate = true);
    void            createMesh(NurbsSwungSurfaceData &data);
    virtual void    findInterpolators(InterpolatorInfo& info);
    void           *initializeData(void);
    void            loadDataFromInterpolators(void *data, Interpolator *inter,
                                              int field, float key);
    void            createMeshFromData(void* data, bool optimize);
    void            finalizeData(void* data);
    void            copyData(NurbsSwungSurfaceData *data);

protected:
    NodeExtrusion        *m_extrusion;
    bool                  m_extrusionDirty;
    bool                  m_extrusionNew;
    NodeNurbsCurve       *m_profileCurve;
    NodeNurbsCurve       *m_spineCurve;
    MyArray<float>        m_chain;
    NurbsSwungSurfaceData m_tempStoreData;
};

