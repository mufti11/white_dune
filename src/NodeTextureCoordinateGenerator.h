/*
 * NodeTextureCoordinateGenerator.h
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

#ifndef _NODE_TEXTURE_COORDINATE_GENERATOR_H
#define _NODE_TEXTURE_COORDINATE_GENERATOR_H

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

class ProtoTextureCoordinateGenerator : public Proto {
public:
                    ProtoTextureCoordinateGenerator(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const 
                       { return X3D_TEXTURE_COORDINATE_GENERATOR; }
    virtual int     getNodeClass() const 
                       { 
                       return TEXTURE_COORDINATE_NODE | 
                              GENERATED_TEXTURE_COORDINATE_NODE; 
                       }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex mode;
    FieldIndex parameter;
};

class NodeTextureCoordinateGenerator : public Node {
public:
                    NodeTextureCoordinateGenerator(Scene *scene, Proto *proto);
                    NodeTextureCoordinateGenerator(NodeTextureCoordinateGenerator &node);
    virtual        ~NodeTextureCoordinateGenerator();

    virtual const char* getComponentName(void) const { return "Texturing"; }
    virtual int         getComponentLevel(void) const { return 2; }
    virtual int     getX3dVersion(void) const { return 2; }
    virtual Node *copy() { return new NodeTextureCoordinateGenerator(
                           m_scene, m_proto); }

    virtual bool    isInvalidChildNode(void) { return true; }

    fieldMacros(SFString, mode,      ProtoTextureCoordinateGenerator)
    fieldMacros(MFFloat,  parameter, ProtoTextureCoordinateGenerator)
};

#endif // _NODE_TEXTURE_COORDINATE_GENERATOR_H
