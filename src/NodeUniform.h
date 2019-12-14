/*
 * NodeUniform.h
 *
 * Copyright (C) 1999 Stephen F. White, 2018 J. "MUFTI" Scheurich
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

#ifndef _NODE_UNIFORM_H
#define _NODE_UNIFORM_H

#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"

class ProtoUniform : public Proto {
public:
                    ProtoUniform(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_UNIFORM; }

    FieldIndex name;
    FieldIndex type;
    FieldIndex value;
};

class NodeUniform : public Node {
public:
                    NodeUniform(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual int     getX3dVersion(void) const { return -1; }
    virtual Node   *copy() { return new NodeUniform(m_scene, m_proto); }

    fieldMacros(SFString, name,  ProtoUniform)
    fieldMacros(SFString, type,  ProtoUniform)
    fieldMacros(SFString, value, ProtoUniform)
};


#endif
