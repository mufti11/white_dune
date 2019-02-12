/*
 * NodeTwoSidedMaterial.cpp
 *
 * Copyright (C) 2009 J. "MUFTI" Scheurich
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

#include "NodeTwoSidedMaterial.h"
#include "Proto.h"
#include "FieldValue.h"
#include "ExposedField.h"
#include "SFFloat.h"
#include "SFColor.h"
#include "Field.h"
#include "ExposedField.h"
#include "Util.h"
#include "Scene.h"
#include "LdrawDefines.h"
#include "resource.h"
#include "swt.h"

ProtoTwoSidedMaterial::ProtoTwoSidedMaterial(Scene *scene)
  : Proto(scene, "TwoSidedMaterial")
{
    ambientIntensity.set(
        addExposedField(SFFLOAT, "ambientIntensity", new SFFloat(0.2)));
    backAmbientIntensity.set(
        addExposedField(SFFLOAT, "backAmbientIntensity", new SFFloat(0.2)));
    backDiffuseColor.set(
        addExposedField(SFCOLOR, "backDiffuseColor", new SFColor(0.8, 0.8, 0.8)));
    backEmissiveColor.set(
        addExposedField(SFCOLOR, "backEmissiveColor", new SFColor(0, 0, 0)));
    backShininess.set(
        addExposedField(SFFLOAT, "backShininess", new SFFloat(0.2)));
    backSpecularColor.set(
        addExposedField(SFCOLOR, "backSpecularColor", new SFColor(0, 0, 0)));
    backTransparency.set(
        addExposedField(SFFLOAT, "backTransparency", new SFFloat(0)));
    diffuseColor.set(
        addExposedField(SFCOLOR, "diffuseColor", new SFColor(0.8, 0.8, 0.8)));
    emissiveColor.set(
        addExposedField(SFCOLOR, "emissiveColor", new SFColor(0, 0, 0)));
    shininess.set(
        addExposedField(SFFLOAT, "shininess", new SFFloat(0.2)));
    separateBackColor.set(
        addExposedField(SFBOOL, "separateBackColor", new SFBool(false)));
    specularColor.set(
        addExposedField(SFCOLOR, "specularColor", new SFColor(0, 0, 0)));
    transparency.set(
        addExposedField(SFFLOAT, "transparency", new SFFloat(0)));
}

Node *
ProtoTwoSidedMaterial::create(Scene *scene)
{ 
    return new NodeTwoSidedMaterial(scene, this); 
}

NodeTwoSidedMaterial::NodeTwoSidedMaterial(Scene *scene, Proto *def)
  : Node(scene, def)
{
    m_ac3dMaterialIndex = -1;
    m_isFirstLdrawColor = false;
    m_verifiedFrontColor = NULL;
    m_verifiedBackColor = NULL;
}


void NodeTwoSidedMaterial::bind()
{
    float ambientColor[4], dc[4], ec[4]; 

    for (int i = 0; i < 3; i++) {
        ec[i] = emissiveColor()->getValue()[i];
        dc[i] = diffuseColor()->getValue()[i];
        ambientColor[i] = diffuseColor()->getValue()[i] * 
                          ambientIntensity()->getValue();
    }
    ambientColor[3] = 1.0f;
    ec[3] = dc[3] = 1.0f - transparency()->getValue();

    int side = GL_FRONT_AND_BACK;
    if (separateBackColor()->getValue())
        side = GL_FRONT;
    Util::myGlMaterialfv(side, GL_AMBIENT, ambientColor);
    Util::myGlMaterialfv(side, GL_DIFFUSE, dc);
    Util::myGlMaterialfv(side, GL_EMISSION, ec);
    glMaterialf(side, GL_SHININESS, 128.0f * shininess()->getValue());
    Util::myGlMaterialfv(side, GL_SPECULAR, specularColor()->getValue());

    if (separateBackColor()->getValue()) {
        float backAmbientColor[4], bdc[4], bec[4];
        for (int i = 0; i < 3; i++) {
            bec[i] = backEmissiveColor()->getValue()[i];
            bdc[i] = backDiffuseColor()->getValue()[i];
            backAmbientColor[i] = backDiffuseColor()->getValue()[i] * 
                                  backAmbientIntensity()->getValue();
        }

        backAmbientColor[3] = 1.0f;
        bec[3] = bdc[3] = 1.0f - backTransparency()->getValue();

        Util::myGlMaterialfv(GL_BACK, GL_AMBIENT, backAmbientColor);
        Util::myGlMaterialfv(GL_BACK, GL_DIFFUSE, bdc);
        Util::myGlMaterialfv(GL_BACK, GL_EMISSION, bec);
        glMaterialf(GL_BACK, GL_SHININESS, 128.0f * shininess()->getValue());
        Util::myGlMaterialfv(GL_BACK, GL_SPECULAR, specularColor()->getValue());
    }

    if ((transparency()->getValue() > 0.0f) || 
         ((separateBackColor()->getValue()) &&
          (backTransparency()->getValue() > 0.0f))) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
    }
}

void NodeTwoSidedMaterial::unbind()
{
    float ambientColor[4];

    for (int i = 0; i < 3; i++) {
        ambientColor[i] = diffuseColor()->getValue()[i] * 
                          ambientIntensity()->getValue();
    }
    ambientColor[3] = 1.0f;

    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientColor);
    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseColor()->getValue());
    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissiveColor()->getValue());
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess()->getValue());
    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor()->getValue());

    glDisable(GL_BLEND);
}

void
NodeTwoSidedMaterial::emissive2diffuse(void)
{
    m_scene->addNextCommand();
    SFColor *color = new SFColor(emissiveColor()->getValue(0),
                                 emissiveColor()->getValue(1),
                                 emissiveColor()->getValue(2));
    m_scene->backupField(this, diffuseColor_Field()); 
    m_scene->setField(this, diffuseColor_Field(), color);
    color = new SFColor(0, 0, 0);
    m_scene->backupField(this, emissiveColor_Field()); 
    m_scene->setField(this, emissiveColor_Field(), color);
}

void
NodeTwoSidedMaterial::diffuse2emissive(void)
{
    m_scene->addNextCommand();
    SFColor *color = new SFColor(diffuseColor()->getValue(0),
                                 diffuseColor()->getValue(1),
                                 diffuseColor()->getValue(2));
    m_scene->backupField(this, emissiveColor_Field()); 
    m_scene->setField(this, emissiveColor_Field(), color);
    color = new SFColor(0, 0, 0);
    m_scene->backupField(this, diffuseColor_Field()); 
    m_scene->setField(this, diffuseColor_Field(), color);
}

bool    
NodeTwoSidedMaterial::isTransparent(void)
{
    if (transparency()->getValue() > 0.0)
        return true;
    else
        return false;
}

int 
NodeTwoSidedMaterial::writeAc3dMaterial(int f, int indent, const char *materialName)
{
    RET_ONERROR( mywritestr(f, "MATERIAL \"") )
    RET_ONERROR( mywritestr(f, materialName) )
    RET_ONERROR( mywritestr(f, "\" ") ) 
    RET_ONERROR( mywritef(f, "rgb %f %f %f  ", 
                          diffuseColor()->getValue(0),
                          diffuseColor()->getValue(1),
                          diffuseColor()->getValue(2)) )
    RET_ONERROR( mywritef(f, "amb %f %f %f  ",
                          ambientIntensity()->getValue(),
                          ambientIntensity()->getValue(),
                          ambientIntensity()->getValue()) )
    RET_ONERROR( mywritef(f, "emis %f %f %f  ", 
                          emissiveColor()->getValue(0),
                          emissiveColor()->getValue(1),
                          emissiveColor()->getValue(2)) )
    RET_ONERROR( mywritef(f, "spec %f %f %f  ", 
                          specularColor()->getValue(0),
                          specularColor()->getValue(1),
                          specularColor()->getValue(2)) )
    RET_ONERROR( mywritef(f, "shi %d  ", ((int)(shininess()->getValue() * 128))))
    RET_ONERROR( mywritef(f, "trans %f\n", transparency()->getValue()) )
    return 0;
}

int
NodeTwoSidedMaterial::getAnimationCommentID(void) 
{ 
    return IDS_ANIMATION_HELP_MATERIAL + swGetLang(); 
}

/*
    In VRML97/X3D the name of a material is simply the DEF name of the 
    Material node, so the following function is not needed when dealing
    with VRML data.
    Instead, this function is used when exporting to non VRML/X3D files.
    Some export file formats are used for sound simulation, where 
    the material name is used for applying sound related parameters to
    geometry. 
    In sound simulation it is important to have a easy way to apply
    sound parameters to all surfaces. Therefore the following function
    search for other DEF names in higher levels of the scenegraph, if
    the current node do not have a DEF name.
    Cause some 3D tools write hard to use DEF names, a preference can be
    used to select, if the part before the first _ or the part after the
    last _ or the full DEF name is used for material name generation
 */

void    
NodeTwoSidedMaterial::handleAc3dMaterial(ac3dMaterialCallback callback, Scene* scene)
{
    callback(scene, getExportMaterialName(TheApp->GetDefaultAc3dMaterialName()),
             this);
}

int
NodeTwoSidedMaterial::writeLdrawDat(int filedes, int indent)
{
    if (m_isFirstLdrawColor) {
        m_scene->setCurrentLdrawColor(LDRAW_CURRENT_COLOR);
        return 0;
    }
    int currentColor = m_scene->getCurrentLdrawColor();
    if (currentColor == -1) {
        m_scene->setCurrentLdrawColor(LDRAW_CURRENT_COLOR);
        m_isFirstLdrawColor = true;
    } else {
        float r = diffuseColor()->getValue(0);
        float g = diffuseColor()->getValue(1);
        float b = diffuseColor()->getValue(2);
        float a = 1.0f - transparency()->getValue();
        m_scene->setCurrentLdrawColor(Util::getLdrawColorFromRGBA(r, g, b, a, 
                                                                 true));
    }
    return 0;
}

void    
NodeTwoSidedMaterial::reInit() 
{
    m_isFirstLdrawColor = false; 
    Node::reInit();
}

