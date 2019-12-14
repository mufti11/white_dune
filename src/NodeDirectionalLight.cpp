/*
 * NodeDirectionalLight.cpp
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

#include "NodeDirectionalLight.h"
#include "Scene.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFFloat.h"
#include "SFVec3f.h"
#include "SFBool.h"
#include "SFColor.h"
#include "Node.h"
#include "ExposedField.h"
#include "resource.h"


ProtoDirectionalLight::ProtoDirectionalLight(Scene *scene)
  : Proto(scene, "DirectionalLight")
{
    ambientIntensity.set(
          addExposedField(SFFLOAT, "ambientIntensity", new SFFloat(0.0f),
                          new SFFloat(0.0f), new SFFloat(1.0f)));

    color.set(
          addExposedField(SFCOLOR, "color", new SFColor(1.0f, 1.0f, 1.0f)));

    direction.set(    
          addExposedField(SFVEC3F, "direction", 
                          new SFVec3f(0.0f, 0.0f, -1.0f)));

    global.set(
          addExposedField(SFBOOL, "global", new SFBool(false)));
    setFieldFlags(global, FF_X3D_ONLY);

    intensity.set(
          addExposedField(SFFLOAT, "intensity", new SFFloat(1.0f), 
                          new SFFloat(0.0f), new SFFloat(1.0f)));

    on.set(
          addExposedField(SFBOOL, "on", new SFBool(true)));

    projectionRectangle.set(
        addExposedField(SFVEC4F, "projectionRectangle", new SFVec4f(-10, 10, 
                                                                    -10, 10)));
    setFieldFlags(projectionRectangle, FF_KAMBI_ONLY);
    projectionLocation.set(
        addExposedField(SFVEC3F, "projectionLocation", new SFVec3f(0, 0, 0)));
    setFieldFlags(projectionLocation, FF_KAMBI_ONLY);

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
ProtoDirectionalLight::create(Scene *scene)
{ 
    return new NodeDirectionalLight(scene, this); 
}

NodeDirectionalLight::NodeDirectionalLight(Scene *scene, Proto *def)
  : Node(scene, def)
{
}


void
NodeDirectionalLight::bind()
{
    const float *fcolor = color()->getValue();
    const float *fdirection = direction()->getValue();

    float ambientColor[4], diffuseColor[4];
    float pos[4];
    for (int i = 0; i < 3; i++) {
        ambientColor[i] = fcolor[i] * ambientIntensity()->getValue();
        diffuseColor[i] = fcolor[i] * intensity()->getValue();
        pos[i] = -fdirection[i];
    }
    ambientColor[3] = diffuseColor[3] = 1.0f;
    pos[3] = 0.0f;

    if (on()->getValue()) {
        m_light = (GLenum) m_scene->allocateLight();

        glLightfv(m_light, GL_AMBIENT, ambientColor);
        glLightfv(m_light, GL_DIFFUSE, diffuseColor);
        glLightfv(m_light, GL_POSITION, pos);
        glLightfv(m_light, GL_SPECULAR, diffuseColor);
        glLightf(m_light, GL_SPOT_CUTOFF, 180.0f);
        glLightf(m_light, GL_SPOT_EXPONENT, 0.0f);
        glLightf(m_light, GL_CONSTANT_ATTENUATION, 1.0f);
        glLightf(m_light, GL_LINEAR_ATTENUATION, 0.0f);
        glLightf(m_light, GL_QUADRATIC_ATTENUATION, 0.0f);
        glEnable(m_light);
    }
}

void NodeDirectionalLight::unbind()
{
    if (on()->getValue()) {
        glDisable(m_light);
        m_scene->freeLight();
    }
}

int
NodeDirectionalLight::getAnimationCommentID(void) 
{  
    return IDS_ANIMATION_HELP_DIRECTIONALLIGHT + swGetLang(); 
}

const char* 
NodeDirectionalLight::getComponentName(void) const
{
    static const char* name = "Lighting";
    return name;
}

int         
NodeDirectionalLight::getComponentLevel(void) const
{
    return -1;
}

int
NodeDirectionalLight::writeRib(int f, int indent)
{
    const float *c = color()->getValue();

    float amb = ambientIntensity()->getValue();
    if (amb > 0) {
        RET_ONERROR( mywritestr(f, "LightSource \"ambientlight\" 1 ") )
        RET_ONERROR( mywritef(f, "\"intensity\" [%f] ", amb) )
        RET_ONERROR( mywritef(f, "\"lightcolor\" [%f %f %f]\n", 
                              c[0], c[1], c[2]) )
    }
    Vec3f dir = direction()->getVec();
    Vec3f farvec(TheApp->GetFarClippingPlaneDist(),
              TheApp->GetFarClippingPlaneDist(),
              TheApp->GetFarClippingPlaneDist());
    Vec3f to = -dir * farvec;
    Vec3f from = dir * farvec;
    RET_ONERROR( mywritestr(f, "LightSource \"distantlight\" 1 ") )
    RET_ONERROR( mywritef(f, "\"from\" [%f %f %f] ", from.x, from.y, from.z) )
    RET_ONERROR( mywritef(f, "\"to\" [%f %f %f] ", to.x, to.y, to.z))
    RET_ONERROR( mywritef(f, "\"intensity\" [%f] ", intensity()->getValue()) )
    RET_ONERROR( mywritef(f, "\"lightcolor\" [%f %f %f]\n", c[0], c[1], c[2]) )

    return 0;
}
