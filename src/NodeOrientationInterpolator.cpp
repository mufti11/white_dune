/*
 * NodeOrientationInterpolator.cpp
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

#include "NodeOrientationInterpolator.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFFloat.h"
#include "MFRotation.h"
#include "SFRotation.h"
#include "Scene.h"

ProtoOrientationInterpolator::ProtoOrientationInterpolator(Scene *scene)
  : ProtoInterpolator(scene, "OrientationInterpolator", SFROTATION, MFROTATION,
                      new MFRotation())
{
}

Node *
ProtoOrientationInterpolator::create(Scene *scene)
{ 
    return new NodeOrientationInterpolator(scene, this); 
}

NodeOrientationInterpolator::NodeOrientationInterpolator(Scene *scene, Proto *def)
  : Interpolator(scene, def)
{
}

int
NodeOrientationInterpolator::getNumChannels()
{
    return 4;
}

FieldValue *
NodeOrientationInterpolator::createKey(void *value)
{
    FieldValue *r = new SFRotation((float *)value);
    return r;
}

FieldValue *
NodeOrientationInterpolator::createKeys(void *value, int numKeys)
{
    return new MFRotation((float *)value, numKeys * 4);
}

void
NodeOrientationInterpolator::interpolate(float k, float *values)
{
    if (keyValue()->getSize() == 0) {
        values[0] = values[2] = values[3] = 0.0f;
        values[1] = 1.0f;
        return;
    }

    float k1 = 0.0;
    float k2 = 1.0;
    int pos1 = 0;
    int pos2 = 0;

    getNearestKeys(k, &k1, &k2, &pos1, &pos2);

    float alpha;

    if ((k1 == k2) || (pos1==pos2)) {
        alpha = 0.0f;
    } else {
        alpha = (k - k1) / (k2 - k1);
    }

    SFRotation r1(keyValue()->getValue(pos1));
    SFRotation r2(keyValue()->getValue(pos2));

    r1.fixAngle(m_scene->getUnitAngle());
    r1.generateQuaternion();

    r2.fixAngle(m_scene->getUnitAngle());
    r2.generateQuaternion();

    Quaternion q1(r1.getQuat()), q2(r2.getQuat()), q;

    float beta;         /* complementary interp parameter */
    float theta;        /* angle between A and B */
    float sin_t, cos_t; /* sine, cosine of theta */
    bool bflip;         /* use negation of B? */

    /* cosine theta = dot product of q1 and q2 */
    cos_t = q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;

    /* if B is on opposite hemisphere from A, use -B instead */
    if (cos_t < 0.0) {
        cos_t = -cos_t;
        bflip = true;
    } else
        bflip = false;

    /* if q2 is (within precision limits) the same as q1,
     * just linear interpolate between q1 and q2.
     */
    if (1.0 - cos_t < FEPSILON) {
        beta = 1.0f - alpha;
    } else {  /* normal case */
        theta = (float) acos(cos_t);
        sin_t = (float) sin(theta);
        beta = (float) sin(theta - alpha*theta) / sin_t;
        alpha = (float) sin(alpha*theta) / sin_t;
    }

    if (bflip)
        alpha = -alpha;

    q.x = q1.x * beta + q2.x * alpha;
    q.y = q1.y * beta + q2.y * alpha;
    q.z = q1.z * beta + q2.z * alpha;
    q.w = q1.w * beta + q2.w * alpha;

    float rlen = (float) sqrt(q.x * q.x + q.y * q.y + q.z * q.z);
    if (rlen > 0.0f) rlen = 1.0f / rlen; else rlen = 1.0f;
    values[0] = q.x * rlen;
    values[1] = q.y * rlen;
    values[2] = q.z * rlen;
    values[3] = 2.0f * (float) acos(q.w);

    double angleUnit = m_scene->getUnitAngle();    
    if (angleUnit != 0)
        values[3] /= angleUnit;

    if ((values[0] == 0.0f) &&
        (values[1] == 0.0f) &&
        (values[2] == 0.0f) &&
        (values[3] == 0.0f)) {
        values[2] = 1;
    }
}
