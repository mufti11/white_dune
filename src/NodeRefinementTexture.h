/*
 * NodeRefinementTexture.h
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

#pragma once

#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "SFMFTypes.h"

class ProtoRefinementTexture : public Proto {
public:
                    ProtoRefinementTexture(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_REFINEMENT_TEXTURE; }
    virtual int     getNodeClass() const { return TEXTURE_NODE; }

    FieldIndex autoRefinement;
    FieldIndex background;
    FieldIndex crossOrigin;
    FieldIndex depthMap;
    FieldIndex dimensions;
    FieldIndex excludeNodes;
    FieldIndex fog;
    FieldIndex format;
    FieldIndex interpupillaryDistance;
    FieldIndex iterations;
    FieldIndex maxLevel;
    FieldIndex oculusRiftVersion;
    FieldIndex origChannelCount;
    FieldIndex repeatS;
    FieldIndex repeatT;
    FieldIndex scale;
    FieldIndex scene;
    FieldIndex showNormals;
    FieldIndex stamp0;
    FieldIndex stamp1;
    FieldIndex stereoMode;
    FieldIndex textureProperties;
    FieldIndex update;
    FieldIndex url;
    FieldIndex viewpoint;
};

class NodeRefinementTexture : public Node {
public:
                    NodeRefinementTexture(Scene *scene, Proto *proto);
    virtual        ~NodeRefinementTexture();

    virtual int     getX3dVersion(void) const { return -1; }
    virtual Node   *copy() const { return new NodeRefinementTexture(*this); }

    virtual void    load();

    fieldMacros(SFBool,   autoRefinement,    ProtoRefinementTexture);
    fieldMacros(SFNode,   background,        ProtoRefinementTexture);
    fieldMacros(SFString, crossOrigin,       ProtoRefinementTexture);
    fieldMacros(SFBool,   depthMap,          ProtoRefinementTexture);
    fieldMacros(MFInt32,  dimensions,        ProtoRefinementTexture);
    fieldMacros(MFNode,   excludeNodes,      ProtoRefinementTexture);
    fieldMacros(SFNode,   fog,               ProtoRefinementTexture);
    fieldMacros(SFString, format,            ProtoRefinementTexture);
    fieldMacros(SFFloat,  interpupillaryDistance, ProtoRefinementTexture);
    fieldMacros(SFInt32,  iterations,        ProtoRefinementTexture);
    fieldMacros(SFInt32,  maxLevel,          ProtoRefinementTexture);
    fieldMacros(SFFloat,  oculusRiftVersion, ProtoRefinementTexture);
    fieldMacros(SFInt32,  origChannelCount,  ProtoRefinementTexture);
    fieldMacros(SFBool,   repeatS,           ProtoRefinementTexture)
    fieldMacros(SFBool,   repeatT,           ProtoRefinementTexture)
    fieldMacros(SFBool,   scale,             ProtoRefinementTexture);
    fieldMacros(SFNode,   scene,             ProtoRefinementTexture);
    fieldMacros(SFBool,   showNormals,       ProtoRefinementTexture)
    fieldMacros(SFString, stamp0,            ProtoRefinementTexture);
    fieldMacros(SFString, stamp1,            ProtoRefinementTexture);
    fieldMacros(SFString, stereoMode,        ProtoRefinementTexture);
    fieldMacros(SFNode,   textureProperties, ProtoRefinementTexture);
    fieldMacros(SFString, update,            ProtoRefinementTexture);
    fieldMacros(MFString, url,               ProtoRefinementTexture);
    fieldMacros(SFNode,   viewpoint,         ProtoRefinementTexture);
};

