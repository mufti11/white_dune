/*
 * NodeGeoTransform.h
 *
 * Copyright (C) 2009 J. "MUFTI" Scheurich
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
#include "GeoNode.h"
#include "SFMFTypes.h"

class ProtoGeoTransform : public GeoProto {
public:
                    ProtoGeoTransform(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_GEO_TRANSFORM; }
    virtual int     getNodeClass() const { return GROUPING_NODE | CHILD_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex children;
    FieldIndex geoCenter;
    FieldIndex rotation;
    FieldIndex scale;
    FieldIndex scaleOrientation;
    FieldIndex translation;
    FieldIndex bboxCenter;
    FieldIndex bboxSize;
    FieldIndex globalGeoOrigin;
    FieldIndex render;
};

class NodeGeoTransform : public GeoNode {
public:
                    NodeGeoTransform(Scene *scene, Proto *proto);

    virtual int     getComponentLevel(void) const { return 2; }
    virtual int     getX3dVersion(void) const { return 2; }
    virtual Node   *copy() const { return new NodeGeoTransform(*this); }

    virtual bool    showFields() { return true; }

    virtual void    transform();

    virtual void    setField(int field, FieldValue *value, int cf = -1)
                        { 
                        m_matrixDirty = true; 
                        Node::setField(field, value, cf);
                        }

    virtual void    preDraw();
    virtual void    draw(int pass); 

    fieldMacros(MFNode, children, ProtoGeoTransform);
    fieldMacros(SFVec3d, geoCenter, ProtoGeoTransform);
    fieldMacros(SFRotation, rotation, ProtoGeoTransform);
    fieldMacros(SFVec3f, scale, ProtoGeoTransform);
    fieldMacros(SFRotation, scaleOrientation, ProtoGeoTransform);
    fieldMacros(SFVec3f, translation, ProtoGeoTransform);
    fieldMacros(SFVec3f, bboxCenter, ProtoGeoTransform);
    fieldMacros(SFVec3f, bboxSize, ProtoGeoTransform);
    fieldMacros(SFBool, globalGeoOrigin, ProtoGeoTransform);
    fieldMacros(SFBool, render, ProtoGeoTransform)

protected:
    bool              m_matrixDirty;
    Matrixd           m_matrix;
};

