/*
 * NodeLOD.h
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

#ifndef _NODE_LOD_H
#define _NODE_LOD_H

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

class ProtoLOD : public WonderlandExportProto {
public:
                    ProtoLOD(Scene *scene);
                    ProtoLOD(Scene *scene, const char *name);
    void            addElements(void);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_LOD; }
    virtual int     getNodeClass() const 
                       { return CHILD_NODE | LOD_NODE | GROUPING_NODE; }

    FieldIndex level; // x3dname: children
    FieldIndex center;
    FieldIndex range;
    FieldIndex bboxCenter;
    FieldIndex bboxSize;
    FieldIndex forceTransitions;
    FieldIndex render;
};

class NodeLOD : public Node {
public:
                    NodeLOD(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_IMMERSIVE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() { return new NodeLOD(m_scene, m_proto); }

    virtual bool    hasNumbers4kids(void) { return true; } 

    virtual int     countPolygons(void) { return level()->countPolygons(); } 
    virtual int     countPrimitives(void) { return level()->countPrimitives(); }

    virtual int     countPolygons1Sided(void) 
                       { return level()->countPolygons1Sided(); }
    virtual int     countPolygons2Sided(void) 
                       { return level()->countPolygons2Sided(); }

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual void    preDraw();
    virtual void    draw(int pass);

    void accountNodeToDrawIndex();

    virtual int     getChildrenField(void) { return level_Field(); }

    fieldMacros(MFNode,  level,            ProtoLOD)
    fieldMacros(SFVec3f, center,           ProtoLOD)
    fieldMacros(MFFloat, range,            ProtoLOD)
    fieldMacros(SFVec3f, bboxCenter,       ProtoLOD)
    fieldMacros(SFVec3f, bboxSize,         ProtoLOD)
    fieldMacros(SFBool,  forceTransitions, ProtoLOD)
    fieldMacros(SFBool,  render,           ProtoLOD)

protected:
    int m_nodeToDrawIndex;
};

#endif // _NODE_LOD_H
