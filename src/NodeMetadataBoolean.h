/*
 * NodeMetadataBoolean.h
 *
 * Copyright (C) 2011 J. "MUFTI" Scheurich
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

#ifndef _NODE_METADATA_BOOLEAN_H
#define _NODE_METADATA_BOOLEAN_H

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

class ProtoMetadataBoolean : public Proto {
public:
                    ProtoMetadataBoolean(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_METADATA_BOOLEAN; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex name;
    FieldIndex reference;
    FieldIndex value;
};

class NodeMetadataBoolean : public Node {
public:
                    NodeMetadataBoolean(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return 3; }
    virtual Node *copy() { return new NodeMetadataBoolean(m_scene, m_proto); }

    fieldMacros(SFString, name, ProtoMetadataBoolean);
    fieldMacros(SFString, reference, ProtoMetadataBoolean);
    fieldMacros(MFBool, value, ProtoMetadataBoolean);
};

#endif
