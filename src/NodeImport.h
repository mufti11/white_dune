/*
 * NodeImport.h
 *
 * Fake node to store Information for IMPORT statements in X3D files
 *
 * Copyright (C) 1999 Stephen F. White, 2004 J. "MUFTI" Scheurich
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
#include "DynamicFieldsNode.h"
#include "SFMFTypes.h"

class ProtoImport : public Proto {
public:
                    ProtoImport(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_IMPORT; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex importedDEF;
    FieldIndex inlineDEF;
};

class NodeImport : public DynamicFieldsNode {
public:
                    NodeImport(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_CORE; }
    virtual int     getX3dVersion(void) const { return 0; }    
    virtual Node   *copy() { return new NodeImport(m_scene, m_proto); }

    virtual int     write(int filedes, int indent, bool avoidUse = false);
    virtual int     writeXml(int filedes, int indent, int containerField = -1, 
                             bool avoidUse = false);

    virtual void    update();

    fieldMacros(SFString, importedDEF, ProtoImport)
    fieldMacros(SFString, inlineDEF,   ProtoImport)
};

