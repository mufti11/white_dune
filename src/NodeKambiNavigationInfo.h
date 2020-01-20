/*
 * NodeKambiNavigationInfo.h
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
#include "KambiCommonFunctions.h"
#include "NodeNavigationInfo.h"

class ProtoKambiNavigationInfo : public ProtoNavigationInfo {
public:
                    ProtoKambiNavigationInfo(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return KAMBI_KAMBI_NAVIGATION_INFO; }

    FieldIndex timeOriginAtLoad;
    FieldIndex octreeRendering;
    FieldIndex octreeDynamicCollisions;
    FieldIndex octreeVisibleTriangles;
    FieldIndex octreeCollidableTriangles;
    FieldIndex headBobbing;
    FieldIndex headBobbingTime;
    FieldIndex headBobbingDistance;
    FieldIndex headlightNode;

    virtual bool    isKambiProto(void) { return true; }
};

class NodeKambiNavigationInfo : public NodeNavigationInfo {
public:
                    NodeKambiNavigationInfo(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeKambiNavigationInfo(*this); }

    kambiCommonFunctions()

    fieldMacros(SFBool,  timeOriginAtLoad,          ProtoKambiNavigationInfo);
    fieldMacros(SFNode,  octreeRendering,           ProtoKambiNavigationInfo);
    fieldMacros(SFNode,  octreeDynamicCollisions,   ProtoKambiNavigationInfo);
    fieldMacros(SFNode,  octreeVisibleTriangles,    ProtoKambiNavigationInfo);
    fieldMacros(SFNode,  octreeCollidableTriangles, ProtoKambiNavigationInfo);
    fieldMacros(SFFloat, headBobbing,               ProtoKambiNavigationInfo);
    fieldMacros(SFFloat, headBobbingDistance,       ProtoKambiNavigationInfo);
    fieldMacros(SFFloat, headBobbingTime,           ProtoKambiNavigationInfo);
    fieldMacros(SFNode,  headlightNode,             ProtoKambiNavigationInfo);
};

