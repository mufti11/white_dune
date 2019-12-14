/*
 * NodeSwitch.h
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

#ifndef _NODE_SWITCH_H
#define _NODE_SWITCH_H

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

class ProtoSwitch : public WonderlandExportProto {
public:
                    ProtoSwitch(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_SWITCH; }
    virtual int     getNodeClass() const { return GROUPING_NODE | 
                                                  CHILD_NODE |
                                                  BOUNDED_OBJECT_NODE; }

    virtual bool    isDeclaredInRwd_h() { return true; }      

    FieldIndex choice;
    FieldIndex whichChoice;
    FieldIndex bboxCenter;
    FieldIndex bboxSize;
    FieldIndex render;
};

class NodeSwitch : public Node {
public:
                    NodeSwitch(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERACTIVE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() { return new NodeSwitch( m_scene, m_proto); }

    virtual int     countPolygons(void)
                       { return choice()->countPolygons(); } 
    virtual int     countPrimitives(void) 
                       { return choice()->countPrimitives(); } 

    virtual int     countPolygons1Sided(void)
                       { return choice()->countPolygons1Sided(); } 
    virtual int     countPolygons2Sided(void)
                       { return choice()->countPolygons2Sided(); } 

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual void    preDraw();
    virtual void    draw(int pass);

    virtual int     writeAc3d(int filedes, int indent);
    virtual int     writeRib(int filedes, int indent);
    virtual int     writeCattGeo(int filedes, int indent);

    int accountWhich();

    virtual int     getChildrenField(void) { return choice_Field(); }

    fieldMacros(MFNode,  choice,      ProtoSwitch)
    fieldMacros(SFInt32, whichChoice, ProtoSwitch)
    fieldMacros(SFVec3f, bboxCenter,  ProtoSwitch)
    fieldMacros(SFVec3f, bboxSize,    ProtoSwitch)
    fieldMacros(SFBool,  render,      ProtoSwitch)
};

#endif // _NODE_SWITCH_H
