/*
 * FontStyleNode.h
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

#pragma once

#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "SFMFTypes.h"

class FontStyleProto : public WonderlandExportProto {
public:
                    FontStyleProto(Scene *scene, const char *name);

    virtual int     getNodeClass() const { return FONT_STYLE_NODE; }

    void            addElements();

    FieldIndex family;
    FieldIndex horizontal;
    FieldIndex justify;
    FieldIndex language;
    FieldIndex leftToRight;
    FieldIndex spacing;
    FieldIndex style;
    FieldIndex topToBottom;
    FieldIndex quality;
};

class FontStyleNode : public Node {
public:
                    FontStyleNode(Scene *scene, Proto *proto);

    virtual bool    hasNumbers4kids(void) { return true; } 

    virtual float   getSizeX(void) const = 0;
    virtual float   getSizeY(void) const = 0;
    virtual bool    isScreenFontStyle(void) { return false; }

    void            setField(int index, FieldValue *value, int cf = -1);

    fieldMacros(MFString, family,      FontStyleProto)
    fieldMacros(SFBool,   horizontal,  FontStyleProto)
    fieldMacros(MFString, justify,     FontStyleProto)
    fieldMacros(SFString, language,    FontStyleProto)
    fieldMacros(SFBool,   leftToRight, FontStyleProto)
    fieldMacros(SFFloat,  spacing,     FontStyleProto)
    fieldMacros(SFString, style,       FontStyleProto)
    fieldMacros(SFBool,   topToBottom, FontStyleProto)
    fieldMacros(SFFloat,  quality,     FontStyleProto)
};

