/*
 * NodeNurbsSurfaceInterpolator.h
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

#ifndef _NODE_NURBS_SurfaceInterpolator_H
#define _NODE_NURBS_SurfaceInterpolator_H

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

class Mesh;
class NodeNurbsGroup;

class ProtoNurbsSurfaceInterpolator : public Proto {
public:
                    ProtoNurbsSurfaceInterpolator(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_NURBS_SURFACE_INTERPOLATOR; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex uDimension;
    FieldIndex vDimension;
    FieldIndex uKnot;
    FieldIndex vKnot;
    FieldIndex uOrder;
    FieldIndex vOrder;
    FieldIndex controlPoint;
    FieldIndex weight;
};

class NodeNurbsSurfaceInterpolator : public Node {
public:
                    NodeNurbsSurfaceInterpolator(Scene *scene, Proto *proto);
protected:
    virtual        ~NodeNurbsSurfaceInterpolator();

public:
    virtual const char* getComponentName(void) const { return "NURBS"; }
    virtual int     getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeNurbsSurfaceInterpolator(*this); }

    virtual void    drawHandles(void);

    virtual Vec3f   getHandle(int handle, int *constraint, int *field);
    virtual void    setHandle(int handle, const Vec3f &v);

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    void            rotate(SFRotation rot);

    virtual Node   *degreeElevate(int newUDegree, int newVDegree); 

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual bool    maySetDefault(void) { return false; }

    fieldMacros(SFInt32,  uDimension,      ProtoNurbsSurfaceInterpolator)
    fieldMacros(SFInt32,  vDimension,      ProtoNurbsSurfaceInterpolator)
    fieldMacros(MFDouble, uKnot,           ProtoNurbsSurfaceInterpolator)
    fieldMacros(MFDouble, vKnot,           ProtoNurbsSurfaceInterpolator)
    fieldMacros(SFInt32,  uOrder,          ProtoNurbsSurfaceInterpolator)
    fieldMacros(SFInt32,  vOrder,          ProtoNurbsSurfaceInterpolator)
    fieldMacros(SFNode,   controlPoint,    ProtoNurbsSurfaceInterpolator)
    fieldMacros(MFDouble, weight,          ProtoNurbsSurfaceInterpolator)

    virtual void    setHandle(MFVec3f *newValue, int handle, float newWeight,
                              const Vec3f &newV, const Vec3f &oldV,
                              bool already_changed = false);
    virtual void    setHandle(float newWeight, 
                              const Vec3f &newV, const Vec3f &oldV);
    NodeNurbsGroup *findNurbsGroup();
    void            linearUknot(void);
    void            linearVknot(void);

    MFVec3f        *getControlPoints(void);
    void            setControlPoints(const MFVec3f *points);
    void            createControlPoints(const MFVec3f *points);

protected:
    static int      findSpan(int dimension, int order, float u,
                             const float knots[]);
    static void     basisFuns(int span, float u, int order,
                              const float knots[], float basis[], 
                              float deriv[]);
    static Vec3f    SurfaceInterpolatorPoint(int uDimension, int uOrder,
                                 const float uKnots[],
                                 int vDimension, int vOrder,
                                 const float vKnots[],
                                 const Vec3f controlPoints[],
                                 const float weights[],
                                 float u, float v, Vec3f &normal);
   void             repairNormal(MFVec3f *normal, MFVec3f *vertices, 
                                 int c1, int c2, int c3);
};

#endif

