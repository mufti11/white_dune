/*
 * NodeSky.h
 *
 * Copyright (C) 1999 Stephen F. White, 2006 J. "MUFTI" Scheurich
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

#include "CoverNode.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "SFMFTypes.h"

class ProtoSky : public Proto {
public:
                    ProtoSky(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return COVER_SKY; }

    FieldIndex enabled;
    FieldIndex timeLapse;
    FieldIndex currentTime;
    FieldIndex year;
    FieldIndex month;
    FieldIndex day;
    FieldIndex hour;
    FieldIndex minute;
    FieldIndex radius;
    FieldIndex latitude;
    FieldIndex longitude;
    FieldIndex altitude;

    virtual bool          isCoverProto(void) { return true; }
};

class NodeSky : public CoverNode {
public:
                    NodeSky(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return -1; } 
    virtual Node   *copy() const { return new NodeSky(*this); }

    fieldMacros(SFBool,  enabled,     ProtoSky)
    fieldMacros(SFBool,  timeLapse,   ProtoSky)
    fieldMacros(SFBool,  currentTime, ProtoSky)
    fieldMacros(SFInt32, year,        ProtoSky)
    fieldMacros(SFInt32, month,       ProtoSky)
    fieldMacros(SFInt32, day,         ProtoSky)
    fieldMacros(SFInt32, hour,        ProtoSky)
    fieldMacros(SFInt32, minute,      ProtoSky)
    fieldMacros(SFFloat, radius,      ProtoSky)
    fieldMacros(SFFloat, latitude,    ProtoSky)
    fieldMacros(SFFloat, longitude,   ProtoSky)
    fieldMacros(SFFloat, altitude,    ProtoSky)
};

