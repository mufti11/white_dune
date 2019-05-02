/*
 * NodeNurbsTrimmedSurface.h
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

#ifndef _NODE_NURBS_TRIMMED_SURFACE_H
#define _NODE_NURBS_TRIMMED_SURFACE_H

#ifndef _NODE_NURBS_SURFACE_H
#include "NodeNurbsSurface.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"

class ProtoNurbsTrimmedSurface : public ProtoNurbsSurface {
public:
                    ProtoNurbsTrimmedSurface(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_NURBS_TRIMMED_SURFACE; }
    virtual int     getNodeClass() const
                       { return PARAMETRIC_GEOMETRY_NODE | GEOMETRY_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex trimmingContour;

    virtual const MyString &getName(bool x3d) const 
       { return m_name; }
};

class NodeNurbsTrimmedSurface : public NodeNurbsSurface {
public:
                    NodeNurbsTrimmedSurface(Scene *scene, Proto *proto);
    virtual int     getComponentLevel(void) const { return 4; }
    // we need the double version of trimming NURBS
    virtual int     getX3dVersion(void) const { return 1; } 

    virtual Node   *copy() const { return new NodeNurbsTrimmedSurface(*this); }

    virtual bool    showFields() { return true; }

    virtual Node   *toTrimmedSurface(void);

    virtual void    draw();
    virtual void    createMesh(bool cleanDoubleVertices = true, 
                               bool triangulateMesh = true);

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual Node   *convert2Vrml(void);

    virtual void    update() { m_meshDirty = true; }

    void            drawOrCreateMesh(void);

    fieldMacros(MFNode, trimmingContour,   ProtoNurbsTrimmedSurface)

protected:
    GLUnurbsObj *m_theNurb; 
};

#endif
