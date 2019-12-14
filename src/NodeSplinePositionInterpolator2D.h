/*
 * NodeSplinePositionInterpolator2D.h
 *
 * Copyright (C) 2009 J. "MUFTI" Scheurich
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

#ifndef _NODE_SPLINE_POSITION_INTERPOLATOR_2D_H
#define _NODE_SPLINE_POSITION_INTERPOLATOR_2D_H

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

#include "SFMFTypes.h"

class ProtoSplinePositionInterpolator2D : public Proto {
public:
                    ProtoSplinePositionInterpolator2D(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_SPLINE_POSITION_INTERPOLATOR_2D; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex closed;
    FieldIndex key;
    FieldIndex keyValue;
    FieldIndex keyVelocity;
    FieldIndex normalizeVelocity;
};

class NodeSplinePositionInterpolator2D : public Node {
public:
                    NodeSplinePositionInterpolator2D(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const { return "Interpolation"; }
    virtual int         getComponentLevel(void) const { return 4; }
    virtual int     getX3dVersion(void) const { return 2; } 
    virtual Node   *copy()     
                    { return new NodeSplinePositionInterpolator2D(
                      m_scene, m_proto); }

    fieldMacros(SFBool,  closed,            ProtoSplinePositionInterpolator2D);
    fieldMacros(MFFloat, key,               ProtoSplinePositionInterpolator2D);
    fieldMacros(MFVec2f, keyValue,          ProtoSplinePositionInterpolator2D);
    fieldMacros(MFVec2f, keyVelocity,       ProtoSplinePositionInterpolator2D);
    fieldMacros(SFBool,  normalizeVelocity, ProtoSplinePositionInterpolator2D);
};

#endif
