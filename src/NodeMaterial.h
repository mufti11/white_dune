/*
 * NodeMaterial.h
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

#ifndef _NODE_MATERIAL_H
#define _NODE_MATERIAL_H

#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "swt.h"

#include "SFMFTypes.h"

class ProtoMaterial : public WonderlandExportProto {
public:
                    ProtoMaterial(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_MATERIAL; }
    virtual int     getNodeClass() const { return MATERIAL_NODE; }

    virtual bool    isDeclaredInRwd_h() { return true; }

    FieldIndex ambientIntensity;
    FieldIndex diffuseColor;
    FieldIndex emissiveColor;
    FieldIndex shininess;
    FieldIndex specularColor;
    FieldIndex transparency;
    FieldIndex mirror;
    FieldIndex reflSpecular;
    FieldIndex reflDiffuse;
    FieldIndex transSpecular;
    FieldIndex transDiffuse;
    FieldIndex reflSpecularExp;
    FieldIndex transSpecularExp;
};

class NodeMaterial : public Node {
public:
                    NodeMaterial(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const;
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeMaterial(*this); }

    virtual bool    isInvalidChildNode(void) { return true; }
    virtual int     getAnimationCommentID(void);

    virtual void    bind();
    virtual void    unbind();

    void            diffuse2emissive(void);
    void            emissive2diffuse(void);

    virtual bool    isTransparent(void);
    virtual float   getTransparency(void) { return transparency()->getValue(); }

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

    fieldMacros(SFFloat, ambientIntensity, ProtoMaterial)
    fieldMacros(SFColor, diffuseColor,     ProtoMaterial)
    fieldMacros(SFColor, emissiveColor,    ProtoMaterial)
    fieldMacros(SFFloat, shininess,        ProtoMaterial)
    fieldMacros(SFColor, specularColor,    ProtoMaterial)
    fieldMacros(SFFloat, transparency,     ProtoMaterial)
    fieldMacros(SFFloat, mirror,           ProtoMaterial)
    fieldMacros(MFColor, reflSpecular,     ProtoMaterial)
    fieldMacros(MFColor, reflDiffuse,      ProtoMaterial)
    fieldMacros(MFColor, transSpecular,    ProtoMaterial)
    fieldMacros(MFColor, transDiffuse,     ProtoMaterial)
    fieldMacros(SFFloat, reflSpecularExp,  ProtoMaterial)
    fieldMacros(SFFloat, transSpecularExp, ProtoMaterial)

protected:

    int  m_ac3dMaterialIndex;
    bool m_isFirstLdrawColor;
};

#endif // _NODE_MATERIAL_H
