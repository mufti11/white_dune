/*
 * NodeKambiHeadLight.cpp
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

#include "NodeKambiHeadLight.h"
#include "Scene.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFFloat.h"
#include "SFVec3f.h"
#include "SFColor.h"
#include "SFFloat.h"
#include "SFBool.h"


ProtoKambiHeadLight::ProtoKambiHeadLight(Scene *scene)
  : Proto(scene, "KambiHeadLight")
{
    ambientIntensity.set(
          addExposedField(SFFLOAT, "ambientIntensity", new SFFloat(0.0f), 
                          new SFFloat(0.0f), new SFFloat(1.0f)));
    attenuation.set(
          addExposedField(SFVEC3F, "attenuation", new SFVec3f(1.0f, 0.0f, 0.0f),
                          new SFFloat(0.0f)));
    color.set(
          addExposedField(SFCOLOR, "color", new SFColor(1.0f, 1.0f, 1.0f)));
    intensity.set(
          addExposedField(SFFLOAT, "intensity", new SFFloat(1.0f), 
                          new SFFloat(0.0f), new SFFloat(1.0f)));
    spot.set(
          addExposedField(SFBOOL, "spot", new SFBool(false)));
    spotDropOffRate.set(
          addExposedField(SFFLOAT, "spotDropOffRate", new SFFloat(0.785398f), 
                          new SFFloat(0.0f), new SFFloat(M_PI/2.0f)));
    spotCutOffAngle.set(
          addExposedField(SFFLOAT, "spotCutOffAngle", new SFFloat(0.785398f), 
                          new SFFloat(0.0f), new SFFloat(M_PI/2.0f)));

    addURLs(URL_KAMBI);
}

Node *
ProtoKambiHeadLight::create(Scene *scene)
{ 
    return new NodeKambiHeadLight(scene, this); 
}

NodeKambiHeadLight::NodeKambiHeadLight(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeKambiHeadLight::preDraw()
{
/*
//    float fbeamWidth = beamWidth()->getValue();
//    float fradius = radius()->getValue();

    if (true) {
        float ambientColor[4], diffuseColor[4];
        float pos[4];
        for (int i = 0; i < 3; i++) {
            ambientColor[i] = color()->getValue()[i] * 
                              ambientIntensity()->getValue();
            diffuseColor[i] = color()->getValue()[i] * 
                              intensity()->getValue();
            pos[i] = m_scene->getCamera()->position()->getValue()[i];
        }
        ambientColor[3] = diffuseColor[3] = 1.0f;
        pos[3] = 1.0f;

        m_light = (GLenum) m_scene->allocateLight();

        glLightfv(m_light, GL_AMBIENT, ambientColor);
        glLightfv(m_light, GL_DIFFUSE, diffuseColor);
        glLightfv(m_light, GL_POSITION, pos);
        glLightfv(m_light, GL_SPECULAR, diffuseColor);
        glLightfv(m_light, GL_KambiHead_DIRECTION, direction()->getValue());
        glLightf(m_light, GL_KambiHead_CUTOFF, RAD2DEG(cutOffAngle()->getValue()));
        glLightf(m_light, GL_KambiHead_EXPONENT, 0.0f);
        glLightf(m_light, GL_CONSTANT_ATTENUATION, attenuation()->getValue()[0]);
        glLightf(m_light, GL_LINEAR_ATTENUATION, attenuation()->getValue()[1]);
        glLightf(m_light, GL_QUADRATIC_ATTENUATION, attenuation()->getValue()[2]);
        glEnable(m_light);
    }
*/
}

