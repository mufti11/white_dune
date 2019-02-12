/*
 * NodeBlendedVolumeStyle.h
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

#ifndef _NODE_BLENDED_VOLUME_STYLE_H
#define _NODE_BLENDED_VOLUME_STYLE_H

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

class ProtoBlendedVolumeStyle : public Proto {
public:
                    ProtoBlendedVolumeStyle(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_BLENDED_VOLUME_STYLE; }
    virtual int     getNodeClass() const
        { return COMPOSABLE_VOLUME_RENDER_STYLE_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex enabled;
    FieldIndex renderStyle;
    FieldIndex voxels;
    FieldIndex weightConstant1;
    FieldIndex weightConstant2;
    FieldIndex weightFunction1;
    FieldIndex weightFunction2;
    FieldIndex weightTransferFunction1;
    FieldIndex weightTransferFunction2;
    FieldIndex surfaceNormals;
};

class NodeBlendedVolumeStyle : public Node {
public:
                    NodeBlendedVolumeStyle(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const 
        { return "VolumeRendering"; }
    virtual int     getComponentLevel(void) const { return 3; }
    virtual int     getX3dVersion(void) const { return 3; }
    virtual Node   *copy() const { return new NodeBlendedVolumeStyle(*this); }

    fieldMacros(SFBool, enabled, ProtoBlendedVolumeStyle);
    fieldMacros(SFNode, renderStyle, ProtoBlendedVolumeStyle);
    fieldMacros(SFNode, voxels, ProtoBlendedVolumeStyle);
    fieldMacros(SFFloat, weightConstant1, ProtoBlendedVolumeStyle);
    fieldMacros(SFFloat, weightConstant2, ProtoBlendedVolumeStyle);
    fieldMacros(SFString, weightFunction1, ProtoBlendedVolumeStyle);
    fieldMacros(SFString, weightFunction2, ProtoBlendedVolumeStyle);
    fieldMacros(SFNode, weightTransferFunction1, ProtoBlendedVolumeStyle);
    fieldMacros(SFNode, weightTransferFunction2, ProtoBlendedVolumeStyle);
    fieldMacros(SFNode, surfaceNormals, ProtoBlendedVolumeStyle);
public:
    int *m_fbohandles;
    int m_weightFunction1;
    int m_weightFunction2;
    float m_weightConstant1;
    float m_weightConstant2;
};

#endif
