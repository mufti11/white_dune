/*
 * NodeIndexedFaceSet.h
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

#include "MeshBasedNode.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "DuneApp.h"
#include "Vec3f.h"
#include "NodeCoordinate.h"
#include "Colored.h"
#include "SFMFTypes.h"
#include "ComposedGeometryMacros.h"

class Matrix;

enum {
    UNION,
    INTERSECTION,
    SUBTRACT
};

class ProtoIndexedFaceSet : public GeometryProto {
public:
                    ProtoIndexedFaceSet(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_INDEXED_FACE_SET; }

    virtual bool    isMesh(void) { return true; }
    virtual bool    isExportTargetMesh(void) { return true; }

    virtual bool    isDeclaredInRwd_h() { return true; }      

    FieldIndex color;
    FieldIndex coord;
    FieldIndex normal;
    FieldIndex texCoord;
    FieldIndex texCoord2;
    FieldIndex texCoord3;
    FieldIndex texCoord4;
    FieldIndex ccw;
    FieldIndex colorIndex;
    FieldIndex colorPerVertex;
    FieldIndex convex;
    FieldIndex coordIndex;
    FieldIndex creaseAngle;
    FieldIndex normalIndex;
    FieldIndex normalPerVertex;
    FieldIndex solid;
    FieldIndex texCoordIndex;
    FieldIndex texCoordIndex2;
    FieldIndex texCoordIndex3;
    FieldIndex texCoordIndex4;
    ComposedGeometryProtoMacro()
    x3domGeometryCommonFieldIndex()
    FieldIndex normalUpdateMode;
};

class FacesetAndMatrix {
public:
    NodeIndexedFaceSet *node;
    NodeMaterial *material;
    Matrix matrix;
};

class NodeIndexedFaceSet : public MeshBasedNode, Colored {
public:
                    NodeIndexedFaceSet(Scene *scene, Proto *proto);

protected:
    virtual        ~NodeIndexedFaceSet();

public:
    virtual int     getProfile(void) const;
    virtual int     getX3dVersion(void) const { return 0; }
    virtual void    setField(int index, FieldValue *value, int cf = -1);
    virtual Node   *copy() const { return new NodeIndexedFaceSet(*this); }

    virtual void    draw();

    virtual bool    shouldConvertToIndexedFaceSet(void) { return false; }

    virtual void    addToConvertedNodes(int writeFlags);

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual int     getNormalField() { return normal_Field(); }
    virtual int     getTexCoordField() { return texCoord_Field(); }

    virtual void    setNormalFromMesh(Node *nnormal);
    virtual void    setTexCoordFromMesh(Node *ntexCoord);

    virtual bool    hasCoverFields(void) { return true; }   

    virtual bool    hasTwoSides(void) { return true; }
    virtual bool    isDoubleSided(void) { return !solid()->getValue(); }
    virtual void    toggleDoubleSided(void)
                       { solid(new SFBool(!solid()->getValue())); }
    virtual int     getConvexField() { return convex_Field(); }
    virtual int     getSolidField() { return solid_Field(); }
    virtual void    flipSide(void) { ccw(new SFBool(!ccw()->getValue())); }

    virtual bool    maySetDefault(void) { return false; }
 
    virtual NodeColor *getColorNode() 
                    { return (NodeColor *)color()->getValue(); }
    virtual NodeColorRGBA *getColorRGBANode() 
                    { return (NodeColorRGBA *)color()->getValue(); }
    virtual NodeCoordinate *getCoordinateNode(void) 
                    { return (NodeCoordinate *)coord()->getValue(); }
    virtual Colored *getColored() { return this; }

    virtual int     colorPerVertexField() const 
                       { return colorPerVertex_Field(); }
    virtual int     colorIndexField() const
                       { return colorIndex_Field(); }

    MFVec3f        *getCoordinates();
    MFInt32        *getColorIndex();
    MFInt32        *getCoordIndex();
    MFInt32        *getNormalIndex();
    MFVec2f        *getTextureCoordinates();
    MFInt32        *getTexCoordIndex();       

    Node           *toIndexedLineSet(void);

    virtual bool    showFields() { return true; }

    NodeIndexedFaceSet *splitSelectedFaces(void);
    void            extrudeFaces(float dist);
    void            insetFaces(float factor, int numX, int numY);
    int             symetricPointIndex(int ci, int iface);
    int             symetricFace(int iface);
    bool            isSymetricFace(int iface);
    bool            checkBorderMidPoint(int icoordIndex, MyArray<int> symFaces);
    bool            checkBorderFace(MyArray<int> innerBorder, 
                                    MyArray<int> outerBorder, 
                                    MFInt32 *coordIndex, 
                                    int borderIndex1, int borderIndex2,
                                    MyArray<int> symFaces, bool sym);
    void            optimize(void);
    void            optimizeCoordIndex(void);

    bool            buildQuad(void);
    bool            canSplitFace(int face);
    bool            canSplitFaces(void);

    static Node    *simpleJoin(MyArray<FacesetAndMatrix> data);

    bool            checkMidpoint(Vec3f midPoint, int jLoop, int nLoop,
                                  int point1, int point2,
                                  int uPieces, int vPieces);
    int             getEgdeCoordIndex(int iface, Vec3f midPoint,
                                      int jLoop, int nLoop,
                                      int uPieces, int vPieces);
    void            splitIntoPieces(int piecesU, int piecesV);
#ifdef HAVE_LIBCGAL
    NodeIndexedFaceSet *csg(NodeIndexedFaceSet *face, int operation,
                            Matrix matrix1, Matrix matrix2);
#endif
    float               getOffFactor(int tabCount, MyArray<char *> strings,
                                     int beginData, int endData);
    NodeIndexedFaceSet *readOff(const char *filename);
    void                accountOffData(int f);
    void                writeOffInit(void) 
                            {
                            m_sumVertices = 0; 
                            m_sumVerticesPerFaces = 0;
                            }
    void                writeOffVerticesAndColors(int f, Node *node);
    void                writeOffIndicesAndColors(int f, int startIndex, 
                                                 Node *node);
    void                writeOffNormals(int f, Node *node);
    int                 getSumVertices(void)
                            { return m_sumVertices; }
    int                 getSumVerticesPerFaces(void) 
                            { return m_sumVerticesPerFaces; }

#ifdef HAVE_LIBVCG
    NodeIndexedFaceSet *meshReduce(float percent);
#endif
    float          *intersectVector0Plane(Vec3f endPoint, Vec3f startPoint,
                                          int direction);
    void            makeSymetric(int direction, bool plus);
    void            deleteFaces(MFInt32 *coordIndex, MyArray<int> *faces);
    void            snapTogether(void);

    void            changeToColorPerVertex(void);
    void            changeToColorPerFace(void);
    bool            getColorPerVertex(void) 
                        { return colorPerVertex()->getValue(); }

    fieldMacros(SFNode,   color,            ProtoIndexedFaceSet)
    fieldMacros(SFNode,   coord,            ProtoIndexedFaceSet)
    fieldMacros(SFNode,   normal,           ProtoIndexedFaceSet)
    fieldMacros(SFNode,   texCoord,         ProtoIndexedFaceSet)
    fieldMacros(SFNode,   texCoord2,        ProtoIndexedFaceSet)
    fieldMacros(SFNode,   texCoord3,        ProtoIndexedFaceSet)
    fieldMacros(SFNode,   texCoord4,        ProtoIndexedFaceSet)
    fieldMacros(SFBool,   ccw,              ProtoIndexedFaceSet)
    fieldMacros(MFInt32,  colorIndex,       ProtoIndexedFaceSet)
    fieldMacros(SFBool,   colorPerVertex,   ProtoIndexedFaceSet)
    fieldMacros(SFBool,   convex,           ProtoIndexedFaceSet)
    fieldMacros(MFInt32,  coordIndex,       ProtoIndexedFaceSet)
    fieldMacros(SFFloat,  creaseAngle,      ProtoIndexedFaceSet)
    fieldMacros(MFInt32,  normalIndex,      ProtoIndexedFaceSet)
    fieldMacros(SFBool,   normalPerVertex,  ProtoIndexedFaceSet)
    fieldMacros(SFBool,   solid,            ProtoIndexedFaceSet)
    fieldMacros(MFInt32,  texCoordIndex,    ProtoIndexedFaceSet)
    fieldMacros(MFInt32,  texCoordIndex2,   ProtoIndexedFaceSet)
    fieldMacros(MFInt32,  texCoordIndex3,   ProtoIndexedFaceSet)
    fieldMacros(MFInt32,  texCoordIndex4,   ProtoIndexedFaceSet)
    ComposedGeometryFieldMacros(ProtoIndexedFaceSet)
    x3domGeometryCommonFieldMacros(ProtoIndexedFaceSet)
    fieldMacros(SFString, normalUpdateMode, ProtoIndexedFaceSet)     

protected:
    void            createMesh(bool cleanDoubleVertices = true,
                               bool triangulate = true);
                               
protected:
    bool m_alreadyInChangeColorPerVertex;
    int m_sumVertices; 
    int m_sumVerticesPerFaces ;                           
};

