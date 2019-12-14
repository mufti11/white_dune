/*
 * NodeCoordinateInterpolator2D.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2007 J. "MUFTI" Scheurich
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

#include "NodeCoordinateInterpolator2D.h"
#include "Scene.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFFloat.h"
#include "MFVec2f.h"

ProtoCoordinateInterpolator2D::ProtoCoordinateInterpolator2D(Scene *scene)
  : ProtoInterpolator(scene, "CoordinateInterpolator2D", MFVEC2F, MFVEC2F,
                      new MFVec2f())
{
    addURLs(URL_X3D);
}

Node *
ProtoCoordinateInterpolator2D::create(Scene *scene)
{ 
    return new NodeCoordinateInterpolator2D(scene, this); 
}

NodeCoordinateInterpolator2D::NodeCoordinateInterpolator2D(Scene *scene, Proto *def)
  : Interpolator(scene, def)
{
    m_keySize = 0;
}

int
NodeCoordinateInterpolator2D::getNumChannels()
{
    if (m_keySize == 0) {
        MFFloat *key = (MFFloat *)getField(key_Field());
        int numKeys = key->getSize();
        MFVec2f *keyValue = (MFVec2f *)getField(keyValue_Field());
        if (numKeys != 0) {
            return keyValue->getSize() / numKeys;
        }
    }
    return m_keySize;
}

FieldValue *
NodeCoordinateInterpolator2D::createKey(void *value)
{
    return new MFVec2f((float *)value, ((NodeCoordinateInterpolator2D *)
                       this)->getNumChannels());
}

FieldValue *
NodeCoordinateInterpolator2D::createKeys(void *value, int numKeys)
{
    return new MFVec2f((float *)value, numKeys *
                       ((NodeCoordinateInterpolator2D *)
                        this)->getNumChannels());
}

void 
NodeCoordinateInterpolator2D::recordValue(int key, FieldValue *value)
{
    MFVec2f *mfkeyValue = keyValue();
 
    m_keySize = ((MFVec2f *) value)->getSize();

    for (int i = 0; i < m_keySize; i++) {
        mfkeyValue->setValue(key * m_keySize + i,
                             ((MFFloat *) value)->getValue(i));
    }
}
