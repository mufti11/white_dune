/*
 * NodeMetadataDouble.h
 *
 * Copyright (C) 2006 J. "MUFTI" Scheurich
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

#ifndef _NODE_METADATA_DOUBLE_H
#define _NODE_METADATA_DOUBLE_H

#include "Metadata.h"

class ProtoMetadataDouble : public ProtoMetadata {
public:
                    ProtoMetadataDouble(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_METADATA_DOUBLE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex value;
};

class NodeMetadataDouble : public Metadata {
public:
                    NodeMetadataDouble(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() { return new NodeMetadataDouble(m_scene, m_proto); }

    fieldMacros(MFDouble, value, ProtoMetadataDouble)
};

#endif // _NODE_METADATA_DOUBLE_H
