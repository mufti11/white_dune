/*
 * NodeViewport.h
 *
 * Copyright (C) 2009 J. "MUFTI" Scheurich
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

#ifndef _NODE_VIEWPORT_H
#define _NODE_VIEWPORT_H

#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "NodeGroup.h"
#include "Scene.h"

#include "SFMFTypes.h"

class ProtoViewport : public ProtoGroup {
public:
                    ProtoViewport(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_VIEWPORT; }
    virtual int     getNodeClass() const { return GROUPING_NODE | 
                                                  BOUNDED_OBJECT_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex clipBoundary;
};

class NodeViewport : public NodeGroup {
public:
                    NodeViewport(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const { return "Layering"; }
    virtual int         getComponentLevel(void) const { return 1; }
    virtual Node   *copy() const { return new NodeViewport(*this); }

    virtual void    preDraw() 
                        { 
                        if (m_scene->getDrawViewports())
                            children()->preDraw(); 
                        }
    virtual void    draw(int pass) 
                        { 
                        if (m_scene->getDrawViewports())
                            children()->draw(pass, children_Field()); 
                        }

    virtual void    setField(int index, FieldValue *value, 
                             int containerField = -1);


    fieldMacros(MFFloat, clipBoundary, ProtoViewport);
};

#endif
