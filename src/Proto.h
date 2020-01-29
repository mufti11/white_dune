/*
 * Proto.h
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

# include "Array.h"
# include "Map.h"
# include "MyString.h"
# include "Node.h"

#define INVALID_INDEX (-1)

class Element;
class EventIn;
class EventOut;
class Field;
class ExposedField;
class FieldValue;
class Scene;
class Node;

#include "ProtoMacros.h"
#include "StringArray.h"
#include "NodeList.h"
#include "InterfaceArray.h"

class NodeScript;
class ScriptDialog;

enum UrlSchemas {
    URL_NULL,
    URL_VRML97_AMENDMENT1,
    URL_X3D,
    URL_SCRIPTED_NODES,
    URL_COVER,
    URL_KAMBI,
    URL_EXPORT_CONTAINER
};

class RouteInfo {
public:
                    RouteInfo(Node *s, int out, Node *d, int in)
                    { src = s; eventOut = out; dest = d; eventIn = in; }
                    RouteInfo()
                    { src = NULL; eventOut = -1; dest = NULL; eventIn = -1; }

    Node           *src;
    Node           *dest;
    int             eventOut;
    int             eventIn;
};


class Proto {
public:

    // unlike other nodes, script nodes need to change their Proto dynamically
    friend class DynamicFieldsNode;

                          Proto(Scene *scene, const MyString &name);
                          // create a Proto as a X3D/Cover extension
                          Proto(Scene *scene, Proto *proto, int extensionFlag);
                          Proto() {}
    virtual              ~Proto();

    FieldIndex metadata;

    int metadata_Field(void) const { return metadata; }

    void                  protoInitializer(Scene *scene, const MyString &name);

    virtual bool          isDynamicFieldsProto(void) { return false; }

    void                  finishEvents(void);

    bool                  avoidElement(Element *element, bool x3d);
    bool                  avoidElement(Element *element, int flag);

    virtual int           getType() const { return -1; }
    virtual int           getNodeClass() const { return CHILD_NODE; }

    bool                  matchNodeClass(int childType) const;

    virtual Node         *create(Scene *scene);
    virtual bool          needUpdate(void) { return false; }

    EventIn              *getEventIn(int index) const 
                             { return m_eventIns[index]; }
    EventOut             *getEventOut(int index) const 
                             { return m_eventOuts[index]; }
    Field                *getField(int index) const { return m_fields[index]; }
    ExposedField         *getExposedField(int index) const 
                             { return m_exposedFields[index]; }
    Element              *getElement(int elementType, int index) const;

    int                   getNumFields() const { return m_fields.size(); }
    int                   getNumEventIns() const { return m_eventIns.size(); }
    int                   getNumEventOuts() const { return m_eventOuts.size(); }
    int                   getNumExposedFields() const 
                             { return m_exposedFields.size(); }

    int                   getExposedOfField(Field *field);
    int                   getFieldOfExposed(ExposedField *field);

    int                   lookupEventIn(const MyString &name, 
                                        bool x3d = false) const;
    int                   lookupEventOut(const MyString &name, 
                                         bool x3d = false) const;
    int                   lookupField(const MyString &name, 
                                      bool x3d = false) const;
    int                   lookupExposedField(const MyString &name,
                                             bool x3d = false) const;

    int                   lookupIsEventIn(const char *name, 
                                          int elementType = -1) const;
    int                   lookupIsExposedField(const char *name,
                                               int elementType = -1) const;

    int                   lookupIsEventIn(Node *node, int eventIn,
                                          int elementType = -1) const;
    int                   lookupIsEventOut(Node *node, int eventOut,
                                           int elementType = -1) const;
    int                   lookupIsField(Node *node, int field) const;
    int                   lookupIsExposedField(Node *node, int field) const;

    int                   getNumIsMSNodes(void) const;
    Node                 *getIsMSNode(int numNode) const;
    int                   getIsMSNodeField(int numNode) const;

    virtual const MyString &getName(bool x3d) const { return m_name; }

    bool                  canWriteElement(Element *element, bool x3d) const;
    int                   write(int filedes, int indent, int flags) const;
    int                   writeEvents(int filedes, int indent, int flags) const;

    virtual int           writeCDeclaration(int filedes, int languageFlag);
    int                   writeCDeclarationEventIn(int f, int i, 
                                                   int languageFlag);
    int                   writeCDeclarationEventOut(int f, int i, 
                                                    int languageFlag);
    int                   writeCDeclarationField(int f, int i, 
                                                 int languageFlag);
    virtual int           writeCExtraFields(int f, int languageFlag);
    int                   writeCConstructorField(int f, int i, 
                                                 int languageFlag);
    int                   writeCCallTreeField(int f, int i, 
                                              const char *treeFunctionName, 
                                              int languageFlag,
                                              const char *functionName = NULL,
                                              bool useObject = false);

    void                  buildExportNames(const char *nodeName = NULL);
    const char           *getClassName(void)
                             { return m_className; }

    MyString              buildCallbackClass(const char* name);
    MyString              buildCallbackName(const char* name);

    const char           *getRenderCallbackClass(void)
                             { return m_renderCallbackClass; }
    const char           *getRenderCallbackName(void)
                             { return m_renderCallbackName; }

    const char           *getTreeRenderCallbackClass(void)
                             { return m_treeRenderCallbackClass; }
    const char           *getTreeRenderCallbackName(void)
                             { return m_treeRenderCallbackName; }

    const char           *getCreateNormalsCallbackClass(void)
                             { return m_createNormalsCallbackClass; }
    const char           *getCreateNormalsCallbackName(void)
                             { return m_createNormalsCallbackName; }

    const char           *getDoWithDataCallbackClass(void)
                             { return m_doWithDataCallbackClass; }
    const char           *getDoWithDataCallbackName(void)
                             { return m_doWithDataCallbackName; }

    const char           *getTreeDoWithDataCallbackClass(void)
                             { return m_treeDoWithDataCallbackClass; }
    const char           *getTreeDoWithDataCallbackName(void)
                             { return m_treeDoWithDataCallbackName; }

    const char           *getProcessEventCallbackClass(void)
                             { return m_processEventCallbackClass; }
    const char           *getProcessEventCallbackName(void)
                             { return m_processEventCallbackName; }

    virtual int           translateField(int field) const { return field; }

    virtual const MyString &getCName(bool x3d) const { return m_cName; }

    int                   addElement(Element *element);
    int                   addOrUpdateElement(Element *element);
    int                   addField(Field *field);
    int                   addExposedField(ExposedField *exposedField);

    void                  deleteElements(void);

    void                  define(Node *primaryNode, NodeList *nodes);
    int                   getNumNodes(void) { return m_protoNodes.size(); }
    Node                 *getNode(long i) 
                             { 
                             if (i >= m_protoNodes.size())
                                 return NULL;
                             return m_protoNodes[i]; 
                             }
    void                  addNode(Node *node) 
                             {
                             m_protoNodes.append(node); 
                             }
    void                  removeNode(int i); 
    void                  removeFromIs(Node *node);
    void                  compareToIsNodes(Node *node);
    void                  addURLs(FieldValue *urls);
    void                  addURLs(int urlSchema);
    FieldValue *          getURLs(void) { return m_urls; }
    bool                  isInternUrl(void) { return m_isInternUrl; }

    bool                  isX3dExtensionProto(void) 
                             { return isExtensionProto(FF_X3D_ONLY); }
    bool                  isCoverExtensionProto(void) 
                             { return isExtensionProto(FF_COVER_ONLY); }
    bool                  isKambiExtensionProto(void) 
                             { return isExtensionProto(FF_KAMBI_ONLY); }
    bool                  isX3domExtensionProto(void) 
                             { return isExtensionProto(FF_X3DOM_ONLY); }
    bool                  isExtensionProto(int flag);
    bool                  isMismatchingProto(void);
    FieldValue           *getUrls(void) { return m_urls; }
    bool                  isExternProto(void) { return m_urls != NULL; }
    bool                  checkIsExtension(Element *element, int flag);

    virtual bool          isCoverProto(void) { return false; }
    virtual bool          isKambiProto(void) { return false; } 
    virtual bool          isX3domProto(void) { return false; } 
    virtual bool          isScriptedProto(void) { return false; }
    virtual bool          isScriptedExternProto(void) { return false; }
    virtual bool          isX3dInternalProto(void) { return false; }

    void                  setScene(Scene *scene) { m_scene = scene; }
    Scene                *getScene(void) { return m_scene; }

    void                  convert2X3d(void);
    void                  convert2Vrml(void); 

    bool                  isLoading(void) { return m_loading; }
    void                  setLoading(bool flag) { m_loading = flag; }
    bool                  isLoaded(void)
                             { return m_loaded && (m_protoNodes.size() > 0); }
    void                  setLoaded(bool flag) { m_loaded = flag; }

    bool                  isDefined(void) { return m_defined; } 

    void                  setAppinfo(const MyString& appinfo) 
                             { m_appinfo = appinfo; } 
    MyString              getAppinfo() const 
                             { return m_appinfo; } 
    void                  setDocumentation(const MyString& documentation) 
                             { m_documentation = documentation; } 
    MyString              getDocumentation() const 
                             { return m_documentation; } 

    bool                  isDynamicProto(void) { return m_dynamicProto; } 
    void                  setDynamicProto(void) { m_dynamicProto = true; } 

    virtual bool          isMesh(void) { return false; }
    virtual bool          isExportTargetMesh(void) { return false; }
    virtual bool          isWonderlandExported(void) 
                             { 
                             if (isMesh())
                                 return true;
                             return false; 
                             }

    bool                  isWriteCDeclarationWritten()
                              { return m_writeCDeclarationWritten; }
    void                  setWriteCDeclarationWritten(bool flag)
                              { m_writeCDeclarationWritten = flag; } 

    bool                  isInScene() 
                              { 
#ifdef HAVE_NULL_COMPARE
                              if (this == NULL) 
                                  return false; 
#endif
                              return m_isInScene; 
                              }
    void                  setIsInScene(bool flag) { m_isInScene = flag; }

    virtual bool          isDeclaredInRwd_h() { return false; }

    bool                  isInUse(void) { return m_inUse; }
    void                  setInUse(bool flag) { m_inUse = flag; }

    bool                  fromProtoLibrary() { return m_fromProtoLibrary; }
    void                  setFromProtoLibrary() { m_fromProtoLibrary = true; }

    bool                  showFields(bool x3d);
    virtual bool          showFields() { return false; }
    bool                  hasNumbers4kids(void);

    InterfaceArray       *getInterfaceData(void) 
                              {
                              return &m_interface;
                              } 
    InterfaceArray       *getInterfaceData(bool protoFlag) 
                              {
                              buildInterfaceData(protoFlag);
                              return &m_interface;
                              } 
    void                  buildInterfaceData(bool protoFlag); 

    int                   addField(int fieldType, const MyString &name,
                                   FieldValue *defaultValue = NULL,
                                   FieldValue *min = NULL,
                                   FieldValue *max = NULL);
    int                   addExposedField(int fieldType, const MyString &name,
                                          FieldValue *defaultValue = NULL,
                                          FieldValue *min = NULL,
                                          FieldValue *max = NULL,
                                          const MyString &x3dName = "");
    int                   addEventIn(int fieldType, const MyString &name);
    int                   addEventOut(int fieldType, const MyString &name);

    void                  getGraphPosition(float *x, float *y) const
                             { *x = m_graphX; *y = m_graphY; }
    void                  setGraphPosition(float x, float y)
                             { m_graphX = x; m_graphY = y; }

    void                  getGraphSize(int *width, int *height) const
                             { *width = m_graphWidth; *height = m_graphHeight; }
    void                  setGraphSize(int width, int height)
                             { m_graphWidth = width; m_graphHeight = height; }
    void                  deleteField(int i);
    void                  deleteEventIn(int i);
    void                  deleteEventOut(int i);
    void                  deleteExposedField(int i);
    void                  setIsNodeIndex(void);

    double                getUnitLength(void) { return m_unitLength; }
    double                getUnitAngle(void) { return m_unitAngle; }

    void                  setUnitLength(double u) { m_unitLength = u; }
    void                  setUnitAngle(double u) { m_unitAngle = u; }

    virtual bool          isCRouteSource(void) { return false; } 

    bool                  hasTimeSensor(void)
                              { return m_hasTimeSensor; }

    bool                  specialInit(void) 
                              { 
                              static bool init = true; 
                              if (init) {
                                  init = false;
                                  return true;
                              }
                              return false;
                              }

protected:
    Proto                *copy() const { return new Proto(*this); }
    int                   addField(int fieldType, const MyString &name,
                                   FieldValue *defaultValue, int nodeType);
    int                   addField(int fieldType, const MyString &name,
                                   FieldValue *defaultValue, int flags,
                                   const char **strings);
    int                   addEventIn(int fieldType, const MyString &name,
                                     int flags);
    int                   addEventIn(int fieldType, const MyString &name,
                                     int flags, int field);
    int                   addEventOut(int fieldType, const MyString &name,
                                      int flags);
    int                   addExposedField(int fieldType, const MyString &name,
                                          FieldValue *defaultValue, 
                                          int nodeType,
                                          const MyString &x3dName = "");
    int                   addExposedField(int fieldType, const MyString &name,
                                          FieldValue *defaultValue, int flags,
                                          const char **strings);
    int                   addExposedField(int fieldType, const MyString &name, 
                                          FieldValue *defaultValue, 
                                          FieldValue *min, FieldValue *max,
                                          int nodeType, int flags, 
                                          const char **strings,
                                          const MyString &x3dName);
    int                   lookupSimpleEventIn(const MyString &name, 
                                              bool x3d) const;
    int                   lookupSimpleEventOut(const MyString &name,
                                               bool x3d) const;
    void                  setFieldFlags(int index, int flags);
 
protected:
    Scene                *m_scene;
    MyString              m_name;
    MyArray<EventIn *>      m_eventIns;
    MyArray<EventOut *>     m_eventOuts;
    MyArray<Field *>        m_fields;
    MyArray<ExposedField *> m_exposedFields;

    MyString              m_appinfo;
    MyString              m_documentation;

    InterfaceArray        m_interface;

    bool                  m_writeCDeclarationWritten; 
    bool                  m_isInScene;

    bool                  m_showFieldsInit;
    bool                  m_showFields;

    bool                  m_numbers4Kids;
    bool                  m_numbers4KidsInit;

    // for PROTO's:
    MyArray<Node *>       m_protoNodes;
    int                   m_nodeIndex;
    FieldValue           *m_urls;
    bool                  m_isInternUrl;
    bool                  m_loaded;
    bool                  m_loading;
    bool                  m_defined;
    StringArray           m_nameOfFieldsWithDefaultValue;

    float                 m_graphX, m_graphY;
    int                   m_graphWidth, m_graphHeight;

    MyString              m_cName;
    MyString              m_className;

    MyString              m_renderCallbackClass;
    MyString              m_renderCallbackName;

    MyString              m_treeRenderCallbackClass;
    MyString              m_treeRenderCallbackName;

    MyString              m_createNormalsCallbackClass;
    MyString              m_createNormalsCallbackName;

    MyString              m_doWithDataCallbackClass;
    MyString              m_doWithDataCallbackName;

    MyString              m_treeDoWithDataCallbackClass;
    MyString              m_treeDoWithDataCallbackName;

    MyString              m_processEventCallbackClass;
    MyString              m_processEventCallbackName;

    bool                  m_dynamicProto;
    bool                  m_inUse;

    bool                  m_fromProtoLibrary;
    double                m_unitLength;
    double                m_unitAngle;

    bool                  m_hasTimeSensor;
};

#include "DynamicFieldsNode.h"

class NodePROTO : public DynamicFieldsNode
{
public:
                        NodePROTO(Scene *scene, Proto *proto);
    void                createPROTO(bool copy = true);

    virtual int         getX3dVersion(void) const { return 0; }

    virtual bool        isPROTO(void) const { return true; }

    virtual int         write(int filedes, int indent, bool avoidUse);

    virtual int         writeXml(int filedes, int indent, 
                                 int containerField = -1, 
                                 bool avoidUse = false);

    virtual int         writeFields(int filedes, int indent)
                            { return Node::writeFields(filedes, indent); }

    virtual int         writeProto(int f)  { return Node::writeProto(f); }

    virtual int         writeCDynamicNodeCallback(int f, int languageFlag) 
                        { 
                            return Node::writeCDynamicNodeCallback(f, 
                                       languageFlag);
                        }

    virtual int         getType() const; 
    virtual int         getNodeClass() const;
    virtual int         getProfile(void) const;
    virtual Node       *copy() const { return new NodePROTO(*this); }

    virtual Proto      *getPrimaryProto() const
                           { return m_indexedNodes[0]->getProto(); }

    virtual void        preDraw();
    virtual void        draw();
    virtual void        draw(int pass);

    virtual void        update();
    virtual void        reInit(void);

    virtual FieldValue *getField(int index) const;
    virtual void        setField(int index, FieldValue *value, int cf = -1);
    void                receiveProtoEvent(int eventOut, double timestamp, 
                                         FieldValue *value);
    virtual void        sendEvent(int eventOut, double timestamp, 
                                  FieldValue *value);
    virtual void        receiveEvent(int eventIn, double timestamp, 
                                     FieldValue *value);
    Node               *getIsNode(int nodeIndex);
    void                appendToIndexedNodes(Node *node);
    int                 getNumIndexedNodes(void) 
                            { return m_indexedNodes.size(); }
    Node               *getIndexedNode(int i) { return m_indexedNodes[i]; }
    virtual bool        isJoint(void) { return m_jointRotationField != -1; }
    virtual bool        isHumanoid(void) { return m_isHumanoid; }
    virtual void        drawHandles(void);
    virtual int         countPolygons(void);
    virtual int         countPrimitives(void);
    virtual int         countPolygons1Sided(void);
    virtual int         countPolygons2Sided(void);
    virtual Vec3f       getHandle(int handle, int *constraint, int *field);
    virtual void        setHandle(int handle, const Vec3f &v);
    virtual void        transform();
    virtual bool        isEXTERNPROTO(void);
    void                getComponentsInBranch(DoWithNodeCallback callback,
                                              void *data);
    virtual bool        canWriteAc3d();
    virtual int         writeAc3d(int filedes, int indent);
    virtual void        handleAc3dMaterial(ac3dMaterialCallback callback, 
                                           Scene* scene);
    virtual int         writeRib(int filedes, int indent);

    virtual bool        canWriteCattGeo();
    virtual int         writeCattGeo(int filedes, int indent);

    virtual bool        hasProtoNodes(void) 
                            { return m_indexedNodes.size() > 0; }
    Node               *getProtoRoot(void);

    Node               *getProtoNode(int i) { return m_nodes.get(i); }
    int                 getNumProtoNodes(void) { return m_nodes.size(); }

    bool                isLoaded(void) 
                             { return getProto()->isLoaded() && 
                                      (m_indexedNodes.size() > 0); }

    virtual bool        showFields() { return true; }

protected:
    NodeList            m_nodes;
    MyArray<Node *>     m_indexedNodes;
    int                 m_jointRotationField;
    bool                m_isHumanoid;
};

typedef MyArray<Proto *>  ProtoArray;
int getMaskedNodeClass(int nodeClass);
bool matchNodeClass(int nodeType, int childType, bool repeat = true);

class WonderlandExportProto : public Proto {
public:
                        WonderlandExportProto(Scene *scene, const char *name) : 
                                              Proto(scene, name) {}

    virtual bool        isWonderlandExported(void) { return true; }
};

