/*
 * NodeVrmlScene.h
 *
 * Copyright (C) 1999 Stephen F. White, 2006 J. "MUFTI" Scheurich
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

class ProtoVrmlScene : public Proto {
public:
                    ProtoVrmlScene(Scene *scene);
    virtual Node   *create(Scene *scene);

    FieldIndex children;

    // not fields but eventIn/eventOut
    FieldIndex timeIn;
    FieldIndex timeOut;
    FieldIndex bindOut;

    virtual int     getType() const { return DUNE_VRML_SCENE; }
    virtual int     getNodeClass() const { return CHILD_NODE; }

    virtual bool    isScriptedProto(void) { return true; }
};

class NodeVrmlScene : public Node {
public:
                    NodeVrmlScene(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual int     getX3dVersion(void) const { return -1; }    
    virtual Node   *copy() const { return new NodeVrmlScene(*this); }
    
    virtual bool    hasNumbers4kids(void) { return true; } 

    virtual int     writeProto(int filedes);
    virtual int     writeXmlProto(int filedes);

    virtual void    preDraw() { children()->preDraw(); }
    virtual void    draw(int pass) { children()->draw(pass, children_Field()); }

    virtual void    flip(int index) { children()->flip(index); }    
    virtual void    swap(int fromTo) { children()->swap(fromTo); }

    virtual int     countPolygons(void)
                       { return children()->countPolygons(); } 
    virtual int     countPrimitives(void) 
                       { return children()->countPrimitives(); } 

    virtual int     countPolygons1Sided(void)
                       { return children()->countPolygons1Sided(); } 
    virtual int     countPolygons2Sided(void)
                       { return children()->countPolygons2Sided(); } 

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual void    onAddRoute(Node *s, int out, Node *d, int in);

    fieldMacros(MFNode, children, ProtoVrmlScene)
    fieldMacros(SFTime, timeIn,  ProtoVrmlScene)
    fieldMacros(SFTime, timeOut,  ProtoVrmlScene)
    fieldMacros(SFBool, bindOut,  ProtoVrmlScene)
};

