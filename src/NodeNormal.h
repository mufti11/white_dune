/*
 * NodeNormal.h
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

#ifndef _NODE_NORMAL_H
#define _NODE_NORMAL_H

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

#include "SFMFTypes.h"

class ProtoNormal : public WonderlandExportProto {
public:
                    ProtoNormal(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_NORMAL; }
    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }

    virtual bool    isDeclaredInRwd_h() { return true; }   

    FieldIndex vector;
};

class NodeNormal : public Node {
public:
                    NodeNormal(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() { return new NodeNormal(m_scene, m_proto); }

    virtual bool    isInvalidChildNode(void) { return true; }

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual void    drawHandles(void);
    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    fieldMacros(MFVec3f, vector, ProtoNormal)
};

#endif // _NODE_NORMAL_H
