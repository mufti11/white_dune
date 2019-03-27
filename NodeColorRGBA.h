/*
 * NodeColorRGBA.h
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

#ifndef _NODE_COLOR_RGBA_H
#define _NODE_COLOR_RGBA_H

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

class ProtoColorRGBA : public WonderlandExportProto {
public:
                    ProtoColorRGBA(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_COLOR_RGBA; }
    virtual int     getNodeClass() const { return COLOR_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex      color;
};

class NodeColorRGBA : public Node {
public:
                    NodeColorRGBA(Scene *scene, Proto *proto);
    virtual const char* getComponentName(void) const { return "Rendering"; }
    virtual int         getComponentLevel(void) const;
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeColorRGBA(*this); }

    virtual bool    isInvalidChildNode(void) { return true; }

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual NodeColorRGBA *getColorRGBANode() { return this; }

    void            drawHandles(void);
    Vec3f           getHandle(int handle, int *constraint, int *field);
    void            setHandle(int handle, const Vec3f &v);

    virtual int     writeProto(int f) { return writeX3dProto(f); }

    fieldMacros(MFColorRGBA, color, ProtoColorRGBA)

};

#endif
