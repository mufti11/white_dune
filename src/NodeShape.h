/*
 * NodeShape.h
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

#ifndef _NODE_SHAPE_H
#define _NODE_SHAPE_H

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

class ProtoShape : public WonderlandExportProto {
public:
                    ProtoShape(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_SHAPE; }
    virtual int     getNodeClass() const { return SHAPE_NODE | CHILD_NODE; }

    virtual bool    isDeclaredInRwd_h() { return true; }      

    FieldIndex appearance;
    FieldIndex geometry;
    FieldIndex effect;
    FieldIndex bboxCenter;
    FieldIndex bboxSize;
    FieldIndex octreeTriangles;
    FieldIndex shading;
    FieldIndex isPickable;
    FieldIndex render;
};

class NodeShape : public Node {
public:
                    NodeShape(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() { return new NodeShape(m_scene, m_proto); }

    virtual void    draw(int pass);
    virtual bool    isLit();

    virtual int     countPolygons(void);
    virtual int     countPrimitives(void);

    virtual int     countPolygons1Sided(void);
    virtual int     countPolygons2Sided(void);

    virtual Vec3f   getMinBoundingBox(void)
                        {
                        Node *ngeometry = geometry()->getValue();
                        if (ngeometry)
                            return ngeometry->getMinBoundingBox(); 
                        return Vec3f(0, 0, 0); 
                        }
    virtual Vec3f   getMaxBoundingBox(void)
                        {
                        Node *ngeometry = geometry()->getValue();
                        if (ngeometry)
                            return ngeometry->getMaxBoundingBox(); 
                        return Vec3f(0, 0, 0); 
                        }

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual float   getTransparency(void);

    virtual bool    hasCoverFields(void) { return true; }   

    virtual int     write(int filedes, int indent, bool avoidUse = false);

    virtual Node   *convert2Vrml(void);

    virtual bool    canWriteAc3d();
    virtual bool    canWriteCattGeo();


    virtual NodeCoordinate *getCoordinateNode() 
                    {
                    Node *ngeometry = geometry()->getValue();
                    if (ngeometry)
                        return ngeometry->getCoordinateNode(); 
                    return NULL; 
                    }

    virtual NodeColor  *getColorNode()
                    {
                    Node *ngeometry = geometry()->getValue();
                    if (ngeometry)
                        return ngeometry->getColorNode(); 
                    return NULL; 
                    }
    virtual NodeColorRGBA *getColorRGBANode()
                    {
                    Node *ngeometry = geometry()->getValue();
                    if (ngeometry)
                        return ngeometry->getColorRGBANode(); 
                    return NULL; 
                    }

    void            createNewAppearance(bool emissiveDefaultColor);

    virtual bool    showFields() { return false; }

    fieldMacros(SFNode,   appearance,      ProtoShape)
    fieldMacros(SFNode,   geometry,        ProtoShape)
    fieldMacros(SFNode,   effect,          ProtoShape)
    fieldMacros(SFVec3f,  bboxCenter,      ProtoShape)
    fieldMacros(SFVec3f,  bboxSize,        ProtoShape)
    fieldMacros(SFNode,   octreeTriangles, ProtoShape);
    fieldMacros(SFString, shading,         ProtoShape);
    fieldMacros(SFBool,   isPickable,      ProtoShape);
    fieldMacros(SFBool,   render,          ProtoShape);

protected:
    void            doSpecularPass(Node *appearance, Node *geometry);

protected:
    FieldIndex      m_appearance;
    FieldIndex      m_geometry;
};

#endif // _NODE_SHAPE_H
