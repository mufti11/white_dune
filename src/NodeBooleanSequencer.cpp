/*
 * NodeBooleanSequencer.cpp
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

#include "NodeBooleanSequencer.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFImage.h"
#include "SFBool.h"
#include "Texture.h"
#include "Scene.h"
#include "FieldCommand.h"

ProtoBooleanSequencer::ProtoBooleanSequencer(Scene *scene)
  : ProtoInterpolator(scene, "BooleanSequencer", SFBOOL, MFBOOL, new MFBool())
{
}

Node *
ProtoBooleanSequencer::create(Scene *scene)
{ 
    return new NodeBooleanSequencer(scene, this); 
}

NodeBooleanSequencer::NodeBooleanSequencer(Scene *scene, Proto *def)
  : Interpolator(scene, def)
{
}

NodeBooleanSequencer::~NodeBooleanSequencer()
{
}

int
NodeBooleanSequencer::getComponentLevel(void) const
{
    return 1;
}

const char* 
NodeBooleanSequencer::getComponentName(void) const
{
    static const char* name = "EventUtilities";
    return name;
}

float
NodeBooleanSequencer::getKeyValue(int channel, int index) const
{
    MFBool *keyValue = (MFBool *) getField(m_keyValueField);
    int i = index * getNumChannels() + channel;
    if (i < keyValue->getSize()) {
        return keyValue->getValue(i);
    } else {
        return 0.0f;
    }
}

FieldValue *
NodeBooleanSequencer::getInterpolatedFieldValue(float k)
{
    m_fraction = k;
    MFBool *keyValue = (MFBool *) getField(m_keyValueField);
    int i = findKey(k);
    if (i == key()->getSize())
        i--;
    return keyValue->getSFValue(i);
}

FieldValue *
NodeBooleanSequencer::createKey(void *value) const
{
    return new MFBool((bool *)value, getNumChannels());
}

FieldValue *
NodeBooleanSequencer::createKeys(void *value, int numKeys) const
{
    return new MFBool((bool *)value, numKeys * getNumChannels());
}


void
NodeBooleanSequencer::setKeyValue(int channel, int index, float value)
{
    MFBool *keyValue = (MFBool *) getField(m_keyValueField);

    bool val = false;
    if (value >= 0.5f)
        val = true;
    keyValue->setSFValue(index * getNumChannels() + channel, val);
    m_scene->OnFieldChange(this, m_keyValueField, index);
}

void
NodeBooleanSequencer::insertKey(int pos, float key, const float *values)
{
    MFFloat *keys = (MFFloat *) getField(m_keyField);
    MFBool *keyValues = (MFBool *) getField(m_keyValueField);
    int numKeys = keys->getSize();
    int numChannels = 1;
    const float *k = keys->getValues();
    const bool *v = keyValues->getValues();

    float *newK = new float[numKeys + 1];
    bool *newV = new bool[(numKeys + 1) * numChannels];

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
            newV[pos * numChannels + i] = (values[i] >= 1.0f) ? true : false;
        } else {
            newV[pos * numChannels + i] = false;
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


