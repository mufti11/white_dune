/*
 * NodeHAnimHumanoid.h
 *
 * Copyright (C) 1999 Stephen F. White, 2008 J. "MUFTI" Scheurich
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

#ifndef _NODE_HAnim_HUMANOID_H
#define _NODE_HAnim_HUMANOID_H

#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif
#ifndef _MATRIX_H
#include "Matrix.h"
#endif
#ifndef _MY_MESH_H
#include "MyMesh.h"
#endif

typedef MyMesh* MyMeshPtr;

#include "swt.h"

#include "SFMFTypes.h"
#include "TransformNode.h"

class NodeMaterial;

class ProtoHAnimHumanoid : public TransformProto {
public:
                    ProtoHAnimHumanoid(Scene *scene);

    void            addElements(void);

    virtual int     getType() const { return X3D_HANIM_HUMANOID; }

    virtual Node   *create(Scene *scene);

    virtual bool    isDeclaredInRwd_h() { return true; }      

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex      info;
    FieldIndex      joints;
    FieldIndex      name;
    FieldIndex      segments;
    FieldIndex      sites;
    FieldIndex      skeleton;
    FieldIndex      skin;
    FieldIndex      skinCoord;
    FieldIndex      skinNormal;
    FieldIndex      version;
    FieldIndex      viewpoints;
};

class NodeHAnimHumanoid : public TransformNode {
public:
                      NodeHAnimHumanoid(Scene *scene, Proto *proto);

protected:
                     ~NodeHAnimHumanoid();

public:
    virtual int       getChildrenField(void) const
                         { return skeleton_Field(); }

    virtual const char* getComponentName(void) const;
    virtual int         getComponentLevel(void) const;
    virtual int         getX3dVersion(void) const { return 0; }
    virtual Node     *copy() const { return new NodeHAnimHumanoid(*this); }

    virtual void      setField(int index, FieldValue *value, int cf = -1);

    virtual bool      showFields() { return true; }

    virtual void      preDraw();
    virtual void      draw(int pass); 

    virtual void      drawHandles(void);
    virtual void      updateHandles();

    virtual Vec3f     getHandle(int handle, int *constraint, int *field);
    virtual void      setHandle(int handle, const Vec3f &v);

    void              drawJointHandle(float scale, Node *that);

    bool              supportCurveAnimation(void) { return true; }
    void              update();

    virtual void      createMeshes(bool cleanDoubleVertices = true,
                                   bool triangulateMesh = true);

    void              buildJointHasWeightArray(void);
    MyArray<int>     *getJointHasNoWeightArray(void) 
                          { return &m_hasNoWeightArray; }

    bool              jointHasNoWeight(int vertex);
    Node             *getJointParent(Node *node);

    virtual int       countPolygons(void);
    virtual int       countPolygons1Sided(void);
    virtual int       countPolygons2Sided(void);

    virtual int       writeRib(int filedes, int indent);

    fieldMacros(MFString, info,        ProtoHAnimHumanoid)
    fieldMacros(MFNode,   joints,      ProtoHAnimHumanoid)
    fieldMacros(SFString, name,        ProtoHAnimHumanoid)
    fieldMacros(MFNode,   segments,    ProtoHAnimHumanoid)
    fieldMacros(MFNode,   sites,       ProtoHAnimHumanoid)
    fieldMacros(MFNode,   skeleton,    ProtoHAnimHumanoid)
    fieldMacros(MFNode,   skin,        ProtoHAnimHumanoid)
    fieldMacros(SFNode,   skinCoord,   ProtoHAnimHumanoid)
    fieldMacros(SFNode,   skinNormal,  ProtoHAnimHumanoid)
    fieldMacros(SFString, version,     ProtoHAnimHumanoid)
    fieldMacros(MFNode,   viewpoints,  ProtoHAnimHumanoid)

    int           m_numMeshes;
    MyMeshPtr    *m_meshes;
    MyArray<MeshBasedNode *> m_meshNodes;
    MFVec3f     **m_origVertices;
    NodeMaterial *m_material;
    MyArray<int>  m_hasNoWeightArray;
    bool          m_hasNoWeightArrayDirty;
};

#endif
