/*
 * NodeNavigationInfo.cpp
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

#include "NodeNavigationInfo.h"
#include "Scene.h"
#include "Proto.h"
#include "FieldValue.h"
#include "Field.h"
#include "MFFloat.h"
#include "SFBool.h"
#include "SFFloat.h"
#include "MFString.h"
#include "SFString.h"
#include "Node.h"

void 
ProtoNavigationInfo::addElements(void)
{
    addEventIn(SFBOOL, "set_bind");
    if (TheApp->getCoverMode()) {
        // non standard Covise/COVER extensions
        addEventIn(SFBOOL, "set_bindLast", FF_COVER_ONLY);
    }
    float values[] = { 0.25f, 1.6f, 0.75f };
    float *v = new float[3];  
    memcpy(v, values, 3 * sizeof(float));
    avatarSize.set(
          addExposedField(MFFLOAT, "avatarSize", new MFFloat(v, 3), 
          new SFFloat(0.0f)));
    headlight.set(
          addExposedField(SFBOOL, "headlight", new SFBool(true)));
    speed.set(
          addExposedField(SFFLOAT, "speed", new SFFloat(1.0f), 
                          new SFFloat(0.0f)));
    transitionTime.set(
        addExposedField(SFTIME, "transitionTime", new SFTime(1.0)));
    setFieldFlags(transitionTime, FF_X3D_ONLY | FF_KAMBI_ONLY);
    static const char *transitionTypes[] = { "TELEPORT", "LINEAR", "ANIMATE", 
                                             NULL};
    StringArray *defaulttransitionTypes = new StringArray();
    defaulttransitionTypes->append("LINEAR");
    transitionType.set(
          addExposedField(MFSTRING, "transitionType", 
                          new MFString(defaulttransitionTypes), 0, 
                          transitionTypes));
    setFieldFlags(transitionType, FF_X3D_ONLY | FF_KAMBI_ONLY);

    static const char *types[] = { "ANY", "WALK", "EXAMINE", "FLY", "NONE", 
                                   NULL};
    StringArray *defaulttypes = new StringArray();
    defaulttypes->append("WALK");
    defaulttypes->append("ANY");
    type.set(
          addExposedField(MFSTRING, "type", new MFString(defaulttypes), 0, 
                          types));
    setFieldFlags(type, FF_VRML_ONLY);
    static const char *typesX3d[] = { "ANY", "WALK", "EXAMINE", "FLY", "NONE", 
                                   "LOOKAT",NULL};
    StringArray *defaulttypesX3d = new StringArray();
    defaulttypesX3d->append("WALK");
    defaulttypesX3d->append("EXAMINE");
    defaulttypesX3d->append("ANY");
    typeX3D.set(
          addExposedField(MFSTRING, "type", new MFString(defaulttypesX3d), 0, 
                          typesX3d));
    setFieldFlags(typeX3D, FF_X3D_ONLY);
    visibilityLimit.set(
          addExposedField(SFFLOAT, "visibilityLimit", new SFFloat(0.0f), 
                          new SFFloat(0.0f)));
    if (TheApp->getCoverMode()) {
        // non standard Covise/COVER extensions
        scale.set(
          addExposedField(SFFLOAT, "scale", new SFFloat(-1.0f)));
        setFieldFlags(scale, FF_COVER_ONLY);
    }

    blendingSort.set(
          addExposedField(SFSTRING, "blendingSort", new SFString("DEFAULT")));
    setFieldFlags(blendingSort, FF_KAMBI_ONLY);

    bind.set(
          addExposedField(SFBOOL, "bind", new SFBool(false)));
    setFieldFlags(bind, FF_X3DOM_ONLY);
    
    explorationMode.set(
          addExposedField(SFSTRING, "explorationMode", new SFString("all")));
    setFieldFlags(explorationMode, FF_X3DOM_ONLY);
    
    isActive.set(
          addExposedField(SFBOOL, "isActive", new SFBool(false)));
    setFieldFlags(isActive, FF_X3DOM_ONLY);
    
    typeParams.set(
          addExposedField(MFFLOAT, "typeParams", new MFFloat(8)));
    setFieldFlags(typeParams, FF_X3DOM_ONLY);
    
    addEventOut(SFTIME, "bindTime", FF_X3D_ONLY);
    addEventOut(SFBOOL, "isBound");
    addEventOut(SFBOOL, "transitionComplete", FF_X3D_ONLY);
}

ProtoNavigationInfo::ProtoNavigationInfo(Scene *scene, const char *name)
  : Proto(scene, name)
{
    addElements();
}
ProtoNavigationInfo::ProtoNavigationInfo(Scene *scene)
  : Proto(scene, "NavigationInfo")
{
    addElements();
}

Node *
ProtoNavigationInfo::create(Scene *scene)
{ 
    return new NodeNavigationInfo(scene, this); 
}

NodeNavigationInfo::NodeNavigationInfo(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeNavigationInfo::preDraw()
{
    m_scene->setHeadlight(headlight()->getValue());
}

int NodeNavigationInfo::getProfile(void) const
{ 
    if (!isDefault(type_Field()))
        if (type())
            for (int i = 0; i < type()->getSFSize(); i++) {        
                const char* typeStr = type()->getValue(i);
                if (strcmp(typeStr, "ANY") == 0)
                    return PROFILE_INTERACTIVE;
                if (strcmp(typeStr, "FLY") == 0)
                    return PROFILE_INTERACTIVE;
                if (strcmp(typeStr, "EXAMINE") == 0)
                    return PROFILE_INTERACTIVE;
                if (strcmp(typeStr, "LOOKAT") == 0)
                    return PROFILE_INTERACTIVE;
            }
    if (!isDefault(avatarSize_Field()))
        return PROFILE_IMMERSIVE;
    if (!isDefault(speed_Field()))
        return PROFILE_IMMERSIVE;
    if (!isDefault(visibilityLimit_Field()))
        return PROFILE_IMMERSIVE;
    return PROFILE_INTERCHANGE;
}

