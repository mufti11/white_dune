/*
 * NodeMaterial.cpp
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

#include "NodeMaterial.h"
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

ProtoMaterial::ProtoMaterial(Scene *scene)
  : WonderlandExportProto(scene, "Material")
{
    ambientIntensity.set(
          addExposedField(SFFLOAT, "ambientIntensity", new SFFloat(0.2f), 
                          new SFFloat(0.0f), new SFFloat(1.0f)));
    diffuseColor.set(
          addExposedField(SFCOLOR, "diffuseColor", 
                          new SFColor(0.8f, 0.8f, 0.8f)));
    setFieldFlags(diffuseColor, EIF_RECOMMENDED);
    emissiveColor.set(
          addExposedField(SFCOLOR, "emissiveColor", 
                          new SFColor(0.0f, 0.0f, 0.0f)));
    shininess.set(
          addExposedField(SFFLOAT, "shininess", new SFFloat(0.2f),
                          new SFFloat(0.0f), new SFFloat(1.0f)));
    specularColor.set(

          addExposedField(SFCOLOR, "specularColor", 
                          new SFColor(0.0f, 0.0f, 0.0f)));
    transparency.set(
          addExposedField(SFFLOAT, "transparency", new SFFloat(0.0f), 
                          new SFFloat(0.0f), new SFFloat(1.0f)));
    setFieldFlags(transparency, EIF_RECOMMENDED);

    mirror.set(
          addExposedField(SFFLOAT, "mirror", new SFFloat(0.0f), 
                          new SFFloat(0.0f), new SFFloat(1.0f)));
    setFieldFlags(mirror, FF_KAMBI_ONLY);
    reflSpecular.set(
        addExposedField(MFCOLOR, "reflSpecular", new MFColor()));
    setFieldFlags(reflSpecular, FF_KAMBI_ONLY);
    reflDiffuse.set(
        addExposedField(MFCOLOR, "reflDiffuse", new MFColor()));
    setFieldFlags(reflDiffuse, FF_KAMBI_ONLY);
    transSpecular.set(
        addExposedField(MFCOLOR, "transSpecular", new MFColor()));
    setFieldFlags(transSpecular, FF_KAMBI_ONLY);
    transDiffuse.set(
        addExposedField(MFCOLOR, "transDiffuse", new MFColor()));
    setFieldFlags(transDiffuse, FF_KAMBI_ONLY);
    reflSpecularExp.set(
        addExposedField(SFFLOAT, "reflSpecularExp", new SFFloat(1000000)));
    setFieldFlags(reflSpecularExp, FF_KAMBI_ONLY);
    transSpecularExp.set(
        addExposedField(SFFLOAT, "transSpecularExp", new SFFloat(1000000)));
    setFieldFlags(transSpecularExp, FF_KAMBI_ONLY);
}

Node *
ProtoMaterial::create(Scene *scene)
{ 
    return new NodeMaterial(scene, this);
}

NodeMaterial::NodeMaterial(Scene *scene, Proto *def)
  : Node(scene, def)
{
    m_ac3dMaterialIndex = -1;
    m_isFirstLdrawColor = false;
}


void NodeMaterial::bind()
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

    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientColor);
    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dc);
    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, ec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0f * 
                                                 shininess()->getValue());
    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor()->getValue());

    if (transparency()->getValue() > 0.0f) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
    }
}

void NodeMaterial::unbind()
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
NodeMaterial::emissive2diffuse(void)
{
    m_scene->addNextCommand();
    SFColor *color = new SFColor(emissiveColor()->getValue(0),
                                 emissiveColor()->getValue(1),
                                 emissiveColor()->getValue(2));
    m_scene->backupField(this, diffuseColor_Field()); 
    m_scene->setField(this, diffuseColor_Field(), color);
    SFColor *ecolor = new SFColor(0, 0, 0);
    m_scene->backupField(this, emissiveColor_Field()); 
    m_scene->setField(this, emissiveColor_Field(), ecolor);
}

void
NodeMaterial::diffuse2emissive(void)
{
    m_scene->addNextCommand();
    SFColor *color = new SFColor(diffuseColor()->getValue(0),
                                 diffuseColor()->getValue(1),
                                 diffuseColor()->getValue(2));
    m_scene->backupField(this, emissiveColor_Field()); 
    m_scene->setField(this, emissiveColor_Field(), color);
    SFColor *colord = new SFColor(0, 0, 0);
    m_scene->backupField(this, diffuseColor_Field()); 
    m_scene->setField(this, diffuseColor_Field(), colord);
}

bool    
NodeMaterial::isTransparent(void)
{
    if (transparency()->getValue() > 0.0)
        return true;
    else
        return false;
}

int 
NodeMaterial::getProfile(void) const
{ 
    if (!isDefault(ambientIntensity_Field()))
        return PROFILE_IMMERSIVE;
    if (!isDefault(shininess_Field()))
        return PROFILE_IMMERSIVE;
    if (!isDefault(specularColor_Field()))
        return PROFILE_IMMERSIVE;
    if (!isDefault(emissiveColor_Field()))
        return PROFILE_IMMERSIVE;
    return PROFILE_INTERCHANGE;
}

int 
NodeMaterial::writeAc3dMaterial(int f, int indent, const char *materialName)
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
NodeMaterial::getAnimationCommentID(void) 
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
NodeMaterial::handleAc3dMaterial(ac3dMaterialCallback callback, Scene* scene)
{
    callback(scene, getExportMaterialName(TheApp->GetDefaultAc3dMaterialName()),
             this);
}

int
NodeMaterial::writeLdrawDat(int filedes, int indent)
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
NodeMaterial::reInit()
{ 
    m_isFirstLdrawColor = false; 
    Node::reInit();
}

