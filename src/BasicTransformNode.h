/*
 * BasicTransformNode.h
 *
 * Copyright (C) 1999 Stephen F. White, 2008 J. "MUFTI" Scheurich
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

// basic class for the Transform node, but without center, scale 
// and scaleOrientation fields (usefull for nodes like CollidableOffset)

#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "Matrix.h"

#include "swt.h"

#include "SFMFTypes.h"

class MFNode;

class BasicTransformProto : public WonderlandExportProto {
public:
                    BasicTransformProto(Scene *scene, const char *name);

    void            addElements(void);

    FieldIndex      rotation;
    FieldIndex      translation;
    FieldIndex      bboxCenter;
    FieldIndex      bboxSize;
};

class BasicTransformNode : public Node {
public:
                      BasicTransformNode(Scene *scene, Proto *proto);
                     ~BasicTransformNode();

public:
    virtual int       getBasicChildrenField(void) = 0;

    virtual MFNode   *getBasicChildren(void) 
                         { return (MFNode *)getField(getBasicChildrenField()); }

    // X3D profile changes addChildren, removeChildren
    virtual void      setField(int index, FieldValue *value, int cf = -1);
    virtual void      drawHandles(void);
    virtual void      transform();
    virtual void      transformForHandle(int handle);

    virtual void      preDraw();
    virtual void      draw(int pass); 

    virtual void      flip(int index);
    virtual void      swap(int fromTo);

    virtual int       countPolygons(void)
                         { return getBasicChildren()->countPolygons(); } 
    virtual int       countPrimitives(void) 
                         { return getBasicChildren()->countPrimitives(); } 

    virtual int       countPolygons1Sided(void)
                         { return getBasicChildren()->countPolygons1Sided(); } 
    virtual int       countPolygons2Sided(void)
                         { return getBasicChildren()->countPolygons2Sided(); } 

    virtual Vec3f     getHandle(int handle, int *constraint, int *field);
    virtual void      setHandle(int handle, const Vec3f &v);

    virtual void      receiveEvent(int eventIn, double timestamp, FieldValue *value);

    void              getMatrix(double* matrix);
    void              setMatrix(double* matrix);
    Quaternion        getQuat(void);
    void              setQuat(Quaternion quat);    

    virtual int       writeAc3d(int filedes, int indent);
    virtual int       writeRib(int filedes, int indent);
    virtual int       writeCattGeo(int filedes, int indent);
    virtual int       writeLdrawDat(int filedes, int indent);

    fieldMacros(SFRotation, rotation,         BasicTransformProto)
    fieldMacros(SFVec3f,    translation,      BasicTransformProto)
    fieldMacros(SFVec3f,    bboxCenter,       BasicTransformProto)
    fieldMacros(SFVec3f,    bboxSize,         BasicTransformProto)

    void              drawRotationHandles(float scale); 
    bool              modelViewIsIdentity(void);
protected:
    bool              m_matrixDirty;
    Matrixd           m_matrix;
    float             m_handleScale;
};
