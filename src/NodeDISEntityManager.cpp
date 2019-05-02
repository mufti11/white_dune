/*
 * NodeDISEntityManager.cpp
 *
 * Copyright (C) 2009 J. "MUFTI" Scheurich
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

#include "NodeDISEntityManager.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFString.h"
#include "SFInt32.h"
#include "MFNode.h"
#include "SFInt32.h"
#include "SFInt32.h"
#include "DuneApp.h"

ProtoDISEntityManager::ProtoDISEntityManager(Scene *scene)
  : Proto(scene, "DISEntityManager")
{
    address.set(
        addExposedField(SFSTRING, "address", new SFString("localhost")));
    applicationID.set(
        addExposedField(SFINT32, "applicationID", new SFInt32(1)));
    mapping.set(
        addExposedField(MFNODE, "mapping", new MFNode(), 
                        X3D_DIS_ENTITY_TYPE_MAPPING));
    port.set(
        addExposedField(SFINT32, "port", new SFInt32(0)));
    siteID.set(
        addExposedField(SFINT32, "siteID", new SFInt32(0)));
}

Node *
ProtoDISEntityManager::create(Scene *scene)
{ 
    return new NodeDISEntityManager(scene, this); 
}

NodeDISEntityManager::NodeDISEntityManager(Scene *scene, Proto *def)
  : Node(scene, def)
{
}
