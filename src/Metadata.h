/*
 * Metadata.h
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


#ifndef _NODE_METADATA_H
#define _NODE_METADATA_H

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

class ProtoMetadata : public Proto {
public:
                    ProtoMetadata(Scene *scene, const char *nodeName);

    virtual int     getNodeClass() const { return METADATA_NODE; }

    FieldIndex name;
    FieldIndex reference;
};

class Metadata : public Node {
public:
                    Metadata(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_CORE; }
    virtual Node   *copy() = 0;

    fieldMacros(SFString, name,      ProtoMetadata)
    fieldMacros(SFString, reference, ProtoMetadata)
};

#endif // _NODE_METADATA_H
