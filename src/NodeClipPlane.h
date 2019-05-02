/*
 * NodeClipPlane.h
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

#ifndef _NODE_CLIP_PLANE_H
#define _NODE_CLIP_PLANE_H

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

class ProtoClipPlane : public Proto {
public:
                    ProtoClipPlane(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_CLIP_PLANE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex enabled;
    FieldIndex plane;
    FieldIndex cappingColor;
    FieldIndex cappingStrength;
    FieldIndex on;

};

class NodeClipPlane : public Node {
public:
                    NodeClipPlane(Scene *scene, Proto *proto);
                    ~NodeClipPlane();

    virtual const char* getComponentName(void) const { return "Rendering"; }
    virtual int         getComponentLevel(void) const { return 5; }
    virtual int     getX3dVersion(void) const { return 2; }
    virtual Node   *copy() const { return new NodeClipPlane(*this); }

    void            bind();
    void            unbind();

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual Vec3f   getHandle(int handle, int *constraint, int *field);
    virtual void    setHandle(int handle, const Vec3f &v);

    void            drawHandles(void);

    fieldMacros(SFBool,  enabled,         ProtoClipPlane);
    fieldMacros(SFVec4f, plane,           ProtoClipPlane);
    fieldMacros(SFColor, cappingColor,    ProtoClipPlane);
    fieldMacros(SFFloat, cappingStrength, ProtoClipPlane);
    fieldMacros(SFBool,  on,              ProtoClipPlane);

protected:
    int m_clipPlane;
};

#endif
