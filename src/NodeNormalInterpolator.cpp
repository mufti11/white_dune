/*
 * NodeNormalInterpolator.cpp
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

#include "NodeNormalInterpolator.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFFloat.h"
#include "MFVec3f.h"
#include "SFVec3f.h"

ProtoNormalInterpolator::ProtoNormalInterpolator(Scene *scene)
  : ProtoInterpolator(scene, "NormalInterpolator", MFVEC3F, MFVEC3F,
                      new MFVec3f())
{
}

Node *
ProtoNormalInterpolator::create(Scene *scene)
{
    return new NodeNormalInterpolator(scene, this); 
}

NodeNormalInterpolator::NodeNormalInterpolator(Scene *scene, Proto *def)
  : Interpolator(scene, def)
{
    m_keySize = 0;
}

int
NodeNormalInterpolator::getNumChannels()
{
    if (m_keySize == 0) {
        MFFloat *key = (MFFloat *)getField(
                            ((NodeNormalInterpolator *)this)->key_Field());
        int numKeys = key->getSize();
        const MFVec3f *keyValue = (MFVec3f *) getField(
               ((NodeNormalInterpolator *)this)->keyValue_Field());

        if (numKeys != 0) {
            return ((MFVec3f *)keyValue)->getSize() / numKeys;
        }
    }
    return m_keySize;
}

FieldValue *
NodeNormalInterpolator::createKey(void *value)
{
    return new MFVec3f((float *)value, 
                       ((NodeNormalInterpolator *)this)->getNumChannels());
}

FieldValue *
NodeNormalInterpolator::createKeys(void *value, int numKeys)
{
    return new MFVec3f((float *)value, 
                       numKeys * 
                       ((NodeNormalInterpolator *)this)->getNumChannels());
}

void 
NodeNormalInterpolator::recordValue(int key, FieldValue *value)
{
    MFVec3f *mfkeyValue = keyValue();
 
    m_keySize = ((MFVec3f *) value)->getSize();

    for (int i = 0; i < m_keySize; i++) {
        mfkeyValue->setValue(key * m_keySize + i,
                             ((MFFloat *) value)->getValue(i));
    }
}
