/*
 * NodeSpotLight.h
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

#ifndef _NODE_SPOTLIGHT_H
#define _NODE_SPOTLIGHT_H

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

#include "KambiLightCommonFields.h"
#include "X3domLightCommonFields.h"

class ProtoSpotLight : public Proto {
public:
                    ProtoSpotLight(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_SPOT_LIGHT; }
    virtual int     getNodeClass() const 
                       { return CHILD_NODE | LIGHT_NODE; }

    virtual bool    isDeclaredInRwd_h() { return true; }      

    virtual bool    needUpdate(void) { return true; }

    FieldIndex ambientIntensity;
    FieldIndex attenuation;
    FieldIndex beamWidth;
    FieldIndex color;
    FieldIndex cutOffAngle;
    FieldIndex direction;
    FieldIndex global;
    FieldIndex intensity;
    FieldIndex location;
    FieldIndex on;
    FieldIndex radius;
    FieldIndex projectionAngle;
    kambiLightCommonFieldIndex()
    x3domLightCommonFieldIndex()
    FieldIndex shadowCascades;
    FieldIndex shadowSplitFactor;
    FieldIndex shadowSplitOffset;
};

class NodeSpotLight : public Node {
public:
                    NodeSpotLight(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const;
    virtual const char* getComponentName(void) const;
    virtual int     getComponentLevel(void) const;
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeSpotLight(*this); }

    virtual bool    hasNumbers4kids(void) { return true; } 

    virtual bool    hasKambiFields(void) { return true; }

    virtual int     writeRib(int filedes, int indent);

    void            preDraw();

    fieldMacros(SFFloat, ambientIntensity, ProtoSpotLight)
    fieldMacros(SFVec3f, attenuation,      ProtoSpotLight)
    fieldMacros(SFFloat, beamWidth,        ProtoSpotLight)
    fieldMacros(SFColor, color,            ProtoSpotLight)
    fieldMacros(SFFloat, cutOffAngle,      ProtoSpotLight)
    fieldMacros(SFVec3f, direction,        ProtoSpotLight)
    fieldMacros(SFBool,  global,           ProtoSpotLight)
    fieldMacros(SFFloat, intensity,        ProtoSpotLight)
    fieldMacros(SFVec3f, location,         ProtoSpotLight)
    fieldMacros(SFBool,  on,               ProtoSpotLight)
    fieldMacros(SFFloat, radius,           ProtoSpotLight)
    fieldMacros(SFFloat, projectionAngle,  ProtoSpotLight)
    kambiLightCommonFieldMacros(ProtoSpotLight)
    x3domLightCommonFieldMacros(ProtoSpotLight)
    fieldMacros(SFInt32, shadowCascades, ProtoSpotLight);
    fieldMacros(SFFloat, shadowSplitFactor, ProtoSpotLight);
    fieldMacros(SFFloat, shadowSplitOffset, ProtoSpotLight);
protected:
    GLenum          m_light;
};

#endif // _NODE_SPOTLIGHT_H
