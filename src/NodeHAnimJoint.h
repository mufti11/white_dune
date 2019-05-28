/*
 * NodeHAnimJoint.h
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

#ifndef _NODE_HANIN_JOINT_H
#define _NODE_HANIN_JOINT_H

#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"

#include "NodeTransform.h"

class ProtoHAnimJoint : public ProtoTransform {
public:
                    ProtoHAnimJoint(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_HANIM_JOINT; }
    virtual int     getNodeClass() const { return HANIM_CHILD_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    virtual bool    isDeclaredInRwd_h() { return true; }      

    FieldIndex displacers;
    FieldIndex limitOrientation;
    FieldIndex llimit; 
    FieldIndex name;
    FieldIndex skinCoordIndex;
    FieldIndex skinCoordWeight;
    FieldIndex stiffness;
    FieldIndex ulimit;
};

class NodeHAnimJoint : public NodeTransform {
public:
                    NodeHAnimJoint(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const;
    virtual int         getComponentLevel(void) const;
    virtual int         getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeHAnimJoint(*this); }

    void            setField(int field, FieldValue *value, int cf = -1);

    virtual bool    showFields() { return true; }
    virtual void    applyJoint(int skinNum, MyMesh *mesh, 
                               MFVec3f *origVertices, Node *parent);
    void            accountJointMatrix(Node *parent);
    virtual void    drawHandles(void);
    virtual void    drawJointHandles(float scale, Node *parent, Node *that);
    virtual void    updateHandles() 
                        { if (hasParent()) getParent()->updateHandles(); }
    virtual bool    getMatrixDirty(void) { return m_matrixDirty; }

    virtual void    setHandle(int handle, const Vec3f &v);

    void            getJointMatrix(Matrix &matrix) 
                        { m_jointMatrix.copyTo(matrix); } 

    Node           *getJointParent(Node *node, Node *parent);

    fieldMacros(MFNode,     displacers,       ProtoHAnimJoint)
    fieldMacros(SFRotation, limitOrientation, ProtoHAnimJoint)
    fieldMacros(MFFloat,    llimit,           ProtoHAnimJoint)
    fieldMacros(SFString,   name,             ProtoHAnimJoint)
    fieldMacros(MFInt32,    skinCoordIndex,   ProtoHAnimJoint)
    fieldMacros(MFFloat,    skinCoordWeight,  ProtoHAnimJoint)
    fieldMacros(MFFloat,    stiffness,        ProtoHAnimJoint)
    fieldMacros(MFFloat,    ulimit,           ProtoHAnimJoint)
protected:
    Matrix m_jointMatrix;
    NodeCoordinate *m_coord;
};

#endif
