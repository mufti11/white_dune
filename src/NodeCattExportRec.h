/*
 * NodeCattExportRec.h
 *
 * Copyright (C) 2007 J. "MUFTI" Scheurich
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

class ProtoCattExportRec : public Proto {
public:
                    ProtoCattExportRec(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return DUNE_CATT_EXPORT_REC; }
    virtual int     getNodeClass() const { return CHILD_NODE; }

    virtual bool    isScriptedExternProto(void) { return true; }

    FieldIndex id;
    FieldIndex receiverLocation;
    FieldIndex furtherParameters;
};

class NodeCattExportRec : public Node {
public:
                    NodeCattExportRec(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeCattExportRec(*this); }
    
    virtual int     writeProto(int filedes);

    virtual void    preDraw();
    virtual void    draw(int pass);

    virtual void    drawHandles(void);
    virtual Vec3f   getHandle(int handle, int *constraint, int *field);
    virtual void    setHandle(int handle, const Vec3f &v);

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual bool    canWriteCattGeo() { return true; }
    virtual int     writeCattGeo(int filedes, int indent);

    fieldMacros(SFInt32,  id,                ProtoCattExportRec)
    fieldMacros(SFVec3f,  receiverLocation,  ProtoCattExportRec)
    fieldMacros(SFString, furtherParameters, ProtoCattExportRec)
};
