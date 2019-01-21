/*
 * Interpolator.cpp
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

#include "stdafx.h"

#include "Interpolator.h"
#include "FieldValue.h"
#include "MFFloat.h"
#include "Scene.h"
#include "MFieldCommand.h"
#include "FieldCommand.h"
#include "CommandList.h"
#include "InputDeviceTime.h"
#include "Field.h"
#include "swt.h"
#include "NodeCurveAnimation.h"

ProtoInterpolator::ProtoInterpolator(Scene *scene, const char *name, 
                                     int keyType, int keysType, 
                                     FieldValue *defaultValue)
  : WonderlandExportProto(scene, name)
{
    addEventIn(SFFLOAT, "set_fraction", EIF_RECOMMENDED);
    key.set(addExposedField(MFFLOAT, "key", new MFFloat()));
    keyValue.set(addExposedField(keysType, "keyValue", defaultValue));
    addEventOut(keyType, "value_changed", EOF_RECOMMENDED);
}

Interpolator::Interpolator(Scene *scene, Proto *def)
  : Node(scene, def)
{
    m_fraction = 0.0f;
    m_oldRecordedFraction = 1.0f;
    m_set_fractionField.set(getProto()->lookupEventIn("set_fraction"));
    m_keyField.set(((ProtoInterpolator *)def)->key);
    m_keyValueField.set(((ProtoInterpolator *)def)->keyValue);
    m_value_changedField.set(getProto()->lookupEventOut("value_changed"));
}

int
Interpolator::getNumKeys() const
{
    MFFloat *key = (MFFloat *) getField(m_keyField);

    return key->getSize();
}

float
Interpolator::getKey(int index) const
{
    MFFloat *key = (MFFloat *) getField(m_keyField);
    if (index < key->getSize()) {
        return key->getValue(index);
    } else {
        return 0.0f;
    }
}

float
Interpolator::getKeyValue(int channel, int index) const
{
    MFFloat *keyValue = (MFFloat *) getField(m_keyValueField);
    int i = index * getNumChannels() + channel;
    if (i < keyValue->getSize()) {
        return keyValue->getValue(i);
    } else {
        return 0.0f;
    }
}

void
Interpolator::setKey(int index, float value)
{
    MFFloat *key = (MFFloat *) getField(m_keyField);

    key->setValue(index, value);
    m_scene->OnFieldChange(this, m_keyField, index);
}

void
Interpolator::setKeyValue(int channel, int index, float value)
{
    MFFloat *keyValue = (MFFloat *) getField(m_keyValueField);

    keyValue->setValue(index * getNumChannels() + channel, value);
    m_scene->OnFieldChange(this, m_keyValueField, index);
}


void
Interpolator::backupKey(int index)
{
    CommandList *list = new CommandList();

    list->append(new MFieldCommand(this, m_keyField, index));
    list->append(new MFieldCommand(this, m_keyValueField, index));
    m_scene->add(list);
}

void
Interpolator::backup(CommandList *list)
{
    int pos = findKeyInclusive(m_fraction);

    if (pos != getNumKeys()) {
        list->append(new MFieldCommand(this, m_keyField, pos));
        list->append(new MFieldCommand(this, m_keyValueField, pos));
    }
    list->append(new FieldCommand(this, m_keyField));
    list->append(new FieldCommand(this, m_keyValueField));
}

//
// findKey() - return the closest key stricty greater than the given value,
//             or n if none found
//

int
Interpolator::findKey(float value) const
{
    MFFloat *key = (MFFloat *) getField(m_keyField);
    int i, numKeys = key->getSize();

    for (i = 0; i < numKeys; i++) {
        float k = key->getValue(i);
        if (k > value) break;
    }
    return i;
}

//
// findKeyInclusive() - return the closest key greater than or equal to 
//                      the given value, or numKeys if none found
//

int
Interpolator::findKeyInclusive(float value) const
{
    MFFloat *key = (MFFloat *) getField(m_keyField);
    int i, numKeys = key->getSize();

    for (i = 0; i < numKeys; i++) {
        float k = key->getValue(i);
        if (k >= value) break;
    }
    return i;
}

//
// findKeyExclusive() - return the closest key greater than given value, 
//                      or numKeys+1 if none found

int
Interpolator::findKeyExclusive(float value) const
{
    MFFloat *key = (MFFloat *) getField(m_keyField);
    int i, numKeys = key->getSize();

    for (i = 0; i < numKeys; i++) {
        float k = key->getValue(i);
        if (k > value) return(i);
    }
    return numKeys+1;
}


bool
Interpolator::getNearestKeys(float k, float *k1, float *k2, int *pos1, int *pos2)
{
    MFFloat *key = (MFFloat *) getField(m_keyField);
    int numKeys = key->getSize();

    if (numKeys == 0) {
        return false;
    }

    int pos = findKey(k);

    if (pos == 0) {
        *k1 = 0.0f;
        *pos1 = 0;
    } else {
        *k1 = key->getValue(pos - 1);
        *pos1 = pos - 1;
    }

    if (pos == numKeys) {
        *k2 = 1.0f;
        *pos2 = numKeys-1;
    } else {
        *k2 = key->getValue(pos);
        *pos2 = pos;
    }
    return true;
}

void
Interpolator::receiveEvent(int eventIn, double timestamp, FieldValue *value)
{
    float fraction=((SFFloat *) value)->getValue();
    if (eventIn == m_set_fractionField)
        sendInterpolatedEvent(timestamp, fraction);
    else
        Node::receiveEvent(eventIn, timestamp, value);
}

FieldValue *
Interpolator::getInterpolatedFieldValue(float k)
{
    m_fraction = k;
    float *values = new float[getNumChannels()];
    interpolate(k, values);
    FieldValue *val = createKey(values);
    val->ref();
    return val;
}

void
Interpolator::sendInterpolatedEvent(double timestamp, float k)
{
    sendEvent(m_value_changedField, timestamp, getInterpolatedFieldValue(k));
}

void
Interpolator::sendInterpolatedValue(double timestamp, float k)
{
    sendInterpolatedEvent(timestamp,k);          
    // send same values to all Interpolators driven by the same EventIn
    int eventInField = lookupEventIn("set_fraction", false);
    if (eventInField == INVALID_INDEX) {
       swDebugf("Problem: Interpolater without set_fraction ???\n");
       return;
    }
    // for all EventIn's
    SocketList::Iterator* in;
    for (in = m_inputs[eventInField].first(); in != NULL; in = in->next()) {
       Node *eventInSrcNode = in->item().getNode();
       int eventInSrcEventOutIndex = in->item().getField();
       // for all EventOuts of Node of EventIn
       SocketList::Iterator* out;
       for (out = eventInSrcNode->getOutput(eventInSrcEventOutIndex).first(); 
            out != NULL; out = out->next()) 
           if (out->item().getNode() != this) {
               // is node a Interpolator ?
               Node *node = out->item().getNode();
               Interpolator* Inod = dynamic_cast_Interpolator(node);
               if (Inod != NULL)
                   Inod->sendInterpolatedEvent(timestamp, k);
               else if (node->getType() == DUNE_CURVE_ANIMATION) {
                   NodeCurveAnimation *curve = (NodeCurveAnimation *)node;
                   int eventIn = curve->getFraction_Field();
                   SFFloat *fraction = new SFFloat(k);
                   curve->receiveEvent(eventIn, k, fraction);
               }
           }
    }
}


void
Interpolator::interpolate(float key, float *values)
{
    MFFloat *keyValue = (MFFloat *) getField(m_keyValueField);
    int numChannels = getNumChannels();

    float key1 = 0.0;  
    float key2 = 1.0;  
    int   pos1 = 0;
    int   pos2 = 0;  

    if (keyValue->getSize() == 0) {
        for (int i = 0; i < numChannels; i++) {
            values[i] = 0.0f;
        }
        return;
    }

    getNearestKeys(key, &key1, &key2, &pos1, &pos2);

    const float *value = keyValue->getValues();

    float alpha;

    if (key1 == key2) {
        alpha = 0.0;
    } else {
        alpha = (key - key1) / (key2 - key1);
    }

    for (int i = 0; i < numChannels; i++) {
        float val1 = value[pos1 * numChannels + i];
        float val2 = value[pos2 * numChannels + i];
        values[i] = val1 + (val2 - val1) * alpha;
    }
}

void
Interpolator::insertKey(int pos, float key, const float *values, int numValues)
{
    MFFloat *keys = (MFFloat *) getField(m_keyField);
    MFFloat *keyValues = (MFFloat *) getField(m_keyValueField);
    int numKeys = keys->getSize();
    int numChannels = numValues;
    const float *k = keys->getValues();
    const float *v = keyValues->getValues();

    float *newK = new float[numKeys + 1];
    float *newV = new float[(numKeys + 1) * numChannels];

    if (pos > 0) {
        memcpy(newK, k, pos * sizeof(float));
        memcpy(newV, v, pos * numChannels * sizeof(float));
    }

    if (pos < numKeys) {
        memcpy(newK + pos + 1, k + pos, (numKeys - pos) * sizeof(float));
        memcpy(newV + (pos + 1) * numChannels, v + pos * numChannels, 
               (numKeys - pos) * numChannels * sizeof(float));
    }

    newK[pos] = key;
    for (int i = 0; i < numChannels; i++) {
        if (values) {
            newV[pos * numChannels + i] = values[i];
        } else {
            newV[pos * numChannels + i] = 0.0;
        }
    }

    MFFloat *newKey = new MFFloat(newK, numKeys + 1);
    FieldValue *newKeyValue = createKeys(newV, numKeys + 1);
    newKeyValue->ref();    

    if (values) {
        CommandList *list = new CommandList();

        list->append(new FieldCommand(this, m_keyField, newKey));
        list->append(new FieldCommand(this, m_keyValueField, newKeyValue));
        m_scene->execute(list);
    } else {
        setField(m_keyField, newKey);
        setField(m_keyValueField, newKeyValue);
        m_scene->OnFieldChange(this, m_keyField);
        m_scene->OnFieldChange(this, m_keyValueField);
    }
}

void
Interpolator::insertKey(int pos, float key, const float *values)
{
    insertKey(pos, key, values, getNumChannels());
}

void
Interpolator::deleteKeys(int start, int end)
{
    MFFloat *key = (MFFloat *) getField(m_keyField);
    MFFloat *keyValue = (MFFloat *) getField(m_keyValueField);
    int numKeys = key->getSize();
    int numChannels = getNumChannels();
    const float *k = key->getValues();
    const float *v = keyValue->getValues();

    if (start >= end) return;

    int newLen = numKeys - (end - start);
    if (newLen <= 0) {
        newLen = 1;
        start++;
    }
    float *newK = new float[newLen];
    float *newV = new float[newLen * numChannels];

    if (start > 0) {
        memcpy(newK, k, start * sizeof(float));
        memcpy(newV, v, start * numChannels * sizeof(float));
    }

    if (end < numKeys) {
        memcpy(newK + start, k + end, (numKeys - end) * sizeof(float));
        memcpy(newV + start * numChannels, v + end * numChannels, 
               (numKeys - end) * numChannels * sizeof(float));
    }

    MFFloat *newKey = new MFFloat(newK, newLen);
    FieldValue *newKeyValue = createKeys(newV, newLen);

    CommandList *list = new CommandList();

    list->append(new FieldCommand(this, m_keyField, newKey));
    list->append(new FieldCommand(this, m_keyValueField, newKeyValue));
    m_scene->execute(list);
}

void 
Interpolator::recordValue(int key, FieldValue *value)
{
    MFFloat *keyValue = (MFFloat *) getField(m_keyValueField);
    keyValue->setSFValue(key, value);
}

//
// recordKey() -- record a key at the current position (m_fraction)
//

void
Interpolator::recordKey(FieldValue *value, bool isrunning)
{
    int key = findKeyInclusive(m_fraction);

    if (isrunning) {
        int oldKey = findKey(m_oldRecordedFraction);
        int maxOldKey = findKey(m_fraction - 0.01f);
        if (maxOldKey > oldKey)
            oldKey = maxOldKey;
        if (key > oldKey) {
            deleteKeys(oldKey, key);
            key = oldKey;
        }                   
    }
    if (key == getNumKeys() || !EQUALF(getKey(key), m_fraction)) {
        // no exact key found, create one

        insertKey(key, m_fraction, NULL);
    }
    recordValue(key, value);
    m_scene->OnFieldChange(this, m_keyField);
    m_scene->OnFieldChange(this, m_keyValueField);
    if (isrunning)
        m_oldRecordedFraction = m_fraction;
}

//
// this is a ugly "dynamic_cast<Interpolator *>(Node* node)"
// but works with compilers without or defect rtti implementations...
//

Interpolator       *dynamic_cast_Interpolator(Node* node)
{
    if (node == NULL)
       return NULL;
    if (node->isInterpolator())
       return (Interpolator *)(void *)node;
    else
       return NULL;
}     

