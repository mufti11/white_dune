/*
 * NodePopGeometry.h
 *
 * Copyright (C) 1999 Stephen F. White, 2018 J. "MUFTI" Scheurich
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
#include "DuneApp.h"
#include "GeometryNode.h"
#include "SFMFTypes.h"

class ProtoPopGeometry : public Proto {
public:
                    ProtoPopGeometry(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_POP_GEOMETRY; }

    virtual bool    isX3domProto(void) { return true; }

    x3domGeometryCommonFieldIndex()
    FieldIndex attributeStride;
    FieldIndex bbMaxModF;
    FieldIndex bbMin;
    FieldIndex bbMinModF;
    FieldIndex bbShiftVec;
    FieldIndex ccw;
    FieldIndex colorOffset;
    FieldIndex colorPrecision;
    FieldIndex colorType;
    FieldIndex coordType;
    FieldIndex indexedRendering;
    FieldIndex levels;
    FieldIndex maxBBSize;
    FieldIndex maxPrecisionLevel;
    FieldIndex minPrecisionLevel;
    FieldIndex normalOffset;
    FieldIndex normalPrecision;
    FieldIndex normalType;
    FieldIndex numAnchorVertices;
    FieldIndex originalVertexCount;
    FieldIndex position;
    FieldIndex positionOffset;
    FieldIndex positionPrecision;
    FieldIndex precisionFactor;
    FieldIndex primType;
    FieldIndex size;
    FieldIndex solid;
    FieldIndex sphericalNormals;
    FieldIndex texcoordOffset;
    FieldIndex texcoordPrecision;
    FieldIndex texCoordType;
    FieldIndex tightSize;
    FieldIndex vertexBufferSize;
    FieldIndex vertexCount;
};

class NodePopGeometry : public GeometryNode {
public:
                    NodePopGeometry(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodePopGeometry(*this); }

    virtual int     getX3dVersion(void) const { return -1; }

    virtual bool    showFields() { return true; }

    x3domGeometryCommonFieldMacros(ProtoPopGeometry) 
    fieldMacros(SFInt32, attributeStride, ProtoPopGeometry)
    fieldMacros(SFVec3f, bbMaxModF, ProtoPopGeometry)
    fieldMacros(SFVec3f, bbMin, ProtoPopGeometry)
    fieldMacros(SFVec3f, bbMinModF, ProtoPopGeometry)
    fieldMacros(SFVec3f, bbShiftVec, ProtoPopGeometry)
    fieldMacros(SFBool, ccw, ProtoPopGeometry)
    fieldMacros(SFInt32, colorOffset, ProtoPopGeometry)
    fieldMacros(SFInt32, colorPrecision, ProtoPopGeometry)
    fieldMacros(SFString, colorType, ProtoPopGeometry)
    fieldMacros(SFString, coordType, ProtoPopGeometry)
    fieldMacros(SFBool, indexedRendering, ProtoPopGeometry)
    fieldMacros(MFNode, levels, ProtoPopGeometry)
    fieldMacros(SFVec3f, maxBBSize, ProtoPopGeometry)
    fieldMacros(SFInt32, maxPrecisionLevel, ProtoPopGeometry)
    fieldMacros(SFInt32, minPrecisionLevel, ProtoPopGeometry)
    fieldMacros(SFInt32, normalOffset, ProtoPopGeometry)
    fieldMacros(SFInt32, normalPrecision, ProtoPopGeometry)
    fieldMacros(SFString, normalType, ProtoPopGeometry)
    fieldMacros(SFInt32, numAnchorVertices, ProtoPopGeometry)
    fieldMacros(MFInt32, originalVertexCount, ProtoPopGeometry)
    fieldMacros(SFVec3f, position, ProtoPopGeometry)
    fieldMacros(SFInt32, positionOffset, ProtoPopGeometry)
    fieldMacros(SFInt32, positionPrecision, ProtoPopGeometry)
    fieldMacros(SFFloat, precisionFactor, ProtoPopGeometry)
    fieldMacros(MFString, primType, ProtoPopGeometry)
    fieldMacros(SFVec3f, size, ProtoPopGeometry)
    fieldMacros(SFBool, solid, ProtoPopGeometry)
    fieldMacros(SFBool, sphericalNormals, ProtoPopGeometry)
    fieldMacros(SFInt32, texcoordOffset, ProtoPopGeometry)
    fieldMacros(SFInt32, texcoordPrecision, ProtoPopGeometry)
    fieldMacros(SFString, texCoordType, ProtoPopGeometry)
    fieldMacros(SFVec3f, tightSize, ProtoPopGeometry)
    fieldMacros(SFInt32, vertexBufferSize, ProtoPopGeometry)
    fieldMacros(MFInt32, vertexCount, ProtoPopGeometry)
};

