/*
 * NodeGeoElevationGrid.h
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

#pragma once

#include "GeoNode.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "NodeAppearance.h"
#include "NodeMaterial.h"
#include "NodeImageTexture.h"
#include "NodeTextureTransform.h"
#include "NodeGeoOrigin.h"
#include "GeometryNode.h"
#include "MeshBasedNode.h"
#include "SFMFTypes.h"

class ProtoGeoElevationGrid : public GeoProto {
public:
                    ProtoGeoElevationGrid(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_GEO_ELEVATION_GRID; }
    virtual int     getNodeClass() const { return GEOMETRY_NODE; }

    virtual bool    isMesh(void) { return true; }

    FieldIndex color;
    FieldIndex normal;
    FieldIndex texCoord;
    FieldIndex yScale;
    FieldIndex yScaleX3D;
    FieldIndex ccw;
    FieldIndex colorPerVertex;
    FieldIndex creaseAngle;
    FieldIndex creaseAngleX3D;
    FieldIndex geoGridOrigin;
    FieldIndex geoGridOriginX3D;
    FieldIndex height;
    FieldIndex heightX3D;
    FieldIndex normalPerVertex;
    FieldIndex solid;
    FieldIndex xDimension;
    FieldIndex xSpacing;
    FieldIndex xSpacingX3D;
    FieldIndex zDimension;
    FieldIndex zSpacing;
    FieldIndex zSpacingX3D;
    x3domGeometryCommonFieldIndex()

    virtual int     translateField(int field) const;
};

class NodeGeoElevationGrid : public MeshBasedNode {
public:
                    NodeGeoElevationGrid(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual const char* getComponentName(void) const { return "Geospatial"; }
    virtual int         getComponentLevel(void) const { return 1; }
    virtual Node   *copy() const { return new NodeGeoElevationGrid(*this); }

    virtual void    draw();

    virtual bool    hasTwoSides(void) { return true; }
    virtual bool    isDoubleSided(void) { return !solid()->getValue(); }
    virtual void    toggleDoubleSided(void)
                       { solid(new SFBool(!solid()->getValue())); }
    virtual int     getSolidField() { return solid_Field(); }
    virtual void    flipSide(void) { ccw(new SFBool(!ccw()->getValue())); }

    virtual bool    validHandle(int handle);
    virtual bool    checkHandle(int handle);
    virtual int     getMaxHandle(void);

    virtual NodeColor *getColorNode() 
                    { return (NodeColor *)color()->getValue(); }
    virtual NodeColorRGBA *getColorRGBANode() 
                    { return (NodeColorRGBA *)color()->getValue(); }

    GeometryNodeDeclarationMacros()

    void            setField(int index, FieldValue *value, int cf = -1);
    Node           *convert2Vrml(void);

    virtual bool    showFields() { return true; } 

    fieldMacros(SFNode,   color,            ProtoGeoElevationGrid)
    fieldMacros(SFNode,   normal,           ProtoGeoElevationGrid)
    fieldMacros(SFNode,   texCoord,         ProtoGeoElevationGrid)
    fieldMacros(SFFloat,  yScale,           ProtoGeoElevationGrid)
    fieldMacros(SFFloat,  yScaleX3D,        ProtoGeoElevationGrid)
    fieldMacros(MFFloat,  height,           ProtoGeoElevationGrid)
    fieldMacros(MFDouble, heightX3D,        ProtoGeoElevationGrid)
    fieldMacros(SFBool,   ccw,              ProtoGeoElevationGrid)
    fieldMacros(SFBool,   colorPerVertex,   ProtoGeoElevationGrid)
    fieldMacros(SFDouble, creaseAngle,      ProtoGeoElevationGrid)
    fieldMacros(SFDouble, creaseAngleX3D,   ProtoGeoElevationGrid)
    fieldMacros(SFString, geoGridOrigin,    ProtoGeoElevationGrid)
    fieldMacros(SFVec3d,  geoGridOriginX3D, ProtoGeoElevationGrid)
    fieldMacros(SFBool,   normalPerVertex,  ProtoGeoElevationGrid)
    fieldMacros(SFBool,   solid,            ProtoGeoElevationGrid)
    fieldMacros(SFInt32,  xDimension,       ProtoGeoElevationGrid)
    fieldMacros(SFString, xSpacing,         ProtoGeoElevationGrid)
    fieldMacros(SFDouble, xSpacingX3D,      ProtoGeoElevationGrid)
    fieldMacros(SFInt32,  zDimension,       ProtoGeoElevationGrid)
    fieldMacros(SFString, zSpacing,         ProtoGeoElevationGrid)
    fieldMacros(SFDouble, zSpacingX3D,      ProtoGeoElevationGrid)

    fieldMacros(SFNode,   geoOrigin, GeoProto)
    fieldMacros(MFString, geoSystem, GeoProto)


    x3domGeometryCommonFieldMacros(ProtoGeoElevationGrid)

protected:
    void            createMesh(bool cleanDoubleVertices = true,
                               bool triangulate = true);
};


