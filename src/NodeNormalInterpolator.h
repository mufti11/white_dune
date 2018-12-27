/*
 * NodeNormalInterpolator.h
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

#ifndef _NODE_NORMALINTERPOLATOR_H
#define _NODE_NORMALINTERPOLATOR_H

#ifndef _INTERPOLATOR_H
#include "Interpolator.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif

#include "SFMFTypes.h"

class ProtoNormalInterpolator : public ProtoInterpolator {
public:
                    ProtoNormalInterpolator(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_NORMAL_INTERPOLATOR; }

    virtual bool    isDeclaredInRwd_h() { return true; }      
};

class NodeNormalInterpolator : public Interpolator {
public:
                        NodeNormalInterpolator(Scene *scene, Proto *proto);

    virtual int         getX3dVersion(void) const { return 0; }
    virtual Node       *copy() const 
                           { return new NodeNormalInterpolator(*this); }

    virtual int         getNumChannels() const;
    virtual FieldValue *createKey(float *value) const;
    virtual FieldValue *createKeys(float *value, int numKeys) const;
    virtual void        recordValue(int key, FieldValue *value);

    ADD_FLIP
    ADD_SWAP

    fieldMacros(MFVec3f, keyValue, ProtoNormalInterpolator)
protected:
    int                 m_keySize;
};

#endif // _NODE_NORMALINTERPOLATOR_H
