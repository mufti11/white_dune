/*
 * NodeWonderlandImportJava.cpp
 *
 * Copyright (C) 2011 J. "MUFTI" Scheurich
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

#include "NodeWonderlandImportJava.h"

ProtoWonderlandImportJava::ProtoWonderlandImportJava(Scene *scene)
  : Proto(scene, "WonderlandImportJava")
{
    code.set(
         addField(MFSTRING, "code", new MFString()));

   addURLs(URL_EXPORT_CONTAINER);
}

Node *
ProtoWonderlandImportJava::create(Scene *scene)
{ 
    return new NodeWonderlandImportJava(scene, this); 
}

NodeWonderlandImportJava::NodeWonderlandImportJava(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

int
NodeWonderlandImportJava::writeProto(int f)
{
    return ((Node *)this)->writeProto(f, "", "exportContainers"
#ifdef HAVE_EXPORT_CONTAINER_PROTO_URL
                                            , HAVE_EXPORT_CONTAINER_PROTO_URL
#endif
                                            );
}

int
NodeWonderlandImportJava::writeCode(int f)
{
    for (int i = 0; i < code()->getSize(); i++) {
        MyString string = "";
        string += code()->getValue(i)->getValue();
        string.gsub("\\\"","\""); 
        RET_ONERROR( mywritestr(f, (const char *)string) )
        RET_ONERROR( mywritestr(f, "\n") )
    }
    return 0;
}
