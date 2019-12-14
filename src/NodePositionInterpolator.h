/*
 * NodePositionInterpolator.h
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

#ifndef _NODE_POSITIONINTERPOLATOR_H
#define _NODE_POSITIONINTERPOLATOR_H

#ifndef _INTERPOLATOR_H
#include "Interpolator.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif

#include "SFMFTypes.h"

class ProtoPositionInterpolator : public ProtoInterpolator {
public:
                    ProtoPositionInterpolator(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_POSITION_INTERPOLATOR; }
};

class NodePositionInterpolator : public Interpolator {
public:
                        NodePositionInterpolator(Scene *scene, Proto *proto);

    virtual int         getX3dVersion(void) const { return 0; }
    virtual Node       *copy() { return new NodePositionInterpolator(
                                 getScene(), getProto()); }

    virtual int         getStride() { return 3; } 
    virtual int         getNumChannels() { return 3; }
    virtual FieldValue *createKey(void *value);
    virtual FieldValue *createKeys(void *values, int numKeys);

    ADD_FLIP
    ADD_SWAP

    fieldMacros(MFVec3f, keyValue, ProtoPositionInterpolator)

    virtual void    setInternal(bool flag) { m_isInternal = flag; }
    bool            isInternal() { return m_isInternal; }

    virtual void    setField(int index, FieldValue *value, int cf = -1);

private:
    bool            m_isInternal;
};

#endif // _NODE_POSITIONINTERPOLATOR_H
