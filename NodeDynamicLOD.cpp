/*
 * NodeDynamicLOD.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2008 J. "MUFTI" Scheurich
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

#include "stdafx.h"

#include "swt.h"
#include "NodeDynamicLOD.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFVec3f.h"
#include "NodeShape.h"
#include "Scene.h"

ProtoDynamicLOD::ProtoDynamicLOD(Scene *scene, const char *name)
  : ProtoLOD(scene, name)
{
     addElements(); 
}

ProtoDynamicLOD::ProtoDynamicLOD(Scene *scene)
  : ProtoLOD(scene, "DynamicLOD")
{                    
     addElements();     
}

void ProtoDynamicLOD::addElements(void) 
{
    root.set(
        addExposedField(SFNODE, "root", new SFNode(NULL), VRML_SHAPE));
    setFieldFlags(root, FF_X3DOM_ONLY);

    size.set(
        addExposedField(SFVEC2F, "size", new SFVec2f(2, 2)));
    setFieldFlags(size, FF_X3DOM_ONLY);

    subdivision.set(
        addExposedField(SFVEC2F, "subdivision", new SFVec2f(1, 1)));
    setFieldFlags(subdivision, FF_X3DOM_ONLY);

    subScale.set(
        addExposedField(SFFLOAT, "subScale", new SFFloat(5)));
    setFieldFlags(subScale, FF_X3DOM_ONLY);

    urlCenter.set(
        addExposedField(SFSTRING, "urlCenter", new SFString("")));
    setFieldFlags(urlCenter, FF_X3DOM_ONLY);

    urlHead.set(
        addExposedField(SFSTRING, "urlHead", new SFString("")));
    setFieldFlags(urlHead, FF_X3DOM_ONLY);

    urlTail.set(
        addExposedField(SFSTRING, "urlTail", new SFString("")));
    setFieldFlags(urlTail, FF_X3DOM_ONLY);
}

Node *
ProtoDynamicLOD::create(Scene *scene)
{ 
    return new NodeDynamicLOD(scene, this); 
}

NodeDynamicLOD::NodeDynamicLOD(Scene *scene, Proto *def)
  : NodeLOD(scene, def)
{
}

