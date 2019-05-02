/*
 * NodeStippleVolumeStyle.h
 *
 * Copyright (C) 2018 J. "MUFTI" Scheurich
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

#ifndef _NODE_STIPPLE_VOLUME_STYLE_H
#define _NODE_STIPPLE_VOLUME_STYLE_H

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

class ProtoStippleVolumeStyle : public Proto {
public:
                    ProtoStippleVolumeStyle(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_STIPPLE_VOLUME_STYLE; }

    FieldIndex distanceFactor;
    FieldIndex enabled;
    FieldIndex gradientBoundaryOpacity;
    FieldIndex gradientOpacityFactor;
    FieldIndex gradientRetainedOpacity;
    FieldIndex gradientThreshold;
    FieldIndex interiorFactor;
    FieldIndex lightingFactor;
    FieldIndex resolutionFactor;
    FieldIndex silhouetteBoundaryOpacity;
    FieldIndex silhouetteOpacityFactor;
    FieldIndex silhouetteRetainedOpacity;
};

class NodeStippleVolumeStyle : public Node {
public:
                    NodeStippleVolumeStyle(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeStippleVolumeStyle(*this); }

    virtual int     getX3dVersion(void) const { return -1; }


    fieldMacros(SFFloat, distanceFactor, ProtoStippleVolumeStyle)
    fieldMacros(SFBool, enabled, ProtoStippleVolumeStyle)
    fieldMacros(SFFloat, gradientBoundaryOpacity, ProtoStippleVolumeStyle)
    fieldMacros(SFFloat, gradientOpacityFactor, ProtoStippleVolumeStyle)
    fieldMacros(SFFloat, gradientRetainedOpacity, ProtoStippleVolumeStyle)
    fieldMacros(SFFloat, gradientThreshold, ProtoStippleVolumeStyle)
    fieldMacros(SFFloat, interiorFactor, ProtoStippleVolumeStyle)
    fieldMacros(SFFloat, lightingFactor, ProtoStippleVolumeStyle)
    fieldMacros(SFFloat, resolutionFactor, ProtoStippleVolumeStyle)
    fieldMacros(SFFloat, silhouetteBoundaryOpacity, ProtoStippleVolumeStyle)
    fieldMacros(SFFloat, silhouetteOpacityFactor, ProtoStippleVolumeStyle)
    fieldMacros(SFFloat, silhouetteRetainedOpacity, ProtoStippleVolumeStyle)
};

#endif
