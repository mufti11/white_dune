/*
 * NodeCubeTexture.h
 *
 * Copyright (C) 1999 Stephen F. White, 2005 J. "MUFTI" Scheurich
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
 * 
 * Implements the Cover/Covise CubeTexture node
 */

#pragma once

#include "CoverNode.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "SFMFTypes.h"

class ProtoCubeTexture : public Proto {
public:
                    ProtoCubeTexture(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return COVER_CUBE_TEXTURE; }
    virtual int     getNodeClass() const { return TEXTURE_NODE | URL_NODE; }

    FieldIndex urlXP;
    FieldIndex urlXN;
    FieldIndex urlYP;
    FieldIndex urlYN;
    FieldIndex urlZP;
    FieldIndex urlZN;
    FieldIndex repeatS;
    FieldIndex repeatT;
    FieldIndex blendMode;

    virtual bool          isCoverProto(void) { return true; }
};

class NodeCubeTexture : public CoverNode {
public:
                    NodeCubeTexture(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return -1; }
    virtual Node   *copy() const { return new NodeCubeTexture(*this); }

    fieldMacros(MFString, urlXP,     ProtoCubeTexture)
    fieldMacros(MFString, urlXN,     ProtoCubeTexture)
    fieldMacros(MFString, urlYP,     ProtoCubeTexture)
    fieldMacros(MFString, urlYN,     ProtoCubeTexture)
    fieldMacros(MFString, urlZP,     ProtoCubeTexture)
    fieldMacros(MFString, urlZN,     ProtoCubeTexture)
    fieldMacros(SFBool,   repeatS,   ProtoCubeTexture)
    fieldMacros(SFBool,   repeatT,   ProtoCubeTexture)
    fieldMacros(SFInt32,  blendMode, ProtoCubeTexture)
};
