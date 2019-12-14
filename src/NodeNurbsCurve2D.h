/*
 * NodeNurbsCurve2D.h
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

#ifndef _NODE_NURBS_CURVE_2D_H
#define _NODE_NURBS_CURVE_2D_H

#ifndef _NODE_H
#include "ExternTheApp.h"
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif
#ifndef _VEC2F_H
#include "Vec2f.h"
#endif
#ifndef _SFMFTYPES_H
#include "SFMFTypes.h"
#endif

enum {
    HANDLE_RENDERING_XY,
    HANDLE_RENDERING_XZ,
    HANDLE_RENDERING_YZ,
    HANDLE_RENDERING_ZY
};

class ProtoNurbsCurve2D : public Proto {
public:
                    ProtoNurbsCurve2D(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_NURBS_CURVE_2D; }
    virtual int     getNodeClass() const
                       { return PARAMETRIC_GEOMETRY_NODE | GEOMETRY_NODE; }

    FieldIndex controlPoint;
    FieldIndex tessellation;
    FieldIndex weight;
    FieldIndex closed;
    FieldIndex knot;
    FieldIndex order;
};

class NodeNurbsCurve2D : public Node {
public:
                    NodeNurbsCurve2D(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() { return new NodeNurbsCurve2D(m_scene, m_proto); }

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual bool    avoidProtoOnPureVrml(void) { return true; }
    int             writeProto(int filedes);
    int             write(int filedes, int indent, bool avoidUse = false);

    virtual void    drawHandles(void);

    virtual Vec3f   getHandle(int handle, int *constraint, int *field);
    virtual void    setHandle(int handle, const Vec3f &v);

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual bool    maySetDefault(void) { return false; }

    Node           *degreeElevate(int newDegree);

    virtual void    setInternal(bool flag) { m_isInternal = flag; }
    virtual void    setHandleRendering(int num) { m_handleRendering = num; }

    void            update(void);
    void            reInit(void);

    Node           *setWeightsTo1(void);

    fieldMacros(MFVec2d, controlPoint,  ProtoNurbsCurve2D)
    fieldMacros(SFInt32, tessellation,  ProtoNurbsCurve2D)
    fieldMacros(MFFloat, weight,        ProtoNurbsCurve2D)
    fieldMacros(SFBool,  closed,        ProtoNurbsCurve2D)
    fieldMacros(MFFloat, knot,          ProtoNurbsCurve2D)
    fieldMacros(SFInt32, order,         ProtoNurbsCurve2D)

    virtual bool    hasControlPoints(void) 
        { return controlPoint()->getSize(); }

private:
    bool drawXz(void) 
       { return m_isInternal && hasParent() &&
                (getParent()->getType() == X3D_NURBS_SWEPT_SURFACE); }
private:
    int             m_dimension;
    MyArray<Vec2f>  m_chain;
    bool            m_chainDirty;
    bool            m_isInternal;
    int             m_handleRendering;
};

#endif
