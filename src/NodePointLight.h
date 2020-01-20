/*
 * NodePointLight.h
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

class ProtoPointLight : public Proto {
public:
                    ProtoPointLight(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_POINT_LIGHT; }
    virtual int     getNodeClass() const 
                       { return CHILD_NODE | LIGHT_NODE; }

    virtual bool    isDeclaredInRwd_h() { return true; }      

    FieldIndex ambientIntensity;
    FieldIndex attenuation;
    FieldIndex color;
    FieldIndex global;
    FieldIndex intensity;
    FieldIndex location;
    FieldIndex on;
    FieldIndex radius;
    kambiLightCommonFieldIndex()
    x3domLightCommonFieldIndex()
};

class NodePointLight : public Node {
public:
                    NodePointLight(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const;
    virtual const char* getComponentName(void) const;
    virtual int     getComponentLevel(void) const;
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodePointLight(*this); }

    virtual bool    hasNumbers4kids(void) { return true; } 

    virtual bool    hasKambiFields(void) { return true; }

    virtual void    preDraw();
    virtual void    drawHandles(void);

    virtual int     getAnimationCommentID(void);

    virtual Vec3f   getHandle(int handle, int *constraint, int *field);
    virtual void    setHandle(int handle, const Vec3f &v);

    virtual int     writeRib(int filedes, int indent);

    fieldMacros(SFFloat, ambientIntensity, ProtoPointLight)
    fieldMacros(SFVec3f, attenuation,      ProtoPointLight)
    fieldMacros(SFColor, color,            ProtoPointLight)
    fieldMacros(SFBool,  global,           ProtoPointLight)
    fieldMacros(SFFloat, intensity,        ProtoPointLight)
    fieldMacros(SFVec3f, location,         ProtoPointLight)
    fieldMacros(SFBool,  on,               ProtoPointLight)
    fieldMacros(SFFloat, radius,           ProtoPointLight)
    kambiLightCommonFieldMacros(ProtoPointLight)
    x3domLightCommonFieldMacros(ProtoPointLight)
protected:
    GLenum          m_light;
};

