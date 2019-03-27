/*
 * NodeSegmentedVolumeData.h
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

#ifndef _NODE_SEGMENTED_VOLUME_DATA_H
#define _NODE_SEGMENTED_VOLUME_DATA_H

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

class ProtoSegmentedVolumeData : public Proto {
public:
                    ProtoSegmentedVolumeData(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_SEGMENTED_VOLUME_DATA; }
    virtual int     getNodeClass() const 
        { return VOLUME_DATA_NODE | CHILD_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex dimensions;
    FieldIndex renderStyle;
    FieldIndex segmentEnabled;
    FieldIndex segmentIdentifiers;
    FieldIndex voxels;
    FieldIndex bboxCenter;
    FieldIndex bboxSize;
    FieldIndex allowViewpointInside;
    FieldIndex appearance;
    FieldIndex geometry;
    FieldIndex isPickable;
    FieldIndex numberOfMaxSegments;
    FieldIndex render;
};

class NodeSegmentedVolumeData : public Node {
public:
                    NodeSegmentedVolumeData(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const
        { return "VolumeRendering"; }
    virtual int     getComponentLevel(void) const { return 2; }
    virtual int     getX3dVersion(void) const { return 3; } 
    virtual Node   *copy() const { return new NodeSegmentedVolumeData(*this); }

    fieldMacros(SFVec3f, dimensions, ProtoSegmentedVolumeData);
    fieldMacros(MFNode, renderStyle, ProtoSegmentedVolumeData);
    fieldMacros(MFBool, segmentEnabled, ProtoSegmentedVolumeData);
    fieldMacros(SFNode, segmentIdentifiers, ProtoSegmentedVolumeData);
    fieldMacros(SFNode, voxels, ProtoSegmentedVolumeData);
    fieldMacros(SFVec3f, bboxCenter, ProtoSegmentedVolumeData);
    fieldMacros(SFVec3f, bboxSize, ProtoSegmentedVolumeData);
    fieldMacros(SFBool, allowViewpointInside, ProtoSegmentedVolumeData);
    fieldMacros(SFNode, appearance, ProtoSegmentedVolumeData);
    fieldMacros(SFNode, geometry, ProtoSegmentedVolumeData);
    fieldMacros(SFBool, isPickable, ProtoSegmentedVolumeData);
    fieldMacros(SFFloat, numberOfMaxSegments, ProtoSegmentedVolumeData);
    fieldMacros(SFBool, render, ProtoSegmentedVolumeData);

public:
    int m_renderFlags;
};

#endif

