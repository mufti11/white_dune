/*
 * ViewpointNode.h
 *
 * Copyright (C) 1999 Stephen F. White, 2019 J. "MUFTI" Scheurich
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

#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "Quaternion.h"
#include "Vec3f.h"
#include "swt.h"
#include "SFMFTypes.h"
#include "SFRotation.h"
#include "CoverNode.h"
#include "KambiViewpointCommonFields.h"
#include "DuneApp.h"

class ViewpointProto : public Proto {
public:
                        ViewpointProto(Scene *scene, const char *name);
    virtual Node       *create(Scene *scene);

    FieldIndex centerOfRotation;
    FieldIndex description;
    FieldIndex fieldOfView;
    FieldIndex jump;
    FieldIndex orientation;
    FieldIndex retainUserOffsets;
    FieldIndex type;
    FieldIndex direction;
    FieldIndex up;
    FieldIndex gravityUp;

    FieldIndex bind;
    FieldIndex isActive;
    FieldIndex zFar;
    FieldIndex zNear;

    // not fields but eventIn/eventOut
    FieldIndex set_bind;
};


class ViewpointNode : public Node {
public:
                        ViewpointNode(Scene *scene, Proto *proto);

    virtual int         getProfile(void) const;
    virtual int         getX3dVersion(void) const { return 0; }
    virtual Node       *copy() const { return new ViewpointNode(*this); }

    virtual void        preDraw(bool useStereo = TheApp->useStereo());
    void                apply(bool useStereo, Vec3d vec, SFRotation rot);
    void                transformForViewpoint(bool useStereo, Vec3d vec, 
                                              SFRotation rot);

    virtual Vec3d       getPosition() const;
    
    virtual Quaternion  getOrientation() const;

    virtual void        setOrientation(const Quaternion &quat);

    SFFloat            *fov() { return fieldOfView(); };

    virtual bool        supportCurveAnimation(void) { return true; }

    virtual int         getAnimationCommentID(void);

    void                getMatrix(float* matrix);

    fieldMacros(SFVec3f,    centerOfRotation,  ViewpointProto)
    fieldMacros(SFString,   description,       ViewpointProto)
    fieldMacros(SFFloat,    fieldOfView,       ViewpointProto)
    fieldMacros(SFBool,     jump,              ViewpointProto)
    fieldMacros(SFRotation, orientation,       ViewpointProto)
    fieldMacros(SFBool,     retainUserOffsets, ViewpointProto)
    fieldMacros(SFString,   type,              ViewpointProto)
    fieldMacros(MFVec3f,    direction,         ViewpointProto)
    fieldMacros(MFVec3f,    up,                ViewpointProto)
    fieldMacros(SFVec3f,    gravityUp,         ViewpointProto)

    fieldMacros(SFBool,     bind, ViewpointProto)
    fieldMacros(SFBool,     isActive, ViewpointProto)
    fieldMacros(SFFloat,    zFar, ViewpointProto)
    fieldMacros(SFFloat,    zNear, ViewpointProto)

    fieldMacros(SFBool,     set_bind,          ViewpointProto)
protected:
    float                   m_matrix[16];
};

