/*
 * NodeInlineLoadControl.h
 *
 * Copyright (C) 1999 Stephen F. White, 2004 J. "MUFTI" Scheurich
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

#pragma once

#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "NodeInline.h"
#include "SFMFTypes.h"

class ProtoInlineLoadControl : public ProtoInline {
public:
                    ProtoInlineLoadControl(Scene *scene);

    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_INLINE_LOAD_CONTROL; }
    virtual int     getProfile(void) const { return PROFILE_IMMERSIVE; }

    FieldIndex      load;
    FieldIndex      children;
};

class NodeInlineLoadControl : public NodeInline {
public:
                    NodeInlineLoadControl(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeInlineLoadControl(*this); }

    int             writeProto(int f);
    int             write(int filedes, int indent, bool avoidUse = false);

    virtual void    preDraw(void);
    virtual void    draw(int pass);

    fieldMacros(SFBool, load,     ProtoInlineLoadControl)
    fieldMacros(MFNode, children, ProtoInlineLoadControl)
};
