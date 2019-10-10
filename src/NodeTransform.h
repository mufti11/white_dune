/*
 * NodeTransform.h
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

#ifndef _NODE_TRANSFORM_H
#define _NODE_TRANSFORM_H

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

#include "swt.h"

#include "SFMFTypes.h"
#include "TransformNode.h"

class ProtoTransform : public TransformProto {
public:
                    ProtoTransform(Scene *scene, const char *name, 
                                   int childrenNodeClass = CHILD_NODE);
                    ProtoTransform(Scene *scene);

    void            addElements(int childrenNodeClass);

    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_TRANSFORM; }
    virtual int     getNodeClass() const { return GROUPING_NODE | 
                                                  CHILD_NODE |
                                                  BOUNDED_OBJECT_NODE; }

    virtual bool    isDeclaredInRwd_h() { return true; }

    FieldIndex      children;
    FieldIndex      render;
};

class NodeTransform : public TransformNode {
public:
                      NodeTransform(Scene *scene, Proto *proto);
                     ~NodeTransform();

public:
    virtual int       getChildrenField(void) const 
                         { return children_Field(); }

    // X3D profile changes addChildren, removeChildren
    virtual int       getProfile(void) const;
    virtual int       getX3dVersion(void) const { return 0; }
    virtual Node     *copy() const 
                          { 
                          return new NodeTransform(*this); 
                          }

    virtual void      setField(int field, FieldValue *value, int cf = -1);

    virtual bool      hasNumbers4kids(void) { return true; } 

    virtual int       getAnimationCommentID(void);

    virtual bool      supportCurveAnimation(void) { return true; }

//    virtual int       writeRib(int filedes, int indent);

    fieldMacros(MFNode, children, ProtoTransform)
    fieldMacros(SFBool, render,   ProtoTransform)
};

#endif // _NODE_TRANSFORM_H
