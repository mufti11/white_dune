/*
 * NodeKambiAppearance.cpp
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

#include "NodeKambiAppearance.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFFloat.h"
#include "SFNode.h"
#include "Node.h"
#include "NodeMaterial.h"
#include "Util.h"

ProtoKambiAppearance::ProtoKambiAppearance(Scene *scene)
  : ProtoAppearance(scene, "KambiAppearance")
{
    addURLs(URL_KAMBI);
}

Node *
ProtoKambiAppearance::create(Scene *scene)
{ 
    return new NodeKambiAppearance(scene, this); 
}

NodeKambiAppearance::NodeKambiAppearance(Scene *scene, Proto *def)
  : NodeAppearance(scene, def)
{
}
