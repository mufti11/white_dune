/*
 * NodeEnvironment.h
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

#ifndef _NODE_ENVIRONMENT_H
#define _NODE_ENVIRONMENT_H

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

class ProtoEnvironment : public Proto {
public:
                    ProtoEnvironment(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_ENVIRONMENT; }

    FieldIndex bind;
    FieldIndex description;
    FieldIndex enableARC;
    FieldIndex frustumCulling;
    FieldIndex gammaCorrectionDefault;
    FieldIndex isActive;
    FieldIndex lowPriorityCulling;
    FieldIndex lowPriorityFactor;
    FieldIndex lowPriorityThreshold;
    FieldIndex maxFrameRate;
    FieldIndex minFrameRate;
    FieldIndex occlusionCulling;
    FieldIndex occlusionVisibilityFactor;
    FieldIndex occlusionVisibilityThreshold;
    FieldIndex shadowExcludeTransparentObjects;
    FieldIndex smallFeatureCulling;
    FieldIndex smallFeatureFactor;
    FieldIndex smallFeatureThreshold;
    FieldIndex sortTrans;
    FieldIndex SSAO;
    FieldIndex SSAOamount;
    FieldIndex SSAOblurDepthTreshold;
    FieldIndex SSAOradius;
    FieldIndex SSAOrandomTextureSize;
    FieldIndex tessellationDetailCulling;
    FieldIndex tessellationErrorFactor;
    FieldIndex tessellationErrorThreshold;
    FieldIndex userDataFactor;
};

class NodeEnvironment : public Node {
public:
                    NodeEnvironment(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeEnvironment(*this); }

    virtual int     getX3dVersion(void) const { return -1; }

    fieldMacros(SFBool, bind, ProtoEnvironment)
    fieldMacros(SFString, description, ProtoEnvironment)
    fieldMacros(SFBool, enableARC, ProtoEnvironment)
    fieldMacros(SFBool, frustumCulling, ProtoEnvironment)
    fieldMacros(SFString, gammaCorrectionDefault, ProtoEnvironment)
    fieldMacros(SFBool, isActive, ProtoEnvironment)
    fieldMacros(SFBool, lowPriorityCulling, ProtoEnvironment)
    fieldMacros(SFFloat, lowPriorityFactor, ProtoEnvironment)
    fieldMacros(SFFloat, lowPriorityThreshold, ProtoEnvironment)
    fieldMacros(SFFloat, maxFrameRate, ProtoEnvironment)
    fieldMacros(SFFloat, minFrameRate, ProtoEnvironment)
    fieldMacros(SFBool, occlusionCulling, ProtoEnvironment)
    fieldMacros(SFFloat, occlusionVisibilityFactor, ProtoEnvironment)
    fieldMacros(SFFloat, occlusionVisibilityThreshold, ProtoEnvironment)
    fieldMacros(SFBool, shadowExcludeTransparentObjects, ProtoEnvironment)
    fieldMacros(SFBool, smallFeatureCulling, ProtoEnvironment)
    fieldMacros(SFFloat, smallFeatureFactor, ProtoEnvironment)
    fieldMacros(SFFloat, smallFeatureThreshold, ProtoEnvironment)
    fieldMacros(SFBool, sortTrans, ProtoEnvironment)
    fieldMacros(SFBool, SSAO, ProtoEnvironment)
    fieldMacros(SFFloat, SSAOamount, ProtoEnvironment)
    fieldMacros(SFFloat, SSAOblurDepthTreshold, ProtoEnvironment)
    fieldMacros(SFFloat, SSAOradius, ProtoEnvironment)
    fieldMacros(SFFloat, SSAOrandomTextureSize, ProtoEnvironment)
    fieldMacros(SFBool, tessellationDetailCulling, ProtoEnvironment)
    fieldMacros(SFFloat, tessellationErrorFactor, ProtoEnvironment)
    fieldMacros(SFFloat, tessellationErrorThreshold, ProtoEnvironment)
    fieldMacros(SFFloat, userDataFactor, ProtoEnvironment)
};

#endif
