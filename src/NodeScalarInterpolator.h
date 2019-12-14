/*
 * NodeScalarInterpolator.h
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

#ifndef _NODE_SCALARINTERPOLATOR_H
#define _NODE_SCALARINTERPOLATOR_H

#ifndef _INTERPOLATOR_H
#include "Interpolator.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif

#include "SFMFTypes.h"

class ProtoScalarInterpolator : public ProtoInterpolator {
public:
                        ProtoScalarInterpolator(Scene *scene);
    virtual Node       *create(Scene *scene);

    virtual int         getType() const { return VRML_SCALAR_INTERPOLATOR; }

    virtual bool        isDeclaredInRwd_h() { return true; }      
};

class NodeScalarInterpolator : public Interpolator {
public:
                        NodeScalarInterpolator(Scene *scene, Proto *proto);

    virtual int         getX3dVersion(void) const { return 0; } 
    virtual Node       *copy() { return new NodeScalarInterpolator(
                                 m_scene, m_proto); }

    virtual FieldValue *createKey(void *value);
    virtual FieldValue *createKeys(void *value, int numKeys);

    virtual int         getNumChannels() { return 1; }

    fieldMacros(MFFloat, keyValue, ProtoScalarInterpolator)
};

#endif // _NODE_SCALARINTERPOLATOR_H
