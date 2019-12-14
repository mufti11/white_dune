/*
 * NodeCoordinateDeformer.h
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

#ifndef _NODE_COORDINATE_DEFORMER_H
#define _NODE_COORDINATE_DEFORMER_H

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

class ProtoCoordinateDeformer : public Proto {
public:
                    ProtoCoordinateDeformer(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_COORDINATE_DEFORMER; }
    virtual int     getNodeClass() const { return CHILD_NODE; }

    FieldIndex children;
    FieldIndex controlPoint;
    FieldIndex inputCoord;
    FieldIndex inputTransform;
    FieldIndex outputCoord;
    FieldIndex weight;
    FieldIndex bboxCenter;
    FieldIndex bboxSize;
    FieldIndex uDimension;
    FieldIndex vDimension;
    FieldIndex wDimension;
    FieldIndex uKnot;
    FieldIndex vKnot;
    FieldIndex wKnot;
    FieldIndex uOrder;
    FieldIndex vOrder;
    FieldIndex wOrder;

};

class NodeCoordinateDeformer : public Node {
public:
                    NodeCoordinateDeformer(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return -1; }
    virtual Node   *copy() { return new NodeCoordinateDeformer(
                                    m_scene, m_proto); }

    virtual int     countPolygons(void)
                       { return children()->countPolygons(); } 
    virtual int     countPrimitives(void) 
                       { return children()->countPrimitives(); } 

    virtual int     countPolygons1Sided(void)
                       { return children()->countPolygons1Sided(); } 
    virtual int     countPolygons2Sided(void)
                       { return children()->countPolygons2Sided(); } 

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual bool    showFields() { return true; }

    virtual bool    avoidProtoOnPureVrml(void) { return true; }
    int             writeProto(int filedes);
    int             write(int filedes, int indent, bool avoidUse = false);

    fieldMacros(MFNode,  children,       ProtoCoordinateDeformer)
    fieldMacros(MFVec3f, controlPoint,   ProtoCoordinateDeformer)
    fieldMacros(MFNode,  inputCoord,     ProtoCoordinateDeformer)
    fieldMacros(MFNode,  inputTransform, ProtoCoordinateDeformer)
    fieldMacros(MFNode,  outputCoord,    ProtoCoordinateDeformer)
    fieldMacros(MFFloat, weight,         ProtoCoordinateDeformer)
    fieldMacros(SFVec3f, bboxCenter,     ProtoCoordinateDeformer)
    fieldMacros(SFVec3f, bboxSize,       ProtoCoordinateDeformer)
    fieldMacros(SFInt32, uDimension,     ProtoCoordinateDeformer)
    fieldMacros(SFInt32, vDimension,     ProtoCoordinateDeformer)
    fieldMacros(SFInt32, wDimension,     ProtoCoordinateDeformer)
    fieldMacros(MFFloat, uKnot,          ProtoCoordinateDeformer)
    fieldMacros(MFFloat, vKnot,          ProtoCoordinateDeformer)
    fieldMacros(MFFloat, wKnot,          ProtoCoordinateDeformer)
    fieldMacros(SFInt32, uOrder,         ProtoCoordinateDeformer)
    fieldMacros(SFInt32, vOrder,         ProtoCoordinateDeformer)
    fieldMacros(SFInt32, wOrder,         ProtoCoordinateDeformer)

};

#endif // _NODE_COORDINATE_DEFORMER_H
