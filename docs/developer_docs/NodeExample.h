/*
 * NodeExample.h
 *
 * Copyright (C) 1999 Stephen F. White, 2006 J. "MUFTI" Scheurich
 * 
 * Example of a minimal VRML97 node implementation
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
 * not, write to the Free Software Foundation, Inc., 
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef _NODE_EXAMPLE_H
#define _NODE_EXAMPLE_H

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

class ProtoExample : public Proto {
public:
		    ProtoExample(Scene *scene);
    virtual Node   *create(Scene *scene);

    FieldIndex field1;
    FieldIndex field2;
};

class NodeExample : public Node {
public:
		    NodeExample(Scene *scene, Proto *proto);

    virtual int	    getType() const { return DUNE_EXAMPLE; }
    virtual Node   *copy() const { return new NodeExample(*this); }

    //replace SFExampleTyp or MFExampleTyp with something like SFBool or MFVec3f

    fieldMacros(SFExampleTyp, field1, ProtoExample)
    fieldMacros(MFExampleTyp, field2, ProtoExample)
};

#endif // _NODE_EXAMPLE_H
