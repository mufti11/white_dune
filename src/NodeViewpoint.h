/*
 * NodeViewpoint.h
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

#ifndef _NODE_VIEWPOINT_H
#define _NODE_VIEWPOINT_H

#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif
#ifndef _QUATERNION_H
#include "Quaternion.h"
#endif
#ifndef _VEC3F_H
#include "Vec3f.h"
#endif

#include "swt.h"

#include "SFMFTypes.h"
#include "CoverNode.h"
#include "KambiViewpointCommonFields.h"
#include "DuneApp.h"

class ProtoViewpoint : public Proto {
public:
                        ProtoViewpoint(Scene *scene);
    virtual Node       *create(Scene *scene);

    virtual int         getType() const { return VRML_VIEWPOINT; }
    virtual int         getNodeClass() const 
                           { return CHILD_NODE | VIEWPOINT_NODE; }

    virtual bool        isDeclaredInRwd_h() { return true; }      

    FieldIndex centerOfRotation;
    FieldIndex description;
    FieldIndex descriptionX3D;
    FieldIndex fieldOfView;
    FieldIndex jump;
    FieldIndex orientation;
    FieldIndex position;
    FieldIndex retainUserOffsets;
    FieldIndex type;
    FieldIndex direction;
    FieldIndex up;
    FieldIndex gravityUp;

    kambiViewpointCommonFieldIndex()

    FieldIndex bind;
    FieldIndex isActive;
    FieldIndex zFar;
    FieldIndex zNear;

    // not fields but eventIn/eventOut
    FieldIndex set_bind;
};


class NodeViewpoint : public CoverNode {
public:
                        NodeViewpoint(Scene *scene, Proto *proto);

    virtual int         getProfile(void) const;
    virtual int         getX3dVersion(void) const { return 0; }
    virtual Node       *copy() const { return new NodeViewpoint(*this); }

    virtual bool        hasNumbers4kids(void) { return true; } 

    virtual void        flip(int index);
    virtual void        swap(int fromTo);

    virtual void        preDraw(bool useStereo = TheApp->useStereo());
    void                apply(bool useStereo = TheApp->useStereo());
    void                transformForViewpoint(bool useStereo);

    virtual bool        hasCoverFields(void) { return true; }   
    virtual bool        hasKambiFields(void) { return true; }

    Vec3f               getPosition() const;
    Quaternion          getOrientation() const;

    void                setPosition(const Vec3f &pos);
    void                setOrientation(const Quaternion &quat);

    SFFloat            *fov() { return fieldOfView(); };

    virtual bool        supportCurveAnimation(void) { return true; }

    virtual int         getAnimationCommentID(void);

    void                getMatrix(float* matrix);

    fieldMacros(SFVec3f,    centerOfRotation,  ProtoViewpoint)
    fieldMacros(SFString,   description,       ProtoViewpoint)
    fieldMacros(SFString,   descriptionX3D,    ProtoViewpoint)
    fieldMacros(SFFloat,    fieldOfView,       ProtoViewpoint)
    fieldMacros(SFBool,     jump,              ProtoViewpoint)
    fieldMacros(SFRotation, orientation,       ProtoViewpoint)
    fieldMacros(SFVec3f,    position,          ProtoViewpoint)
    fieldMacros(SFBool,     retainUserOffsets, ProtoViewpoint)
    fieldMacros(SFString,   type,              ProtoViewpoint)
    fieldMacros(MFVec3f,    direction,         ProtoViewpoint)
    fieldMacros(MFVec3f,    up,                ProtoViewpoint)
    fieldMacros(SFVec3f,    gravityUp,         ProtoViewpoint)

    kambiViewpointCommonFieldMacros(ProtoViewpoint)

    fieldMacros(SFBool,     bind, ProtoViewpoint)
    fieldMacros(SFBool,     isActive, ProtoViewpoint)
    fieldMacros(SFFloat,    zFar, ProtoViewpoint)
    fieldMacros(SFFloat,    zNear, ProtoViewpoint)

    fieldMacros(SFBool,     set_bind,          ProtoViewpoint)
protected:
    float                   m_matrix[16];
};

#endif // _NODE_VIEWPOINT_H
