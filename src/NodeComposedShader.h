/*
 * NodeComposedShader.h
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

#pragma once

#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "SFMFTypes.h"
#include "DynamicFieldsNode.h"

class ProtoComposedShader : public Proto {
public:
                    ProtoComposedShader(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_COMPOSED_SHADER; }
    virtual int     getNodeClass() const { return SHADER_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex parts;
    FieldIndex language;
    FieldIndex effects;
};

class NodeComposedShader : public DynamicFieldsNode {
public:
                    NodeComposedShader(Scene *scene, Proto *proto);

    virtual bool        isProgrammableShaderObject(void) { return true; }

    virtual const char* getComponentName(void) const { return "Shaders"; }
    virtual int         getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 1; }
    virtual Node   *copy() const { return new NodeComposedShader(*this); }

    virtual bool    showFields() { return true; }
    virtual void    update();

    fieldMacros(MFNode,   parts,    ProtoComposedShader);
    fieldMacros(SFString, language, ProtoComposedShader);
    fieldMacros(MFNode,   effects,  ProtoComposedShader);
};

