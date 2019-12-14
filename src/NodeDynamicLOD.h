/*
 * NodeDynamicLOD.h
 *
 * Copyright (C) 1999 Stephen F. White, 2018 J. "MUFTI" Scheurich
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

#ifndef _NODE_DYNAMIC_LOD_H
#define _NODE_DYNAMIC_LOD_H

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

#include "NodeLOD.h"

#include "SFMFTypes.h"

class ProtoDynamicLOD : public ProtoLOD {
public:
                    ProtoDynamicLOD(Scene *scene);
                    ProtoDynamicLOD(Scene *scene, const char *name);
    virtual Node   *create(Scene *scene);

    void            addElements(void);

    virtual int     getType() const { return X3DOM_DYNAMIC_LOD; }
    virtual int     getNodeClass() const { return GROUPING_NODE | 
                                                  CHILD_NODE |
                                                  BOUNDED_OBJECT_NODE; }

    FieldIndex root;
    FieldIndex size;
    FieldIndex subdivision;
    FieldIndex subScale;
    FieldIndex urlCenter;
    FieldIndex urlHead;
    FieldIndex urlTail;
};

class NodeDynamicLOD : public NodeLOD {
public:
                    NodeDynamicLOD(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return -1; }
    virtual Node   *copy() { return new NodeDynamicLOD(m_scene, m_proto); }

    virtual bool    showFields() { return true; }

    fieldMacros(SFVec3f, bboxSize,   ProtoDynamicLOD)
    fieldMacros(SFNode,  root,        ProtoDynamicLOD)
    fieldMacros(SFVec2f, size,        ProtoDynamicLOD)
    fieldMacros(SFVec2f, subdivision, ProtoDynamicLOD)
    fieldMacros(SFFloat, subScale,    ProtoDynamicLOD)
    fieldMacros(SFString, urlCenter,   ProtoDynamicLOD)
    fieldMacros(SFString, urlHead,     ProtoDynamicLOD)
    fieldMacros(SFString, urlTail,     ProtoDynamicLOD)
};

#endif
