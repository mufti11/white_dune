/*
 * NodeFontStyle.h
 *
 * Copyright (C) 1999 Stephen F. White
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

#ifndef _NODE_FONTSTYLE_H
#define _NODE_FONTSTYLE_H

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

class ProtoFontStyle : public WonderlandExportProto {
public:
                    ProtoFontStyle(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_FONT_STYLE; }
    virtual int     getNodeClass() const { return FONT_STYLE_NODE; }

    virtual bool    isDeclaredInRwd_h() { return true; }      

    FieldIndex family;
    FieldIndex horizontal;
    FieldIndex justify;
    FieldIndex language;
    FieldIndex leftToRight;
    FieldIndex size;
    FieldIndex spacing;
    FieldIndex style;
    FieldIndex topToBottom;
    FieldIndex quality;
};

class NodeFontStyle : public Node {
public:
                    NodeFontStyle(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_IMMERSIVE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeFontStyle(*this); }

    virtual bool    hasNumbers4kids(void) { return true; } 

    void            setField(int index, FieldValue *value, int cf = -1);

    fieldMacros(MFString, family,      ProtoFontStyle)
    fieldMacros(SFBool,   horizontal,  ProtoFontStyle)
    fieldMacros(MFString, justify,     ProtoFontStyle)
    fieldMacros(SFString, language,    ProtoFontStyle)
    fieldMacros(SFBool,   leftToRight, ProtoFontStyle)
    fieldMacros(SFFloat,  size,        ProtoFontStyle)
    fieldMacros(SFFloat,  spacing,     ProtoFontStyle)
    fieldMacros(SFString, style,       ProtoFontStyle)
    fieldMacros(SFBool,   topToBottom, ProtoFontStyle)
    fieldMacros(SFFloat,  quality,     ProtoFontStyle)
};

#endif // _NODE_FONTSTYLE_H
