/*
 * NodePositionInterpolator2D.h
 *
 * Copyright (C) 1999 Stephen F. White, 2007 J. "MUFTI" Scheurich
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

#ifndef _NODE_POSITION_INTERPOLATOR_2D_H
#define _NODE_POSITION_INTERPOLATOR_2D_H

#ifndef _INTERPOLATOR_H
#include "Interpolator.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif

#include "SFMFTypes.h"

class ProtoPositionInterpolator2D : public ProtoInterpolator {
public:
                    ProtoPositionInterpolator2D(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_POSITION_INTERPOLATOR_2D; }

    virtual bool    isX3dInternalProto(void) { return true; }

    virtual bool    isDeclaredInRwd_h() { return true; }      
};

class NodePositionInterpolator2D : public Interpolator {
public:
                        NodePositionInterpolator2D(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const { return "Interpolation"; }
    virtual int         getComponentLevel(void) const { return 3; }
    virtual int         getX3dVersion(void) const { return 0; } 
    virtual Node       *copy() const 
                           { return new NodePositionInterpolator2D(*this); }

    virtual int         getNumChannels() const { return 2; }
    virtual FieldValue *createKey(void *value) const;
    virtual FieldValue *createKeys(void *values, int numKeys) const;

    virtual int         writeProto(int f) { return writeX3dProto(f); }

    fieldMacros(MFVec2f, keyValue, ProtoPositionInterpolator2D)
};

#endif 
