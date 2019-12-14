/*
 * NodeTUISlider.h
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
 * Implements the Cover/Covise TUISlider node
 */

#ifndef _NODE_TUI_SLIDER_H
#define _NODE_TUI_SLIDER_H

#ifndef _NODE_H
#include "ExternTheApp.h"
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

#ifdef min
# undef min
#endif

#ifdef max 
# undef max
#endif

class ProtoTUISlider : public ProtoTUIElement {
public:
                    ProtoTUISlider(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return COVER_TUI_SLIDER; }

    FieldIndex min;
    FieldIndex max;
    FieldIndex value;
};

class NodeTUISlider : public TUIElement {
public:
                    NodeTUISlider(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return -1; }    
    virtual Node   *copy() { return new NodeTUISlider(m_scene, m_proto); }

    fieldMacros(SFInt32, min,   ProtoTUISlider)
    fieldMacros(SFInt32, max,   ProtoTUISlider)
    fieldMacros(SFInt32, value, ProtoTUISlider)
};

#endif // _NODE_TUI_SLIDER_H
