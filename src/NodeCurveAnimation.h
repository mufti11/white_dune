/*
 * NodeCurveAnimation.h
 *
 * Copyright (C) 1999 Stephen F. White, 2011 J. "MUFTI" Scheurich
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

#ifndef _NODE_CURVE_ANIMATION_H
#define _NODE_CURVE_ANIMATION_H

#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif
#include "NodeNurbsCurve.h"
#include "NodePositionInterpolator.h"
#include "NodeOrientationInterpolator.h"

#include "SFMFTypes.h"
class Interpolator;

class ProtoCurveAnimation : public ProtoNurbsCurve {
public:
                    ProtoCurveAnimation(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return DUNE_CURVE_ANIMATION; }
    virtual int     getNodeClass() const { return CHILD_NODE; }

    virtual bool    isScriptedExternProto(void) { return true; }

    FieldIndex      rotationAxis;
    FieldIndex      enableRotation;
    FieldIndex      hover; 
};

class NodeCurveAnimation : public NodeNurbsCurve {
public:
                    NodeCurveAnimation(Scene *scene, Proto *proto);
                   ~NodeCurveAnimation();

    virtual int     getProfile(void) const { return PROFILE_IMMERSIVE; }  

    virtual const char* getComponentName(void) const { return ""; }
    virtual int       getComponentLevel(void) const { return -1; }

    virtual Node   *copy() const { return new NodeCurveAnimation(*this); }

    void            createNurbsCurve(void);

    virtual void    drawHandles(void);

    virtual Vec3f   getHandle(int handle, int *constraint, int *field);
    virtual void    setHandle(int handle, const Vec3f &v);

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual bool    hasBoundingBox(void) { return true; }
    virtual Vec3f   getMinBoundingBox(void);
    virtual Vec3f   getMaxBoundingBox(void);

    virtual void    flip(int index);
    virtual void    swap(int fromTo);
   
    Node           *toNurbsCurve(void);
    Node           *copyFromNurbsCurve(Node *nurbsCurve);

    void            receiveEvent(int eventIn, double timestamp, 
                                 FieldValue *value);

    virtual bool    maySetDefault(void) { return false; }

    virtual bool    avoidProtoOnPureVrml(void) { return true; }
    virtual int     writeProto(int filedes);

    virtual bool    canConvertToPositionAndOrientationInterpolators(void) 
                           { return true; }
    virtual void    toPositionAndOrientationInterpolators(NodeList *nodes);

    virtual Node   *degreeElevate(int newDegree); 

    virtual int     write(int filedes, int indent);
    virtual int     writeXml(int filedes, int indent, int containerField = -1) 
                       { return write(filedes, indent); }

    virtual int     writeC(int filedes, int languageFlag);
    virtual int     writeCDeclaration(int filedes, int languageFlag);

    virtual void    addToConvertedNodes(int writeFlags);

    Node           *getPositionInterpolator() 
                       { return m_positionInterpolator;  }
    Node           *getOrientationInterpolator() 
                       { return m_orientationInterpolator;  }

    int             getFraction_Field() { return m_set_fractionField; }
    FieldIndex      getPosition_Field() { return m_position_changedField; }
    FieldIndex      getOrientation_Field() { return m_orientation_changedField; }

    virtual void    addInput(int eventIn, Node *src, int eventOut);
    virtual void    addOutput(int eventOut, Node *dst, int eventIn);
    virtual void    removeInput(int eventIn, Node *src, int eventOut);
    virtual void    removeOutput(int eventOut, Node *dst, int eventIn);

    void            createRoutes(Interpolator *node, bool appendToScene);

    float           setNewFraction(float fraction);

    void            update(void) { m_curveDirty = true; }
    void            reInit(void);

    fieldMacros(SFVec3f, rotationAxis,   ProtoCurveAnimation)
    fieldMacros(SFBool,  enableRotation, ProtoCurveAnimation)
    fieldMacros(SFBool,  hover,          ProtoCurveAnimation)

protected:
    void               createInterpolators();
    void               sendPosition(double timestamp, float fraction);
    void               sendOrientation(double timestamp, float fraction);
protected:
    FieldIndex         m_set_fractionField;
    FieldIndex         m_position_changedField;
    FieldIndex         m_orientation_changedField;
    NodePositionInterpolator    *m_positionInterpolator; 
    NodeOrientationInterpolator *m_orientationInterpolator; 
    bool               m_curveDirty;
    Array<float>       m_chain;
};
#endif 
 
