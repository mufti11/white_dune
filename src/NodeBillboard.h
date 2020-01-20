/*
 * NodeBillboard.h
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

#pragma once

#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "SFMFTypes.h"

class ProtoBillboard : public WonderlandExportProto {
public:
                    ProtoBillboard(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_BILLBOARD; }
    virtual int     getNodeClass() const { return GROUPING_NODE | CHILD_NODE; }

    FieldIndex axisOfRotation;
    FieldIndex children;
    FieldIndex bboxCenter;
    FieldIndex bboxSize;
    FieldIndex render;
};

class NodeBillboard : public Node {
public:
                    NodeBillboard(Scene *scene, Proto *proto);
    virtual int     getProfile(void) const { return PROFILE_IMMERSIVE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeBillboard(*this); }

    virtual void    transform();

    virtual void    preDraw();
    virtual void    draw(int pass);

    virtual bool    hasNumbers4kids(void) { return true; } 

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual int     countPolygons(void)
                       { return children()->countPolygons(); } 
    virtual int     countPrimitives(void) 
                       { return children()->countPrimitives(); } 

    virtual int     countPolygons1Sided(void)
                       { return children()->countPolygons1Sided(); } 
    virtual int     countPolygons2Sided(void)
                       { return children()->countPolygons2Sided(); } 

    virtual void    writeCWarning(void);

    virtual int     getChildrenField(void) const { return children_Field(); }

    fieldMacros(SFVec3f, axisOfRotation, ProtoBillboard)
    fieldMacros(MFNode,  children,       ProtoBillboard)
    fieldMacros(SFVec3f, bboxCenter,     ProtoBillboard)
    fieldMacros(SFVec3f, bboxSize,       ProtoBillboard)
    fieldMacros(SFBool,   render,        ProtoBillboard)
};
