/*
 * NodeIsoSurfaceVolumeData.h
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

#ifndef _NODE_ISO_SURFACE_VOLUME_DATA_H
#define _NODE_ISO_SURFACE_VOLUME_DATA_H

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

class ProtoIsoSurfaceVolumeData : public Proto {
public:
                    ProtoIsoSurfaceVolumeData(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_ISO_SURFACE_VOLUME_DATA; }
    virtual int     getNodeClass() const 
        { return VOLUME_DATA_NODE | CHILD_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex contourStepSize;
    FieldIndex dimensions;
    FieldIndex gradients;
    FieldIndex renderStyle;
    FieldIndex surfaceTolerance;
    FieldIndex surfaceValues;
    FieldIndex voxels;
    FieldIndex bboxCenter;
    FieldIndex bboxSize;
    FieldIndex allowViewpointInside;
    FieldIndex appearance;
    FieldIndex geometry;
    FieldIndex isPickable;
    FieldIndex render;
};

class NodeIsoSurfaceVolumeData : public Node {
public:
                    NodeIsoSurfaceVolumeData(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const
        { return "VolumeRendering"; }
    virtual int     getComponentLevel(void) const { return 2; }
    virtual int     getX3dVersion(void) const { return 3; }
    virtual Node   *copy() const { return new NodeIsoSurfaceVolumeData(*this); }

    fieldMacros(SFFloat, contourStepSize, ProtoIsoSurfaceVolumeData);
    fieldMacros(SFVec3f, dimensions, ProtoIsoSurfaceVolumeData);
    fieldMacros(SFNode, gradients, ProtoIsoSurfaceVolumeData);
    fieldMacros(MFNode, renderStyle, ProtoIsoSurfaceVolumeData);
    fieldMacros(SFFloat, surfaceTolerance, ProtoIsoSurfaceVolumeData);
    fieldMacros(MFFloat, surfaceValues, ProtoIsoSurfaceVolumeData);
    fieldMacros(SFNode, voxels, ProtoIsoSurfaceVolumeData);
    fieldMacros(SFVec3f, bboxCenter, ProtoIsoSurfaceVolumeData);
    fieldMacros(SFVec3f, bboxSize, ProtoIsoSurfaceVolumeData);
    fieldMacros(SFBool, allowViewpointInside, ProtoIsoSurfaceVolumeData);
    fieldMacros(SFNode, appearance, ProtoIsoSurfaceVolumeData);
    fieldMacros(SFNode, geometry, ProtoIsoSurfaceVolumeData);
    fieldMacros(SFBool, isPickable, ProtoIsoSurfaceVolumeData);
    fieldMacros(SFBool, render, ProtoIsoSurfaceVolumeData);
};

#endif
