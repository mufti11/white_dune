/*
 * NodeComposedCubeMapTexture.h
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

#ifndef _NODE_COMPOSED_CUBE_MAP_TEXTURE_H
#define _NODE_COMPOSED_CUBE_MAP_TEXTURE_H

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

class ProtoComposedCubeMapTexture : public Proto {
public:
                    ProtoComposedCubeMapTexture(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_COMPOSED_CUBE_MAP_TEXTURE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex back;
    FieldIndex bottom;
    FieldIndex front;
    FieldIndex left;
    FieldIndex right;
    FieldIndex top;
    FieldIndex origChannelCount;
    FieldIndex repeatS;
    FieldIndex repeatT;
    FieldIndex scale;
    FieldIndex textureProperties;
    FieldIndex url; 
};

class NodeComposedCubeMapTexture : public Node {
public:
                    NodeComposedCubeMapTexture(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const { return "CubeMapTexturing"; }
    virtual int         getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 1; }
    virtual Node   *copy() const { return new NodeComposedCubeMapTexture(*this); }

    fieldMacros(SFNode, back, ProtoComposedCubeMapTexture);
    fieldMacros(SFNode, bottom, ProtoComposedCubeMapTexture);
    fieldMacros(SFNode, front, ProtoComposedCubeMapTexture);
    fieldMacros(SFNode, left, ProtoComposedCubeMapTexture);
    fieldMacros(SFNode, right, ProtoComposedCubeMapTexture);
    fieldMacros(SFNode, top, ProtoComposedCubeMapTexture);
    fieldMacros(SFInt32, origChannelCount, ProtoComposedCubeMapTexture);
    fieldMacros(SFBool, repeatS, ProtoComposedCubeMapTexture);
    fieldMacros(SFBool, repeatT, ProtoComposedCubeMapTexture);
    fieldMacros(SFBool, scale, ProtoComposedCubeMapTexture);
    fieldMacros(SFNode, textureProperties, ProtoComposedCubeMapTexture);
    fieldMacros(MFString, url, ProtoComposedCubeMapTexture); 
};

#endif
