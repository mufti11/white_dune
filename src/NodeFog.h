/*
 * NodeFog.h
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

#ifndef _NODE_FOG_H
#define _NODE_FOG_H

#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif
#include "FogMacros.h"

#include "swt.h"

#include "SFMFTypes.h"

class ProtoFog : public Proto {
public:
                    ProtoFog(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_FOG; }

    FieldIndex color;
    FieldIndex fogType;
    FieldIndex visibilityRange;
    FieldIndex alternative;
    FogProtoMacro()
};

class NodeFog : public Node {
public:
                    NodeFog(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_IMMERSIVE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeFog(*this); }
    virtual void    preDraw();
    virtual void    apply();

    virtual bool    hasNumbers4kids(void) { return true; } 

    virtual bool    hasKambiFields(void) { return true; }

    virtual int     getAnimationCommentID(void);

    fieldMacros(SFColor,  color,                     ProtoFog)
    fieldMacros(SFString, fogType,                   ProtoFog)
    fieldMacros(SFFloat,  visibilityRange,           ProtoFog)
    fieldMacros(SFNode,   alternative,               ProtoFog)
    FogFieldMacros(ProtoFog)
protected:
    float           m_matrix[16];
};

#endif // _NODE_FOG_H
