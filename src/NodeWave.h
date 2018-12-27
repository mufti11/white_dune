/*
 * NodeWave.h
 *
 * Copyright (C) 1999 Stephen F. White, 2005 J. "MUFTI" Scheurich
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
 * 
 * Implements the Cover/Covise Wave node
 */

#ifndef _NODE_WAVE_H
#define _NODE_WAVE_H

#ifndef _COVER_NODE_H
#include "CoverNode.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"

class ProtoWave : public Proto {
public:
                    ProtoWave(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return COVER_WAVE; }

    FieldIndex fraction;
    FieldIndex freq1;
    FieldIndex height1;
    FieldIndex damping1;
    FieldIndex dir1;
    FieldIndex freq2;
    FieldIndex height2;
    FieldIndex damping2;
    FieldIndex dir2;
    FieldIndex speed1;
    FieldIndex speed2;
    FieldIndex coeffSin;
    FieldIndex coeffCos;
    FieldIndex fileName;

    virtual bool          isCoverProto(void) { return true; }
};

class NodeWave : public CoverNode {
public:
                    NodeWave(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return -1; }    
    virtual Node   *copy() const { return new NodeWave(*this); }

    fieldMacros(SFFloat,    fraction,   ProtoWave)
    fieldMacros(SFFloat,    freq1,      ProtoWave)
    fieldMacros(SFFloat,    height1,    ProtoWave)
    fieldMacros(SFFloat,    damping1,   ProtoWave)
    fieldMacros(SFVec3f,    dir1,       ProtoWave)
    fieldMacros(SFFloat,    freq2,      ProtoWave)
    fieldMacros(SFFloat,    height2,    ProtoWave)
    fieldMacros(SFFloat,    damping2,   ProtoWave)
    fieldMacros(SFVec3f,    dir2,       ProtoWave)
    fieldMacros(SFFloat,    speed1,     ProtoWave)
    fieldMacros(SFFloat,    speed2,     ProtoWave)
    fieldMacros(SFRotation, coeffSin,   ProtoWave)
    fieldMacros(SFRotation, coeffCos,   ProtoWave)
    fieldMacros(SFString,   fileName,   ProtoWave)

};

#endif // _NODE_WAVE_H
