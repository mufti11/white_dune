/*
 * NodeIntegerSequencer.cpp
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

#include <stdio.h>
#include "stdafx.h"

#include "swt.h"
#include "DuneApp.h"
#include "resource.h"

#include "NodeIntegerSequencer.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFImage.h"
#include "SFBool.h"
#include "Texture.h"
#include "Scene.h"
#include "FieldCommand.h"

ProtoIntegerSequencer::ProtoIntegerSequencer(Scene *scene)
  : ProtoInterpolator(scene, "IntegerSequencer", SFINT32, MFINT32,
                      new MFInt32())
{
}

Node *
ProtoIntegerSequencer::create(Scene *scene)
{ 
    return new NodeIntegerSequencer(scene, this); 
}

NodeIntegerSequencer::NodeIntegerSequencer(Scene *scene, Proto *def)
  : Interpolator(scene, def)
{
}

NodeIntegerSequencer::~NodeIntegerSequencer()
{
}

int
NodeIntegerSequencer::getComponentLevel(void) const
{
    return 1;
}

const char* 
NodeIntegerSequencer::getComponentName(void) const
{
    static const char* name = "EventUtilities";
    return name;
}

float
NodeIntegerSequencer::getKeyValue(int channel, int index) const
{
    MFInt32 *keyValue = (MFInt32 *) getField(m_keyValueField);
    int i = index * getNumChannels() + channel;
    if (i < keyValue->getSize()) {
        return keyValue->getValue(i);
    } else {
        return 0.0f;
    }
}

FieldValue *
NodeIntegerSequencer::getInterpolatedFieldValue(float k)
{
    m_fraction = k;
    MFInt32 *keyValue = (MFInt32 *) getField(m_keyValueField);
    int i = findKey(k);
    if (i == key()->getSize())
        i--;
    return keyValue->getSFValue(i);
}

FieldValue *
NodeIntegerSequencer::createKey(void *value) const
{
    return new MFInt32((int *)value, getNumChannels());
}

FieldValue *
NodeIntegerSequencer::createKeys(void *value, int numKeys) const
{
    return new MFInt32((int *)value, numKeys * getNumChannels());
}

void
NodeIntegerSequencer::setKeyValue(int channel, int index, float value)
{
    MFInt32 *keyValue = (MFInt32 *) getField(m_keyValueField);

    keyValue->setSFValue(index * getNumChannels() + channel, (int)value);
    m_scene->OnFieldChange(this, m_keyValueField, index);
}

void
NodeIntegerSequencer::insertKey(int pos, float key, const float *values)
{
    MFFloat *keys = (MFFloat *) getField(m_keyField);
    MFInt32 *keyValues = (MFInt32 *) getField(m_keyValueField);
    int numKeys = keys->getSize();
    int numChannels = 1;
    const float *k = keys->getValues();
    const int *v = keyValues->getValues();

    float *newK = new float[numKeys + 1];
    int *newV = new int[(numKeys + 1) * numChannels];

    if (pos > 0) {
        memcpy(newK, k, pos * sizeof(float));
        memcpy(newV, v, pos * numChannels * sizeof(int));
    }

    if (pos < numKeys) {
        memcpy(newK + pos + 1, k + pos, (numKeys - pos) * sizeof(float));
        memcpy(newV + (pos + 1) * numChannels, v + pos * numChannels, 
               (numKeys - pos) * numChannels * sizeof(int));
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

