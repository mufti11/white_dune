/*
 * NodeNurbsOrientationInterpolator.h
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

#include "ProtoMacros.h"
#include "Proto.h"
#include "NodeNurbsCurve.h"
#include "SFMFTypes.h"

class Mesh;

class ProtoNurbsOrientationInterpolator : public Proto {
public:
                    ProtoNurbsOrientationInterpolator(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const 
                       { return X3D_NURBS_ORIENTATION_INTERPOLATOR; }
    int             getNodeClass() const 
                       { return CHILD_NODE | INTERPOLATOR_NODE; }
    virtual bool    showFields() { return true; }  

    FieldIndex      dimension;
    FieldIndex      controlPoint;
    FieldIndex      knot;
    FieldIndex      order;
    FieldIndex      weight;
};

class NodeNurbsOrientationInterpolator : public Node, NurbsCurve {
public:
                    NodeNurbsOrientationInterpolator(Scene *scene, 
                                                     Proto *proto);
                    ~NodeNurbsOrientationInterpolator();

    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const 
                       { return new NodeNurbsOrientationInterpolator(*this); }

    virtual const char* getComponentName(void) const { return "NURBS"; }
    virtual int     getComponentLevel(void) const { return 1; }

    virtual void    draw();
    virtual void    drawHandles(void);

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual Vec3f   getHandle(int handle, int *constraint, int *field);
    virtual void    setHandle(int handle, const Vec3f &v);

    virtual bool    avoidProtoOnPureVrml(void) { return true; }
    virtual int     writeProto(int filedes);
    int             write(int filedes, int indent, bool avoidUse = false);

    void            createNurbsCurve();

    virtual Node   *toNurbsCurve(void);

    void            receiveEvent(int eventIn, double timestamp, 
                                 FieldValue *value);

    void            update() { m_nurbsCurveDirty = true; }

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual bool    maySetDefault(void) { return false; }

    MFVec3f        *getControlPoints(void);
    void            setControlPoints(const MFVec3f *points);

    fieldMacros(SFInt32,  dimension,     ProtoNurbsOrientationInterpolator)
    fieldMacros(SFNode,   controlPoint,  ProtoNurbsOrientationInterpolator)
    fieldMacros(MFDouble, knot,          ProtoNurbsOrientationInterpolator)
    fieldMacros(SFInt32,  order,         ProtoNurbsOrientationInterpolator)
    fieldMacros(MFDouble, weight,        ProtoNurbsOrientationInterpolator)

protected:
    NodeNurbsCurve *m_nurbsCurve;
    bool            m_nurbsCurveDirty;
    MyArray<float>  m_chain;
    float           m_chainLength;
    FieldIndex      m_set_fractionField;
    FieldIndex      m_value_changedField;
};

