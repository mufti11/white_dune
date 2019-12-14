/*
 * NodeCADPart.h
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

#include "ExternTheApp.h"
#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "SFMFTypes.h"
#include "NodeTransform.h"

class ProtoCADPart : public ProtoTransform {
public:
                    ProtoCADPart(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_CAD_PART; }
    virtual int     getNodeClass() const 
                       { return PRODUCT_STRUCTURE_CHILD_NODE | GROUPING_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    virtual bool    isDeclaredInRwd_h() { return false; }

    FieldIndex name;
    FieldIndex render;
};

class NodeCADPart : public NodeTransform {
public:
                    NodeCADPart(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_CAD_INTERCHANGE; }
    virtual const char* getComponentName(void) const { return "CADGeometry"; }
    virtual int         getComponentLevel(void) const { return 2; }
    virtual int     getX3dVersion(void) const { return 1; }
    virtual Node   *copy() { return new NodeCADPart(m_scene, m_proto); }

    fieldMacros(SFString, name, ProtoCADPart)
    fieldMacros(SFBool,   render, ProtoCADPart)
};
