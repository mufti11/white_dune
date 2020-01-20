/*
 * NodeTrimmedSurface.h
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

#include "GeometryNode.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "SFMFTypes.h"

class ProtoTrimmedSurface : public GeometryProto {
public:
                    ProtoTrimmedSurface(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_TRIMMED_SURFACE; }
    virtual int     getNodeClass() const 
                       { return PARAMETRIC_GEOMETRY_NODE | GEOMETRY_NODE; }

    FieldIndex trimmingContour;
    FieldIndex surface;
};

class NodeTrimmedSurface : public GeometryNode {
public:
                    NodeTrimmedSurface(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return -1; }    
    virtual Node   *copy() const { return new NodeTrimmedSurface(*this); }

    virtual void    draw();

    virtual bool    hasBoundingBox(void) { return true; }
    virtual Vec3f   getMinBoundingBox(void);
    virtual Vec3f   getMaxBoundingBox(void);

    virtual void    update(void);
    virtual void    reInit(void);

    virtual int     countPolygons(void);
    virtual int     countPrimitives(void);

    virtual int     countPolygons1Sided(void);
    virtual int     countPolygons2Sided(void);

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual bool    showFields() { return true; }

    virtual bool    avoidProtoOnPureVrml(void) { return true; }
    virtual int     writeProto(int f);
    virtual int     write(int filedes, int indent, bool avoidUse = false);

    virtual Node   *convert2X3d(void);
 
    fieldMacros(MFNode, trimmingContour,   ProtoTrimmedSurface)
    fieldMacros(SFNode, surface,           ProtoTrimmedSurface)
};

