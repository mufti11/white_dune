/*
 * NodeSpotLight.cpp
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

#include "NodeSpotLight.h"
#include "Scene.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFFloat.h"
#include "SFVec3f.h"
#include "SFColor.h"
#include "SFFloat.h"
#include "SFBool.h"


ProtoSpotLight::ProtoSpotLight(Scene *scene)
  : Proto(scene, "SpotLight")
{
    float unitAngle = (float) scene->getUnitAngle();

    ambientIntensity.set(
          addExposedField(SFFLOAT, "ambientIntensity", new SFFloat(0.0f), 
                          new SFFloat(0.0f), new SFFloat(1.0f)));
    attenuation.set(
          addExposedField(SFVEC3F, "attenuation", new SFVec3f(1.0f, 0.0f, 0.0f),
                          new SFFloat(0.0f)));
    beamWidth.set(
          addExposedField(SFFLOAT, "beamWidth", 
                          new SFFloat(M_PI/2.0f/unitAngle), new SFFloat(0.0f), 
                          new SFFloat(M_PI/2.0f/unitAngle)));
    color.set(
          addExposedField(SFCOLOR, "color", new SFColor(1.0f, 1.0f, 1.0f)));
    cutOffAngle.set(
          addExposedField(SFFLOAT, "cutOffAngle", 
                          new SFFloat(M_PI/4.0f/unitAngle), 
                          new SFFloat(0.0f), new SFFloat(M_PI/2.0f/unitAngle)));
    direction.set(
          addExposedField(SFVEC3F, "direction", 
                          new SFVec3f(0.0f, 0.0f, -1.0f)));
    global.set(
          addExposedField(SFBOOL, "global", new SFBool(true)));
    setFieldFlags(global, FF_X3D_ONLY);

    intensity.set(
          addExposedField(SFFLOAT, "intensity", new SFFloat(1.0f), 
                          new SFFloat(0.0f), new SFFloat(1.0f)));
    location.set(
          addExposedField(SFVEC3F, "location", new SFVec3f(0.0f, 0.0f, 0.0f)));
    on.set(
          addExposedField(SFBOOL, "on", new SFBool(true)));
    radius.set(
          addExposedField(SFFLOAT, "radius", new SFFloat(100.0f), 
                          new SFFloat(0.0f)));
    projectionAngle.set(
          addExposedField(SFFLOAT, "projectionAngle", new SFFloat(0.0f)));
    setFieldFlags(projectionAngle, FF_KAMBI_ONLY);

    kambiLightCommonFields()

    x3domLightCommonFields()

    shadowCascades.set(
          addExposedField(SFINT32, "shadowCascades", new SFInt32(1)));
    setFieldFlags(shadowCascades, FF_X3DOM_ONLY);

    shadowSplitFactor.set(
          addExposedField(SFFLOAT, "shadowSplitFactor", new SFFloat(1)));
    setFieldFlags(shadowSplitFactor, FF_X3DOM_ONLY);

    shadowSplitOffset.set(
          addExposedField(SFFLOAT, "shadowSplitOffset", new SFFloat(1)));
    setFieldFlags(shadowSplitOffset, FF_X3DOM_ONLY);
}

Node *
ProtoSpotLight::create(Scene *scene)
{ 
    return new NodeSpotLight(scene, this); 
}

NodeSpotLight::NodeSpotLight(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeSpotLight::preDraw()
{
//    float fradius = radius()->getValue();
    float fbeamWidth = beamWidth()->getFixedAngle(m_scene->getUnitAngle());

    if (on()->getValue()) {
        float ambientColor[4], diffuseColor[4];
        float pos[4];
        for (int i = 0; i < 3; i++) {
            ambientColor[i] = color()->getValue()[i] * 
                              ambientIntensity()->getValue();
            diffuseColor[i] = color()->getValue()[i] * 
                              intensity()->getValue();
            pos[i] = location()->getValue()[i];
        }
        ambientColor[3] = diffuseColor[3] = 1.0f;
        pos[3] = 1.0f;

        m_light = (GLenum) m_scene->allocateLight();

        glLightfv(m_light, GL_AMBIENT, ambientColor);
        glLightfv(m_light, GL_DIFFUSE, diffuseColor);
        glLightfv(m_light, GL_POSITION, pos);
        glLightfv(m_light, GL_SPECULAR, diffuseColor);
        glLightfv(m_light, GL_SPOT_DIRECTION, direction()->getValue());
        float fcutOffAngle = cutOffAngle()->getFixedAngle(
                                                m_scene->getUnitAngle());
        glLightf(m_light, GL_SPOT_CUTOFF, RAD2DEG(fcutOffAngle));
        glLightf(m_light, GL_SPOT_EXPONENT, fbeamWidth < 
                                            fcutOffAngle ? 1.0f : 0.0f);
        glLightf(m_light, GL_CONSTANT_ATTENUATION, attenuation()->getValue()[0]);
        glLightf(m_light, GL_LINEAR_ATTENUATION, attenuation()->getValue()[1]);
        glLightf(m_light, GL_QUADRATIC_ATTENUATION, attenuation()->getValue()[2]);
        glEnable(m_light);
    }
}

int NodeSpotLight::getProfile(void) const
{ 
    return PROFILE_INTERACTIVE;
}

const char* 
NodeSpotLight::getComponentName(void) const
{
    static const char* name = "Lighting";
    return name;
}

int         
NodeSpotLight::getComponentLevel(void) const
{
    if (!isDefault(radius_Field()))
        return 3;
    if (!isDefaultAngle(beamWidth_Field()))
        return 3;
    if (attenuation()->getValue()[2] != 0)
        return 3;
    return -1;
}


int
NodeSpotLight::writeRib(int f, int indent)
{
    Vec3f dir = direction()->getVec();
    Vec3f loc = location()->getVec();
    Vec3f to = loc + dir * radius()->getValue();
    Vec3f from = loc;
    RET_ONERROR( mywritestr(f, "LightSource \"spotlight\" 1 ") )
    RET_ONERROR( mywritef(f, "\"from\" [%f %f %f] ", from.x, from.y, -from.z) )
    RET_ONERROR( mywritef(f, "\"to\" [%f %f %f] ", to.x, to.y, -to.z))
    RET_ONERROR( mywritef(f, "\"intensity\" [%f] ", intensity()->getValue()) )
    RET_ONERROR( mywritef(f, "\"coneangle\" [%f] ", 
                          RAD2DEG(beamWidth()->getValue())) )
    RET_ONERROR( mywritef(f, "\"conedeltaangle\" [%f] ", 
                          RAD2DEG(cutOffAngle()->getValue())) )
    const float *c = color()->getValue();
    RET_ONERROR( mywritef(f, "\"lightcolor\" [%f %f %f]\n", c[0], c[1], c[2]) )

    return 0;
}

