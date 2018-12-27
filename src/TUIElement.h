/*
 * TUIElement.h
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
 * Implements the Cover/Covise TUIElement node
 */

#ifndef _TUI_ELEMENT_H
#define _TUI_ELEMENT_H

#ifndef _COVER_NODE_H
#include "CoverNode.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"

class ProtoTUIElement : public Proto {
public:
                    ProtoTUIElement(Scene *scene, const char* nodeName);
                    ProtoTUIElement(Scene *scene);
    void            addElements(void);

    virtual Node   *create(Scene *scene);

    FieldIndex elementName;
    FieldIndex parent;
    FieldIndex pos;

    virtual bool          isCoverProto(void) { return true; }
};

class TUIElement : public CoverNode {
public:
                    TUIElement(Scene *scene, Proto *proto);

    virtual int     getType() const { return -1; }
    virtual int     getX3dVersion(void) const { return -1; }    
    virtual Node   *copy() const { return new TUIElement(*this); }

    fieldMacros(SFString, elementName,     ProtoTUIElement)
    fieldMacros(SFString, parent,          ProtoTUIElement)
    fieldMacros(SFVec2f,  pos,             ProtoTUIElement)
};

#endif // _TUI_ELEMENT_H
