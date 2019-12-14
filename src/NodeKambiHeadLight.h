/*
 * NodeKambiHeadLight.h
 *
 * Copyright (C) 1999 Stephen F. White, 2008 J. "MUFTI" Scheurich
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

#ifndef _NODE_KAMBI_HEAD_LIGHT_H
#define _NODE_KAMBI_HEAD_LIGHT_H

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

#include "KambiCommonFunctions.h"

#include "SFMFTypes.h"

class ProtoKambiHeadLight : public Proto {
public:
                    ProtoKambiHeadLight(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return KAMBI_KAMBI_HEAD_LIGHT; }
    virtual int     getNodeClass() const 
                       { return CHILD_NODE | LIGHT_NODE; }

    FieldIndex ambientIntensity;
    FieldIndex attenuation;
    FieldIndex color;
    FieldIndex intensity;
    FieldIndex spot;
    FieldIndex spotDropOffRate;
    FieldIndex spotCutOffAngle;

    virtual bool          isKambiProto(void) { return true; }
};

class NodeKambiHeadLight : public Node {
public:
                    NodeKambiHeadLight(Scene *scene, Proto *proto);

    virtual Node   *copy() { return new NodeKambiHeadLight(m_scene, m_proto); }
    void            preDraw();

    kambiCommonFunctions()

    fieldMacros(SFFloat, ambientIntensity, ProtoKambiHeadLight)
    fieldMacros(SFVec3f, attenuation,      ProtoKambiHeadLight)
    fieldMacros(SFColor, color,            ProtoKambiHeadLight)
    fieldMacros(SFFloat, intensity,        ProtoKambiHeadLight)
    fieldMacros(SFBool,  spot,             ProtoKambiHeadLight)
    fieldMacros(SFFloat, spotDropOffRate,  ProtoKambiHeadLight)
    fieldMacros(SFFloat, spotCutOffAngle,  ProtoKambiHeadLight)

protected:
    GLenum          m_light;
};

#endif 
