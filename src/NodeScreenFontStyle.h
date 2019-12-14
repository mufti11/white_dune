/*
 * NodeScreenFontStyle.h
 *
 * Copyright (C) 1999 Stephen F. White, 2019 J. "MUFTI" Scheurich
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

#ifndef _NODE_SCREEN_FONTSTYLE_H
#define _NODE_SCREEN_FONTSTYLE_H

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

#include "FontStyleNode.h"

#include "SFMFTypes.h"

class ProtoScreenFontStyle : public FontStyleProto {
public:
                    ProtoScreenFontStyle(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_SCREEN_FONT_STYLE; }

    virtual bool    isDeclaredInRwd_h() { return true; }      

    FieldIndex pointSize;
};

class NodeScreenFontStyle : public FontStyleNode {
public:
                    NodeScreenFontStyle(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_IMMERSIVE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() { return new NodeScreenFontStyle(m_scene, m_proto); }

    virtual bool    hasNumbers4kids(void) { return true; } 

    virtual float   getSizeX(void);
    virtual float   getSizeY(void);
    virtual bool    isScreenFontStyle(void) { return false; }

    void            setField(int index, FieldValue *value, int cf = -1);

    fieldMacros(SFFloat,  pointSize, ProtoScreenFontStyle)
};

#endif
