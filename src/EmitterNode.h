/*
 * EmitterNode.h
 *
 * Copyright (C) 1999 Stephen F. White, 2009 J. "MUFTI" Scheurich
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
#include "NodeAppearance.h"
#include "NodeMaterial.h"
#include "NodeImageTexture.h"
#include "NodeTextureTransform.h"

class EmitterProto : public Proto {
public:
                    EmitterProto(Scene *scene, const char *name);

    virtual int     getNodeClass() const { return PARTICLE_EMITTER_NODE; }

    FieldIndex speed;
    FieldIndex variation;
    FieldIndex mass;
    FieldIndex surfaceArea;
};

class EmitterNode : public Node {
public:
                         EmitterNode(Scene *scene, Proto *proto);
public:
    virtual const char* getComponentName(void) const 
       { return "ParticleSystems"; }

    fieldMacros(SFFloat, speed,       EmitterProto);
    fieldMacros(SFFloat, variation,   EmitterProto);
    fieldMacros(SFFloat, mass,        EmitterProto);
    fieldMacros(SFFloat, surfaceArea, EmitterProto);
};

