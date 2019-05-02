/*
 * NodeEnvironment.cpp
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

#include <stdio.h>
#include "stdafx.h"

#include "NodeEnvironment.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "MFNode.h"
#include "MFNode.h"
#include "MFFloat.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "DuneApp.h"

ProtoEnvironment::ProtoEnvironment(Scene *scene)
  : Proto(scene, "Environment")
{
    bind.set(
        addExposedField(SFBOOL, "bind", new SFBool(false)));
    setFieldFlags(bind, FF_X3DOM_ONLY);

    description.set(
        addExposedField(SFSTRING, "description", new SFString("")));
    setFieldFlags(description, FF_X3DOM_ONLY);

    enableARC.set(
        addExposedField(SFBOOL, "enableARC", new SFBool(false)));
    setFieldFlags(enableARC, FF_X3DOM_ONLY);

    frustumCulling.set(
        addExposedField(SFBOOL, "frustumCulling", new SFBool(true)));
    setFieldFlags(frustumCulling, FF_X3DOM_ONLY);

    gammaCorrectionDefault.set(
        addExposedField(SFSTRING, "gammaCorrectionDefault", new SFString("linear")));
    setFieldFlags(gammaCorrectionDefault, FF_X3DOM_ONLY);

    isActive.set(
        addExposedField(SFBOOL, "isActive", new SFBool(false)));
    setFieldFlags(isActive, FF_X3DOM_ONLY);

    lowPriorityCulling.set(
        addExposedField(SFBOOL, "lowPriorityCulling", new SFBool(false)));
    setFieldFlags(lowPriorityCulling, FF_X3DOM_ONLY);

    lowPriorityFactor.set(
        addExposedField(SFFLOAT, "lowPriorityFactor", new SFFloat(-1)));
    setFieldFlags(lowPriorityFactor, FF_X3DOM_ONLY);

    lowPriorityThreshold.set(
        addExposedField(SFFLOAT, "lowPriorityThreshold", new SFFloat(0)));
    setFieldFlags(lowPriorityThreshold, FF_X3DOM_ONLY);

    maxFrameRate.set(
        addExposedField(SFFLOAT, "maxFrameRate", new SFFloat(5)));
    setFieldFlags(maxFrameRate, FF_X3DOM_ONLY);

    minFrameRate.set(
        addExposedField(SFFLOAT, "minFrameRate", new SFFloat(0)));
    setFieldFlags(minFrameRate, FF_X3DOM_ONLY);

    occlusionCulling.set(
        addExposedField(SFBOOL, "occlusionCulling", new SFBool(false)));
    setFieldFlags(occlusionCulling, FF_X3DOM_ONLY);

    occlusionVisibilityFactor.set(
        addExposedField(SFFLOAT, "occlusionVisibilityFactor", new SFFloat(-1)));
    setFieldFlags(occlusionVisibilityFactor, FF_X3DOM_ONLY);

    occlusionVisibilityThreshold.set(
        addExposedField(SFFLOAT, "occlusionVisibilityThreshold", new SFFloat(0)));
    setFieldFlags(occlusionVisibilityThreshold, FF_X3DOM_ONLY);

    shadowExcludeTransparentObjects.set(
        addExposedField(SFBOOL, "shadowExcludeTransparentObjects", new SFBool(false)));
    setFieldFlags(shadowExcludeTransparentObjects, FF_X3DOM_ONLY);

    smallFeatureCulling.set(
        addExposedField(SFBOOL, "smallFeatureCulling", new SFBool(false)));
    setFieldFlags(smallFeatureCulling, FF_X3DOM_ONLY);

    smallFeatureFactor.set(
        addExposedField(SFFLOAT, "smallFeatureFactor", new SFFloat(-1)));
    setFieldFlags(smallFeatureFactor, FF_X3DOM_ONLY);

    smallFeatureThreshold.set(
        addExposedField(SFFLOAT, "smallFeatureThreshold", new SFFloat(0)));
    setFieldFlags(smallFeatureThreshold, FF_X3DOM_ONLY);

    sortTrans.set(
        addExposedField(SFBOOL, "sortTrans", new SFBool(true)));
    setFieldFlags(sortTrans, FF_X3DOM_ONLY);

    SSAO.set(
        addExposedField(SFBOOL, "SSAO", new SFBool(false)));
    setFieldFlags(SSAO, FF_X3DOM_ONLY);

    SSAOamount.set(
        addExposedField(SFFLOAT, "SSAOamount", new SFFloat(0)));
    setFieldFlags(SSAOamount, FF_X3DOM_ONLY);

    SSAOblurDepthTreshold.set(
        addExposedField(SFFLOAT, "SSAOblurDepthTreshold", new SFFloat(5)));
    setFieldFlags(SSAOblurDepthTreshold, FF_X3DOM_ONLY);

    SSAOradius.set(
        addExposedField(SFFLOAT, "SSAOradius", new SFFloat(4)));
    setFieldFlags(SSAOradius, FF_X3DOM_ONLY);

    SSAOrandomTextureSize.set(
        addExposedField(SFFLOAT, "SSAOrandomTextureSize", new SFFloat(4)));
    setFieldFlags(SSAOrandomTextureSize, FF_X3DOM_ONLY);

    tessellationDetailCulling.set(
        addExposedField(SFBOOL, "tessellationDetailCulling", new SFBool(false)));
    setFieldFlags(tessellationDetailCulling, FF_X3DOM_ONLY);

    tessellationErrorFactor.set(
        addExposedField(SFFLOAT, "tessellationErrorFactor", new SFFloat(-1)));
    setFieldFlags(tessellationErrorFactor, FF_X3DOM_ONLY);

    tessellationErrorThreshold.set(
        addExposedField(SFFLOAT, "tessellationErrorThreshold", new SFFloat(0)));
    setFieldFlags(tessellationErrorThreshold, FF_X3DOM_ONLY);

    userDataFactor.set(
        addExposedField(SFFLOAT, "userDataFactor", new SFFloat(-1)));
    setFieldFlags(userDataFactor, FF_X3DOM_ONLY);
}

Node *
ProtoEnvironment::create(Scene *scene)
{ 
    return new NodeEnvironment(scene, this); 
}

NodeEnvironment::NodeEnvironment(Scene *scene, Proto *def)
  : Node(scene, def)
{
}
