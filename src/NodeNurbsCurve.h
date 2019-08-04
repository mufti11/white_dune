/*
 * NodeNurbsCurve.h
 *
 * Copyright (C) 2003 Th. Rothermel
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

#ifndef _NODE_NURBS_CURVE_H
#define _NODE_NURBS_CURVE_H

#include "NurbsCurve.h"

typedef enum {
    NURBS_ROT_X_AXIS,
    NURBS_ROT_Y_AXIS,
    NURBS_ROT_Z_AXIS,
    NURBS_ROT_POINT_TO_POINT
} NurbsRot;

class ProtoNurbsCurve : public Proto {
public:
                    ProtoNurbsCurve(Scene *scene);
                    ProtoNurbsCurve(Scene *scene, const char *name,
                                    bool curveAnimation = false);
    void            addElements(bool curveAnimation);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_NURBS_CURVE; }
    virtual int     getNodeClass() const
                       { return PARAMETRIC_GEOMETRY_NODE | GEOMETRY_NODE; }

    virtual bool    showFields() { return true; }

    FieldIndex controlPoint;
    FieldIndex controlPointX3D;
    FieldIndex tessellation;
    FieldIndex weight;
    FieldIndex closed;
    FieldIndex knot;
    FieldIndex order;
};

class NodeNurbsCurve : public ChainBasedGeometryNode, NurbsCurve {
public:
                    NodeNurbsCurve(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeNurbsCurve(*this); }

    virtual const char* getComponentName(void) const { return "NURBS"; }
    virtual int     getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 0; }

    virtual void    createChain(void);

    virtual void    drawHandles(void);
    virtual void    drawAsHandle(int handle);

    virtual Vec3f   getHandle(int handle, int *constraint, int *field);
    virtual void    setHandle(int handle, const Vec3f &v);

    virtual int     repairField(int field) const;
    virtual void    setField(int index, FieldValue *value, int cf = -1);

    void            repairKnotAndWeight();

    void            extrudePoints(int from, int to, int points, 
                                  bool xSymetric = true);

    Node           *convert2X3d(void);
    Node           *convert2Vrml(void);

    virtual void    addToConvertedNodes(int writeFlags);

    virtual void    update();

    virtual bool    avoidProtoOnPureVrml(void) { return true; }
    int             writeProto(int filedes);
    int             write(int filedes, int indent);

    bool            revolveFlatten(int direction);
    void            revolveFlatter(int zero, int change);

    Node           *toSuperExtrusion(void);
    Node           *toSuperRevolver(void);
    virtual Node   *toCurveAnimation(void);
    Node           *toNurbsPositionInterpolator(void);
    Node           *toNurbsOrientationInterpolator(void);

    virtual Node   *toNurbs(int narcs, int pDegree, float rDegree, Vec3f &P1, Vec3f &P2);

    virtual Node   *degreeElevate(int newDegree); 
    virtual bool    hasBoundingBox(void) { return true; }

    virtual void    flip(int index);
    virtual void    swap(int fromTo);
    virtual bool    canMoveTo(int direction) { return true; }

    virtual bool    maySetDefault(void) { return false; }

    virtual bool    supportAnimation(void) { return true; }

    fieldMacros(MFVec3f, controlPoint,    ProtoNurbsCurve)
    fieldMacros(SFNode,  controlPointX3D, ProtoNurbsCurve)
    fieldMacros(SFInt32, tessellation,    ProtoNurbsCurve)
    fieldMacros(MFFloat, weight,          ProtoNurbsCurve)
    fieldMacros(SFBool,  closed,          ProtoNurbsCurve)
    fieldMacros(MFFloat, knot,            ProtoNurbsCurve)
    fieldMacros(SFInt32, order,           ProtoNurbsCurve)

    virtual void    setInternal(bool flag) { m_isInternal = flag; }
    bool            isInternal() { return m_isInternal; }

    MFVec3f        *getControlPoints(void);
    void            setControlPoints(MFVec3f *points);
    void            createControlPoints(MFVec3f *points);
    void            backupFieldsAppend(int field);

    virtual void    setHandle(MFVec3f *newValue, int handle, float newWeight,
                              const Vec3f &newV, const Vec3f &oldV,
                              bool already_changed = false);
    virtual void    setHandle(float newWeight, 
                              const Vec3f &newV, const Vec3f &oldV);
    virtual Vec3f   getVertex(int i)
                        { 
                        return getControlPoints()->getVec(i) / 
                               weight()->getValue(i); 
                        }
    virtual bool    getValidVertex(void) { return true; }   

private:
    bool            m_inRepair;
    bool            m_isInternal;
};

#endif // _NODE_NURBS_CURVE_H
