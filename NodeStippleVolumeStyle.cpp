/*
 * NodeStippleVolumeStyle.cpp
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

#include "NodeStippleVolumeStyle.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "MFNode.h"
#include "MFNode.h"
#include "MFFloat.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "DuneApp.h"

ProtoStippleVolumeStyle::ProtoStippleVolumeStyle(Scene *scene)
  : Proto(scene, "StippleVolumeStyle")
{
    distanceFactor.set(
        addExposedField(SFFLOAT, "distanceFactor", new SFFloat(1)));
    setFieldFlags(distanceFactor, FF_X3DOM_ONLY);

    enabled.set(
        addExposedField(SFBOOL, "enabled", new SFBool(true)));
    setFieldFlags(enabled, FF_X3DOM_ONLY);

    gradientBoundaryOpacity.set(
        addExposedField(SFFLOAT, "gradientBoundaryOpacity", new SFFloat(0)));
    setFieldFlags(gradientBoundaryOpacity, FF_X3DOM_ONLY);

    gradientOpacityFactor.set(
        addExposedField(SFFLOAT, "gradientOpacityFactor", new SFFloat(1)));
    setFieldFlags(gradientOpacityFactor, FF_X3DOM_ONLY);

    gradientRetainedOpacity.set(
        addExposedField(SFFLOAT, "gradientRetainedOpacity", new SFFloat(1)));
    setFieldFlags(gradientRetainedOpacity, FF_X3DOM_ONLY);

    gradientThreshold.set(
        addExposedField(SFFLOAT, "gradientThreshold", new SFFloat(4)));
    setFieldFlags(gradientThreshold, FF_X3DOM_ONLY);

    interiorFactor.set(
        addExposedField(SFFLOAT, "interiorFactor", new SFFloat(1)));
    setFieldFlags(interiorFactor, FF_X3DOM_ONLY);

    lightingFactor.set(
        addExposedField(SFFLOAT, "lightingFactor", new SFFloat(1)));
    setFieldFlags(lightingFactor, FF_X3DOM_ONLY);

    resolutionFactor.set(
        addExposedField(SFFLOAT, "resolutionFactor", new SFFloat(1)));
    setFieldFlags(resolutionFactor, FF_X3DOM_ONLY);

    silhouetteBoundaryOpacity.set(
        addExposedField(SFFLOAT, "silhouetteBoundaryOpacity", new SFFloat(0)));
    setFieldFlags(silhouetteBoundaryOpacity, FF_X3DOM_ONLY);

    silhouetteOpacityFactor.set(
        addExposedField(SFFLOAT, "silhouetteOpacityFactor", new SFFloat(1)));
    setFieldFlags(silhouetteOpacityFactor, FF_X3DOM_ONLY);

    silhouetteRetainedOpacity.set(
        addExposedField(SFFLOAT, "silhouetteRetainedOpacity", new SFFloat(1)));
    setFieldFlags(silhouetteRetainedOpacity, FF_X3DOM_ONLY);
}

Node *
ProtoStippleVolumeStyle::create(Scene *scene)
{ 
    return new NodeStippleVolumeStyle(scene, this); 
}

NodeStippleVolumeStyle::NodeStippleVolumeStyle(Scene *scene, Proto *proto)
  : Node(scene, proto)
{
}
