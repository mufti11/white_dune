/*
 * NodeAppearance.h
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

#include "ExternTheApp.h"
#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "DuneApp.h"
#include "SFMFTypes.h"
#include "MFNode.h"

class ProtoAppearance : public WonderlandExportProto {
public:
                    ProtoAppearance(Scene *scene);
                    ProtoAppearance(Scene *scene, const char *name);
    void            addElements();
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_APPEARANCE; }

    virtual bool    isDeclaredInRwd_h() { return true; }

    FieldIndex fillProperties;
    FieldIndex lineProperties;
    FieldIndex material;
    FieldIndex texture;
    FieldIndex textureTransform;
    FieldIndex texture2;
    FieldIndex textureTransform2;    
    FieldIndex texture3;
    FieldIndex textureTransform3;
    FieldIndex texture4;
    FieldIndex textureTransform4;
    FieldIndex shaders;
    FieldIndex receiveShadows;
    FieldIndex shadowCaster;
    FieldIndex normalMap;
    FieldIndex heightMap;
    FieldIndex heightMapScale;
    FieldIndex blendMode;
    FieldIndex effects;
    FieldIndex alphaClipThreshold;
    FieldIndex colorMaskMode;
    FieldIndex depthMode;
    FieldIndex sortKey;
    FieldIndex sortType;
};


class NodeAppearance : public Node {
public:
                    NodeAppearance(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const;
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() { return new NodeAppearance(m_scene, m_proto); }

    void            bind();
    void            unbind();

    virtual bool    isInvalidChildNode(void) { return true; }

    virtual bool    isTransparent(void);
    virtual float   getTransparency(void);

    virtual bool    showFields() 
                        { return TheApp->getCoverMode() ||
                                 TheApp->getKambiMode(); }

    virtual bool    hasCoverFields(void) { return true; }   
    virtual bool    hasKambiFields(void) { return true; }   

    fieldMacros(SFNode,   fillProperties,     ProtoAppearance)
    fieldMacros(SFNode,   lineProperties,     ProtoAppearance)
    fieldMacros(SFNode,   material,           ProtoAppearance)
    fieldMacros(SFNode,   texture,            ProtoAppearance)
    fieldMacros(SFNode,   textureTransform,   ProtoAppearance)
    fieldMacros(SFNode,   texture2,           ProtoAppearance)
    fieldMacros(SFNode,   textureTransform2,  ProtoAppearance)
    fieldMacros(SFNode,   texture3,           ProtoAppearance)
    fieldMacros(SFNode,   textureTransform3,  ProtoAppearance)
    fieldMacros(SFNode,   texture4,           ProtoAppearance)
    fieldMacros(SFNode,   textureTransform4,  ProtoAppearance)
    fieldMacros(MFNode,   shaders,            ProtoAppearance)
    fieldMacros(MFNode,   receiveShadows,     ProtoAppearance)
    fieldMacros(SFBool,   shadowCaster,       ProtoAppearance)
    fieldMacros(SFNode,   normalMap,          ProtoAppearance)
    fieldMacros(SFNode,   heightMap,          ProtoAppearance)
    fieldMacros(SFFloat,  heightMapScale,     ProtoAppearance)
    fieldMacros(SFNode,   blendMode,          ProtoAppearance);
    fieldMacros(MFNode,   effects,            ProtoAppearance);
    fieldMacros(SFFloat,  alphaClipThreshold, ProtoAppearance);
    fieldMacros(SFNode,   colorMaskMode,      ProtoAppearance);
    fieldMacros(SFNode,   depthMode,          ProtoAppearance);
    fieldMacros(SFInt32,  sortKey,            ProtoAppearance);
    fieldMacros(SFString, sortType,           ProtoAppearance);
};

