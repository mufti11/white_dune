/*
 * NodeTUIMap.h
 *
 * Copyright (C) 1999 Stephen F. White, 2006 J. "MUFTI" Scheurich
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
 * 
 * Implements the Cover/Covise TUIMap node
 */

#ifndef _NODE_TUI_MAP_H
#define _NODE_TUI_MAP_H

#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"

#include "TUIElement.h"

class ProtoTUIMap : public ProtoTUIElement {
public:
                    ProtoTUIMap(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return COVER_TUI_MAP; }

    FieldIndex ox;
    FieldIndex oy;
    FieldIndex xSize;
    FieldIndex ySize;
    FieldIndex height;
    FieldIndex maps;
};

class NodeTUIMap : public TUIElement {
public:
                    NodeTUIMap(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return -1; }    
    virtual Node   *copy() const { return new NodeTUIMap(*this); }

    fieldMacros(MFFloat,  ox,     ProtoTUIMap)
    fieldMacros(MFFloat,  oy,     ProtoTUIMap)
    fieldMacros(MFFloat,  xSize,  ProtoTUIMap)
    fieldMacros(MFFloat,  ySize,  ProtoTUIMap)
    fieldMacros(MFFloat,  height, ProtoTUIMap)
    fieldMacros(MFString, maps,   ProtoTUIMap)
};

#endif // _NODE_TUI_MAP_H
