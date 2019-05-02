/*
 * NodeOrthoViewpoint.h
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

#ifndef _NODE_ORTHO_VIEWPOINT_H
#define _NODE_ORTHO_VIEWPOINT_H

#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"
#include "DuneApp.h"
#include "KambiViewpointCommonFields.h"

class ProtoOrthoViewpoint : public Proto {
public:
                    ProtoOrthoViewpoint(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_ORTHO_VIEWPOINT; }

    virtual bool    isX3dInternalProto(void) { return true; }

    virtual bool    needUpdate(void) { return true; }

    FieldIndex centerOfRotation;
    FieldIndex description;
    FieldIndex fieldOfView;
    FieldIndex jump;
    FieldIndex orientation;
    FieldIndex position;
    FieldIndex retainUserOffsets;

    kambiViewpointCommonFieldIndex()

    FieldIndex bind;
    FieldIndex isActive;
    FieldIndex zFar;
    FieldIndex zNear;
};

class NodeOrthoViewpoint : public Node {
public:
                    NodeOrthoViewpoint(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const { return "Navigation"; }
    virtual int         getComponentLevel(void) const { return 2; }
    virtual int         getX3dVersion(void) const { return 2; } 
    virtual Node       *copy() const { return new NodeOrthoViewpoint(*this); }

    void                apply(bool useStereo = TheApp->useStereo());
    void                transformForViewpoint(bool useStereo);

    Vec3d               getPosition() const;
    Quaternion          getOrientation() const;

    void                setPosition(const Vec3f &pos);
    void                setOrientation(const Quaternion &quat);

    SFFloat            *fov() { static SFFloat f(M_PI / 4); return &f; }

    void                getMatrix(float* matrix);

    fieldMacros(SFVec3f,    centerOfRotation, ProtoOrthoViewpoint)
    fieldMacros(SFString,   description, ProtoOrthoViewpoint)
    fieldMacros(MFFloat,    fieldOfView, ProtoOrthoViewpoint)
    fieldMacros(SFBool,     jump, ProtoOrthoViewpoint)
    fieldMacros(SFRotation, orientation, ProtoOrthoViewpoint)
    fieldMacros(SFVec3f,    position, ProtoOrthoViewpoint)
    fieldMacros(SFBool,     retainUserOffsets, ProtoOrthoViewpoint)

    kambiViewpointCommonFieldMacros(ProtoOrthoViewpoint)

    fieldMacros(SFBool,     bind, ProtoOrthoViewpoint)
    fieldMacros(SFBool,     isActive, ProtoOrthoViewpoint)
    fieldMacros(SFFloat,    zFar, ProtoOrthoViewpoint)
    fieldMacros(SFFloat,    zNear, ProtoOrthoViewpoint)
protected:
    float                   m_matrix[16];
};

#endif
