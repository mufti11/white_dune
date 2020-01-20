/*
 * NodeVolumeData.h
 *
 * Copyright (C) 2011 J. "MUFTI" Scheurich
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

class ProtoVolumeData : public Proto {
public:
                    ProtoVolumeData(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_VOLUME_DATA; }
    virtual int     getNodeClass() const 
        { return VOLUME_DATA_NODE | CHILD_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex allowViewpointInside;
    FieldIndex appearance;
    FieldIndex dimensions;
    FieldIndex geometry;
    FieldIndex isPickable;
    FieldIndex render;
    FieldIndex renderStyle;
    FieldIndex voxels;
    FieldIndex bboxCenter;
    FieldIndex bboxSize;
};

class NodeVolumeData : public Node {
public:
                    NodeVolumeData(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const
        { return "VolumeRendering"; }
    virtual int     getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 3; }    
    virtual Node   *copy() const { return new NodeVolumeData(*this); }

    virtual void preDraw();
    virtual void draw(int pass);

    fieldMacros(SFVec3f, dimensions, ProtoVolumeData);
    fieldMacros(SFNode, renderStyle, ProtoVolumeData);
    fieldMacros(SFNode, voxels, ProtoVolumeData);
    fieldMacros(SFVec3f, bboxCenter, ProtoVolumeData);
    fieldMacros(SFVec3f, bboxSize, ProtoVolumeData);
    fieldMacros(SFBool, allowViewpointInside, ProtoVolumeData);
    fieldMacros(SFNode, appearance, ProtoVolumeData);
    fieldMacros(SFNode, geometry, ProtoVolumeData);
    fieldMacros(SFBool, isPickable, ProtoVolumeData);
    fieldMacros(SFBool, render, ProtoVolumeData);

public:
    float *m_boxtris; 
};

