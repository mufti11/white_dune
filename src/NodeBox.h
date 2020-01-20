/*
 * NodeBox.h
 *
 * Copyright (C) 1999 Stephen F. White
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

class ProtoBox : public Proto {
public:
                    ProtoBox(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_BOX; }
    virtual int     getNodeClass() const 
                       { return PRIMITIVE_GEOMETRY_OR_MASS_DENSITY_MODEL_NODE |
                                GEOMETRY_NODE; }

    virtual bool    isMesh(void) { return true; }

    FieldIndex size;
    FieldIndex solid;
    FieldIndex texCoord;
    x3domGeometryCommonFieldIndex()
    FieldIndex ccw;
    FieldIndex hasHelperColors;
};

class NodeBox : public MeshBasedNode {
public:
                    NodeBox(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeBox(*this); }
    virtual bool    isInvalidChildNode(void) { return true; }
    virtual void    draw() { meshDraw(); }
    virtual void    drawHandles(void);
    virtual Vec3f   getHandle(int handle, int *constraint, int *field);
    virtual void    setHandle(int handle, const Vec3f &v);
    virtual bool    validHandle(int handle);

    virtual Node   *toNurbs(int nuAreaPoints, int uDegree, 
                            int nvAreaPoints, int vDegree);
    virtual Node   *toNurbs(int nuAreaPoints, int uDegree, 
                            int nvAreaPoints, int vDegree, int nzAreaPoints);

    virtual void    flip(int index) {}
    virtual void    swap(int fromTo);

    virtual int     countPrimitives(void) {return 1;}
    virtual int     countPolygons(void) { return 0; }

    virtual bool    isFlat(void) { return true; }

    virtual bool    shouldConvertToIndexedFaceSet(void) { return false; }

    fieldMacros(SFVec3f, size,            ProtoBox)
    fieldMacros(SFBool,  solid,           ProtoBox)
    fieldMacros(SFNode,  texCoord,        ProtoBox)
    x3domGeometryCommonFieldMacros(ProtoBox) 
    fieldMacros(SFBool,  ccw,             ProtoBox)
    fieldMacros(SFBool,  hasHelperColors, ProtoBox)


protected:
    void            createMesh(bool cleanDoubleVertices = true,
                               bool triangulate = true);
    void            makeKnotvectors(int uOrder, int uDimension, 
                                    int vOrder, int vDimension);
    Node           *makeNurbsSurfaces(int surfaces);
    void            makeNurbsRectangle(float xSize, float ySize, 
                                       float zPosition, int nuAreapoints, 
                                       int uDegree);

protected:
    int             m_uKnotSize() {return m_uKnots.size();}
    int             m_vKnotSize() {return m_vKnots.size();}
    int             m_uOrder;
    int             m_vOrder;
    int             m_uDimension;
    int             m_vDimension;
    int             m_nuAreaPoints;
    int             m_nvAreaPoints;
    MyArray<float>  m_uKnots;
    MyArray<float>  m_vKnots;
    MyArray<Vec3f>  m_rectangle;
};
