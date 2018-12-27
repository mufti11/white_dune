/*
 * NodeText3D.cpp
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
#include "NodeText3D.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFVec3f.h"
#include "SFFloat.h"
#include "NodeShape.h"
#include "NodeAnchor.h"
#include "Scene.h"

ProtoText3D::ProtoText3D(Scene *scene)
  : ProtoText(scene, "Text3D")
{
    depth.set(
       addExposedField(SFFLOAT, "depth", new SFFloat(0.1f), new SFFloat(0.0f)));
    setFieldFlags(depth, FF_KAMBI_ONLY);

    addURLs(URL_KAMBI);
}

Node *
ProtoText3D::create(Scene *scene)
{ 
    return new NodeText3D(scene, this); 
}

NodeText3D::NodeText3D(Scene *scene, Proto *def)
  : NodeText(scene, def)
{
}

int
NodeText3D::writeProto(int f)
{
    if (m_scene->isX3dXml())
        return writeX3dProto(f);

    RET_ONERROR( mywritestr(f ,"EXTERNPROTO Text3D[\n") )    
    TheApp->incSelectionLinenumber();
    RET_ONERROR( writeProtoArguments(f) )
    RET_ONERROR( mywritestr(f ," ]\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ,"[\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ," \"urn:castle-engine.sourceforge.net:node:Text3D\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ," \"http://castle-engine.sourceforge.net/fallback_prototypes.wrl#Text3D\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ,"]\n") )
    TheApp->incSelectionLinenumber();
    return 0;
}

