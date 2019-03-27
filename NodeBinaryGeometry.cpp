/*
 * NodeBinaryGeometry.cpp
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

#include "NodeBinaryGeometry.h"
#include "Scene.h"

ProtoBinaryGeometry::ProtoBinaryGeometry(Scene *scene)
  : Proto(scene, "BinaryGeometry")
{
    x3domGeometryCommonFields()

    binormal.set(
          addExposedField(SFSTRING, "binormal", new SFString("")));
    setFieldFlags(binormal, FF_X3DOM_ONLY);

    binormalType.set(
          addExposedField(SFSTRING, "binormalType", new SFString("Float32")));
    setFieldFlags(binormalType, FF_X3DOM_ONLY);

    ccw.set(
          addExposedField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_X3DOM_ONLY);

    color.set(
          addExposedField(SFSTRING, "color", new SFString("")));
    setFieldFlags(color, FF_X3DOM_ONLY);

    colorType.set(
          addExposedField(SFSTRING, "colorType", new SFString("Float32")));
    setFieldFlags(colorType, FF_X3DOM_ONLY);

    compressed.set(
          addExposedField(SFBOOL, "compressed", new SFBool(false)));
    setFieldFlags(compressed, FF_X3DOM_ONLY);

    coord.set(
          addExposedField(SFSTRING, "coord", new SFString("")));
    setFieldFlags(coord, FF_X3DOM_ONLY);

    coordType.set(
          addExposedField(SFSTRING, "coordType", new SFString("Float32")));
    setFieldFlags(coordType, FF_X3DOM_ONLY);

    idsPerVertex.set(
          addExposedField(SFBOOL, "idsPerVertex", new SFBool(false)));
    setFieldFlags(idsPerVertex, FF_X3DOM_ONLY);

    index.set(
          addExposedField(SFSTRING, "index", new SFString("")));
    setFieldFlags(index, FF_X3DOM_ONLY);

    indexType.set(
          addExposedField(SFSTRING, "indexType", new SFString("Uint16")));
    setFieldFlags(indexType, FF_X3DOM_ONLY);

    normal.set(
          addExposedField(SFSTRING, "normal", new SFString("")));
    setFieldFlags(normal, FF_X3DOM_ONLY);

    normalAsSphericalCoordinates.set(
          addExposedField(SFBOOL, "normalAsSphericalCoordinates", new SFBool(false)));
    setFieldFlags(normalAsSphericalCoordinates, FF_X3DOM_ONLY);

    normalPerVertex.set(
          addExposedField(SFBOOL, "normalPerVertex", new SFBool(true)));
    setFieldFlags(normalPerVertex, FF_X3DOM_ONLY);

    normalType.set(
          addExposedField(SFSTRING, "normalType", new SFString("Float32")));
    setFieldFlags(normalType, FF_X3DOM_ONLY);

    numTexCoordComponents.set(
          addExposedField(SFINT32, "numTexCoordComponents", new SFInt32(2), 
                   new SFInt32(1)));
    setFieldFlags(numTexCoordComponents, FF_X3DOM_ONLY);

    position.set(
          addExposedField(SFVEC3F, "position", new SFVec3f(0, 0, 0)));
    setFieldFlags(position, FF_X3DOM_ONLY);

    primType.set(
          addExposedField(MFSTRING, "primType", new MFString("TRIANGLES")));
    setFieldFlags(primType, FF_X3DOM_ONLY);

    rgbaColors.set(
          addExposedField(SFBOOL, "rgbaColors", new SFBool(false)));
    setFieldFlags(rgbaColors, FF_X3DOM_ONLY);

    size.set(
          addExposedField(SFVEC3F, "size", new SFVec3f(1, 1, 1)));
    setFieldFlags(size, FF_X3DOM_ONLY);

    solid.set(
          addExposedField(SFBOOL, "solid", new SFBool(true)));
    setFieldFlags(solid, FF_X3DOM_ONLY);

    tangent.set(
          addExposedField(SFSTRING, "tangent", new SFString("")));
    setFieldFlags(tangent, FF_X3DOM_ONLY);

    tangentType.set(
          addExposedField(SFSTRING, "tangentType", new SFString("Float32")));
    setFieldFlags(tangentType, FF_X3DOM_ONLY);

    texCoord.set(
          addExposedField(SFSTRING, "texCoord", new SFString("")));
    setFieldFlags(texCoord, FF_X3DOM_ONLY);

    texCoordType.set(
          addExposedField(SFSTRING, "texCoordType", new SFString("Float32")));
    setFieldFlags(texCoordType, FF_X3DOM_ONLY);

    vertexCount.set(
          addExposedField(MFINT32, "vertexCount", new MFInt32(0)));
    setFieldFlags(vertexCount, FF_X3DOM_ONLY);
}

Node *
ProtoBinaryGeometry::create(Scene *scene)
{ 
    return new NodeBinaryGeometry(scene, this); 
}

NodeBinaryGeometry::NodeBinaryGeometry(Scene *scene, Proto *def)
  : GeometryNode(scene, def)
{
}

void
NodeBinaryGeometry::load()
{
    MyString baseURL = "";
    baseURL += TheApp->getImportURL();
    MyString path;
    URL url1(baseURL, binormal()->getValue());
    if (strlen(binormal()->getValue()) > 0) 
        m_scene->Download(url1, &path);
    URL url2(baseURL, color()->getValue());
    if (strlen(color()->getValue()) > 0) 
        m_scene->Download(url2, &path);
    URL url3(baseURL, coord()->getValue());
    if (strlen(coord()->getValue()) > 0) 
        m_scene->Download(url3, &path);
    URL url4(baseURL, index()->getValue());
    if (strlen(index()->getValue()) > 0) 
        m_scene->Download(url4, &path);
    URL url5(baseURL, normal()->getValue());
    if (strlen(normal()->getValue()) > 0) 
        m_scene->Download(url5, &path);
    URL url6(baseURL, tangent()->getValue());
    if (strlen(tangent()->getValue()) > 0) 
        m_scene->Download(url6, &path);
    URL url7(baseURL, texCoord()->getValue());
    if (strlen(texCoord()->getValue()) > 0) 
        m_scene->Download(url7, &path);

}
