/*
 * NodeIntegerTrigger.h
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

#ifndef _NODE_INTEGER_TRIGGER_H
#define _NODE_INTEGER_TRIGGER_H

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

class ProtoIntegerTrigger : public Proto {
public:
                    ProtoIntegerTrigger(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_INTEGER_TRIGGER; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex integerKey;
};

class NodeIntegerTrigger : public Node {
public:
                    NodeIntegerTrigger(Scene *scene, Proto *proto);
                    NodeIntegerTrigger(const NodeIntegerTrigger &node);
    virtual        ~NodeIntegerTrigger();

    virtual const char* getComponentName(void) const;
    virtual int         getComponentLevel(void) const;
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeIntegerTrigger(*this); }

    virtual int     writeProto(int f) { return writeX3dProto(f); }

    fieldMacros(SFInt32, integerKey, ProtoIntegerTrigger)
};

#endif // _NODE_INTEGER_TRIGGER_H
