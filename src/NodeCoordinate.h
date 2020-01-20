/*
 * NodeCoordinate.h
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

#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "SFMFTypes.h"

class NodeNurbsGroup;
class MyMesh;

class ProtoCoordinate : public WonderlandExportProto {
public:
                    ProtoCoordinate(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_COORDINATE; }
    virtual int     getNodeClass() const { return COORDINATE_NODE; }

    virtual bool    isDeclaredInRwd_h() { return true; }

    FieldIndex point;
};



class NodeCoordinate : public Node {
public:
                    NodeCoordinate(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeCoordinate(*this); }

    void            drawHandles(void);
    Vec3f           getHandle(int handle, int *constraint, int *field);
    void            setHandle(int handle, const Vec3f &v);
    void            setBeginHandles(void);
    void            setEndHandles(void);
    void            setHandleLines(int handle, const Vec3f &v);
    void            setHandleFaces(int handle, const Vec3f &v);
    void            setHandleVertices(int handle, const Vec3f &v);
//    virtual bool    isInvalidChildNode(void) { return true; }
    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual int     getNumVertex(void) 
                        { return point()->getSFSize(); }  
    virtual Vec3f   getVertex(int i)
                        { return point()->getVec(i); }
    virtual bool    getValidVertex(void) { return true; }   

    virtual bool    validHandle(int handle);
    virtual bool    checkHandle(int handle);
    virtual int     getMaxHandle(void);

    bool            setHandle(MFVec3f *value, int handle,
                              const Vec3f &newV, const Vec3f &oldV,
                              bool changeNow,
                              bool bypassChecks = false);

    void            selectSymetricHandles(void);

    virtual void    update();

    virtual void    flip(int index);
    virtual void    swap(int fromTo);
    virtual bool    canMoveTo(int direction) { return true; }

    virtual bool    hasTwoSides(void);
    virtual bool    isDoubleSided(void);
    virtual void    toggleDoubleSided(void);
    virtual void    flipSide(void);
    
    virtual NodeColor *getColorNode() { 
                        if (hasParent())
                            return getParent()->getColorNode();
                        return NULL;     
                    }
    virtual NodeColorRGBA *getColorRGBANode() { 
                        if (hasParent())
                            return getParent()->getColorRGBANode();
                        return NULL;     
                    }


    fieldMacros(MFVec3f,  point, ProtoCoordinate)

    void subdivide(void);
    MyMesh *getMesh(void);
    void draw(Node *node);
    NodeNurbsGroup *findNurbsGroup();
protected:
    MyArray<Vec3f> m_selectedVertices;
    MyArray<Vec3f> m_selectedVertices2;
    MyArray<Vec3f> m_selectedVerticesWithoutX;
    MyArray<bool> m_validSymVerticesHandles;
    MyArray<int> m_selectedVerticesHandles;
};


