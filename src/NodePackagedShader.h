/*
 * NodePackagedShader.h
 *
 * Copyright (C) 2009 J. "MUFTI" Scheurich
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

#ifndef _NODE_PACKAGED_SHADER_H
#define _NODE_PACKAGED_SHADER_H

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
#include "DynamicFieldsNode.h"

class ProtoPackagedShader : public Proto {
public:
                    ProtoPackagedShader(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_PACKAGED_SHADER; }
    virtual int     getNodeClass() const { return SHADER_NODE | URL_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex url;
    FieldIndex language;
};

class NodePackagedShader : public DynamicFieldsNode {
public:
                    NodePackagedShader(Scene *scene, Proto *proto);

    virtual bool        isProgrammableShaderObject(void) { return true; }

    virtual const char* getComponentName(void) const { return "Shaders"; }
    virtual int         getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 1; } 
    virtual Node   *copy() const { return new NodePackagedShader(*this); }

    virtual bool    showFields() { return true; }
    virtual void    update();

    fieldMacros(MFString, url,      ProtoPackagedShader);
    fieldMacros(SFString, language, ProtoPackagedShader);
};

#endif
