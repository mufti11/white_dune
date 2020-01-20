/*
 * StaticGroupNode.h
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

#pragma once

#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "GroupNode.h"
#include "SFMFTypes.h"

class StaticGroupProto : public GroupProto {
public:
                    StaticGroupProto(Scene *scene);
                    StaticGroupProto(Scene *scene, const char *name, 
                                     int extraChrildrenNodeClass = 0);
    virtual Node   *create(Scene *scene);

    void            addElements(void);

    virtual int     getType() const { return X3D_STATIC_GROUP; }
    virtual int     getNodeClass() const { return GROUPING_NODE | 
                                                  CHILD_NODE |
                                                  BOUNDED_OBJECT_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex bboxCenter;
    FieldIndex bboxSize;
};

class StaticGroupNode : public GroupNode {
public:
                    StaticGroupNode(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const;
    virtual int         getComponentLevel(void) const;
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new StaticGroupNode(*this); }

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    fieldMacros(SFVec3f, bboxCenter, StaticGroupProto)
    fieldMacros(SFVec3f, bboxSize,   StaticGroupProto)
};

