/*
 * NodeTextureTransform.h
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

#ifndef _NODE_TEXTURETRANSFORM_H
#define _NODE_TEXTURETRANSFORM_H

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

class ProtoTextureTransform : public WonderlandExportProto {
public:
                    ProtoTextureTransform(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_TEXTURE_TRANSFORM; }
    virtual int     getNodeClass() const { return TEXTURE_TRANSFORM_NODE; }

    virtual bool    isDeclaredInRwd_h() { return true; }      

    FieldIndex center;
    FieldIndex rotation;
    FieldIndex scale;
    FieldIndex translation;
};

class NodeTextureTransform : public Node {
public:
                    NodeTextureTransform(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeTextureTransform(*this); }

    virtual bool    isInvalidChildNode(void) { return true; }

    virtual bool    hasNumbers4kids(void) { return true; }

    virtual void    bind();
    virtual void    unbind();

    fieldMacros(SFVec2f, center,      ProtoTextureTransform)
    fieldMacros(SFFloat, rotation,    ProtoTextureTransform)
    fieldMacros(SFVec2f, scale,       ProtoTextureTransform)
    fieldMacros(SFVec2f, translation, ProtoTextureTransform)
};

#endif // _NODE_TEXTURETRANSFORM_H
