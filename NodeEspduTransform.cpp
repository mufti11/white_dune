/*
 * NodeEspduTransform.cpp
 *
 * Copyright (C) 1999 Stephen F. White
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

#include "NodeEspduTransform.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFString.h"
#include "SFInt32.h"
#include "MFInt32.h"
#include "SFFloat.h"
#include "MFFloat.h"
#include "SFVec3f.h"

ProtoEspduTransform::ProtoEspduTransform(Scene *scene)
  : ProtoTransform(scene, "EspduTransform")
{
    addEventIn(SFFLOAT, "set_articulationParameterValue0");

    addEventIn(SFFLOAT, "set_articulationParameterValue1");

    addEventIn(SFFLOAT, "set_articulationParameterValue2");

    addEventIn(SFFLOAT, "set_articulationParameterValue3");

    addEventIn(SFFLOAT, "set_articulationParameterValue4");

    addEventIn(SFFLOAT, "set_articulationParameterValue5");

    addEventIn(SFFLOAT, "set_articulationParameterValue6");

    addEventIn(SFFLOAT, "set_articulationParameterValue7");

    articulationParameterCount.set(
          addExposedField(SFINT32, "articulationParameterCount", new SFInt32(0),
                          new SFInt32(0), new SFInt32(78)));

    articulationParameterDesignatorArray.set(
          addExposedField(MFINT32, "articulationParameterDesignatorArray", 
                          new MFInt32() /* */));

    articulationParameterChangeIndicatorArray.set(
          addExposedField(MFINT32, "articulationParameterChangeIndicatorArray", 
                          new MFInt32() /* */));

    articulationParameterIdPartAttachedToArray.set(
          addExposedField(MFINT32, "articulationParameterIdPartAttachedToArray", 
                          new MFInt32() /* */));

    articulationParameterTypeArray.set(
          addExposedField(MFINT32, "articulationParameterTypeArray", 
                          new MFInt32() /* */));

    articulationParameterArray.set(
          addExposedField(MFFLOAT, "articulationParameterArray", 
                          new MFFloat()));

    collisionType.set(
          addExposedField(SFINT32, "collisionType", new SFInt32(0),
                          new SFInt32(0), new SFInt32(255)));

    deadReckoning.set(
          addExposedField(SFINT32, "deadReckoning", new SFInt32(0),
                          new SFInt32(0), new SFInt32(255)));

    detonationLocation.set(
          addExposedField(SFVEC3F, "detonationLocation", new SFVec3f()));

    detonationRelativeLocation.set(
          addExposedField(SFVEC3F, "detonationRelativeLocation", 
                          new SFVec3f()));

    detonationResult.set(
          addExposedField(SFINT32, "detonationResult", new SFInt32(0),
                          new SFInt32(0), new SFInt32(255)));

    entityCategory.set(
          addExposedField(SFINT32, "entityCategory", new SFInt32(0),
                          new SFInt32(0), new SFInt32(255)));

    entityCountry .set(
          addExposedField(SFINT32, "entityCountry", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));

    entityDomain.set(
          addExposedField(SFINT32, "entityDomain", new SFInt32(0),
                          new SFInt32(0), new SFInt32(255)));

    entityExtra.set(
          addExposedField(SFINT32, "entityExtra", new SFInt32(0),
                          new SFInt32(0), new SFInt32(255)));

    entityKind.set(
          addExposedField(SFINT32, "entityKind", new SFInt32(0),
                          new SFInt32(0), new SFInt32(255)));

    entitySpecific.set(
          addExposedField(SFINT32, "entitySpecific", new SFInt32(0),
                          new SFInt32(0), new SFInt32(255)));

    eventApplicationID.set(
          addExposedField(SFINT32, "eventApplicationID", new SFInt32(1),
                          new SFInt32(0), new SFInt32(65535)));

    eventEntityID.set(
          addExposedField(SFINT32, "eventEntityID", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));

    eventNumber.set(
          addExposedField(SFINT32, "eventNumber", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));

    eventSiteID.set(
          addExposedField(SFINT32, "eventSiteID", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));

    fired1.set(
          addExposedField(SFBOOL, "fired1", new SFBool(false)));

    fired2.set(
          addExposedField(SFBOOL, "fired2", new SFBool(false)));

    fireMissionIndex.set(
          addExposedField(SFINT32, "fireMissionIndex", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));

    firingRange.set(
          addExposedField(SFFLOAT, "firingRange", new SFFloat(0.0f), 
                          new SFFloat(0.0f)));

    firingRate.set(
          addExposedField(SFINT32, "firingRate", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));

    forceID.set(
          addExposedField(SFINT32, "forceID", new SFInt32(0),
                          new SFInt32(0), new SFInt32(255)));

    fuse.set(
          addExposedField(SFINT32, "fuse", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));

    linearVelocity.set(
          addExposedField(SFVEC3F, "linearVelocity", new SFVec3f()));

    linearAcceleration.set(
          addExposedField(SFVEC3F, "linearAcceleration", new SFVec3f()));

    marking.set(
          addExposedField(SFSTRING, "marking", new SFString()));

    munitionApplicationID.set(
          addExposedField(SFINT32, "munitionApplicationID", new SFInt32(1),
                          new SFInt32(0), new SFInt32(65535)));

    munitionEndPoint.set(
          addExposedField(SFVEC3F, "munitionEndPoint", new SFVec3f()));

    munitionEntityID.set(
          addExposedField(SFINT32, "munitionEntityID", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));

    munitionQuantity.set(
          addExposedField(SFINT32, "munitionQuantity", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));

    munitionSiteID.set(
          addExposedField(SFINT32, "munitionSiteID", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));

    munitionStartPoint.set(
          addExposedField(SFVEC3F, "munitionStartPoint", new SFVec3f()));

    warhead.set(
          addExposedField(SFINT32, "warhead", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));

    addEventOut(SFFLOAT, "articulationParameterValue0_changed");
    addEventOut(SFFLOAT, "articulationParameterValue1_changed");
    addEventOut(SFFLOAT, "articulationParameterValue2_changed");
    addEventOut(SFFLOAT, "articulationParameterValue3_changed");
    addEventOut(SFFLOAT, "articulationParameterValue4_changed");
    addEventOut(SFFLOAT, "articulationParameterValue5_changed");
    addEventOut(SFFLOAT, "articulationParameterValue6_changed");
    addEventOut(SFFLOAT, "articulationParameterValue7_changed");
    addEventOut(SFTIME, "collideTime");
    addEventOut(SFTIME, "detonateTime");
    addEventOut(SFTIME, "firedTime");
    addEventOut(SFBOOL, "isCollided");
    addEventOut(SFBOOL, "isDetonated");
    disCommonBehaviorFields()
    disCommonIdentificationFields()
    disCommonCommunicationFields()
}

Node *
ProtoEspduTransform::create(Scene *scene)
{ 
    return new NodeEspduTransform(scene, this); 
}

NodeEspduTransform::NodeEspduTransform(Scene *scene, Proto *def)
  : NodeTransform(scene, def)
{
}

