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

#ifndef _NODE_NURBS_ORIENTATION_INTERPOLATOR_H
#define _NODE_NURBS_ORIENTATION_INTERPOLATOR_H

#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif
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

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex      dimension;
    FieldIndex      controlPoints;
    FieldIndex      knot;
    FieldIndex      order;
    FieldIndex      weight;
};

class NodeNurbsOrientationInterpolator : public Node {
public:
                    NodeNurbsOrientationInterpolator(Scene *scene, 
                                                     Proto *proto);
                    ~NodeNurbsOrientationInterpolator();

    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const 
                       { return new NodeNurbsOrientationInterpolator(*this); }

    virtual const char* getComponentName(void) const { return "NURBS"; }
    virtual int     getComponentLevel(void) const { return 1; }

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual bool    maySetDefault(void) { return false; }

    fieldMacros(SFInt32,  dimension,     ProtoNurbsOrientationInterpolator)
    fieldMacros(SFNode,   controlPoints, ProtoNurbsOrientationInterpolator)
    fieldMacros(MFDouble, knot,          ProtoNurbsOrientationInterpolator)
    fieldMacros(SFInt32,  order,         ProtoNurbsOrientationInterpolator)
    fieldMacros(MFDouble, weight,        ProtoNurbsOrientationInterpolator)
};
#endif 
