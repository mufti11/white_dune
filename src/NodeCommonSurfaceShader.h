/*
 * NodeCommonSurfaceShader.h
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

#pragma once

#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "NodeGroup.h"
#include "SFMFTypes.h"

class ProtoCommonSurfaceShader : public ProtoGroup {
public:
                    ProtoCommonSurfaceShader(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_COMMON_SURFACE_SHADER; }
    virtual int     getNodeClass() const { return SHADER_NODE; }

    FieldIndex alphaFactor;
    FieldIndex alphaTexture;
    FieldIndex alphaTextureChannelMask;
    FieldIndex alphaTextureCoordinatesId;
    FieldIndex alphaTextureId;
    FieldIndex ambientFactor;
    FieldIndex ambientTexture;
    FieldIndex ambientTextureChannelMask;
    FieldIndex ambientTextureCoordinatesId;
    FieldIndex ambientTextureId;
    FieldIndex binormalTextureCoordinatesId;
    FieldIndex diffuseDisplacementTexture;
    FieldIndex diffuseFactor;
    FieldIndex diffuseTexture;
    FieldIndex diffuseTextureChannelMask;
    FieldIndex diffuseTextureCoordinatesId;
    FieldIndex diffuseTextureId;
    FieldIndex displacementAxis;
    FieldIndex displacementFactor;
    FieldIndex displacementTexture;
    FieldIndex displacementTextureCoordinatesId;
    FieldIndex displacementTextureId;
    FieldIndex emissiveDisplacementTexture;
    FieldIndex emissiveFactor;
    FieldIndex emissiveTexture;
    FieldIndex emissiveTextureChannelMask;
    FieldIndex emissiveTextureCoordinatesId;
    FieldIndex emissiveTextureId;
    FieldIndex environmentDisplacementTexture;
    FieldIndex environmentFactor;
    FieldIndex environmentTexture;
    FieldIndex environmentTextureChannelMask;
    FieldIndex environmentTextureCoordinatesId;
    FieldIndex environmentTextureId;
    FieldIndex fresnelBlend;
    FieldIndex invertAlphaTexture;
    FieldIndex language;
    FieldIndex multiDiffuseAlphaTexture;
    FieldIndex multiEmmisiveAmbientIntensityTexture;
    FieldIndex multiSpecularShininessTexture;
    FieldIndex multiVisibilityTexture;
    FieldIndex normalBias;
    FieldIndex normalFormat;
    FieldIndex normalScale;
    FieldIndex normalSpace;
    FieldIndex normalTexture;
    FieldIndex normalTextureChannelMask;
    FieldIndex normalTextureCoordinatesId;
    FieldIndex normalTextureId;
    FieldIndex reflectionFactor;
    FieldIndex reflectionTexture;
    FieldIndex reflectionTextureChannelMask;
    FieldIndex reflectionTextureCoordinatesId;
    FieldIndex reflectionTextureId;
    FieldIndex relativeIndexOfRefraction;
    FieldIndex shininessFactor;
    FieldIndex shininessTexture;
    FieldIndex shininessTextureChannelMask;
    FieldIndex shininessTextureCoordinatesId;
    FieldIndex shininessTextureId;
    FieldIndex specularFactor;
    FieldIndex specularTexture;
    FieldIndex specularTextureChannelMask;
    FieldIndex specularTextureCoordinatesId;
    FieldIndex specularTextureId;
    FieldIndex tangentTextureCoordinatesId;
    FieldIndex transmissionFactor;
    FieldIndex transmissionTexture;
    FieldIndex transmissionTextureChannelMask;
    FieldIndex transmissionTextureCoordinatesId;
    FieldIndex transmissionTextureId;
};

class NodeCommonSurfaceShader : public NodeGroup {
public:
                    NodeCommonSurfaceShader(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeCommonSurfaceShader(*this); }

    fieldMacros(SFFloat, alphaFactor, ProtoCommonSurfaceShader)
    fieldMacros(SFNode, alphaTexture, ProtoCommonSurfaceShader)
    fieldMacros(SFString, alphaTextureChannelMask, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, alphaTextureCoordinatesId, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, alphaTextureId, ProtoCommonSurfaceShader)
    fieldMacros(SFVec3f, ambientFactor, ProtoCommonSurfaceShader)
    fieldMacros(SFNode, ambientTexture, ProtoCommonSurfaceShader)
    fieldMacros(SFString, ambientTextureChannelMask, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, ambientTextureCoordinatesId, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, ambientTextureId, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, binormalTextureCoordinatesId, ProtoCommonSurfaceShader)
    fieldMacros(SFNode, diffuseDisplacementTexture, ProtoCommonSurfaceShader)
    fieldMacros(SFVec3f, diffuseFactor, ProtoCommonSurfaceShader)
    fieldMacros(SFNode, diffuseTexture, ProtoCommonSurfaceShader)
    fieldMacros(SFString, diffuseTextureChannelMask, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, diffuseTextureCoordinatesId, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, diffuseTextureId, ProtoCommonSurfaceShader)
    fieldMacros(SFString, displacementAxis, ProtoCommonSurfaceShader)
    fieldMacros(SFFloat, displacementFactor, ProtoCommonSurfaceShader)
    fieldMacros(SFNode, displacementTexture, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, displacementTextureCoordinatesId, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, displacementTextureId, ProtoCommonSurfaceShader)
    fieldMacros(SFNode, emissiveDisplacementTexture, ProtoCommonSurfaceShader)
    fieldMacros(SFVec3f, emissiveFactor, ProtoCommonSurfaceShader)
    fieldMacros(SFNode, emissiveTexture, ProtoCommonSurfaceShader)
    fieldMacros(SFString, emissiveTextureChannelMask, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, emissiveTextureCoordinatesId, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, emissiveTextureId, ProtoCommonSurfaceShader)
    fieldMacros(SFNode, environmentDisplacementTexture, ProtoCommonSurfaceShader)
    fieldMacros(SFVec3f, environmentFactor, ProtoCommonSurfaceShader)
    fieldMacros(SFNode, environmentTexture, ProtoCommonSurfaceShader)
    fieldMacros(SFString, environmentTextureChannelMask, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, environmentTextureCoordinatesId, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, environmentTextureId, ProtoCommonSurfaceShader)
    fieldMacros(SFFloat, fresnelBlend, ProtoCommonSurfaceShader)
    fieldMacros(SFBool, invertAlphaTexture, ProtoCommonSurfaceShader)
    fieldMacros(SFString, language, ProtoCommonSurfaceShader)
    fieldMacros(SFNode, multiDiffuseAlphaTexture, ProtoCommonSurfaceShader)
    fieldMacros(SFNode, multiEmmisiveAmbientIntensityTexture, ProtoCommonSurfaceShader)
    fieldMacros(SFNode, multiSpecularShininessTexture, ProtoCommonSurfaceShader)
    fieldMacros(SFNode, multiVisibilityTexture, ProtoCommonSurfaceShader)
    fieldMacros(SFVec3f, normalBias, ProtoCommonSurfaceShader)
    fieldMacros(SFString, normalFormat, ProtoCommonSurfaceShader)
    fieldMacros(SFVec3f, normalScale, ProtoCommonSurfaceShader)
    fieldMacros(SFString, normalSpace, ProtoCommonSurfaceShader)
    fieldMacros(SFNode, normalTexture, ProtoCommonSurfaceShader)
    fieldMacros(SFString, normalTextureChannelMask, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, normalTextureCoordinatesId, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, normalTextureId, ProtoCommonSurfaceShader)
    fieldMacros(SFVec3f, reflectionFactor, ProtoCommonSurfaceShader)
    fieldMacros(SFNode, reflectionTexture, ProtoCommonSurfaceShader)
    fieldMacros(SFString, reflectionTextureChannelMask, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, reflectionTextureCoordinatesId, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, reflectionTextureId, ProtoCommonSurfaceShader)
    fieldMacros(SFFloat, relativeIndexOfRefraction, ProtoCommonSurfaceShader)
    fieldMacros(SFFloat, shininessFactor, ProtoCommonSurfaceShader)
    fieldMacros(SFNode, shininessTexture, ProtoCommonSurfaceShader)
    fieldMacros(SFString, shininessTextureChannelMask, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, shininessTextureCoordinatesId, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, shininessTextureId, ProtoCommonSurfaceShader)
    fieldMacros(SFVec3f, specularFactor, ProtoCommonSurfaceShader)
    fieldMacros(SFNode, specularTexture, ProtoCommonSurfaceShader)
    fieldMacros(SFString, specularTextureChannelMask, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, specularTextureCoordinatesId, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, specularTextureId, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, tangentTextureCoordinatesId, ProtoCommonSurfaceShader)
    fieldMacros(SFVec3f, transmissionFactor, ProtoCommonSurfaceShader)
    fieldMacros(SFNode, transmissionTexture, ProtoCommonSurfaceShader)
    fieldMacros(SFString, transmissionTextureChannelMask, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, transmissionTextureCoordinatesId, ProtoCommonSurfaceShader)
    fieldMacros(SFInt32, transmissionTextureId, ProtoCommonSurfaceShader)
};

