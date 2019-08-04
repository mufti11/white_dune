/*
 * NodeNurbsSurface.h
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

#ifndef _NODE_NURBS_SURFACE_H
#define _NODE_NURBS_SURFACE_H

#ifndef _MESH_MORPHING_NODE_H
# include "MeshMorphingNode.h"
#endif
#ifndef _PROTO_MACROS_H
# include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
# include "Proto.h"
#endif
#ifndef _VEC3F
# include "Vec3f.h"
#endif
#ifndef _SFMFTYPES_H
# include "SFMFTypes.h"
#endif

class MyMesh;
class NodeNurbsGroup;
class NodeNurbsSet;

class ProtoNurbsSurface : public Proto {
public:
                    ProtoNurbsSurface(Scene *scene);
                    ProtoNurbsSurface(Scene *scene, const char *name);
    virtual Node   *create(Scene *scene);
    void            addElements(void);

    virtual int     getType() const { return VRML_NURBS_SURFACE; }
    virtual int     getNodeClass() const
                       { return PARAMETRIC_GEOMETRY_NODE | GEOMETRY_NODE; }

    virtual bool    isMesh(void) { return true; }

    FieldIndex uDimension;
    FieldIndex vDimension;
    FieldIndex uKnot;
    FieldIndex vKnot;
    FieldIndex uOrder;
    FieldIndex vOrder;
    FieldIndex uClosed;
    FieldIndex vClosed;
    FieldIndex controlPoint;
    FieldIndex controlPointX3D;
    FieldIndex weight;
    FieldIndex uTessellation;
    FieldIndex vTessellation;
    FieldIndex texCoord;
    FieldIndex ccw;
    FieldIndex solid;

    virtual const MyString &getName(bool x3d) const 
       { 
       if (x3d)
           return m_x3dName;
       else
           return m_name; 
       }
protected:
    MyString m_x3dName;
};

class NodeNurbsSurface : public MeshMorphingNode {
public:
                    NodeNurbsSurface(Scene *scene, Proto *proto);
protected:
    virtual        ~NodeNurbsSurface();

public:
    virtual const char* getComponentName(void) const { return "NURBS"; }
    virtual int     getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeNurbsSurface(*this); }

    virtual void    draw() { meshDraw(); }
    virtual void    drawHandles(void);

    virtual Vec3f   getHandle(int handle, int *constraint, int *field);
    virtual void    setHandle(int handle, const Vec3f &v);
    virtual bool    validHandle(int handle);

    virtual void    update();

    virtual bool    supportAnimation(void) { return true; }

    Node           *toNurbs(int uTessel, int vTessel, 
                            int uDegree, int vDegree);

    Node           *convert2X3d(void);
    Node           *convert2Vrml(void);

    bool            checkXSymetricOrSameHandle(int handle, MFVec3f *points);

    virtual int     repairField(int field) const;
    virtual void    setField(int index, FieldValue *value, int cf = -1);

    void            repairKnotAndWeight();
    void            extrudePoints(int uFrom, int uTo, int uPoints,
                                  int vFrom, int vTo, int vPoints);

    void            rotate(SFRotation rot);

    virtual Node   *degreeElevate(int newUDegree, int newVDegree); 

    Node           *setWeightsTo1(void);

    virtual void    flip(int index);
    virtual void    swap(int fromTo);
    virtual bool    canMoveTo(int direction) { return true; }

    virtual bool    hasTwoSides(void) { return true; }
    virtual bool    isDoubleSided(void) { return !solid()->getValue(); }
    virtual void    toggleDoubleSided(void) 
                       { solid(new SFBool(!solid()->getValue())); }
    virtual int     getSolidField() { return solid_Field(); }
    virtual void    flipSide(void);

    virtual bool    maySetDefault(void) { return false; }

    virtual bool    avoidProtoOnPureVrml(void) { return true; }
    int             writeProto(int filedes);

    virtual int     getAnimatedNodeField(int field);

    virtual int     getNumVertex(void) 
                        { return getControlPoints()->getSFSize(); }  
    virtual Vec3f   getVertex(int i)
                        { 
                        return getControlPoints()->getVec(i) / 
                               weight()->getValue(i); 
                        }
    virtual bool    getValidVertex(void) { return true; }   

    fieldMacros(SFInt32, uDimension,      ProtoNurbsSurface)
    fieldMacros(SFInt32, vDimension,      ProtoNurbsSurface)
    fieldMacros(MFFloat, uKnot,           ProtoNurbsSurface)
    fieldMacros(MFFloat, vKnot,           ProtoNurbsSurface)
    fieldMacros(SFInt32, uOrder,          ProtoNurbsSurface)
    fieldMacros(SFInt32, vOrder,          ProtoNurbsSurface)
    fieldMacros(SFBool,  uClosed,         ProtoNurbsSurface)
    fieldMacros(SFBool,  vClosed,         ProtoNurbsSurface)
    fieldMacros(MFVec3f, controlPoint,    ProtoNurbsSurface)
    fieldMacros(SFNode,  controlPointX3D, ProtoNurbsSurface)
    fieldMacros(MFFloat, weight,          ProtoNurbsSurface)
    fieldMacros(SFInt32, uTessellation,   ProtoNurbsSurface)
    fieldMacros(SFInt32, vTessellation,   ProtoNurbsSurface)
    fieldMacros(SFNode,  texCoord,        ProtoNurbsSurface)
    fieldMacros(SFBool,  ccw,             ProtoNurbsSurface)
    fieldMacros(SFBool,  solid,           ProtoNurbsSurface)

    virtual void    setHandle(MFVec3f *newValue, int handle, float newWeight,
                              const Vec3f &newV, const Vec3f &oldV,
                              bool already_changed = false, 
                              bool bypassChecks = false);
    virtual void    setHandle(float newWeight, 
                              const Vec3f &newV, const Vec3f &oldV, 
                              bool bypassChecks);
    virtual void    setHandle(float newWeight, 
                              const Vec3f &newV, const Vec3f &oldV) 
                       { setHandle(newWeight, newV, oldV, false); }
    NodeNurbsGroup *findNurbsGroup();
    NodeNurbsSet   *findNurbsSet();
    void            createMesh(bool cleanDoubleVertices = true,
                               bool trianglulate = true);
    void            createMesh(const Vec3f *controlPoints,
                               bool cleanDoubleVertices,
                               bool trianglulate);
    void            linearUknot(void);
    void            linearVknot(void);

    MFVec3f        *getControlPoints(void);
    void            setControlPoints(MFVec3f *points);
    void            createControlPoints(MFVec3f *points);
    void            backupFieldsAppend(int field);

    virtual Node   *toNurbsTrimmedSurface(void);

    int             getNumExtraVertices(void) { return m_numExtraVertices; }

    void            makeSymetric(int direction, bool plus);

protected:
    static int      findSpan(int dimension, int order, float u,
                             const float knots[]);
    static void     basisFuns(int span, float u, int order,
                              const float knots[], float basis[], 
                              float deriv[]);
    static Vec3f    surfacePoint(int uDimension, int uOrder,
                                 const float uKnots[],
                                 int vDimension, int vOrder,
                                 const float vKnots[],
                                 const Vec3f controlPoints[],
                                 const float weights[],
                                 float u, float v, Vec3f &normal);
   void             repairNormal(MFVec3f *normal, MFVec3f *vertices, 
                                 int c1, int c2, int c3);
    virtual void   *initializeData(void);
    virtual void    loadDataFromInterpolators(void *data, Interpolator *inter,
                                              int field, float key);
    virtual void    createMeshFromData(void* data, bool optimize);
    virtual void    finalizeData(void* data);
    void            setCreateExtraTess(bool flag) { m_createExtraTess = flag; }

protected:
    bool            m_inRepair;
    int             m_numExtraVertices;
    bool            m_createExtraTess;
};

#endif // _NODE_NURBS_SURFACE_H
