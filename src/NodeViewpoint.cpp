/*
 * NodeViewpoint.cpp
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

#include "DuneApp.h"
#include "NodeViewpoint.h"
#include "Scene.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFFloat.h"
#include "SFRotation.h"
#include "SFString.h"
#include "SFTime.h"
#include "SFVec3f.h"
#include "Field.h"
#include "ExposedField.h"
#include "resource.h"
#include "Util.h"

ProtoViewpoint::ProtoViewpoint(Scene *scene)
  : ViewpointProto(scene, "Viewpoint")
{
    if (TheApp->getCoverMode()) {
        // non standard Covise/COVER extensions
        addEventIn(SFBOOL, "set_bindLast", FF_COVER_ONLY);
    }

    position.set(
         addExposedField(SFVEC3F, "position", new SFVec3f(0, 0, 10)));
    setFieldFlags(position, EIF_RECOMMENDED);

    if (TheApp->getCoverMode()) {
        // non standard Covise/COVER extensions
        type.set(
           addExposedField(SFSTRING, "type", new SFString("")));
        setFieldFlags(type, FF_COVER_ONLY);
    }
    if (TheApp->getKambiMode()) {
        // non standard Kambi game engine extensions
        direction.set(
           addExposedField(MFVEC3F, "direction", new MFVec3f()));
        setFieldFlags(direction, FF_KAMBI_ONLY);
        up.set(
           addExposedField(MFVEC3F, "up", new MFVec3f()));
        setFieldFlags(up, FF_KAMBI_ONLY);
        gravityUp.set(
           addExposedField(SFVEC3F, "gravityUp", new SFVec3f(0, 1, 0)));
        setFieldFlags(gravityUp, FF_KAMBI_ONLY);

        kambiViewpointCommonFields()
    }
}

Node *
ProtoViewpoint::create(Scene *scene)
{ 
    return new NodeViewpoint(scene, this); 
}

NodeViewpoint::NodeViewpoint(Scene *scene, Proto *def)
  : ViewpointNode(scene, def)
{
    Matrix matrix = Matrix::identity();
    for (int i = 0; i < 16; i++)
        m_matrix[i] = matrix[i];
    m_matrix[14] = -10.0f;
}

Vec3d
NodeViewpoint::getPosition() const
{
    if (position() == NULL) {
        SFVec3f *value = (SFVec3f *)m_fields[position_Field()];
printf("%d\n", position_Field());
        const float *v = value->getValue();
        return Vec3d(v[0], v[1], v[2]);
                
    }
    Vec3f vec(position()->getValue());
    return Vec3d(vec.x, vec.y, vec.z);
}

void NodeViewpoint::setPosition(const Vec3d &pos)
{
    m_scene->setField(this, position_Field(), new SFVec3f(pos.x, pos.y, pos.z));
}

void NodeViewpoint::flip(int index)
{
    position()->flip(index);
    orientation()->flip(index);
}

void NodeViewpoint::swap(int fromTo)
{
    position()->swap(fromTo);
    orientation()->swap(fromTo);
    update();
}


