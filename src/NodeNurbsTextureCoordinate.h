/*
 * NodeNurbsTextureCoordinate.h
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

#ifndef _NODE_NURBS_TEXTURE_COORDINATE_H
#define _NODE_NURBS_TEXTURE_COORDINATE_H

#ifndef NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif
#ifndef _VEC3F
# include "Vec3f.h"
#endif
#ifndef _SFMFTYPES_H
# include "SFMFTypes.h"
#endif

class NodeNurbsSurface;

class ProtoNurbsTextureCoordinate : public Proto {
public:
                    ProtoNurbsTextureCoordinate(Scene *scene);
                    ProtoNurbsTextureCoordinate(Scene *scene, const char *name);
    void            addElements(void);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_NURBS_TEXTURE_COORDINATE; }
    virtual int     getNodeClass() const 
                       { return NURBS_TEXTURE_COORDINATE_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex uDimension;
    FieldIndex vDimension;
    FieldIndex uKnot;
    FieldIndex vKnot;
    FieldIndex uOrder;
    FieldIndex vOrder;
    FieldIndex controlPoint;
    FieldIndex weight;
};

class NodeNurbsTextureCoordinate : public Node {
public:
                    NodeNurbsTextureCoordinate(Scene *scene, Proto *proto);
protected:
    virtual        ~NodeNurbsTextureCoordinate();

public:
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeNurbsTextureCoordinate(*this); }

    virtual bool    maySetDefault(void) { return false; }

    MFVec2f        *getPoint();
    virtual void    setField(int index, FieldValue *value, int cf = -1);
    void            createNurbsData(void);

    fieldMacros(SFInt32, uDimension,    ProtoNurbsTextureCoordinate)
    fieldMacros(SFInt32, vDimension,    ProtoNurbsTextureCoordinate)
    fieldMacros(MFFloat, uKnot,         ProtoNurbsTextureCoordinate)
    fieldMacros(MFFloat, vKnot,         ProtoNurbsTextureCoordinate)
    fieldMacros(SFInt32, uOrder,        ProtoNurbsTextureCoordinate)
    fieldMacros(SFInt32, vOrder,        ProtoNurbsTextureCoordinate)
    fieldMacros(MFVec2f, controlPoint,  ProtoNurbsTextureCoordinate)
    fieldMacros(MFFloat, weight,        ProtoNurbsTextureCoordinate)

protected:
    NodeNurbsSurface *m_nurbs;
    MFVec2f *m_points;
    bool m_dirty;
};

#endif

