/*
 * NodeNurbsGroup.h
 *
 * Copyright (C) 1999 Stephen F. White, 2003 J. "MUFTI" Scheurich
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

#ifndef _NODE_NURBSGROUP_H
#define _NODE_NURBSGROUP_H

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
#include "NodeGroup.h"

#include "SFMFTypes.h"

class ProtoNurbsGroup : public ProtoGroup {
public:
                    ProtoNurbsGroup(Scene *scene);

    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_NURBS_GROUP; }
    virtual int     getNodeClass() const 
       { return CHILD_NODE | GEOMETRY_NODE | PARAMETRIC_GEOMETRY_NODE; }

    FieldIndex      tessellationScale;
};

class NodeNurbsGroup : public NodeGroup {
public:
                    NodeNurbsGroup(Scene *scene, Proto *proto);

    virtual Node   *copy() { return new NodeNurbsGroup(m_scene, m_proto); }

    virtual void    draw(void);

    virtual bool    avoidProtoOnPureVrml(void) { return true; }
    virtual int     write(int filedes, int indent, bool avoidUse = false);

    virtual Node   *convert2X3d(void);
    virtual Node   *convert2Vrml(void);

    int             writeProto(int f);

    void            setHandle(Node* caller, float newWeight,
                              const Vec3f &newV, const Vec3f &oldV);
    void            backupFieldsAppend(Node* caller, int field);

    fieldMacros(SFFloat, tessellationScale, ProtoNurbsGroup)
};


#endif // _NODE_NURBSGROUP_H
