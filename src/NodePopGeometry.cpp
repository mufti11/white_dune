/*
 * NodePopGeometry.cpp
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

#include "NodePopGeometry.h"
#include "Scene.h"

ProtoPopGeometry::ProtoPopGeometry(Scene *scene)
  : Proto(scene, "PopGeometry")
{
    x3domGeometryCommonFields()

    attributeStride.set(
          addExposedField(SFINT32, "attributeStride", new SFInt32(0)));
    setFieldFlags(attributeStride, FF_X3DOM_ONLY);

    bbMaxModF.set(
          addExposedField(SFVEC3F, "bbMaxModF", new SFVec3f(1.0f, 1.0f, 1.0f)));
    setFieldFlags(bbMaxModF, FF_X3DOM_ONLY);

    bbMin.set(
          addExposedField(SFVEC3F, "bbMin", new SFVec3f(0.0f, 0.0f, 0.0f)));
    setFieldFlags(bbMin, FF_X3DOM_ONLY);

    bbMinModF.set(
          addExposedField(SFVEC3F, "bbMinModF", new SFVec3f(0.0f, 0.0f, 0.0f)));
    setFieldFlags(bbMinModF, FF_X3DOM_ONLY);

    bbShiftVec.set(
          addExposedField(SFVEC3F, "bbShiftVec", new SFVec3f(0.0f, 0.0f, 0.0f)));
    setFieldFlags(bbShiftVec, FF_X3DOM_ONLY);

    ccw.set(
          addExposedField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_X3DOM_ONLY);

    colorOffset.set(
          addExposedField(SFINT32, "colorOffset", new SFInt32(0)));
    setFieldFlags(colorOffset, FF_X3DOM_ONLY);

    colorPrecision.set(
          addExposedField(SFINT32, "colorPrecision", new SFInt32(1)));
    setFieldFlags(colorPrecision, FF_X3DOM_ONLY);

    colorType.set(
          addExposedField(SFSTRING, "colorType", new SFString("Uint8")));
    setFieldFlags(colorType, FF_X3DOM_ONLY);

    coordType.set(
          addExposedField(SFSTRING, "coordType", new SFString("Uint16")));
    setFieldFlags(coordType, FF_X3DOM_ONLY);

    indexedRendering.set(
          addExposedField(SFBOOL, "indexedRendering", new SFBool(true)));
    setFieldFlags(indexedRendering, FF_X3DOM_ONLY);

    levels.set(
          addExposedField(MFNODE, "levels", new MFNode(), X3DOM_POP_GEOMETRY_LEVEL));
    setFieldFlags(levels, FF_X3DOM_ONLY);

    maxBBSize.set(
          addExposedField(SFVEC3F, "maxBBSize", new SFVec3f(1.0f, 1.0f, 1.0f)));
    setFieldFlags(maxBBSize, FF_X3DOM_ONLY);

    maxPrecisionLevel.set(
          addExposedField(SFINT32, "maxPrecisionLevel", new SFInt32(-1)));
    setFieldFlags(maxPrecisionLevel, FF_X3DOM_ONLY);

    minPrecisionLevel.set(
          addExposedField(SFINT32, "minPrecisionLevel", new SFInt32(-1)));
    setFieldFlags(minPrecisionLevel, FF_X3DOM_ONLY);

    normalOffset.set(
          addExposedField(SFINT32, "normalOffset", new SFInt32(0)));
    setFieldFlags(normalOffset, FF_X3DOM_ONLY);

    normalPrecision.set(
          addExposedField(SFINT32, "normalPrecision", new SFInt32(0)));
    setFieldFlags(normalPrecision, FF_X3DOM_ONLY);

    normalType.set(
          addExposedField(SFSTRING, "normalType", new SFString("Uint8")));
    setFieldFlags(normalType, FF_X3DOM_ONLY);

    numAnchorVertices.set(
          addExposedField(SFINT32, "numAnchorVertices", new SFInt32(0)));
    setFieldFlags(numAnchorVertices, FF_X3DOM_ONLY);

    originalVertexCount.set(
          addExposedField(MFINT32, "originalVertexCount", new MFInt32(0)));
    setFieldFlags(originalVertexCount, FF_X3DOM_ONLY);

    position.set(
          addExposedField(SFVEC3F, "position", new SFVec3f(0, 0, 0)));
    setFieldFlags(position, FF_X3DOM_ONLY);

    positionOffset.set(
          addExposedField(SFINT32, "positionOffset", new SFInt32(0)));
    setFieldFlags(positionOffset, FF_X3DOM_ONLY);

    positionPrecision.set(
          addExposedField(SFINT32, "positionPrecision", new SFInt32(2)));
    setFieldFlags(positionPrecision, FF_X3DOM_ONLY);

    precisionFactor.set(
          addExposedField(SFFLOAT, "precisionFactor", new SFFloat(0)));
    setFieldFlags(precisionFactor, FF_X3DOM_ONLY);

    primType.set(
          addExposedField(MFSTRING, "primType", new MFString("TRIANGLES")));
    setFieldFlags(primType, FF_X3DOM_ONLY);

    size.set(
          addExposedField(SFVEC3F, "size", new SFVec3f(1, 1, 1)));
    setFieldFlags(size, FF_X3DOM_ONLY);

    solid.set(
          addExposedField(SFBOOL, "solid", new SFBool(true)));
    setFieldFlags(solid, FF_X3DOM_ONLY);

    sphericalNormals.set(
          addExposedField(SFBOOL, "sphericalNormals", new SFBool(false)));
    setFieldFlags(sphericalNormals, FF_X3DOM_ONLY);

    texcoordOffset.set(
          addExposedField(SFINT32, "texcoordOffset", new SFInt32(0)));
    setFieldFlags(texcoordOffset, FF_X3DOM_ONLY);

    texcoordPrecision.set(
          addExposedField(SFINT32, "texcoordPrecision", new SFInt32(2)));
    setFieldFlags(texcoordPrecision, FF_X3DOM_ONLY);

    texCoordType.set(
          addExposedField(SFSTRING, "texCoordType", new SFString("Uint16")));
    setFieldFlags(texCoordType, FF_X3DOM_ONLY);

    tightSize.set(
          addExposedField(SFVEC3F, "tightSize", new SFVec3f(1.0f, 1.0f, 1.0f)));
    setFieldFlags(tightSize, FF_X3DOM_ONLY);

    vertexBufferSize.set(
          addExposedField(SFINT32, "vertexBufferSize", new SFInt32(0)));
    setFieldFlags(vertexBufferSize, FF_X3DOM_ONLY);

    vertexCount.set(
          addExposedField(MFINT32, "vertexCount", new MFInt32(0)));
    setFieldFlags(vertexCount, FF_X3DOM_ONLY);
}

Node *
ProtoPopGeometry::create(Scene *scene)
{ 
    return new NodePopGeometry(scene, this); 
}

NodePopGeometry::NodePopGeometry(Scene *scene, Proto *def)
  : GeometryNode(scene, def)
{
}
