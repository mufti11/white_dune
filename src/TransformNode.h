/*
 * TransformNode.h
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

#pragma once

#include "BasicTransformNode.h"

class TransformProto : public BasicTransformProto {
public:
                    TransformProto(Scene *scene, const char *name);
                    TransformProto(Scene *scene);

    void            addElements(void);

    FieldIndex      center;
    FieldIndex      scale;
    FieldIndex      scaleOrientation;
    FieldIndex      render;
};

class TransformNode : public BasicTransformNode {
public:
                      TransformNode(Scene *scene, Proto *proto);
                     ~TransformNode();

public:
    virtual int       getChildrenField(void) const = 0;

    virtual MFNode   *getChildren(void) 
                         { return (MFNode *)getField(getChildrenField()); }

    virtual int       getBasicChildrenField(void) const 
                         { return getChildrenField(); }

    // X3D profile changes addChildren, removeChildren
    virtual void      setField(int index, FieldValue *value, int cf = -1);
    virtual void      drawHandles(void);
    virtual void      drawRotationHandles(float scale);
    virtual void      transform();
    virtual void      transformForHandle(int handle);

    virtual void      preDraw();
    virtual void      draw(int pass); 

    virtual Vec3f     getHandle(int handle, int *constraint, int *field);
    virtual void      setHandle(int handle, const Vec3f &v);

    virtual void      receiveEvent(int eventIn, double timestamp, FieldValue *value);

    const Vec3f      &getScale(void);
    const Vec3f      &getCenter(void);

    void              flip(int index);
    void              swap(int fromTo);

    void              getInvertMatrix(float* matrix);

    bool              modelViewIsIdentity(void);

    virtual int       writeAc3d(int filedes, int indent);
    virtual int       writeCattGeo(int filedes, int indent);
    virtual int       writeLdrawDat(int filedes, int indent);

    fieldMacros(SFVec3f,    center,           TransformProto)
    fieldMacros(SFVec3f,    scale,            TransformProto)
    fieldMacros(SFRotation, scaleOrientation, TransformProto)
    fieldMacros(SFBool,     render,           TransformProto)
};

