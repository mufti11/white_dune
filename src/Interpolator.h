/*
 * Interpolator.h
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

#ifndef _INTERPOLATOR_H
#define _INTERPOLATOR_H

#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "MFFloat.h"

class FieldValue;
class CommandList;

class ProtoInterpolator : public WonderlandExportProto {
public:
                        ProtoInterpolator(Scene *scene, const char *name,
                                          int keyType, int keysType, 
                                          FieldValue *defaultValue);
    virtual Node       *create(Scene *scene) = 0;

    int                 getNodeClass() const 
                           { return CHILD_NODE | INTERPOLATOR_NODE; }

    FieldIndex          key;
    FieldIndex          keyValue;
};


class Interpolator : public Node {
public:
                        Interpolator(Scene *scene, Proto *proto);

    virtual int         getProfile(void) const { return PROFILE_INTERCHANGE; }
    
    int                 getNumKeys() const;
    float               getKey(int index) const;
    virtual float       getKeyValue(int channel, int index) const;
    void                setKey(int index, float value);
    virtual void        setKeyValue(int channel, int index, float value);
    void                backupKey(int index);
    void                backup(CommandList *list);
    void                receiveEvent(int eventIn, double timestamp, 
                                     FieldValue *value);
    virtual void        insertKey(int pos, float key, const float *values);
    void                insertKey(int pos, float key, 
                                  const float *values, int numValues);
    void                deleteKeys(int start, int end);
    virtual void        interpolate(float k, float *values);
    virtual FieldValue *getInterpolatedFieldValue(float k);
    virtual void        sendInterpolatedEvent(double timestamp, float k);
    void                sendInterpolatedValue(double, float);
    virtual int         getNumChannels() const = 0;
    virtual FieldValue *createKey(void *value) const = 0;
    virtual FieldValue *createKeys(void *value, int numKeys) const = 0;

    int                 findKey(float value) const;
    int                 findKeyInclusive(float value) const;
    int                 findKeyExclusive(float value) const;
    bool                getNearestKeys(float k, float *k1, float *k2,
                                       int *pos1, int *pos2);

    float               getFraction() const { return m_fraction; }

    void                recordKey(FieldValue *value, bool isrunning);
    virtual void        recordValue(int key, FieldValue *value);

    bool                isInterpolator() { return true; }
    int                 set_fraction_Field() { return m_set_fractionField; }
    int                 value_changed_Field() { return m_value_changedField; }

    fieldMacros(MFFloat, key, ProtoInterpolator);

    // keyValue can not use FieldMacros cause datatype is various at this point

    virtual bool        hasX3domOnoutputchange(void) { return true; }

protected:
    float               m_fraction;
    float               m_oldRecordedFraction;
    FieldIndex          m_set_fractionField;
    FieldIndex          m_keyValueField;
    FieldIndex          m_keyField;
    FieldIndex          m_value_changedField;
};

Interpolator       *dynamic_cast_Interpolator(Node* node);

#define ADD_FLIP void flip(int index)  { keyValue()->flip(index); }
#define ADD_SWAP void swap(int fromTo) { keyValue()->swap(fromTo); }

#endif
