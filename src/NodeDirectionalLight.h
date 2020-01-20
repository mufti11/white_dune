/*
 * NodeDirectionalLight.h
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
#include "swt.h"
#include "SFMFTypes.h"
#include "KambiLightCommonFields.h"
#include "X3domLightCommonFields.h"

class ProtoDirectionalLight : public Proto {
public:
                    ProtoDirectionalLight(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_DIRECTIONAL_LIGHT; }
    virtual int     getNodeClass() const 
                       { return CHILD_NODE | LIGHT_NODE; }

    virtual bool    isDeclaredInRwd_h() { return true; }      

    FieldIndex ambientIntensity;
    FieldIndex color;
    FieldIndex direction;
    FieldIndex global;
    FieldIndex intensity;
    FieldIndex on;
    FieldIndex projectionRectangle;
    FieldIndex projectionLocation;
    kambiLightCommonFieldIndex()
    x3domLightCommonFieldIndex()
    FieldIndex shadowCascades;
    FieldIndex shadowSplitFactor;
    FieldIndex shadowSplitOffset;
};

class NodeDirectionalLight : public Node {
public:
                    NodeDirectionalLight(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual const char* getComponentName(void) const;
    virtual int     getComponentLevel(void) const;
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeDirectionalLight(*this); }

    virtual bool    hasNumbers4kids(void) { return true; } 

    virtual bool    hasKambiFields(void) { return true; }

    virtual int     writeRib(int filedes, int indent);

    virtual void    bind();
    virtual void    unbind();
    virtual int     getAnimationCommentID(void);

    fieldMacros(SFFloat, ambientIntensity,    ProtoDirectionalLight)
    fieldMacros(SFColor, color,               ProtoDirectionalLight)
    fieldMacros(SFVec3f, direction,           ProtoDirectionalLight)
    fieldMacros(SFBool,  global,              ProtoDirectionalLight)
    fieldMacros(SFFloat, intensity,           ProtoDirectionalLight)
    fieldMacros(SFBool,  on,                  ProtoDirectionalLight)
    fieldMacros(SFVec4f, projectionRectangle, ProtoDirectionalLight);
    fieldMacros(SFVec3f, projectionLocation,  ProtoDirectionalLight);
    kambiLightCommonFieldMacros(ProtoDirectionalLight)
    x3domLightCommonFieldMacros(ProtoDirectionalLight)
    fieldMacros(SFInt32, shadowCascades,      ProtoDirectionalLight);
    fieldMacros(SFFloat, shadowSplitFactor,   ProtoDirectionalLight);
    fieldMacros(SFFloat, shadowSplitOffset,   ProtoDirectionalLight);
protected:
    GLenum             m_light;
};

