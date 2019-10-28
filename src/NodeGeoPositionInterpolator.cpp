/*
 * NodeGeoPositionInterpolator.cpp
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

#include <stdio.h>
#include "stdafx.h"

#include "swt.h"

#include "NodeGeoPositionInterpolator.h"
#include "GeoMacros.h"
#include "Proto.h"
#include "MFVec3d.h"
#include "ExposedField.h"
#include "Field.h"
#include "RenderState.h"
#include "DuneApp.h"
#include "Util.h"
#include "Vec3d.h"
#include "Scene.h"
#include "FieldCommand.h"

ProtoGeoPositionInterpolator::ProtoGeoPositionInterpolator(Scene *scene)
  : ProtoInterpolator(scene, "GeoPositionInterpolator", SFVEC3D, MFVEC3D,
                      new MFVec3d())
{
    setFieldFlags(keyValue, FF_X3D_ONLY);

    keyValueVRML.set(
          addExposedField(MFSTRING, "keyValue", new MFString()));
    setFieldFlags(keyValueVRML, FF_VRML_ONLY);

    onGreatCircle.set(
          addExposedField(SFBOOL, "onGreatCircle", new SFBool(false)));
    setFieldFlags(onGreatCircle, FF_X3DOM_ONLY);

    geovalue_changed.set(
        addEventOut(SFVEC3D , "geovalue_changed", FF_X3D_ONLY));
    addEventOut(SFSTRING, "geovalue_changed", FF_VRML_ONLY);
}

Node *
ProtoGeoPositionInterpolator::create(Scene *scene)
{ 
    return new NodeGeoPositionInterpolator(scene, this); 
}

int 
ProtoGeoPositionInterpolator::translateField(int field) const
{
    bool x3d = m_scene->isX3d();
    if (x3d && (field == keyValue))
        return keyValue;
    else if (!x3d && (field == keyValue))
        return keyValueVRML;
    return field;
}


NodeGeoPositionInterpolator::NodeGeoPositionInterpolator(Scene *scene, 
                                                         Proto *def)
  : Interpolator(scene, def)
{
}

void
NodeGeoPositionInterpolator::setField(int index, FieldValue *value, int cf)
{
    if (index == keyValueVRML_Field()) {
        MFVec3d *value3d = new MFVec3d((MFString *)value);
        Node::setField(keyValue_Field(), value3d);
    }
    Node::setField(index, value, cf);
    update();
}

Node *
NodeGeoPositionInterpolator::convert2Vrml(void) 
{
    const double *values = keyValue()->getValues();
    MFString *strings = new MFString();
    for (int i = 0; i < keyValue()->getSFSize(); i++) {
        char string[4096];
        mysnprintf(string, 4095, "%g %g %g",
                   values[i * 3], values[i * 3 + 1], values[i * 3 + 2]);
        strings->insertSFValue(i, strdup(string));
    }
    keyValueVRML(strings);    

    return NULL;
}

float
NodeGeoPositionInterpolator::getKeyValue(int channel, int index) const
{
    MFVec3d *keyValue = (MFVec3d *) getField(m_keyValueField);
    int i = index * getNumChannels() + channel;
    if (i < keyValue->getSize()) {
        const double *ret = keyValue->getValue(i);
        return (float)*ret;
    } else {
        return 0.0f;
    }
}

FieldValue *
NodeGeoPositionInterpolator::getInterpolatedFieldValue(float k)
{
    m_fraction = k;
    double *values = new double[getNumChannels()];
    interpolate(k, values);
    FieldValue *val = createKey(values);
    val->ref();
    return val;
}

FieldValue *
NodeGeoPositionInterpolator::createKey(void *value) const
{
    return new SFVec3d((double *)value);
}

FieldValue *
NodeGeoPositionInterpolator::createKeys(void *value, int numKeys) const
{
    return new MFVec3d((double *)value, numKeys * getNumChannels());
}

void
NodeGeoPositionInterpolator::setKeyValue(int channel, int index, float value)
{
    MFVec3d *keyValue = (MFVec3d *) getField(m_keyValueField);

    keyValue->setValue(index * getNumChannels() + channel, (double)value);
    m_scene->OnFieldChange(this, m_keyValueField, index);
}

void
NodeGeoPositionInterpolator::insertKey(int pos, float key, const float *values)
{
    MFFloat *keys = (MFFloat *) getField(m_keyField);
    MFVec3d *keyValues = (MFVec3d *) getField(m_keyValueField);
    int numKeys = keys->getSize();
    int numChannels = 1;
    const float *k = keys->getValues();
    const double *v = keyValues->getValues();

    float *newK = new float[numKeys + 1];
    double *newV = new double[(numKeys + 1) * numChannels];

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

    MFDouble *newKey = new MFDouble(newK, numKeys + 1);
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
NodeGeoPositionInterpolator::interpolate(float key, double *values)
{
    MFDouble *keyValue = (MFDouble *) getField(m_keyValueField);
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

    const double *value = keyValue->getValues();

    float alpha;

    if (key1 == key2) {
        alpha = 0.0;
    } else {
        alpha = (key - key1) / (key2 - key1);
    }

    for (int i = 0; i < numChannels; i++) {
        double val1 = value[pos1 * numChannels + i];
        double val2 = value[pos2 * numChannels + i];
        values[i] = val1 + (val2 - val1) * alpha;
    }
}

void
NodeGeoPositionInterpolator::sendInterpolatedEvent(double timestamp, float k)
{
    Interpolator::sendInterpolatedEvent(timestamp, k);
    Node::sendEvent(geovalue_changed_Field(), timestamp, 
                    getInterpolatedFieldValue(k));
}



