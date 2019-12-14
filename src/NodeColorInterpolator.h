/*
 * NodeColorInterpolator.h
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

#pragma one 

#include "Interpolator.h"
#include "ProtoMacros.h"
#include "SFMFTypes.h"

class ProtoColorInterpolator : public ProtoInterpolator {
public:
                    ProtoColorInterpolator(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_COLOR_INTERPOLATOR; }

    virtual bool    isDeclaredInRwd_h() { return true; }      
};

class NodeColorInterpolator : public Interpolator {
public:
                        NodeColorInterpolator(Scene *scene, Proto *proto);
    virtual int         getX3dVersion(void) const { return 0; }
    virtual Node       *copy() { return new NodeColorInterpolator(
                                        m_scene, m_proto); } 

    virtual int         getStride() { return 3; }
    virtual int         getNumChannels() { return 3; }
    virtual FieldValue *createKey(void *values);
    virtual FieldValue *createKeys(void *values, int numKeys);
    virtual void        setKeyValue(int channel, int index, float value);

    fieldMacros(MFColor, keyValue, ProtoColorInterpolator)
};

