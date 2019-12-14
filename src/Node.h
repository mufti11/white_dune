/*
 * Node.h
 *
 * Copyright (C) 1999 Stephen F. White, 2005 J. "MUFTI" Scheurich
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

class Node;
class Scene;

typedef bool (*DoWithNodeCallback)(Node *node, void *data);

#include <stdio.h>
#include "Array.h"
#include "List.h"
#include "Vec3f.h"
#include "MyString.h"
#include "FieldIndex.h"
#include "Element.h"
#include "NodeFlags.h"
#include "MeshFlags.h"
#include "NodeArray.h"
#include "Stack.h"
#include "Matrix.h"
#include "Quaternion.h"
//#include "Colored.h"
#include "ac3dMaterialCallback.h"

class FieldValue;
class NodePROTO;
class Path;
class NodeList;
class EventOut;
class EventIn;
class Field;
class ExposedField;
class MyMesh;
class MFVec3f;
class MFVec2f;
class NodeHAnimHumanoid;
class SFFloat; 
class SFNode;

#include "NodeDefinitions.h"

class RouteSocket {
public:
                RouteSocket() 
                    { 
                    m_node = NULL; 
                    m_field = -1; 
                    }
                RouteSocket(Node *node, int index) 
                    { 
                    m_node = node; 
                    m_field = index; 
                    }

    int         operator==(const RouteSocket &t)
                    { return t.m_node == m_node && t.m_field == m_field; }
 
    int         operator!=(const RouteSocket &t)
                    { return t.m_node != m_node || t.m_field != m_field; }

    Node       *getNode(void) { return m_node; }
    int         getField(void) { return m_field; }
protected:
    Node       *m_node;
    int         m_field;
};

typedef List<RouteSocket> SocketList;

enum parentFlag {
    PARENT_NOT_TOUCHED,
    PARENT_TOUCHED,
    PARENT_RECURSIVE
};

class Parent {
public:
                Parent() 
                    { 
                    m_node = NULL; 
                    m_field = -1;
                    m_self = NULL; 
                    m_parentFlag = PARENT_NOT_TOUCHED;
                    }
  
                Parent(Node *node, int field, Node *self) 
                    { 
                    m_node = node; 
                    m_field = field;
                    m_self = self; 
                    m_parentFlag = PARENT_NOT_TOUCHED;
                    }

    int         operator==(Parent &t)
                    { return t.m_self == m_self; }

    int         operator!=(Parent &t)
                    { return t.m_self != m_self; }

    Node       *m_node;
    int         m_field;
    Node       *m_self;
    parentFlag  m_parentFlag;             
};

typedef MyArray<Parent> ParentArray;

enum Constraint {
    CONSTRAIN_NONE = 0,
    CONSTRAIN_X,
    CONSTRAIN_Y,
    CONSTRAIN_Z,
    CONSTRAIN_XY,
    CONSTRAIN_YZ,
    CONSTRAIN_ZX,
    CONSTRAIN_SPHERE
};

enum {
    PROFILE_CORE,
    PROFILE_INTERCHANGE,
    PROFILE_CAD_INTERCHANGE,
    PROFILE_INTERACTIVE,
    PROFILE_MPEG4_INTERACTIVE,
    PROFILE_IMMERSIVE,
    PROFILE_FULL
};

typedef enum {
    XZ_DIRECTION,
    XY_DIRECTION,
    YZ_DIRECTION
} Direction;

enum xmlWriteFlag {
    XML_PROTO_INSTANCE_FIELD,
    XML_IN_TAG,
    XML_IS,
    XML_NODE
};

class EventOutData {
public:
    int eventOut;
    int numOutput;
    Node *protoNode;
};

class Colored;
class NodeColor;
class NodeColorRGBA;
class NodeCoordinate;

class NodeData {
public:
                      NodeData(Scene *scene, Proto *proto);
                      NodeData(Node node);
    virtual          ~NodeData();

    void              initIdentifier(void);

    void              delete_this(void);
    bool              isClassType(int type) { return type >= ANY_NODE; }
    bool              matchNodeClass(int childType);

    virtual int       writeProto(int filedes);
    int               writeProto(int f, const char *urn, 
                                 const char *directoryName,
                                 const char *haveUrl = "",
                                 bool nameInUrl = true);
    int               writeX3dProto(int filedes);

    int               writeProtoArguments(int filedes) const;
    virtual bool      avoidProtoOnPureVrml(void) { return false; }
    virtual bool      canWriteAc3d();
    virtual int       writeAc3d(int filedes, int indent);
    virtual int       writeAc3dMaterial(int filedes, int indent, 
                                          const char *name) { return 0; }
    virtual void      setAc3dMaterialIndex(int index) {}
    virtual int       getIncAc3dMaterialIndex(void) { return 0; }
    virtual void      handleAc3dMaterial(ac3dMaterialCallback callback, 
                                         Scene* scene);

    virtual int       writeRib(int filedes, int indent);

    virtual bool      canWriteLdrawDat();
    virtual int       writeLdrawDat(int filedes, int indent);

    Scene              *getScene() { return m_scene; }

    virtual void        setField(int index, FieldValue *value, 
                                 int containerField = -1);
    virtual FieldValue *getField(int index);

    virtual FieldValue *getUntranslatedField(int index);

    const char       *getTreeLabel(void) { return m_treeLabel; }
    MyString         &getName(void);
    MyString         &getNameOrNewName(void);
    // setName should only be called from Scene::def
    void              setName(const char *name);
    bool              hasName(void);
    bool              needsDEF();

    virtual void      addToConvertedNodes(int writeFlags) 
                          {} /// add to "m_convertedNodes"
    int               getNumConvertedNodes() { return m_convertedNodes.size(); }
    Node             *getConvertedNode(int i) { return m_convertedNodes[i]; }
    void              deleteConvertedNodes(void);

    bool              needExtraJavaClass(void);
    bool              writeJavaOutsideClass(int languageFlag);

    virtual bool      isConvertedInCurveAnimaton(void) 
                          { return m_isConvertedInCurveAnimaton; }
    void              setConvertedInCurveAnimaton(void)
                          { m_isConvertedInCurveAnimaton = true; }      

    void              getGraphPosition(float *x, float *y)
                          { *x = m_graphX; *y = m_graphY; }
    void              setGraphPosition(float x, float y)
                          { m_graphX = x; m_graphY = y; }

    void              getGraphSize(int *width, int *height)
                          { *width = m_graphWidth; *height = m_graphHeight; }
    void              setGraphSize(int width, int height)
                          { m_graphWidth = width; m_graphHeight = height; }

    void              ref() 
                          { 
                          m_refs++;
                          m_needRef = false; 
                          }
    void              unref() 
                          { 
                          if (--m_refs == 0) 
                              delete_this(); 
                          }
    int               getRefs() { return m_refs; }

    virtual void      onAddRoute(Node *s, int out, Node *d, int in) {}

    virtual int       getType() const;
    virtual int       getNodeClass() const;
    virtual int       getProfile(void) const { return -1; }

    virtual const char* getComponentName(void) const { return ""; }
    virtual int       getComponentLevel(void) const { return -1; }

    int               getMaskedNodeClass(void);

    virtual Node     *copy() { return NULL; }
    void              copyData(NodeData &node);

    int               findChild(Node *child, int field);

    int               lookupEventIn(const char *name, bool x3d);
    int               lookupEventOut(const char *name, bool x3d);

    Proto            *getProto() const { return m_proto; }
    void              setProto(Proto *proto) { m_proto = proto; }
    virtual Proto    *getPrimaryProto() const { return getProto(); }
 
    virtual void      addInput(int eventIn, Node *src, int eventOut);
    virtual void      addOutput(int eventOut, Node *dst, int eventIn);
    virtual void      removeInput(int eventIn, Node *src, int eventOut);
    virtual void      removeOutput(int eventOut, Node *dst, int eventIn);
    SocketList        &getInput(int i) { return m_inputs[i]; }
    SocketList        &getOutput(int i) { return m_outputs[i]; }

    virtual void      update();
    virtual void      reInit();

    virtual void      updateTime() {}

    virtual bool      validate(void) { return true; }

    virtual void      setInternal(bool flag) {}

    virtual bool      hasNumbers4kids(void) { return false; }

    virtual bool      maySetDefault(void) { return true; }

    int               getNumParents() { return m_parents.size(); }
    void              copyParentList(Node &node);

    bool              checkValid(Element *field);
    bool              validChild(int field, Node *child);
    bool              validChildType(int field, int childType);
    int               findValidField(Node *child);
    int               findValidFieldType(int childType);
    int               findFirstValidFieldType(int childType);
    bool              checkChildType(int field, int childType, 
                                     bool checkValid = false);
    int               findChildType(int childType);

    bool              getFlag(int flag) 
                          { return (m_flags & (1 << flag)) != 0; }
    void              setFlag(int flag) { m_flags |= (1 << flag); }
    void              setFlagRec(int flag);
    void              clearFlag(int flag) { m_flags &= ~(1 << flag); }
    void              clearFlagRec(int flag);

    bool              isCollapsed() 
                          { return getFlag(NODE_FLAG_COLLAPSED); }

    virtual void      transform() {}
    virtual void      transformBranch() {}
    virtual void      transformForHandle(int handle) {}
    virtual void      preDraw() {}
    virtual void      specialDraw() {}
    virtual void      draw() {}
    virtual void      draw(int pass) {}
    bool              canDraw() { return m_canDraw; }
    virtual void      bind() {}
    virtual void      unbind() {}
    virtual void      bind(GLuint textureId, GLuint textureUnit) {}
    virtual void      load() {}
    virtual void      drawHandles() {}
    virtual void      updateHandles() {}
    virtual void      drawJointHandles(float scale, Node *parent, Node *that)
                          {}
    /// countPolygons() do not count polygons in primitives
    virtual int       countPolygons(void) {return 0;}
    virtual int       countPrimitives(void) {return 0;}
    virtual int       countPolygons1Sided(void) {return 0;}
    virtual int       countPolygons2Sided(void) {return 0;}

    virtual bool      isMesh(void);
    virtual MyMesh   *getMesh(void) { return NULL; }

    virtual bool      hasTwoSides(void) { return false; }
    virtual bool      isDoubleSided(void) { return false; }
    virtual void      toggleDoubleSided(void) {}
    virtual int       getConvexField() { return -1; }
    virtual int       getNormalField() { return -1; }
    virtual int       getTexCoordField() { return -1; }
    virtual int       getSolidField() { return -1; }
    virtual void      flipSide(void) {}

    virtual void      startSetMultiHandles() {}
    virtual void      endSetMultiHandles() {}
    virtual bool      validHandle(int handle) { return true; }
    virtual bool      checkHandle(int handle) { return true; }
    virtual int       getMaxHandle(void) { return 0; }
    virtual Vec3f     getHandle(int handle, int *constraint, int *field)
                          { 
                          *field = -1;
                          return Vec3f(0.0f, 0.0f, 0.0f); 
                          }
    virtual void      setHandle(int /* handle */, const Vec3f & /* v */) {}

    virtual int       getNumVertex(void) { return 0; }
    virtual Vec3f     getVertex(int i) { return Vec3f(); }
    virtual bool      getValidVertex(void) { return false; }

    virtual void      sendEvent(int eventOut, double timestamp, 
                                FieldValue *value);
    virtual void      receiveEvent(int eventIn, double timestamp, 
                                   FieldValue *value);

    int               writeCSendEvent(int f, int indent, int languageFlag, 
                                      EventIn *target, EventOut *source, 
                                      Node *sNode);

    int               writeCCopyEvent(int f, int indent, int languageFlag, 
                                      EventOut *target, EventOut *source, 
                                      Node *sNode);

    int               writeCCopyEvent(int f, int indent, int languageFlag, 
                                      EventIn *target, EventIn *source, 
                                      Node *sNode);

    int               writeCDowithEvent(int f, int indent, int languageFlag, 
                                        const char *target, const char *source,
                                        Node *sNode, bool isArray, int type);

    virtual bool      isInterpolator() { return false; }

    virtual bool      hasProtoNodes(void) { return false; }

    virtual bool      hasControlPoints(void) { return false; }

    //nurbs conversion virtual
    virtual Node     *toNurbs() {return NULL;}
    virtual Node     *toNurbs(int nshell, int narea, int narcs, 
                              int uDegree, int vDegree) {return NULL;}
    virtual Node     *toNurbs(int narcslong,  int narcslat, 
                              int uDegree, int vDegree) {return NULL;}
    virtual Node     *toNurbs(int nAreaPoints, int Degree) {return NULL;}
    virtual Node     *toNurbs(int narcs, int pDegree, float rDegree, 
                              int axis) {return NULL;}
    virtual Node     *toNurbs(int narcs, int pDegree, float uDegree, 
                              Vec3f &P1, Vec3f &P2) {return NULL;}
    virtual Node     *degreeElevate(int newDegree) {return NULL;}
    virtual Node     *degreeElevate(int newUDegree, int newVDegree)
                          {return NULL;}

    virtual int       getDegree(bool u = true) { return -1; }
    virtual void      elongateNurbs(int handle, float *controlPoints) {}

    // mesh conversion virtual
    virtual Node     *toIndexedFaceSet(int meshFlags = MESH_WANT_NORMAL,
                                       bool cleanVertices = true,
                                       bool triangulate = true)
                          { return NULL; }
    virtual bool      canConvertToIndexedFaceSet(void) { return false; }
    virtual bool      shouldConvertToIndexedFaceSet(void) 
                          { return canConvertToIndexedFaceSet(); }

    virtual Node     *toNurbsTrimmedSurface(void) { return NULL; }

    // indexed triangle set conversion virtual
    virtual Node     *toIndexedTriangleSet(int meshFlags = MESH_TARGET_HAS_CCW)
                          { return NULL; }
    virtual bool      canConvertToIndexedTriangleSet(void) { return false; }
    virtual bool      shouldConvertToIndexedTriangleSet(void) 
                         { return canConvertToIndexedTriangleSet(); }

    // triangle set conversion virtual
    virtual Node     *toTriangleSet(int meshFlags = MESH_TARGET_HAS_CCW) 
                          { return NULL; }
    virtual bool      canConvertToTriangleSet(void) { return false; }
    virtual bool      shouldConvertToTriangleSet(void)
                          { return canConvertToTriangleSet(); }

    virtual bool      canSimpleTriangulate(void) { return false; }
    void              simpleTriangulation(void) {}

    // chain conversion virtual
    virtual Node     *toIndexedLineSet(void) { return NULL; }
    virtual bool      canConvertToIndexedLineSet(void) { return false; }

    // extrusion conversion virtual
    virtual Node     *toExtrusion(void) { return NULL; }
    virtual bool      canConvertToExtrusion(void) { return false; }

    // convertion to interpolators
    virtual bool      canConvertToPositionInterpolator(void) 
                          { return false; }
    virtual Node     *toPositionInterpolator(void)
                          { return NULL; }

    virtual bool      canConvertToOrientationInterpolator(void) 
                          { return false; }
    virtual Node     *toOrientationInterpolator(Direction direction)
                          { return NULL; }

    virtual bool      canConvertToPositionAndOrientationInterpolators(void) 
                          { return false; }
    virtual void      toPositionAndOrientationInterpolators(NodeList *nodes)
                          { return; }

    // other conversions
    virtual Node     *toCurveAnimation(void) { return NULL; }

    virtual void      setMeshDirty(void) {}

    // texture glColorMode
    virtual int       textureGlColorMode(void) { return -1; }

    virtual bool      isMeshBasedNode(void) { return false; }
    virtual bool      isChainBasedNode(void) { return false; }

    // compare content
    bool              isEqual(Node* node);
    friend bool       isEqual(Node* node);

    bool              isEqualCopy(Node* node);
    MyString          newEventName(int typeEnum, bool out);

    bool              hasRoute(SocketList socketlist);

    virtual bool      isInvalidChildNode(void)
                          {
                          // true if node may not be part of a MFNode field
                          // of a other node (or at root of scenegraph)
                          return false;
                          }

    virtual bool      isProgrammableShaderObject(void) { return false; }

    virtual bool      isNodeWithAdditionalEvents(void) 
                          { return isProgrammableShaderObject(); }

    virtual int       getX3dVersion(void) { return -1; }
    virtual bool      isCoverNode(void) { return false; }
    virtual bool      hasCoverFields(void) { return false; }
    bool              hasCoverDefault() { return hasDefault(FF_COVER_ONLY); }
    virtual bool      isKambiNode(void) { return false; }
    virtual bool      hasKambiFields(void) { return false; }
    bool              hasKambiDefault() { return hasDefault(FF_KAMBI_ONLY); }
    bool              hasX3dDefault() { return hasDefault(FF_X3D_ONLY); }
    bool              hasDefault(int flag);

    virtual void      setNormalFromMesh(Node *nnormal) {}
    virtual void      setTexCoordFromMesh(Node *ntexCoord) {}

    virtual bool      isTransparent(void) { return false; }
    virtual float     getTransparency(void) { return 0; }

    virtual bool      isFlat(void) { return false; }

    virtual NodeCoordinate *getCoordinateNode(void) { return NULL; }

    virtual NodeColor *getColorNode() { return NULL; }
    virtual bool      hasColor(void) { return false; }
    virtual bool      hasColorRGBA(void) { return false; }
    virtual NodeColorRGBA *getColorRGBANode(void) { return NULL; }

    virtual bool      hasX3domOnOutputChange(void) { return false; }
    virtual bool      hasX3domOnclick(void) { return false; }

    void              handleIs(void);
    Node             *getIsNode(int nodeIndex);
    void              setScene(Scene *scene) { m_scene = scene; }
    bool              isInsideProto(void) { return m_insideProto != NULL; }
    void              setOutsideProto(Proto *proto) { m_insideProto = proto; }
    Proto            *getOutsideProto(void) { return m_insideProto; }

    NodePROTO        *getNodePROTO(void) { return m_nodePROTO; }

    void              removeChildren(void);
    bool              isDefault(int field) const;
    bool              isDefaultAngle(int field) const;
    bool              hasOutput(const char* routename);
    bool              hasOutputs(void);
    bool              hasOutputsOrIs(void);
    bool              hasInput(const char* routename);
    bool              hasInputs(void);
    bool              hasInputsOrIs(void);
    bool              hasInputsIs(void);
    int               writeRoutes(int f, int indent);
    void              removeRoutes(void);

    void              generateTreeLabel(void);

    void              setDefault(void);
    virtual bool      canMoveTo(int direction) { return false; }

    virtual Node     *convert2X3d(void);
    virtual Node     *convert2Vrml(void);

    virtual void      setupFinished(void) {}

    virtual int       getChildrenField(void) { return -1; }

    virtual const char *getVariableName(void);
    void              setVariableName(const char *name);
    void              unSetVariableName(void) { m_variableName = ""; }

    const char       *getClassName(void);

    virtual bool      isDynamicFieldsNode(void) { return false; }

    virtual bool      isPROTO(void) const;

    virtual NodeList *getLoadedNodes(void) { return NULL; }

    virtual int       repairField(int field);
    virtual int       translateField(int field) const;

    int               writeCVariable(int f, int languageFlag);
    int               getCounter4SceneTreeView(void)
                          { return m_counter4SceneTreeView; }
    void              addCounter4SceneTreeView(void)
                          { m_counter4SceneTreeView++; }
    void              setCounter4SceneTreeViewToZero(void )
                          { m_counter4SceneTreeView = 0; }
    long              getId(void);
    void              setId(long id);

    // for Node*Viewpoint
    virtual Vec3d     getPosition() { return Vec3d(); }
    virtual Quaternion getOrientation() { return Quaternion(); } 
    virtual void      setPosition(Vec3d pos) {}
    virtual void      setOrientation(Quaternion quat)  {}
    virtual SFFloat  *fov() { return NULL; };
    virtual void      getMatrix(float* matrix) {}
    void              apply(bool useStereo = false) {}
    bool              getWritten(void) { return m_written; }
    void              setWritten(bool flag) { m_written = flag; }

    bool              isInAlreadyWrittenEventOuts(int eventOut, 
                                                  int numOutput,
                                                  Node *protoNode)
                          {
                          for (long i = 0; 
                              i < m_alreadyWrittenEventOuts.size(); i++)
                              if ((m_alreadyWrittenEventOuts[i].eventOut
                                   == eventOut) &&                   
                                  (m_alreadyWrittenEventOuts[i].numOutput
                                   == numOutput) &&
                                  (m_alreadyWrittenEventOuts[i].protoNode
                                   == protoNode)
                                 ) 
                                 return true;
                          return false;
                          }

    void              appendToAlreadyWrittenEventOuts(int eventOut, 
                                                      int numOutput,
                                                      Node *protoNode)
                          {
                          EventOutData data;
                          data.eventOut = eventOut;
                          data.numOutput = numOutput;
                          data.protoNode = protoNode;
                          m_alreadyWrittenEventOuts.append(data);
                          }
    void              removeAlreadyWrittenEventOuts(void)
                          { m_alreadyWrittenEventOuts.resize(0); }

    bool              hasRouteForProcessEvent(void);

    const char *      getX3domId(void) 
                          { 
                          if (strlen(m_x3domId) > 0) 
                              return m_x3domId; 
                          return m_name; 
                          }
    void              setX3domId(const char *id) { m_x3domId = strdup(id); }
    void              setX3domOnOutputChange(const char *function) 
                          { m_x3domOnOutputChange = strdup(function); }
    void              setX3domOnClick(const char *function) 
                          { m_x3domOnClick = strdup(function); }

protected:
    const char       *searchIsName(int i, int type);

    int               writeIs(int f, int indent, 
                              const char *name, const char * isName);
    virtual int       writeFields(int f, int indent);
    int               writeField(int f, int indent, int i, 
                                 bool script = false);
    int               writeEventIn(int f, int indent, int i, 
                                   bool eventName = false);
    int               writeEventOut(int f, int indent, int i, 
                                    bool eventName = false);

    int               writeEventInStr(int f);
    int               writeEventOutStr(int f);
    int               writeFieldStr(int f);
    int               writeExposedFieldStr(int f);

    int               writeXmlIs(int f, int indent, 
                                 const char *name, const char *isName);
    int               writeXmlProtoInstanceField(int f, int indent, 
                                                 const char *name, 
                                                 FieldValue *value); 
    int               writeXmlFields(int f, int indent, int when,
                                     int containerField = -1,
                                     bool avoidUse = false);
    int               writeXmlField(int f, int indent, int i, int when,
                                    bool script = false,
                                    int containerField = -1,
                                    bool avoidUse = false);
    int               writeXmlEventIn(int f, int indent, int i, int when,
                                      bool eventName = false);
    int               writeXmlEventOut(int f, int indent, int i, int when,
                                       bool eventName = false);

    void              addIsElement(Node *node, int field, int elementType,
                                   Proto *origProto, int origField, 
                                   int flags = 0);

protected:
    Scene            *m_scene;
    ParentArray       m_parents;
    MyString          m_name;
    MyString          m_treeLabel;
    MyString          m_variableName;
    NodeArray         m_convertedNodes;
    int               m_refs;
    bool              m_needRef;
    int               m_flags;
    Proto            *m_proto;
    FieldValue      **m_fields;
    int               m_numFields;
    int               m_numEventIns;
    int               m_numEventOuts;
    SocketList       *m_inputs;
    SocketList       *m_outputs;
    float             m_graphX, m_graphY;
    int               m_graphWidth, m_graphHeight;
    long              m_identifier;
    long              m_identifierCopy;
    Proto            *m_insideProto;
    NodePROTO        *m_nodePROTO;
    bool              m_canDraw;
    bool              m_isConvertedInCurveAnimaton;
    MyArray<EventIn *>      m_isEventIns;
    MyArray<EventOut *>     m_isEventOuts;
    MyArray<Field *>        m_isFields;
    MyArray<ExposedField *> m_isExposedFields;
    int               m_counter4SceneTreeView;
    bool              m_written;
    MyArray<EventOutData> m_alreadyWrittenEventOuts;
    const char *      m_x3domId;
    const char *      m_x3domOnOutputChange;
    const char *      m_x3domOnClick;
};
   

typedef bool (*DoWithSimilarBranchCallback)(Node *node, Node *similarNode, 
                                            void *data);
typedef void (*DoWithAllElementsCallback)(Element *element, void *data);

class Node : public NodeData
{
public:
                      Node(Scene *scene, Proto *proto);
                      Node(Node &node);
                      Node(Node node, Proto *proto);

    virtual          ~Node();

    void              addParent(Node *parent, int index);
    void              removeParent(void);
    bool              hasParent(void)
                          { 
#ifdef HAVE_NULL_COMPARE
                          if (this == NULL)
                              return false;
#endif
                          if (getNumParents() < 0) 
                              return false;
                          if (m_geometricParentIndex == -1)
                              return false;
                          if (m_geometricParentIndex > getNumParents())
                              return false;
                          return ((Parent *)this)->m_node != NULL;
                          }
    Node             *getParent(void) 
                          { 
                          if (m_parents.size() == 0)
                              return NULL;
                          return getParent(m_geometricParentIndex);
                          }
    bool              hasParentOrProtoParent(void)
                          { return (getParentOrProtoParent() != NULL); }
    Node             *getParentOrProtoParent(void);
    int               getParentField(void)
                          { return  getParentField(m_geometricParentIndex); }

    Node             *getParent(int index) 
                          { 
                          return index == -1 ? NULL : 
                                 ((Parent)this->m_parents[index]).m_node; 
                          }
    int               getParentField(int index)
                          { 
                          return index == -1 ? -1 : m_parents[index].m_field;
                          }

    int               getParentFieldOrProtoParentField(void);
    Node             *searchParent(int nodeType); 
    Node             *searchParentField(int parentfield); 
    int               getParentIndex(void);
    FieldValue       *getParentFieldValue(void);
    NodeList          getParents(void);

    virtual FieldValue *getField(int index) const;

    bool              hasBranchInputs(void);

    virtual int       write(int filedes, int indent, bool avoidUse = false);
    virtual int       writeXml(int filedes, int indent, 
                               int containerField = -1, bool avoidUse = false);

    bool              isInScene(Scene* scene);
    virtual void      addFieldNodeList(int index, NodeList *value,
                                      int containerField = -1);
    bool              hasAncestor(Node *node);
    virtual int       getAnimatedNodeField(int field) { return -1; }
    virtual bool      supportAnimation(void);
    virtual bool      supportCurveAnimation(void) { return false; }
    bool              supportInteraction(void);
    virtual Colored  *getColored() { return NULL; }
    virtual int       getAnimationCommentID(void) { return -1; }
    virtual int       getInteractionCommentID(void) { return -1; }
    virtual bool      isInvalidChild(void);
    virtual bool      hasBoundingBox(void) { return false; }
    virtual void      setBoundingBox(void) {}
    virtual Vec3f     getMinBoundingBox(void);
    virtual Vec3f     getMaxBoundingBox(void);
    Vec3f             getBboxSize(void);
    Vec3f             getBboxCenter(void);
    virtual void      flip(int index) {}
    virtual void      swap(int fromTo) {}
    virtual bool      showFields(); 
    virtual bool      isEXTERNPROTO(void) { return false; }
    // for Hanim
    virtual void      applyJoint(int skinNum, MyMesh *mesh, MFVec3f *vertices,
                                 Node *parent) {}
    virtual bool      getMatrixDirty(void) { return false; }

    void              appendTo(NodeList* nodelist);
    void              appendComment(Node *node);

    void              doWithParents(DoWithNodeCallback callback, void *data,
                                    bool searchInRest = true, 
                                   bool callSelf = true);
    void              doWithSiblings(DoWithNodeCallback callback, void *data,
                                     bool searchInRest = true, 
                                     bool callSelf = true);
    void              doWithNextSiblings(DoWithNodeCallback callback, 
                                         void *data, bool searchInRest = true, 
                                         bool callSelf = true);

    bool              doWithBranch(DoWithNodeCallback callback, void *data,
                                   bool searchInRest = true, 
                                   bool skipBranch = false,
                                   bool skipProto = false,
                                   bool callSelf = true,
                                   bool skipInline = true,
                                   bool searchInConvertedNodes = false);

    bool              doWithSimilarBranch(DoWithSimilarBranchCallback 
                                          callback, Node *similarNode, 
                                          void *data);

    void              doWithAllElements(DoWithAllElementsCallback callback, 
                                        void *data);

    parentFlag        getParentFlag(int index) 
                          { return m_parents.get(index).m_parentFlag; }

    void              setParentFlag(int index, parentFlag flag) 
                          { m_parents[index].m_parentFlag = flag; }

    bool              isFirstUSE(void)
                          { 
                          if (isPROTO())
                              return m_geometricParentIndex == 1; 
                          else
                              return m_geometricParentIndex == 0; 
                          }
    bool              isUnused(void) { return m_geometricParentIndex == -1; }
    bool              getIsUse(void) { return m_isUse; }
    void              setIsUse(bool flag) { m_isUse = flag; }

    void              copyChildrenTo(Node *copyedNode, 
                                     bool copyNonNodes = false);
    void              copyOutputsTo(Node *copyedNode);
    void              copyInputsTo(Node *copyedNode);

    int               getSiblingIndex(void);
    int               getPrevSiblingIndex(void);
    int               getNextSiblingIndex(void);

    Node             *getPrevSibling(void);
    Node             *getNextSibling(void);

    virtual bool      isJoint(void) { return false; }
    virtual bool      isHumanoid(void) { return false; }

    virtual int       writeC(int filedes, int languageFlag);
    int               writeCDataAsFunctions(int filedes, int languageFlag,
                                            bool cont = false);
    int               writeCDataAsClasses(int filedes, int languageFlag);
    int               writeCDataFunction(int filedes, int languageFlag,
                                         bool forward, bool cont = false);
    int               writeCDataFunctionFields(int filedes, int languageFlag,
                                               bool forward, bool cont);
    int               writeCElement(int f, int elementType, int i, 
                                    int languageFlag, bool nodeFlag);
    int               writeCElementFunction(int f, int elementType, 
                                            int i, int languageFlag, 
                                            bool nodeFlag, bool cont = false);
    int               writeCElementClass(int f, int elementType, int i, 
                                         int languageFlag, bool nodeFlag);
    int               writeCAndFollowRoutes(int f, int indent, 
                                            int languageFlag, 
                                            bool writeSensorNodes,
                                            const char *eventName);
    int               writeCProcessEvent(int f, int indent, int languageFlag, 
                                         const char *eventName);
    int               writeCEndSendEvent(int f, int indent, int languageFlag);
    int               writeCProcessEventCallback(int f, int languageFlag);
    virtual int       writeCDynamicNodeCallback(int f, int languageFlag) 
                          { return 0; }
    int               writeCInstallDynamicNodeCallback(int f, int languageFlag,
                                                       Proto *proto);
    int               writeCGetParent(int filedes, int languageFlag);

    virtual void      writeCWarning(void) {}

    virtual bool      canWriteCattGeo();
    virtual int       writeCattGeo(int filedes, int indent);
    int               getGeometricParentIndex(void)
                          { return m_geometricParentIndex; }
    Path             *getPath();
    bool              isDeepInsideProto(void);

    virtual char     *buildExportMaterialName(const char *name)
                          { return mystrdup(name); }
    char*             getExportMaterialName(const char *defaultName);

    int               getContainerField(void); 
    void              setContainerField(int containerField);

    Node             *getProtoParent(void) { return m_protoParent; }
    void              setProtoParent(Node *n) { m_protoParent = n; }
    void              setNodePROTO(NodePROTO *node);
    NodeHAnimHumanoid *getHumanoid();
    bool              hasNeedRef(void) { return m_needRef; }
    void              setNeedRef(void) { m_needRef = true; }
    void              unsetNeedRef(void) { m_needRef = false; }

    // limit USE to 0xffff links to find out invalid nodes
    bool              isValid(void) 
                          { 
                          if (m_refs < 0/* || m_refs > 0xffff*/) 
                              return false;
                          return true;
                          }

protected:
    int               m_geometricParentIndex;
    NodeList         *m_commentsList;
    int               m_numberCDataFunctions;
    int               m_containerField;
    Node             *m_protoParent;
    bool              m_isUse;
};

