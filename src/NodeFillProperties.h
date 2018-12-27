/*
 * NodeFillProperties.h
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

#ifndef _NODE_FILL_PROPERTIES_H
#define _NODE_FILL_PROPERTIES_H

#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif
#ifndef _DUNEAPP_H
#include "DuneApp.h"
#endif

#include "SFMFTypes.h"

class ProtoFillProperties : public Proto {
public:
                    ProtoFillProperties(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_FILL_PROPERTIES; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex filled;
    FieldIndex hatchColor;
    FieldIndex hatched;
    FieldIndex hatchStyle;
};


class NodeFillProperties : public Node {
public:
                    NodeFillProperties(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const { return "Shape"; }
    virtual int         getComponentLevel(void) const { return 3; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeFillProperties(*this); }

    virtual bool    isInvalidChildNode(void) { return true; }

    virtual int     writeProto(int f) { return writeX3dProto(f); }

    fieldMacros(SFBool,  filled,     ProtoFillProperties)
    fieldMacros(SFColor, hatchColor, ProtoFillProperties)
    fieldMacros(SFBool,  hatched,    ProtoFillProperties)
    fieldMacros(SFInt32, hatchStyle, ProtoFillProperties)
};

#endif // _NODE_FILL_PROPERTIES_H
