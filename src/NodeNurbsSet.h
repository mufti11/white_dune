/*
 * NodeNurbsSet.h
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

#ifndef _NODE_NURBS_SET_H
#define _NODE_NURBS_SET_H

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

class ProtoNurbsSet : public Proto {
public:
                    ProtoNurbsSet(Scene *scene);

    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_NURBS_SET; }
    virtual int     getNodeClass() const
                       { return PARAMETRIC_GEOMETRY_NODE | GEOMETRY_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex      geometry;
    FieldIndex      tessellationScale;
    FieldIndex      bboxCenter;
    FieldIndex      bboxSize;
};

class NodeNurbsSet : public Node {
public:
                    NodeNurbsSet(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const { return "NURBS"; }
    virtual int     getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeNurbsSet(*this); }

    virtual void    draw(void);

    virtual bool    avoidProtoOnPureVrml(void) { return true; }
    int             writeProto(int f);
    int             write(int filedes, int indent);

    void            setHandle(Node* caller, float newWeight,
                              const Vec3f &newV, const Vec3f &oldV);
    void            backupFieldsAppend(Node* caller, int field);

    virtual Node   *convert2X3d(void);
    virtual Node   *convert2Vrml(void);

    fieldMacros(MFNode,  geometry,          ProtoNurbsSet)
    fieldMacros(SFFloat, tessellationScale, ProtoNurbsSet)
    fieldMacros(SFVec3f, bboxCenter,        ProtoNurbsSet)
    fieldMacros(SFVec3f, bboxSize,          ProtoNurbsSet)
};


#endif // _NODE_NURBS_SET_H
