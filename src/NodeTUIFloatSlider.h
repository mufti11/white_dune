/*
 * NodeTUIFloatSlider.h
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
 * Implements the Cover/Covise TUIFloatSlider node
 */

#ifndef _NODE_TUI_FLOAT_SLIDER_H
#define _NODE_TUI_FLOAT_SLIDER_H

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

#ifdef min
# undef min
#endif

#ifdef max
# undef max
#endif

class ProtoTUIFloatSlider : public ProtoTUIElement {
public:
                    ProtoTUIFloatSlider(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return COVER_TUI_FLOAT_SLIDER; }

    FieldIndex min;
    FieldIndex max;
    FieldIndex value;
};

class NodeTUIFloatSlider : public TUIElement {
public:
                    NodeTUIFloatSlider(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return -1; }    
    virtual Node   *copy() const { return new NodeTUIFloatSlider(*this); }

    fieldMacros(SFFloat, min,   ProtoTUIFloatSlider)
    fieldMacros(SFFloat, max,   ProtoTUIFloatSlider)
    fieldMacros(SFFloat, value, ProtoTUIFloatSlider)
};

#endif // _NODE_TUI_FLOAT_SLIDER_H
