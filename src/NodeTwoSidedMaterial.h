/*
 * NodeTwoSidedMaterial.h
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

#pragma once

#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "SFMFTypes.h"

class ProtoTwoSidedMaterial : public Proto {
public:
                    ProtoTwoSidedMaterial(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_TWO_SIDED_MATERIAL; }
    virtual int     getNodeClass() const { return MATERIAL_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex ambientIntensity;
    FieldIndex backAmbientIntensity;
    FieldIndex backDiffuseColor;
    FieldIndex backEmissiveColor;
    FieldIndex backShininess;
    FieldIndex backSpecularColor;
    FieldIndex backTransparency;
    FieldIndex diffuseColor;
    FieldIndex emissiveColor;
    FieldIndex shininess;
    FieldIndex separateBackColor;
    FieldIndex specularColor;
    FieldIndex transparency;
};

class NodeTwoSidedMaterial : public Node {
public:
                    NodeTwoSidedMaterial(Scene *scene, Proto *proto);
    virtual int     getX3dVersion(void) const { return 2; }    
    virtual Node   *copy() const { return new NodeTwoSidedMaterial(*this); }

    virtual bool    isInvalidChildNode(void) { return true; }
    virtual int     getAnimationCommentID(void);

    virtual void    bind();
    virtual void    unbind();

    void            diffuse2emissive(void);
    void            emissive2diffuse(void);

    virtual bool    isTransparent(void);
    virtual float   getTransparency(void) { return transparency()->getValue(); }
    virtual float   getBackTransparency(void) 
                       { return backTransparency()->getValue(); }

    virtual void    reInit();

    virtual bool    hasKambiFields(void) { return true; }

    virtual int     writeAc3dMaterial(int filedes, int indent, 
                                      const char *name);
    virtual void    handleAc3dMaterial(ac3dMaterialCallback callback, 
                                       Scene* scene);
    int             getAc3dMaterialIndex(void) { return m_ac3dMaterialIndex; }
    virtual void    setAc3dMaterialIndex(int index) 
                       { m_ac3dMaterialIndex = index; }
    virtual int     getIncAc3dMaterialIndex(void) { return 1; }

    virtual int     writeLdrawDat(int filedes, int indent);

    virtual const char* getComponentName(void) const { return "Shape"; }
    virtual int         getComponentLevel(void) const { return 4; }

    fieldMacros(SFFloat, ambientIntensity,     ProtoTwoSidedMaterial);
    fieldMacros(SFFloat, backAmbientIntensity, ProtoTwoSidedMaterial);
    fieldMacros(SFColor, backDiffuseColor,     ProtoTwoSidedMaterial);
    fieldMacros(SFColor, backEmissiveColor,    ProtoTwoSidedMaterial);
    fieldMacros(SFFloat, backShininess,        ProtoTwoSidedMaterial);
    fieldMacros(SFColor, backSpecularColor,    ProtoTwoSidedMaterial);
    fieldMacros(SFFloat, backTransparency,     ProtoTwoSidedMaterial);
    fieldMacros(SFColor, diffuseColor,         ProtoTwoSidedMaterial);
    fieldMacros(SFColor, emissiveColor,        ProtoTwoSidedMaterial);
    fieldMacros(SFFloat, shininess,            ProtoTwoSidedMaterial);
    fieldMacros(SFBool,  separateBackColor,    ProtoTwoSidedMaterial);
    fieldMacros(SFColor, specularColor,        ProtoTwoSidedMaterial);
    fieldMacros(SFFloat, transparency,         ProtoTwoSidedMaterial);

protected:

    int  m_ac3dMaterialIndex;
    bool m_isFirstLdrawColor;

public:
    float *m_verifiedFrontColor;
    float *m_verifiedBackColor;
};

