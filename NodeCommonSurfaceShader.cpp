/*
 * NodeCommonSurfaceShader.cpp
 *
 * Copyright (C) 2018 J. "MUFTI" Scheurich
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

#include "NodeCommonSurfaceShader.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "MFNode.h"
#include "MFNode.h"
#include "MFFloat.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "DuneApp.h"

ProtoCommonSurfaceShader::ProtoCommonSurfaceShader(Scene *scene)
  : ProtoGroup(scene, "CommonSurfaceShader")
{
    alphaFactor.set(
        addExposedField(SFFLOAT, "alphaFactor", new SFFloat(1)));
    setFieldFlags(alphaFactor, FF_X3DOM_ONLY);

    alphaTexture.set(
        addExposedField(SFNODE, "alphaTexture", new SFNode(NULL),
                        TEXTURE_NODE));
    setFieldFlags(alphaTexture, FF_X3DOM_ONLY);

    alphaTextureChannelMask.set(
        addExposedField(SFSTRING, "alphaTextureChannelMask", 
                        new SFString("a")));
    setFieldFlags(alphaTextureChannelMask, FF_X3DOM_ONLY);

    alphaTextureCoordinatesId.set(
        addExposedField(SFINT32, "alphaTextureCoordinatesId", new SFInt32(0)));
    setFieldFlags(alphaTextureCoordinatesId, FF_X3DOM_ONLY);

    alphaTextureId.set(
        addExposedField(SFINT32, "alphaTextureId", new SFInt32(-1)));
    setFieldFlags(alphaTextureId, FF_X3DOM_ONLY);

    ambientFactor.set(
        addExposedField(SFVEC3F, "ambientFactor", new SFVec3f(2, 2, 2)));
    setFieldFlags(ambientFactor, FF_X3DOM_ONLY);

    ambientTexture.set(
        addExposedField(SFNODE, "ambientTexture", new SFNode(NULL),
                        TEXTURE_NODE));
    setFieldFlags(ambientTexture, FF_X3DOM_ONLY);

    ambientTextureChannelMask.set(
        addExposedField(SFSTRING, "ambientTextureChannelMask", 
                        new SFString("rgba")));
    setFieldFlags(ambientTextureChannelMask, FF_X3DOM_ONLY);

    ambientTextureCoordinatesId.set(
        addExposedField(SFINT32, "ambientTextureCoordinatesId", 
                        new SFInt32(0)));
    setFieldFlags(ambientTextureCoordinatesId, FF_X3DOM_ONLY);

    ambientTextureId.set(
        addExposedField(SFINT32, "ambientTextureId", new SFInt32(-1)));
    setFieldFlags(ambientTextureId, FF_X3DOM_ONLY);

    binormalTextureCoordinatesId.set(
        addExposedField(SFINT32, "binormalTextureCoordinatesId", 
                        new SFInt32(-1)));
    setFieldFlags(binormalTextureCoordinatesId, FF_X3DOM_ONLY);

    diffuseDisplacementTexture.set(
        addExposedField(SFNODE, "diffuseDisplacementTexture", 
                        new SFNode(NULL)));
    setFieldFlags(diffuseDisplacementTexture, FF_X3DOM_ONLY);

    diffuseFactor.set(
        addExposedField(SFVEC3F, "diffuseFactor", new SFVec3f(1, 1, 1)));
    setFieldFlags(diffuseFactor, FF_X3DOM_ONLY);

    diffuseTexture.set(
        addExposedField(SFNODE, "diffuseTexture", new SFNode(NULL),
                        TEXTURE_NODE));
    setFieldFlags(diffuseTexture, FF_X3DOM_ONLY);

    diffuseTextureChannelMask.set(
        addExposedField(SFSTRING, "diffuseTextureChannelMask", 
                        new SFString("rgb")));
    setFieldFlags(diffuseTextureChannelMask, FF_X3DOM_ONLY);

    diffuseTextureCoordinatesId.set(
        addExposedField(SFINT32, "diffuseTextureCoordinatesId", new SFInt32(0)));
    setFieldFlags(diffuseTextureCoordinatesId, FF_X3DOM_ONLY);

    diffuseTextureId.set(
        addExposedField(SFINT32, "diffuseTextureId", new SFInt32(-1)));
    setFieldFlags(diffuseTextureId, FF_X3DOM_ONLY);

    displacementAxis.set(
        addExposedField(SFSTRING, "displacementAxis", new SFString("y")));
    setFieldFlags(displacementAxis, FF_X3DOM_ONLY);

    displacementFactor.set(
        addExposedField(SFFLOAT, "displacementFactor", new SFFloat(0)));
    setFieldFlags(displacementFactor, FF_X3DOM_ONLY);

    displacementTexture.set(
        addExposedField(SFNODE, "displacementTexture", new SFNode(NULL),
                        TEXTURE_NODE));
    setFieldFlags(displacementTexture, FF_X3DOM_ONLY);

    displacementTextureCoordinatesId.set(
        addExposedField(SFINT32, "displacementTextureCoordinatesId",
                        new SFInt32(0)));
    setFieldFlags(displacementTextureCoordinatesId, FF_X3DOM_ONLY);

    displacementTextureId.set(
        addExposedField(SFINT32, "displacementTextureId", new SFInt32(-1)));
    setFieldFlags(displacementTextureId, FF_X3DOM_ONLY);

    emissiveDisplacementTexture.set(
        addExposedField(SFNODE, "emissiveDisplacementTexture", 
                        new SFNode(NULL)));
    setFieldFlags(emissiveDisplacementTexture, FF_X3DOM_ONLY);

    emissiveFactor.set(
        addExposedField(SFVEC3F, "emissiveFactor", new SFVec3f(0, 0, 0)));
    setFieldFlags(emissiveFactor, FF_X3DOM_ONLY);

    emissiveTexture.set(
        addExposedField(SFNODE, "emissiveTexture", new SFNode(NULL),
                        TEXTURE_NODE));
    setFieldFlags(emissiveTexture, FF_X3DOM_ONLY);

    emissiveTextureChannelMask.set(
        addExposedField(SFSTRING, "emissiveTextureChannelMask", 
                        new SFString("rgb")));
    setFieldFlags(emissiveTextureChannelMask, FF_X3DOM_ONLY);

    emissiveTextureCoordinatesId.set(
        addExposedField(SFINT32, "emissiveTextureCoordinatesId", new SFInt32(0)));
    setFieldFlags(emissiveTextureCoordinatesId, FF_X3DOM_ONLY);

    emissiveTextureId.set(
        addExposedField(SFINT32, "emissiveTextureId", new SFInt32(-1)));
    setFieldFlags(emissiveTextureId, FF_X3DOM_ONLY);

    environmentDisplacementTexture.set(
        addExposedField(SFNODE, "environmentDisplacementTexture", 
                        new SFNode(NULL)));
    setFieldFlags(environmentDisplacementTexture, FF_X3DOM_ONLY);

    environmentFactor.set(
        addExposedField(SFVEC3F, "environmentFactor", new SFVec3f(1, 1, 1)));
    setFieldFlags(environmentFactor, FF_X3DOM_ONLY);

    environmentTexture.set(
        addExposedField(SFNODE, "environmentTexture", new SFNode(NULL),
                        TEXTURE_NODE));
    setFieldFlags(environmentTexture, FF_X3DOM_ONLY);

    environmentTextureChannelMask.set(
        addExposedField(SFSTRING, "environmentTextureChannelMask", 
                        new SFString("rgb")));
    setFieldFlags(environmentTextureChannelMask, FF_X3DOM_ONLY);

    environmentTextureCoordinatesId.set(
        addExposedField(SFINT32, "environmentTextureCoordinatesId", new SFInt32(0)));
    setFieldFlags(environmentTextureCoordinatesId, FF_X3DOM_ONLY);

    environmentTextureId.set(
        addExposedField(SFINT32, "environmentTextureId", new SFInt32(-1)));
    setFieldFlags(environmentTextureId, FF_X3DOM_ONLY);

    fresnelBlend.set(
        addExposedField(SFFLOAT, "fresnelBlend", new SFFloat(0)));
    setFieldFlags(fresnelBlend, FF_X3DOM_ONLY);

    invertAlphaTexture.set(
        addExposedField(SFBOOL, "invertAlphaTexture", new SFBool(true)));
    setFieldFlags(invertAlphaTexture, FF_X3DOM_ONLY);

    language.set(
        addExposedField(SFSTRING, "language", new SFString("")));
    setFieldFlags(language, FF_X3DOM_ONLY);

    multiDiffuseAlphaTexture.set(
        addExposedField(SFNODE, "multiDiffuseAlphaTexture", new SFNode(NULL),
                        TEXTURE_NODE));
    setFieldFlags(multiDiffuseAlphaTexture, FF_X3DOM_ONLY);

    multiEmmisiveAmbientIntensityTexture.set(
        addExposedField(SFNODE, "multiEmmisiveAmbientIntensityTexture", 
                        new SFNode(NULL), TEXTURE_NODE));
    setFieldFlags(multiEmmisiveAmbientIntensityTexture, FF_X3DOM_ONLY);

    multiSpecularShininessTexture.set(
        addExposedField(SFNODE, "multiSpecularShininessTexture", 
                        new SFNode(NULL), TEXTURE_NODE));
    setFieldFlags(multiSpecularShininessTexture, FF_X3DOM_ONLY);

    multiVisibilityTexture.set(
        addExposedField(SFNODE, "multiVisibilityTexture", new SFNode(NULL),
                        TEXTURE_NODE));
    setFieldFlags(multiVisibilityTexture, FF_X3DOM_ONLY);

    normalBias.set(
        addExposedField(SFVEC3F, "normalBias", new SFVec3f(1, 1, 1)));
    setFieldFlags(normalBias, FF_X3DOM_ONLY);

    normalFormat.set(
        addExposedField(SFSTRING, "normalFormat", new SFString("UNORM")));
    setFieldFlags(normalFormat, FF_X3DOM_ONLY);

    normalScale.set(
        addExposedField(SFVEC3F, "normalScale", new SFVec3f(2, 2, 2)));
    setFieldFlags(normalScale, FF_X3DOM_ONLY);

    normalSpace.set(
        addExposedField(SFSTRING, "normalSpace", new SFString("TANGENT")));
    setFieldFlags(normalSpace, FF_X3DOM_ONLY);

    normalTexture.set(
        addExposedField(SFNODE, "normalTexture", new SFNode(NULL),
                        TEXTURE_NODE));
    setFieldFlags(normalTexture, FF_X3DOM_ONLY);

    normalTextureChannelMask.set(
        addExposedField(SFSTRING, "normalTextureChannelMask", 
                        new SFString("rgb")));
    setFieldFlags(normalTextureChannelMask, FF_X3DOM_ONLY);

    normalTextureCoordinatesId.set(
        addExposedField(SFINT32, "normalTextureCoordinatesId", new SFInt32(0)));
    setFieldFlags(normalTextureCoordinatesId, FF_X3DOM_ONLY);

    normalTextureId.set(
        addExposedField(SFINT32, "normalTextureId", new SFInt32(-1)));
    setFieldFlags(normalTextureId, FF_X3DOM_ONLY);

    reflectionFactor.set(
        addExposedField(SFVEC3F, "reflectionFactor", new SFVec3f(0, 0, 0)));
    setFieldFlags(reflectionFactor, FF_X3DOM_ONLY);

    reflectionTexture.set(
        addExposedField(SFNODE, "reflectionTexture", new SFNode(NULL),
                        TEXTURE_NODE));
    setFieldFlags(reflectionTexture, FF_X3DOM_ONLY);

    reflectionTextureChannelMask.set(
        addExposedField(SFSTRING, "reflectionTextureChannelMask", 
                        new SFString("rgb")));
    setFieldFlags(reflectionTextureChannelMask, FF_X3DOM_ONLY);

    reflectionTextureCoordinatesId.set(
        addExposedField(SFINT32, "reflectionTextureCoordinatesId", new SFInt32(0)));
    setFieldFlags(reflectionTextureCoordinatesId, FF_X3DOM_ONLY);

    reflectionTextureId.set(
        addExposedField(SFINT32, "reflectionTextureId", new SFInt32(-1)));
    setFieldFlags(reflectionTextureId, FF_X3DOM_ONLY);

    relativeIndexOfRefraction.set(
        addExposedField(SFFLOAT, "relativeIndexOfRefraction", new SFFloat(1)));
    setFieldFlags(relativeIndexOfRefraction, FF_X3DOM_ONLY);

    shininessFactor.set(
        addExposedField(SFFLOAT, "shininessFactor", new SFFloat(2)));
    setFieldFlags(shininessFactor, FF_X3DOM_ONLY);

    shininessTexture.set(
        addExposedField(SFNODE, "shininessTexture", new SFNode(NULL),
                        TEXTURE_NODE));
    setFieldFlags(shininessTexture, FF_X3DOM_ONLY);

    shininessTextureChannelMask.set(
        addExposedField(SFSTRING, "shininessTextureChannelMask", 
                        new SFString("rgb")));
    setFieldFlags(shininessTextureChannelMask, FF_X3DOM_ONLY);

    shininessTextureCoordinatesId.set(
        addExposedField(SFINT32, "shininessTextureCoordinatesId", new SFInt32(0)));
    setFieldFlags(shininessTextureCoordinatesId, FF_X3DOM_ONLY);

    shininessTextureId.set(
        addExposedField(SFINT32, "shininessTextureId", new SFInt32(-1)));
    setFieldFlags(shininessTextureId, FF_X3DOM_ONLY);

    specularFactor.set(
        addExposedField(SFVEC3F, "specularFactor", new SFVec3f(0, 0, 0)));
    setFieldFlags(specularFactor, FF_X3DOM_ONLY);

    specularTexture.set(
        addExposedField(SFNODE, "specularTexture", new SFNode(NULL),
                        TEXTURE_NODE));
    setFieldFlags(specularTexture, FF_X3DOM_ONLY);

    specularTextureChannelMask.set(
        addExposedField(SFSTRING, "specularTextureChannelMask", 
                        new SFString("rgb")));
    setFieldFlags(specularTextureChannelMask, FF_X3DOM_ONLY);

    specularTextureCoordinatesId.set(
        addExposedField(SFINT32, "specularTextureCoordinatesId", new SFInt32(0)));
    setFieldFlags(specularTextureCoordinatesId, FF_X3DOM_ONLY);

    specularTextureId.set(
        addExposedField(SFINT32, "specularTextureId", new SFInt32(-1)));
    setFieldFlags(specularTextureId, FF_X3DOM_ONLY);

    tangentTextureCoordinatesId.set(
        addExposedField(SFINT32, "tangentTextureCoordinatesId", 
                        new SFInt32(-1)));
    setFieldFlags(tangentTextureCoordinatesId, FF_X3DOM_ONLY);

    transmissionFactor.set(
        addExposedField(SFVEC3F, "transmissionFactor", new SFVec3f(0, 0, 0)));
    setFieldFlags(transmissionFactor, FF_X3DOM_ONLY);

    transmissionTexture.set(
        addExposedField(SFNODE, "transmissionTexture", new SFNode(NULL),
                        TEXTURE_NODE));
    setFieldFlags(transmissionTexture, FF_X3DOM_ONLY);

    transmissionTextureChannelMask.set(
        addExposedField(SFSTRING, "transmissionTextureChannelMask", 
                        new SFString("rgb")));
    setFieldFlags(transmissionTextureChannelMask, FF_X3DOM_ONLY);

    transmissionTextureCoordinatesId.set(
        addExposedField(SFINT32, "transmissionTextureCoordinatesId", new SFInt32(0)));
    setFieldFlags(transmissionTextureCoordinatesId, FF_X3DOM_ONLY);

    transmissionTextureId.set(
        addExposedField(SFINT32, "transmissionTextureId", new SFInt32(-1)));
    setFieldFlags(transmissionTextureId, FF_X3DOM_ONLY);
}

Node *
ProtoCommonSurfaceShader::create(Scene *scene)
{ 
    return new NodeCommonSurfaceShader(scene, this); 
}

NodeCommonSurfaceShader::NodeCommonSurfaceShader(Scene *scene, Proto *def)
  : NodeGroup(scene, def)
{
}
