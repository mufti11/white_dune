/*
 * NodeCoordinateInterpolator.h
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

#ifndef _NODE_COORDINATEINTERPOLATOR_H
#define _NODE_COORDINATEINTERPOLATOR_H

#ifndef _INTERPOLATOR_H
#include "Interpolator.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif

#include "SFMFTypes.h"

class ProtoCoordinateInterpolator : public ProtoInterpolator {
public:
                    ProtoCoordinateInterpolator(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_COORDINATE_INTERPOLATOR; }

    virtual bool    isDeclaredInRwd_h() { return true; }      
};

class NodeCoordinateInterpolator : public Interpolator {
public:
                        NodeCoordinateInterpolator(Scene *scene, Proto *proto);

    virtual Node       *copy() const 
                           { return new NodeCoordinateInterpolator(*this); }
    virtual int         getX3dVersion(void) const { return 0; }

    virtual int         getStride() const { return 3; }
    virtual int         getNumChannels() const;
    virtual FieldValue *createKey(void *value) const;
    virtual FieldValue *createKeys(void *values, int numKeys) const;
    virtual void        recordValue(int key, FieldValue *value);

    ADD_FLIP
    ADD_SWAP

    fieldMacros(MFVec3f, keyValue, ProtoCoordinateInterpolator)

protected:
    int                 m_keySize;
};

#endif // _NODE_COORDINATEINTERPOLATOR_H
