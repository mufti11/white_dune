/*
 * NodeDepthMode.h
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

#pragma once

#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "DuneApp.h"
#include "SFMFTypes.h"

class ProtoDepthMode : public Proto {
public:
                    ProtoDepthMode(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_COLOR_MASK_MODE; }

    virtual bool    isX3domProto(void) { return true; }

    FieldIndex depthFunc;
    FieldIndex enableDepthTest;
    FieldIndex readOnly;
    FieldIndex zFarRange;
    FieldIndex zNearRange;
};

class NodeDepthMode : public Node {
public:
                    NodeDepthMode(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeDepthMode(*this); }

    virtual int     getX3dVersion(void) const { return -1; }

    virtual bool    showFields() { return true; }

    fieldMacros(SFString, depthFunc,       ProtoDepthMode)
    fieldMacros(SFBool,   enableDepthTest, ProtoDepthMode)
    fieldMacros(SFBool,   readOnly,        ProtoDepthMode)
    fieldMacros(SFFloat,  zFarRange,       ProtoDepthMode)
    fieldMacros(SFFloat,  zNearRange,      ProtoDepthMode)
};

