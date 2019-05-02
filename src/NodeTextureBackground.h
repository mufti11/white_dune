/*
 * NodeTextureBackground.h
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

#ifndef _NODE_TextureBackground_H
#define _NODE_TextureBackground_H

#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"

class ProtoTextureBackground : public Proto {
public:
                    ProtoTextureBackground(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_TEXTURE_BACKGROUND; }

    virtual bool    isX3dInternalProto(void) { return true; }

    virtual bool    needUpdate(void) { return true; }

    FieldIndex groundAngle;
    FieldIndex groundColor;
    FieldIndex frontTexture;
    FieldIndex rightTexture;
    FieldIndex backTexture;
    FieldIndex leftTexture;
    FieldIndex topTexture;
    FieldIndex bottomTexture;
    FieldIndex skyAngle;
    FieldIndex skyColor;
    FieldIndex transparency;
};


class NodeImageTexture;

class NodeTextureBackground : public Node {
public:

                        NodeTextureBackground(Scene *scene, Proto *proto);

    virtual int         getX3dVersion(void) const { return 0; }

    fieldMacros(MFFloat, groundAngle,   ProtoTextureBackground)
    fieldMacros(MFColor, groundColor,   ProtoTextureBackground)
    fieldMacros(SFNode,  frontTexture,  ProtoTextureBackground)
    fieldMacros(SFNode,  rightTexture,  ProtoTextureBackground)
    fieldMacros(SFNode,  backTexture,   ProtoTextureBackground)
    fieldMacros(SFNode,  leftTexture,   ProtoTextureBackground)
    fieldMacros(SFNode,  topTexture,    ProtoTextureBackground)
    fieldMacros(SFNode,  bottomTexture, ProtoTextureBackground)
    fieldMacros(MFFloat, skyAngle,      ProtoTextureBackground)
    fieldMacros(MFColor, skyColor,      ProtoTextureBackground)
    fieldMacros(SFFloat, transparency,  ProtoTextureBackground)

protected:
    virtual            ~NodeTextureBackground();

public:
    virtual const char* getComponentName(void) const 
                           { return "EnvironmentalEffects"; }
    virtual int         getComponentLevel(void) const { return 3; }
    virtual Node       *copy() const 
                           { return new NodeTextureBackground(*this); }

    virtual void        setField(int field, FieldValue *value, int cf = -1);
    virtual void        preDraw();
    void                apply();
    int                 countPolygons(void);
    int                 countPolygons1Sided(void) { return countPolygons(); }
    void                drawTextureBackgroundTextures();
    void                drawTextureBackgroundSphere();
    void                drawSky(void);
    void                drawGround(void);
    void                drawQuad(float r, float va1, float va2, 
                                 float h1, float h2, const float *newColor);
    void                setColor(const float *newColor);
private:
    NodeImageTexture   *m_imageTextures[6];
};

#endif
