/*
 * NodeRenderedTexture.h
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

#ifndef _NODE_RENDERED_TEXTURE_H
#define _NODE_RENDERED_TEXTURE_H

#ifndef _NODE_H
#include "ExternTheApp.h"
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"
#include "KambiCommonFunctions.h"
#include "KambiTextureCommonFields.h"

class ProtoRenderedTexture : public Proto {
public:
                    ProtoRenderedTexture(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return KAMBI_RENDERED_TEXTURE; }
    virtual int     getNodeClass() const { return TEXTURE_NODE; }

    FieldIndex background;
    FieldIndex crossOrigin;
    FieldIndex depthMap1;
    FieldIndex depthMap2;
    FieldIndex dimensions;
    FieldIndex excludeNodes;
    FieldIndex fog;
    FieldIndex interpupillaryDistance;
    FieldIndex oculusRiftVersion;
    FieldIndex origChannelCount;        
    FieldIndex repeatS;
    FieldIndex repeatT;
    FieldIndex repeatR;
    FieldIndex scale;
    FieldIndex scene;
    FieldIndex showNormals;
    FieldIndex stereoMode;
    FieldIndex textureProperties;
    FieldIndex update;
    FieldIndex url;
    FieldIndex viewpoint;
    kambiTextureCommonFieldIndex()

    virtual bool    isKambiProto(void) { return true; }
    virtual bool    isX3domProto(void) { return true; }
};

class NodeRenderedTexture : public Node {
public:
                    NodeRenderedTexture(Scene *scene, Proto *proto);

    virtual Node *copy() { return new NodeRenderedTexture(m_scene, m_proto); }

    kambiCommonFunctions()

    fieldMacros(SFNode, background, ProtoRenderedTexture);
    fieldMacros(SFString, crossOrigin, ProtoRenderedTexture);
    fieldMacros(SFBool, depthMap1, ProtoRenderedTexture);
    fieldMacros(MFBool, depthMap2, ProtoRenderedTexture);
    fieldMacros(MFInt32, dimensions, ProtoRenderedTexture);
    fieldMacros(MFNode, excludeNodes, ProtoRenderedTexture);
    fieldMacros(SFNode, fog, ProtoRenderedTexture);
    fieldMacros(SFFloat, interpupillaryDistance, ProtoRenderedTexture);
    fieldMacros(SFFloat, oculusRiftVersion, ProtoRenderedTexture);
    fieldMacros(SFInt32, origChannelCount, ProtoRenderedTexture);
    fieldMacros(SFBool, repeatS, ProtoRenderedTexture);
    fieldMacros(SFBool, repeatT, ProtoRenderedTexture);
    fieldMacros(SFBool, repeatR, ProtoRenderedTexture);
    fieldMacros(SFBool, scale, ProtoRenderedTexture);
    fieldMacros(SFNode, scene, ProtoRenderedTexture);
    fieldMacros(SFBool, showNormals, ProtoRenderedTexture)
    fieldMacros(SFString, stereoMode, ProtoRenderedTexture);
    fieldMacros(SFNode, textureProperties, ProtoRenderedTexture);
    fieldMacros(SFString, update, ProtoRenderedTexture);
    fieldMacros(MFString, url, ProtoRenderedTexture);
    fieldMacros(SFNode, viewpoint, ProtoRenderedTexture);
    kambiTextureCommonFieldMacros(ProtoRenderedTexture)
};

#endif
