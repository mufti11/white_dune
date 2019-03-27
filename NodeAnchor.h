/*
 * NodeAnchor.h
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

#ifndef _NODE_ANCHOR_H
#define _NODE_ANCHOR_H

#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif
#include "NodeGroup.h"

#include "SFMFTypes.h"

class ProtoAnchor : public ProtoGroup {
public:
                    ProtoAnchor(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_ANCHOR; }

    FieldIndex     description;
    FieldIndex     parameter;
    FieldIndex     url;
    FieldIndex     render;
};

class NodeAnchor : public NodeGroup {
public:
                    NodeAnchor(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERACTIVE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeAnchor(*this); }

    virtual bool    hasNumbers4kids(void) { return true; }

    virtual void    preDraw() { children()->preDraw(); }
    virtual void    draw(int pass) { children()->draw(pass, children_Field()); }

    fieldMacros(SFString, description, ProtoAnchor)
    fieldMacros(MFString, parameter,   ProtoAnchor)
    fieldMacros(MFString, url,         ProtoAnchor)
    fieldMacros(SFBool,   render,      ProtoAnchor)
};

#endif // _NODE_ANCHOR_H
