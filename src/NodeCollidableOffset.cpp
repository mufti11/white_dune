/*
 * NodeCollidableOffset.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2003 J. "MUFTI" Scheurich
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
#ifndef FLT_MAX
# include <float.h>
#endif
#include "stdafx.h"

#include "NodeCollidableOffset.h"
#include "Proto.h"
#include "Field.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFBool.h"
#include "DuneApp.h"
#include "Scene.h"

ProtoCollidableOffset::ProtoCollidableOffset(Scene *scene)
  : X3DNBodyCollidableProto(scene, "CollidableOffset")
{
    collidable.set(addField(SFNODE, "collidable", new SFNode(NULL), 
                   BODY_COLLIDABLE_NODE));
}

Node *
ProtoCollidableOffset::create(Scene *scene)
{ 
    return new NodeCollidableOffset(scene, this); 
}

NodeCollidableOffset::NodeCollidableOffset(Scene *scene, Proto *def)
  : X3DNBodyCollidableNode(scene, def)
{
}


void
NodeCollidableOffset::transformBranch()
{
    Node *ncollidable = collidable()->getValue();

    if (ncollidable != NULL) {
        glPushMatrix();
        transform();
        ncollidable->transformBranch();
        glPopMatrix();
    }
}

void
NodeCollidableOffset::preDraw()
{
    Node *ncollidable = collidable()->getValue();

    if (ncollidable != NULL) {
        glPushMatrix();
        transform();
        ncollidable->preDraw();
        glPopMatrix();
    }
}

void
NodeCollidableOffset::draw(int pass)
{
    Node *ncollidable = collidable()->getValue();

    if (ncollidable != NULL) {
        glPushMatrix();
        transform();

        ncollidable->bind();

        glPushName(collidable_Field());  // field
        glPushName(0);                 // index

        ncollidable->draw(pass);

        glPopName();
        glPopName();

        ncollidable->unbind();

        glPopMatrix();
    }
}

void
NodeCollidableOffset::flip(int index)
{
    Node *ncollidable = collidable()->getValue();

    if (ncollidable != NULL)
        ncollidable->flip(index);
}

void
NodeCollidableOffset::swap(int fromTo)
{
    Node *ncollidable = collidable()->getValue();

    if (ncollidable != NULL)
        ncollidable->swap(fromTo);
}

int         
NodeCollidableOffset::countPolygons(void)
{
    int ret = 0;    
    Node *ncollidable = collidable()->getValue();
    
    if (ncollidable != NULL)
        ret += ncollidable->countPolygons();
    return ret;
}

int         
NodeCollidableOffset::countPrimitives(void)
{
    int ret = 0;    
    Node *ncollidable = collidable()->getValue();
    
    if (ncollidable != NULL)
        ret += ncollidable->countPrimitives();
    return ret;
}

int         
NodeCollidableOffset::countPolygons1Sided(void)
{
    int ret = 0;    
    Node *ncollidable = collidable()->getValue();
    
    if (ncollidable != NULL)
        ret += ncollidable->countPolygons1Sided();
    return ret;
}

int         
NodeCollidableOffset::countPolygons2Sided(void)
{
    int ret = 0;    
    Node *ncollidable = collidable()->getValue();
    
    if (ncollidable != NULL)
        ret += ncollidable->countPolygons2Sided();
    return ret;
}

float   
NodeCollidableOffset::getTransparency(void)
{
    float ret = 0;    
    Node *ncollidable = collidable()->getValue();
    
    if (ncollidable != NULL)
        ret += ncollidable->countPolygons2Sided();
    return ret;
}

Node *
NodeCollidableOffset::convert2X3d(void)
{
    return NULL;
}

Node *
NodeCollidableOffset::convert2Vrml(void) 
{
    return NULL;
}

bool
NodeCollidableOffset::canWriteAc3d()
{
    Node *ncollidable = collidable()->getValue();
    if (ncollidable)
        return ncollidable->canWriteAc3d();
    return false;
}

bool
NodeCollidableOffset::canWriteCattGeo()
{
    Node *ncollidable = collidable()->getValue();
    if (ncollidable)
        return ncollidable->canWriteCattGeo();
    return false;
}


