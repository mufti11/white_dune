/*
 * NodeAppearance.cpp
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

#include "NodeAppearance.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFFloat.h"
#include "SFNode.h"
#include "Node.h"
#include "NodeMaterial.h"
#include "NodeTwoSidedMaterial.h"
#include "Util.h"

ProtoAppearance::ProtoAppearance(Scene *scene)
  : WonderlandExportProto(scene, "Appearance")
{
    addElements();
}

ProtoAppearance::ProtoAppearance(Scene *scene, const char* name)
  : WonderlandExportProto(scene, name)
{
    addElements();
}

void 
ProtoAppearance::addElements(void)
{
    material.set(
          addExposedField(SFNODE, "material", new SFNode(NULL), MATERIAL_NODE));

    texture.set(
          addExposedField(SFNODE, "texture", new SFNode(NULL), TEXTURE_NODE));

    textureTransform.set(
          addExposedField(SFNODE, "textureTransform", new SFNode(NULL), 
                          TEXTURE_TRANSFORM_NODE));

    fillProperties.set(
          addExposedField(SFNODE, "fillProperties", new SFNode(NULL), 
                          X3D_FILL_PROPERTIES));
    setFieldFlags(fillProperties, FF_X3D_ONLY);

    lineProperties.set(
          addExposedField(SFNODE, "lineProperties", new SFNode(NULL), 
                          X3D_LINE_PROPERTIES));
    setFieldFlags(lineProperties, FF_X3D_ONLY);    

    pointProperties.set(
          addExposedField(SFNODE, "pointProperties", new SFNode(NULL), 
                          X3D_POINT_PROPERTIES));
    setFieldFlags(pointProperties, FF_X3D_ONLY);    

    shaders.set(
          addExposedField(MFNODE, "shaders", new MFNode(), SHADER_NODE));
    setFieldFlags(shaders, FF_X3D_ONLY);    

    receiveShadows.set(
          addField(MFNODE, "receiveShadows", new MFNode(), LIGHT_NODE));
    setFieldFlags(receiveShadows, FF_KAMBI_ONLY);    

    shadowCaster.set(
          addField(SFBOOL, "shadowCaster", new SFBool(TRUE)));
    setFieldFlags(shadowCaster, FF_KAMBI_ONLY);    

    normalMap.set(
       addExposedField(SFNODE, "normalMap", new SFNode(NULL), TEXTURE_NODE));
    setFieldFlags(normalMap, FF_KAMBI_ONLY);

    heightMap.set(
       addExposedField(SFNODE, "heightMap", new SFNode(NULL), TEXTURE_NODE));
    setFieldFlags(heightMap, FF_KAMBI_ONLY);

    heightMapScale.set(
       addExposedField(SFFLOAT, "heightMapScale", new SFFloat(0.01f), 
                       new SFFloat(0.0f)));
    setFieldFlags(heightMapScale, FF_KAMBI_ONLY);

    blendMode.set(
        addExposedField(SFNODE, "blendMode", new SFNode(), KAMBI_BLEND_MODE));
    setFieldFlags(blendMode, FF_KAMBI_ONLY | FF_X3DOM_ONLY);

    effects.set(
        addField(MFNODE, "effects", new MFNode(), KAMBI_EFFECT));
    setFieldFlags(effects, FF_KAMBI_ONLY);

    if (TheApp->getCoverMode()) {
        // non standard Covise/COVER extensions
        texture2.set(
          addExposedField(SFNODE, "texture2", new SFNode(NULL), TEXTURE_NODE));
        setFieldFlags(texture2, FF_COVER_ONLY);

        textureTransform2.set(
          addExposedField(SFNODE, "textureTransform2", new SFNode(NULL), 
                          TEXTURE_TRANSFORM_NODE));
        setFieldFlags(textureTransform2, FF_COVER_ONLY);

        texture3.set(
          addExposedField(SFNODE, "texture3", new SFNode(NULL), TEXTURE_NODE));
        setFieldFlags(texture3, FF_COVER_ONLY);

        textureTransform3.set(
          addExposedField(SFNODE, "textureTransform3", new SFNode(NULL), 
                          TEXTURE_TRANSFORM_NODE));
        setFieldFlags(textureTransform3, FF_COVER_ONLY);

        texture4.set(
          addExposedField(SFNODE, "texture4", new SFNode(NULL), TEXTURE_NODE));
        setFieldFlags(texture4, FF_COVER_ONLY);

        textureTransform4.set(
          addExposedField(SFNODE, "textureTransform4", new SFNode(NULL), 
                          TEXTURE_TRANSFORM_NODE));
        setFieldFlags(textureTransform4, FF_COVER_ONLY);
    }

    alphaClipThreshold.set(
        addExposedField(SFFLOAT, "alphaClipThreshold", new SFFloat(1)));
    setFieldFlags(alphaClipThreshold, FF_X3DOM_ONLY);

    colorMaskMode.set(
       addExposedField(SFNODE, "colorMaskMode", new SFNode(NULL), 
                       X3DOM_COLOR_MASK_MODE));
    setFieldFlags(colorMaskMode, FF_X3DOM_ONLY);

    depthMode.set(
       addExposedField(SFNODE, "depthMode", new SFNode(NULL), 
                       X3DOM_DEPTH_MODE));
    setFieldFlags(depthMode, FF_X3DOM_ONLY);    

    sortKey.set(
       addExposedField(SFINT32, "sortKey", new SFInt32(0)));
    setFieldFlags(sortKey, FF_X3DOM_ONLY);    

    sortType.set(
       addExposedField(SFSTRING, "sortType", new SFString("auto")));
    setFieldFlags(sortType, FF_X3DOM_ONLY);    
}

Node *
ProtoAppearance::create(Scene *scene)
{ 
    return new NodeAppearance(scene, this); 
}

NodeAppearance::NodeAppearance(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void NodeAppearance::bind()
{
    if (material() == NULL)
        return;
    if (texture() == NULL)
        return;
    if (textureTransform() == NULL)
        return;

    Node    *nMaterial = ((SFNode *) getField(material_Field()))->getValue();
    Node    *nTexture = ((SFNode *) getField(texture_Field()))->getValue();
    Node    *nTextureTransform = ((SFNode *) getField(textureTransform_Field()))
                                                      ->getValue();
    float    ftransparency = 0.0f;
    float    fBackTransparency = 0.0f;

    while (nMaterial && (nMaterial->getType() == VRML_MATERIAL) && 
           nMaterial->isPROTO())
        nMaterial = ((NodePROTO *)nMaterial)->getProtoNode(0);
    if (nMaterial && (nMaterial->getType() == VRML_MATERIAL) &&
        ((NodeMaterial *)nMaterial)->transparency()) {
        nMaterial->bind();
        ftransparency = ((NodeMaterial *) 
                         nMaterial)->transparency()->getValue();
        fBackTransparency = ftransparency;
    }
    if (nMaterial && (nMaterial->getType() == X3D_TWO_SIDED_MATERIAL)) {
        nMaterial->bind();
        ftransparency = ((NodeTwoSidedMaterial *) 
                         nMaterial)->transparency()->getValue();
        if (((NodeTwoSidedMaterial *)
             nMaterial)->separateBackColor()->getValue())
            fBackTransparency = ((NodeTwoSidedMaterial *) 
                                 nMaterial)->backTransparency()->getValue();
        else
            fBackTransparency = ftransparency;
    }
    if (nTexture) {
        nTexture->bind();
        if ((nTexture->textureGlColorMode() == GL_RGBA) ||
            (nTexture->textureGlColorMode() == GL_LUMINANCE_ALPHA)) {
            ftransparency = 0;
            fBackTransparency = 0;
        }
        float dc[4];
        dc[0] = dc[1] = dc[2] = 1.0f;  dc[3] = 1.0f - ftransparency;
        Util::myGlMaterialfv(GL_FRONT, GL_DIFFUSE, dc);
        float bdc[4];
        bdc[0] = bdc[1] = bdc[2] = 1.0f;  bdc[3] = 1.0f - fBackTransparency;
        Util::myGlMaterialfv(GL_BACK, GL_DIFFUSE, bdc);
    }
    if (nTextureTransform) nTextureTransform->bind();
}

void NodeAppearance::unbind()
{
    if (material() == NULL)
        return;
    if (texture() == NULL)
        return;
    if (textureTransform() == NULL)
        return;

    Node *nMaterial = ((SFNode *) getField(material_Field()))->getValue();
    Node *nTexture = ((SFNode *) getField(texture_Field()))->getValue();
    Node *nTextureTransform = ((SFNode *) getField(textureTransform_Field()))
                                                   ->getValue();

    if (nMaterial) nMaterial->unbind();
    if (nTexture) nTexture->unbind();
    if (nTextureTransform) nTextureTransform->unbind();
}

bool NodeAppearance::isTransparent(void)
{
    if (getField(material_Field())) {
        Node *nMaterial = ((SFNode *) getField(material_Field()))->getValue();
        if (nMaterial) 
            if (nMaterial->isTransparent())
                return true;
     }

    if (getField(texture_Field())) {
        Node *nTexture = ((SFNode *) getField(texture_Field()))->getValue();
        if (nTexture) 
            return nTexture->isTransparent();
    }
    return false; 
}

float NodeAppearance::getTransparency(void)
{
    float ret = 0;
    if (material()) {
        Node *nMaterial = material()->getValue();
        if (nMaterial) 
            ret = nMaterial->getTransparency();
    }
    return ret; 
}

int NodeAppearance::getProfile(void) const
{ 
//    if (!isDefault(fillProperties_Field()))
//        return PROFILE_FULL;
    if (!isDefault(textureTransform_Field()))
        return PROFILE_IMMERSIVE;
//    if (!isDefault(lineProperties_Field()))
//        return PROFILE_IMMERSIVE;
    return PROFILE_INTERCHANGE; 
}


