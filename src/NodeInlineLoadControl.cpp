/*
 * NodeInlineLoadControl.cpp
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

#include "NodeInlineLoadControl.h"
#include "Proto.h"
#include "Field.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFBool.h"
#include "DuneApp.h"
#include "Scene.h"

ProtoInlineLoadControl::ProtoInlineLoadControl(Scene *scene)
  : ProtoInline(scene, "InlineLoadControl")
{
    load.set(
        addExposedField(SFBOOL, "load", new SFBool(true), 
                        new SFBool(true)));
    setFieldFlags(load, FF_VRML_ONLY);

    children.set(addEventOut(MFNODE, "children"));

    addURLs(URL_VRML97_AMENDMENT1);
}

Node *
ProtoInlineLoadControl::create(Scene *scene)
{ 
    return new NodeInlineLoadControl(scene, this); 
}

NodeInlineLoadControl::NodeInlineLoadControl(Scene *scene, Proto *def)
  : NodeInline(scene, def)
{
}

int
NodeInlineLoadControl::writeProto(int f)
{
    if (m_scene->isX3dXml())
        return writeX3dProto(f);

    RET_ONERROR( mywritestr(f, "EXTERNPROTO InlineLoadControl[\n") )    
    TheApp->incSelectionLinenumber();
    RET_ONERROR( writeProtoArguments(f) )
    RET_ONERROR( mywritestr(f, " ]\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "[\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " \"urn:web3d:vrml97:node:InlineLoadControl\",\n") )
    TheApp->incSelectionLinenumber();
#ifdef HAVE_VRML97_AMENDMENT1_PROTO_URL
    RET_ONERROR( mywritestr(f, " \"") )
    RET_ONERROR( mywritestr(f, HAVE_VRML97_AMENDMENT1_PROTO_URL) )
    RET_ONERROR( mywritestr(f, "/InlineLoadControlPROTO.wrl") )
    RET_ONERROR( mywritestr(f, "\"\n") )
    TheApp->incSelectionLinenumber();
#else
    RET_ONERROR( mywritestr(f, " \"InlineLoadControlPROTO.wrl\",\n") )
    TheApp->incSelectionLinenumber();
#endif
    RET_ONERROR( mywritestr(f, " \"http://wdune.ourproject.org/docs/vrml97Amendment1/InlineLoadControlPROTO.wrl\"\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "]\n") )
    TheApp->incSelectionLinenumber();
    return 0;
}

   
int             
NodeInlineLoadControl::write(int filedes, int indent, bool avoidUse) 
{
    if (m_scene->isPureVRML())
        RET_ONERROR( NodeInline::write(filedes, indent, avoidUse) )
    else
        RET_ONERROR( Node::write(filedes, indent, avoidUse) )
    return 0;
}

void
NodeInlineLoadControl::draw(int pass)
{
    if (load()->getValue())
        NodeInline::draw(pass);
}

void
NodeInlineLoadControl::preDraw()
{
    if (load()->getValue())
        NodeInline::preDraw();
}

