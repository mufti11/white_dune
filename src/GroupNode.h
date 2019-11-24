/*
 * GroupNode.h
 *
 * Copyright (C) 1999 Stephen F. White, 2008 J. "MUFTI" Scheurich
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

#ifndef _GROUP_NODE_H
#define _GROUP_NODE_H

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

class GroupProto : public WonderlandExportProto {
public:
                    GroupProto(Scene *scene, const char *name,
                               int extraChrildrenNodeClass = 0);

    void            addElements(int extraChrildrenNodeClass);

    FieldIndex children;
};

class GroupNode : public Node {
public:
                    GroupNode(Scene *scene, Proto *proto);

    virtual void    preDraw() { if (children()) children()->preDraw(); }
    virtual void    draw(int pass) 
                        { 
                        if (children())
                            children()->draw(pass, children_Field());
                        }

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

    virtual int     getChildrenField(void) const { return children_Field(); }

    fieldMacros(MFNode,  children,   GroupProto)
};

#endif
