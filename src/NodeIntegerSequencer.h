/*
 * NodeIntegerSequencer.h
 *
 * Copyright (C) 1999 Stephen F. White, 2008 J. "MUFTI" Scheurich
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
#include "Interpolator.h"
#include "ProtoMacros.h"
#include "SFMFTypes.h"

class ProtoIntegerSequencer : public ProtoInterpolator {
public:
                    ProtoIntegerSequencer(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_INTEGER_SEQUENCER; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex key;
    FieldIndex keyValue;
};

class NodeIntegerSequencer : public Interpolator {
public:
                    NodeIntegerSequencer(Scene *scene, Proto *proto);
    virtual        ~NodeIntegerSequencer();

    virtual const char* getComponentName(void) const;
    virtual int         getComponentLevel(void) const;
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeIntegerSequencer(*this); }

    virtual FieldValue *createKey(void *value) const;
    virtual FieldValue *createKeys(void *value, int numKeys) const;

    virtual int         getNumChannels() const { return 1; }

    virtual float       getKeyValue(int channel, int index) const;

    virtual FieldValue *getInterpolatedFieldValue(float k);

    virtual void        setKeyValue(int channel, int index, float value);

    virtual void        insertKey(int pos, float key, const float *values);
 
    fieldMacros(MFInt32, keyValue, ProtoIntegerSequencer)
};

