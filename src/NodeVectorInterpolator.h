/*
 * NodeVectorInterpolator.h
 *
 * Copyright (C) 2009 J. "MUFTI" Scheurich
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
#include "SFMFTypes.h"

class ProtoVectorInterpolator : public Proto {
public:
                    ProtoVectorInterpolator(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return KAMBI_VECTOR_INTERPOLATOR; }

    virtual bool    isKambiProto(void) { return true; }

    FieldIndex key;
    FieldIndex keyValue;
};

class NodeVectorInterpolator : public Node {
public:
                    NodeVectorInterpolator(Scene *scene, Proto *proto);

//    virtual int     getProfile(void) const { return PROFILE_; }
//    virtual const char* getComponentName(void) const { return ""; }
//    virtual int         getComponentLevel(void) const { return -1; }
    virtual int     getX3dVersion(void) const { return -1; }    
    virtual Node   *copy() const { return new NodeVectorInterpolator(*this); }

    virtual bool    isKambiNode(void) { return true; }

    fieldMacros(MFFloat, key,      ProtoVectorInterpolator);
    fieldMacros(MFFloat, keyValue, ProtoVectorInterpolator);
};

