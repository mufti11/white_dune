/*
 * NodeMatrix3VertexAttribute.h
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

#ifndef _NODE_MATRIX_3_VERTEX_ATTRIBUTE_H
#define _NODE_MATRIX_3_VERTEX_ATTRIBUTE_H

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

class ProtoMatrix3VertexAttribute : public Proto {
public:
                    ProtoMatrix3VertexAttribute(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_MATRIX_3_VERTEX_ATTRIBUTE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex value;
    FieldIndex name;
};

class NodeMatrix3VertexAttribute : public Node {
public:
                    NodeMatrix3VertexAttribute(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const { return "Shaders"; }
    virtual int     getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 1; }
    virtual Node   *copy() { return new NodeMatrix3VertexAttribute(
                             m_scene, m_proto); }

    fieldMacros(MFMatrix3f, value, ProtoMatrix3VertexAttribute);
    fieldMacros(SFString, name, ProtoMatrix3VertexAttribute);
};

#endif

