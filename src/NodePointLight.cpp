/*
 * NodePointLight.cpp
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

#include "NodePointLight.h"
#include "Scene.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFFloat.h"
#include "SFVec3f.h"
#include "SFColor.h"
#include "SFBool.h"
#include "Util.h"
#include "resource.h"

enum {
    LOCATION
};

ProtoPointLight::ProtoPointLight(Scene *scene)
  : Proto(scene, "PointLight")
{
    ambientIntensity.set(
          addExposedField(SFFLOAT, "ambientIntensity", new SFFloat(0.0f),
                          new SFFloat(0.0f), new SFFloat(1.0f)));
    attenuation.set(
          addExposedField(SFVEC3F, "attenuation", new SFVec3f(1.0f, 0.0f, 0.0f),
                          new SFFloat(0.0f)));
    color.set(
          addExposedField(SFCOLOR, "color", new SFColor(1.0f, 1.0f, 1.0f)));
    global.set(
          addExposedField(SFBOOL, "global", new SFBool(false)));
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
    kambiLightCommonFields()
    x3domLightCommonFields()
}

Node *
ProtoPointLight::create(Scene *scene)
{ 
    return new NodePointLight(scene, this); 
}

NodePointLight::NodePointLight(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodePointLight::preDraw()
{
//    float radius = ((SFFloat *) getField(radius_Field()))->getValue();

    if (color() == NULL)
        return;

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
        glLightfv(m_light, GL_SPECULAR, diffuseColor);
        glLightfv(m_light, GL_POSITION, pos);
        glLightf(m_light, GL_SPOT_CUTOFF, 180.0f);
        glLightf(m_light, GL_SPOT_EXPONENT, 0.0f);
        glLightf(m_light, GL_CONSTANT_ATTENUATION, 
                 attenuation()->getValue()[0]);
        glLightf(m_light, GL_LINEAR_ATTENUATION, 
                 attenuation()->getValue()[1]);
        glLightf(m_light, GL_QUADRATIC_ATTENUATION, 
                 attenuation()->getValue()[2]);
        glEnable(m_light);
    }
}

void
NodePointLight::drawHandles()
{
    const float *flocation = location()->getValue();

    glPushMatrix();
    glTranslatef(flocation[0], flocation[1], flocation[2]);

    glPushName(LOCATION);

    GLUquadricObj *obj = gluNewQuadric();

    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color()->getValue());
    gluSphere(obj, 0.05f, 8, 4);

    glPopName();

    glPopMatrix();

    gluDeleteQuadric(obj);
}

Vec3f
NodePointLight::getHandle(int handle, int *constraint, int *field)
{
    *field = location_Field();
    return location()->getValue();
}

void
NodePointLight::setHandle(int handle, const Vec3f &v)
{
    m_scene->setField(this, location_Field(), new SFVec3f(v));
}

int     
NodePointLight::getAnimationCommentID(void) 
{ 
    return IDS_ANIMATION_HELP_POINTLIGHT + swGetLang(); 
}

int NodePointLight::getProfile(void) const
{ 
    return PROFILE_INTERCHANGE;
}

const char* 
NodePointLight::getComponentName(void) const
{
    static const char* name = "Lighting";
    return name;
}

int         
NodePointLight::getComponentLevel(void) const
{
    if (!isDefault(radius_Field()))
        return 3;
    return -1;
}


int
NodePointLight::writeRib(int f, int indent)
{
    RET_ONERROR( mywritestr(f, "LightSource \"pointlight\" 1 ") )
    const float *p = location()->getValue();    
    RET_ONERROR( mywritef(f, "\"from\" [%f %f %f] ", p[0], p[1], -p[2]) )
    RET_ONERROR( mywritef(f, "\"intensity\" [%f] ", intensity()->getValue()) )
    const float *c = color()->getValue();
    RET_ONERROR( mywritef(f, "\"lightcolor\" [%f %f %f]\n", c[0], c[1], c[2]) )

    return 0;
}

