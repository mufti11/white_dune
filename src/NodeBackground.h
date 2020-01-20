/*
 * NodeBackground.h
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

class ProtoBackground : public Proto {
public:
                    ProtoBackground(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int         getType() const { return VRML_BACKGROUND; }

    virtual bool    needUpdate(void) { return true; }

    FieldIndex groundAngle;
    FieldIndex groundColor;
    FieldIndex frontUrl;
    FieldIndex rightUrl;
    FieldIndex backUrl;
    FieldIndex leftUrl;
    FieldIndex topUrl;
    FieldIndex bottomUrl;
    FieldIndex skyAngle;
    FieldIndex skyColor;
    FieldIndex transparency;
    FieldIndex bind;
    FieldIndex crossOrigin;
    FieldIndex description;
    FieldIndex isActive;
};


class NodeImageTexture;

class NodeBackground : public Node {
public:

                        NodeBackground(Scene *scene, Proto *proto);

    virtual bool    needUpdate(void) { return true; }

    fieldMacros(MFFloat,  groundAngle,  ProtoBackground)
    fieldMacros(MFColor,  groundColor,  ProtoBackground)
    fieldMacros(MFString, frontUrl,     ProtoBackground)
    fieldMacros(MFString, rightUrl,     ProtoBackground)
    fieldMacros(MFString, backUrl,      ProtoBackground)
    fieldMacros(MFString, leftUrl,      ProtoBackground)
    fieldMacros(MFString, topUrl,       ProtoBackground)
    fieldMacros(MFString, bottomUrl,    ProtoBackground)
    fieldMacros(MFFloat,  skyAngle,     ProtoBackground)
    fieldMacros(MFColor,  skyColor,     ProtoBackground)
    fieldMacros(SFFloat,  transparency, ProtoBackground)
    fieldMacros(SFBool,   bind,         ProtoBackground)
    fieldMacros(SFString, crossOrigin,  ProtoBackground) 
    fieldMacros(SFString, description,  ProtoBackground)
    fieldMacros(SFBool,   isActive,     ProtoBackground)

protected:
    virtual            ~NodeBackground();

public:
    virtual int         getProfile(void) const;
    virtual const char* getComponentName(void) const;
    virtual int         getComponentLevel(void) const;
    virtual int         getX3dVersion(void) const { return 0; }
    virtual Node       *copy() const { return new NodeBackground(*this); }

    virtual void        setField(int field, FieldValue *value, int cf = -1);
    virtual void        preDraw();
    void                apply();
    virtual bool        hasNumbers4kids(void) { return true; } 
    int                 countPolygons(void);
    int                 countPolygons1Sided(void) { return countPolygons(); }
    void                drawBackgroundTextures();
    void                drawBackgroundSphere();
    void                drawSky(void);
    void                drawGround(void);
    void                drawQuad(float r, float va1, float va2, 
                                 float h1, float h2, const float *newColor);
    void                setColor(const float *newColor);
private:
    NodeImageTexture   *imageTextures[6];
    float               m_matrix[16];
};
