/*
 * NodeContour2D.h
 *
 * Copyright (C) 1999 Stephen F. White, 2004 J. "MUFTI" Scheurich
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

class ProtoContour2D : public Proto {
public:
                    ProtoContour2D(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_CONTOUR_2D; }

    FieldIndex children;
};

class NodeContour2D : public Node {
public:
                    NodeContour2D(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeContour2D(*this); }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual const char* getComponentName(void) const;
    virtual int         getComponentLevel(void) const;

    virtual bool    avoidProtoOnPureVrml(void) { return true; }

    virtual int     countPolygons(void)
                       { return children()->countPolygons(); } 
    virtual int     countPrimitives(void) 
                       { return children()->countPrimitives(); } 

    virtual int     countPolygons1Sided(void)
                       { return children()->countPolygons1Sided(); } 
    virtual int     countPolygons2Sided(void)
                       { return children()->countPolygons2Sided(); } 

    virtual void    flip(int index) { children()->flip(index); }
    virtual void    swap(int fromTo) { children()->swap(fromTo); }

    int             writeProto(int f);
    int             write(int filedes, int indent, bool avoidUse = false);
 
    virtual Node   *convert2X3d(void);
    virtual Node   *convert2Vrml(void);

    virtual void    setField(int index, FieldValue *value, int cf = -1);
    virtual void    update();
 
    fieldMacros(MFNode, children, ProtoContour2D)
};

