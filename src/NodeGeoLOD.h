/*
 * NodeGeoLOD.h
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

#ifndef _NODE_GEO_LOD_H
#define _NODE_GEO_LOD_H

#ifndef _NODE_H
#include "GeoNode.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"

class ProtoGeoLOD : public GeoProto {
public:
                    ProtoGeoLOD(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_GEO_LOD; }

    FieldIndex bboxCenter;
    FieldIndex bboxSize;
    FieldIndex center;
    FieldIndex centerX3D;
    FieldIndex child1Url;
    FieldIndex child2Url;
    FieldIndex child3Url;
    FieldIndex child4Url;
    FieldIndex range;
    FieldIndex rootUrl;
    FieldIndex rootNode;
    FieldIndex referenceBindableDescription;
    FieldIndex render;

    virtual int     translateField(int field);
};

class NodeGeoLOD : public GeoNode {
public:
                    NodeGeoLOD(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() { return new NodeGeoLOD(m_scene, m_proto); }

    void            setField(int index, FieldValue *value, int cf = -1);
    Node           *convert2Vrml(void);

    virtual bool    showFields() { return true; } 

    virtual void    preDraw();
    virtual void    draw(int pass);
    void            accountNodeToDrawIndex();
    void            loadChild(Node *node, MFString *urls);
    virtual void    load();

    fieldMacros(SFVec3f,  bboxCenter, ProtoGeoLOD)
    fieldMacros(SFVec3f,  bboxSize,   ProtoGeoLOD)
    fieldMacros(SFString, center,     ProtoGeoLOD)
    fieldMacros(SFVec3d,  centerX3D,  ProtoGeoLOD)
    fieldMacros(MFString, child1Url,  ProtoGeoLOD)
    fieldMacros(MFString, child2Url,  ProtoGeoLOD)
    fieldMacros(MFString, child3Url,  ProtoGeoLOD)
    fieldMacros(MFString, child4Url,  ProtoGeoLOD)
    fieldMacros(MFFloat,  range,      ProtoGeoLOD)
    fieldMacros(MFString, rootUrl,    ProtoGeoLOD)
    fieldMacros(MFNode,   rootNode,   ProtoGeoLOD)
    fieldMacros(SFString, referenceBindableDescription, ProtoGeoLOD)
    fieldMacros(SFBool,   render,     ProtoGeoLOD)

protected:
    int m_nodeToDrawIndex;
    Node *m_root;
    Node *m_child1;
    Node *m_child2;
    Node *m_child3;
    Node *m_child4;
};

#endif // _NODE_GEO_LOD_H

