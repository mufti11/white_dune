/*
 * NodeLayout.h
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

class ProtoLayout : public Proto {
public:
                    ProtoLayout(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_LAYOUT; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex align;
    FieldIndex offset;
    FieldIndex offsetUnits;
    FieldIndex scaleMode;
    FieldIndex size;
    FieldIndex sizeUnits;
};

class NodeLayout : public Node {
public:
                    NodeLayout(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const { return "Layout"; }
    virtual int         getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 2; }
    virtual Node   *copy() const { return new NodeLayout(*this); }

    void                  modifyViewportData(float *data, 
                              bool sizeChangeAbleX, bool sizeChangeAbleY,
                              int screenWidth, int screenHeight,
                              float *scaleX, float *scaleY);

    fieldMacros(MFString, align,       ProtoLayout);
    fieldMacros(MFFloat,  offset,      ProtoLayout);
    fieldMacros(MFString, offsetUnits, ProtoLayout);
    fieldMacros(MFString, scaleMode,   ProtoLayout);
    fieldMacros(MFFloat,  size,        ProtoLayout);
    fieldMacros(MFString, sizeUnits,   ProtoLayout);
};

