/*
 * NodeScreenGroup.cpp
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

#include "NodeScreenGroup.h"
#include "Scene.h"


ProtoScreenGroup::ProtoScreenGroup(Scene *scene)
  : ProtoStaticGroup(scene, "ScreenGroup")
{
}

Node *
ProtoScreenGroup::create(Scene *scene)
{ 
    return new NodeScreenGroup(scene, this); 
}

NodeScreenGroup::NodeScreenGroup(Scene *scene, Proto *def)
  : NodeStaticGroup(scene, def)
{
}

void    
NodeScreenGroup::draw(int pass) 
{ 
     float approx = (m_scene->getWidth() + m_scene->getHeight()) / 2.0f;

     float scale = 1;
     if (approx != 0)
         scale = 1.0f / approx;
     glPushMatrix();
     glScalef(scale, scale, scale);
     glScalef(2, 2, 2);
     children()->draw(pass, children_Field()); 
     glPopMatrix();
}
