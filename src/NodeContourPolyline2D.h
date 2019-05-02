/*
 * NodeContourPolyline2D.h
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

#ifndef _NODE_CONTOUR_POLYLINE_2D_H
#define _NODE_CONTOUR_POLYLINE_2D_H

#ifndef _CHAIN_BASED_NODE_H
#include "ChainBasedNode.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"

class ProtoContourPolyline2D : public Proto {
public:
                    ProtoContourPolyline2D(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_CONTOUR_POLYLINE_2D; }
    virtual int     getNodeClass() const { return NURBS_CONTROL_CURVE_NODE; }

    FieldIndex      lineSegments;
};

class NodeContourPolyline2D : public Node {
public:
                    NodeContourPolyline2D(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeContourPolyline2D(*this); }

    virtual const char* getComponentName(void) const { return "NURBS"; }
    virtual int     getComponentLevel(void) const { return 3; }
    virtual int     getX3dVersion(void) const { return 1; }

    virtual bool    isInvalidChildNode(void) { return true; }
    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual void    drawHandles(void);

    virtual Vec3f   getHandle(int handle, int *constraint, int *field);
    virtual void    setHandle(int handle, const Vec3f &v);

    fieldMacros(MFVec2d, lineSegments, ProtoContourPolyline2D)

    virtual bool    hasControlPoints(void) { return lineSegments()->getSize(); }
};

#endif
