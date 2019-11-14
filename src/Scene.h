/*
 * Scene.h
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

#ifndef _SCENE_H
#define _SCENE_H

#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef _ARRAY_H
#include "Array.h"
#endif

#ifndef _MAP_H
#include "Map.h"
#endif

#ifndef _STACK_H
#include "Stack.h"
#endif

#ifndef _COMMAND_LIST_H
#include "CommandList.h"
#endif

#ifndef _NODE_LIST_H
#include "NodeList.h"
#endif

#ifndef _DUNE_STRING_H
#include "MyString.h"
#endif

#ifndef _URL_H
#include "URL.h"
#endif

#ifndef _PROTO_H
#include "Proto.h"
#endif

#ifndef _DUNE_APP_H
#include "DuneApp.h"
#endif

#ifndef _QUATERNION_H
#include "Quaternion.h"
#endif

#include "x3dFlags.h"

#include "TransformMode.h"
#include "RenderState.h"
#include "NodeFlags.h"

#include "ProtoMap.h"

#define MAX_PATH_LEN 1024

class FieldValue;
class Node;
class Path;
class FontInfo;
class NodeViewpoint;
class NodeNavigationInfo;
class NodeViewport;
class NodeFog;
class NodeBackground;
class SFVec3f;
class MFVec3f;
class SceneView;
class Hint;
class NodeInline;
class Element;
class VertexModifier;

class DownloadPathData;

typedef MyStack<Command *> CommandStack;

typedef Map<MyString, Node *> NodeMap;
typedef Map<MyString, int> StringMap;

class Interpolator;

enum {
     HIGHEST_SELECTION_LEVEL = 0,
     DEEPEST_SELECTION_LEVEL = -1
};

enum {
     SELECTION_MODE_FACES = 0,
     SELECTION_MODE_VERTICES = 1,
     SELECTION_MODE_LINES = 2,
     SELECTION_HANIM_JOINT_WEIGHT = 3,
     SELECTION_VERTICES_OF_HANIM_JOINT = 4
};

enum {
     SCAN_FOR_BOTH,
     SCAN_FOR_EXTERN_PROTO,
     SCAN_FOR_INLINE
};

struct WriteCDynamicNodeData {
    int filedes;
    int languageFlag;
    int result;
};

class CGlNameData {
public:
    int glName;
    Node *node;
    MyString nodeName;
};

class Scene {
public:
                        Scene();
                       ~Scene();
    void                def(const char *nodeName, Node *value);
    void                def(Node *value) { m_defNode = value; }
    Node               *use(const char *nodeName);
    bool                use(Node *parentNode, int parentField);
    bool                canUse(Node *parentNode, int parentField);
    void                undef(MyString nodeName);
    bool                hasAlreadyName(const char *name) { return use(name); }

    int                 addSymbol(MyString s);
    const MyString     &getSymbol(int id) const;

    void                setRoot(Node *root) { m_root = root; }
    Node               *getRoot() const { return m_root; }
    int                 getRootField() const { return m_rootField; }

    void                setNodes(NodeList *nodes);
    const NodeList     *getNodes() const { return &m_nodes; }
    void                addNodes(Node *target, int targetField, 
                                 NodeList *nodes, int scanFor);

    void                addMeta(const char *metaKey, const char* metaValue);
    void                addUnit(const char *category, const char *name,
                                double conversionFactor);

    double              getUnitLength(void) 
                           { 
                           if (m_unitLength == 0)
                                return 1;
                           return m_unitLength; 
                           }
    void                setUnitLengthInit(void);
    void                setUnitLength(double f);
    double              getUnitAngle(void) 
                            { 
                            if (m_unitAngle == 0)
                                return 1;
                            return m_unitAngle; 
                            }
    void                setUnitAngleInit(void);
    void                setUnitAngle(double f);

    void                pushUnitLength(void);
    void                popUnitLength(void);
    void                pushUnitAngle(void);
    void                popUnitAngle(void);

    void                scanForInlines(NodeList *nodes);
    void                readInline(NodeInline *node);

    void                scanForExternProtos(NodeList *nodes);
    bool                readExternProto(Proto *proto);

    void                scanForMultimedia(NodeList *nodes);

    NodeList           *searchNodeType(int nodeType);
    void                addToNodeList(Node *node);


    const char         *parse(FILE *f, Node *target, 
                              int targetField, int scanFor);

    Node               *createNode(const char *nodeType, 
                                   int flags = NODE_FLAG_COLLAPSED);
    Node               *createNode(int nodeType);

    int                 write(int filedes, const char *url, int writeFlags = 0,
                              char *wrlFile = NULL);
    int                 writeHead(int f, int writeFlags);
    int                 writeExtensionProtos(void);

    bool                getWriteKanimNow(void) { return m_writeKanimNow; }
    int                 writeKanim(int filedes, const char* url);

    int                 writeC(int filedes, int languageFlag);
    int                 writeCDeclaration(int filedes, int languageFlag);
    int                 writeCTreeCallback(int filedes, 
                                           const char *functionName,
                                           bool callTreeCallback = true);
    int                 writeCDataFunctionsCalls(int filedes, int languageFlag);
    int                 writeCRoutes(int filedes, int languageFlag);

    void                zeroNumDataFunctions(void) { m_numDataFunctions = 0; }
    int                 getNumDataFunctions(void)
                           { return m_numDataFunctions; }
    int                 increaseNumDataFunctions(void) 
                           { return m_numDataFunctions++; }

    void                zeroNumDataClasses(void) { m_numDataClasses = 0; }
    int                 getNumDataClasses(void)
                           { return m_numDataClasses; }
    int                 increaseNumDataClasses(void) 
                           { return m_numDataClasses++; }

    int                 getStartNumDataFunctions(void)
                           { return m_startNumDataFunctions; }
    void                setStartNumDataFunctions(int i)
                           { m_startNumDataFunctions = i; }

    int                 getNumDataFunctionsPerClass(void)
                           { return m_numDataFunctionsPerClass; }
    void                setNumDataFunctionsPerClass(int i)
                           { m_numDataFunctionsPerClass = i; }
                                                 
    int                 writeAc3d(int filedes, bool raven = false);
    void                collectAc3dMaterialInfo(char *name, Node *node);
    bool                hasEmptyMaterial(void);
    int                 getAc3dEmptyMaterial(void)
                           { return m_ac3dEmptyMaterial; }

    int                 writeRib(int f, const char *url); 
    int                 writeRibNextFrame(int f, const char *url, int frame); 

    int                 writeCattGeo(void);
    int                 getCattGeoFileCounter(void) 
                           { return m_cattGeoFileCounter; }
    int                 getAndIncCattGeoFileCounter(void) 
                           { return m_cattGeoFileCounter++; }
    int                 getCattGeoCornerCounter(void) 
                           { return m_cattGeoCornerCounter; }
    int                 getAndIncCattGeoCornerCounter(void) 
                           { return m_cattGeoCornerCounter++; }
    int                 getAndIncCattGeoPlaneCounter(void) 
                           { return m_cattGeoPlaneCounter++; }

    bool                getCattRecIsWritten(void)
                           { return m_cattRecIsWritten; }
    void                setCattRecIsWritten(void)
                           { m_cattRecIsWritten = true; }

    bool                getCattSrcIsWritten(void)
                           { return m_cattSrcIsWritten; }
    void                setCattSrcIsWritten(void)
                           { m_cattSrcIsWritten = true; }

    bool                validateLdrawExport();
    int                 writeLdrawDat(int filedes, const char *path);
    int                 getCurrentLdrawColor(void) 
                           { return m_currentLdrawColor; }
    void                setCurrentLdrawColor(int c) 
                           { m_currentLdrawColor = c; }


    void                deleteExtensionProtos(void);
    Proto              *getExtensionProto(Proto *proto);

    void                addProto(MyString name, Proto *value);
    Proto              *getProto(MyString name);
    Proto              *getProto(const char *name) 
                           {
                           MyString mname = name; 
                           return getProto(mname); 
                           }
    bool                hasProtoName(MyString name)
                           {
                           if (getProto(name) == NULL)
                               return false;
                           return true;
                           }
    void                deleteProto(MyString name);
    Proto              *getExtensionProto(MyString name);

    static bool         validRoute(Node *src, int eventOut, Node *dst, 
                                   int eventIn);
    bool                addRoute(Node *src, int eventOut, Node *dest, 
                                 int eventIn, SceneView *sender = NULL);
    void                deleteRoute(Node *src, int eventOut, Node *dest, 
                                    int eventIn);

    void                execute(Command *cmd, SceneView *sender = NULL);

    void                add(Command *change);
    void                addNextCommand(void);
    void                deleteLastNextCommand(void);

    void                undo();
    void                redo();

    int                 canUndo() const;
    int                 canRedo() const;

    int                 getUndoStackTop() { return m_undoStack.getTop(); }

    void                errorf(const char *fmt, ...);

    void                invalidNode(const char *nodeName);
    void                invalidField(const char *node, const char *field);

    void                backupField(Node *node, int field);
    void                backupFieldsStart(void);
    void                backupFieldsAppend(Node *node, int field);
    void                backupFieldsDone(void);
    void                setField(Node *node, int field, FieldValue *value);
    Interpolator       *findUpstreamInterpolator(const Node *node, 
                                                 int field) const;

    void                drawScene(bool pick = false, int x = 0, int y = 0,
                                  double width = 0, double height = 0,
                                  Node *root = NULL, bool update = false,
                                  float scaleX = 1, float scaleY = 1);

    int                 getNumDraw(void) { return m_numDraw; }

    Path               *pick(int x, int y, float width = 0, float height = 0);
    Path               *searchTransform(void);
    void                drawHandles(Node *root, 
                                    bool drawRigidBodyHandles = false);
    void                finishDrawHandles(void);
    void                drawHandlesRec(Node *node, bool drawOnlyJoints = false) 
                           const;

    void                setSelectedHandle(int handle)
                           { 
                           if (handle == NO_HANDLE)  {
                               m_firstSelectionRangeHandle = -1;
                               setSelectionMode(SELECTION_MODE_VERTICES);
                               return;
                           }
                           m_isNewSelectedHandle = !isInSelectedHandles(handle);
                           m_lastSelectedHandle = handle;
                           addSelectedHandle(handle);
                           if ((m_firstSelectionRangeHandle > 0) &&
                               (m_firstSelectionRangeHandle != handle))
                               addSelectionRange(m_firstSelectionRangeHandle,
                                                 handle);
                           }
    bool                addSelectedHandle(int handle)
                           { 
                           if (m_selectedHandles.find(handle) != -1)
                               return false;
                           m_selectedHandles.append(handle); 
                           return true;
                           }
    void                removeSelectedHandle(int handle)
                           {
                           if (m_firstSelectionRangeHandle > -1)
                               return; 
                           if (m_lastSelectedHandle == handle)
                               m_lastSelectedHandle = -1;
                           int index = m_selectedHandles.find(handle);
                           if (index > -1)
                               m_selectedHandles.remove(index);
                           }
    void                removeSelectedHandles(void)
                           {
                           if (m_selectedHandles.size() == 0)
                               return;
                           if (m_firstSelectionRangeHandle > -1)
                               return; 
                           m_oldSelectedHandles.resize(0); 
                           for (long i = 0; i < m_selectedHandles.size(); 
                                i++)
                               m_oldSelectedHandles.append(
                                     m_selectedHandles[i]);
                           m_selectedHandles.resize(0); 
                           m_oldLastSelectedHandle = m_lastSelectedHandle;
                           m_lastSelectedHandle = -1;
                           m_singleSelectedHandle = true;
                           m_isNewSelectedHandle = true;
                           }
    void                restoreSelectedHandles(void);
    int                 getSelectedHandlesSize() const
                           { return m_selectedHandles.size(); }
    int                 getSelectedHandle(int i) const
                           { return m_selectedHandles[i]; }
    bool                isInSelectedHandles(int handle) 
                           { 
                           if (m_selectedHandles.find(handle) != -1)
                               return true;
                           return false;
                           }
    bool                isNewSelectedHandle(void) 
                          { return m_isNewSelectedHandle; }
    int                 getLastSelectedHandle(void) 
                           { return m_lastSelectedHandle; }
    bool                checkXSymetricOrSameHandle(int handle, MFVec3f *points);

    bool                isSingleSelectedHandle(void)
                           { return m_singleSelectedHandle; }
    void                setSingleSelectedHandle(void)
                           { m_singleSelectedHandle = true; }
    void                setMultipleSelectedHandles(void)
                           { m_singleSelectedHandle = false; }

    int                 getfirstSelectionRangeHandle(void)
                           { return m_firstSelectionRangeHandle; }
    void                setfirstSelectionRangeHandle(void)
                           { m_firstSelectionRangeHandle = 
                                 getLastSelectedHandle(); }
    void                removeFirstSelectionRangeHandle(void)
                           { m_firstSelectionRangeHandle = -1; }
    void                addSelectionRange(int firstRangeHandle, 
                                          int secondRangeHandle);

    void                showOnlySelectedVertices(void);

    void                hideSelectedVertices(void);
    void                unHideVertices(void) { m_hiddenVertices.resize(0); }
                        
    int                 getNumHiddenVertices(void) 
                            { return m_hiddenVertices.size(); }

    int                 getHiddenVertex(int i) 
                            { return m_hiddenVertices[i]; }

    int                 getSelectionMode(void)
                           { return m_selectionMode; }           
    void                setSelectionMode(int mode);

    void                setTransformMode(TMode tm)
                           { m_transformMode->tmode = tm; }
    void                setTransformMode(TDimension td)
                           { m_transformMode->tdimension = td; }
    void                setTransformMode(T2axes t2)
                           { m_transformMode->t2axes = t2; }
    TransformMode      *getTransformMode()
                           { return m_transformMode; }

    void                transform(const Path *path);
    void                projectPoint(float x, float y, float z,
                                     float *wx, float *wy, float *wz);
    void                unProjectPoint(float wx, float wy, float wz,
                                       float *x, float *y, float *z);
    int                 allocateLight();
    int                 freeLight();
    int                 getNumLights() { return m_numLights; }

    int                 allocateClipPlane();
    int                 freeClipPlane();
    int                 getNumClipPlanes() { return m_numClipPlanes; }  

    void                enableHeadlight();
    Path               *processHits(GLint hits, GLuint *pickBuffer,
                                    bool selectMultipleHandles);

    void                addViewpoint(Node *viewpoint);
    void                addFog(Node *fog);
    void                setFog(Node *fog);
    void                addBackground(Node *background);
    void                setBackground(Node *background);
    void                addNavigationInfo(Node *navigationInfo);

    void                setFirstNavigationInfo(void);

    void                addTimeSensor(Node *timeSensor);

    void                setHeadlight(int headlight);

    void                changeTurnPointDistance(float factor);

    void                moveCamera(float dx, float dy, float dz);
    void                walkCamera(Vec3f walk, bool forward);
    void                turnCamera(float x, float y, float z, float ang);
    void                orbitCamera(float dtheta, float dphi);
    void                rollCamera(float dtheta);
    void                standUpCamera(void);

    void                startWalking();

    Node               *getCamera() const;
    void                setCamera(Node *camera);
    void                applyCamera(void);

    NodeNavigationInfo *getNavigationInfoNode() const;
    void                applyNavigationInfo(void);

    void                findBindableNodes(void);

    void                addNode(Node *node);
    Node               *replaceNode(Node *oldNode, Node* newNode);
    void                removeNode(Node *node);

    MyString            getUniqueNodeName(const char *name);
    MyString            getUniqueNodeName(Node *node);
    MyString            generateUniqueNodeName(Node *node, 
                                               const char *name);
    MyString            generateUniqueNodeName(Node *node);

    MyString            generateVariableName(Node *node);

    Path               *getSelection(void) const { return m_selection; }
    void                setSelection(Path *selection);
    void                setSelection(Node *node);
    void                setSelection(Proto *proto);

    int                 getSelectionLevel(void) { return m_selectionLevel; }
    void                setSelectionLevel(int level) 
                           { m_selectionLevel = level; }

    Path               *newPath(Node *node);

    void                setURL(const char *url) { m_URL = ""; m_URL += url; }
    const char         *getURL(void) const { return m_URL; }
    const char         *getNewURL(void) const { return m_newURL; }

    void                resetWriteFlags(int flags);
    bool                isTempSave(void) const 
                           { return m_writeFlags & TEMP_SAVE; }
    bool                isPureVRML97(void) const 
                           { return m_writeFlags & PURE_VRML97; }
    bool                isPureX3dv(void) const
                           { return m_writeFlags & PURE_X3DV; }
    bool                isX3dv(void) const
                           { return ::isX3dv(m_writeFlags); }
    bool                isX3dXml(void) const
                           { return ::isX3dXml(m_writeFlags); }
    bool                isX3d(void) const
                           { return ::isX3d(m_writeFlags); }
    bool                isPureVRML(void) const 
                           { return isPureVRML97() || isPureX3dv(); }
    bool                converts2X3d(void) const 
                           { return m_writeFlags & CONVERT2X3D; }
    bool                converts2VRML(void) const 
                           { return m_writeFlags & CONVERT2VRML; }
    bool                isX3dom(void) const
                           { return m_writeFlags & X3DOM; }
    int                 getWriteFlags(void) { return m_writeFlags; }
    void                setWriteFlags(int flags) { m_writeFlags = flags; }
    void                setX3d(void);
    void                setX3dv(void);
    void                setX3dXml(void);
    void                setVrml(void);
    bool                isInvalidElement(Element *element);
    bool                isValidElement(Element *element, bool x3d);

    void                setPath(const char *path) 
                            { 
                            m_path = "";
                            m_path += path; 
                            }
    const char         *getPath() const { return m_path; }

    void                setPathIntern(const char *path) 
                            { 
                            m_pathIntern = "";
                            m_pathIntern += path; 
                            }
    const char         *getPathIntern() const 
                           {
                           if (m_pathIntern.length() == 0)
                               return m_path;
                           return m_pathIntern; 
                           }

    void                getProtoList(MyArray<int> *protoList, const Node *node);
    void                setPathAllURL(const char *path);

    bool                isModified() const;
    bool                isRunning() const { return m_running; }
    bool                isRecording() const { return m_recording; }
    void                setRecording(bool recording) { m_recording = recording; }

    void                start();
    void                stop();
    void                updateTime();
    void                updateTimeAt(double t);
    double              timeSinceStart(void);

    void                updateNodePROTOs(Proto *protoToUpdate);    

    void                OnFieldChange(Node *node, int field, int index = -1);
    void                OnAddNode(Node *node, Node *dest, int field);
    void                OnRemoveNode(Node *node, Node *src, int field);

    void                OnAddNodeSceneGraph(Node *node, Node *dest, int field);

    void                AddView(SceneView *view);
    void                RemoveView(SceneView *view);
    void                UpdateViews(SceneView *sender, int type,
                                    Hint *hint = NULL);
    void                UpdateViewsNow(SceneView *sender, int type,
                                       Hint *hint = NULL);
    void                setUpdateViewsSelection(bool flag) 
                           { m_canUpdateViewsSelection = flag; }

    void                deleteSelected();
    void                deleteSelectedAppend(CommandList* list);
    void                deleteAll();
    void                selectNext();
    int                 OnDragOver(Node *src, Node *srcParent, int srcField, 
                                   Node *dest, int destField, int modifiers);
    int                 OnDrop(Node *src, Node *srcParent, int srcField, 
                               Node *dest, int destField, int effect);

    bool                Download(const URL &url, MyString *path);
    MyString            downloadPath(const URL &url);
    FontInfo           *LoadGLFont(const char *fontName, const char *style);
    int                 getNumProtoNames(void) { return m_numProtoNames; }
    bool                addProtoName(MyString name);
    void                deleteProtoName(MyString name);
    MyString            getProtoName(int i) { return m_protoNames[i]; }
    void                addProtoDefinition(void);
    void                addToProtoDefinition(const char* string);

    bool                isNestedProto(const char *protoName);
    void                setNestedProto(void);                      

    int                 writeExternProto(int f, const char* protoName);

    void                updateSceneMap(void);

    MyString            createRouteString(const char *fromNodeName,
                                          const char *fromFieldName,
                                          const char *toNodeName,
                                          const char *toFieldName);
    void                addRouteString(MyString string);
    void                addEndRouteString(MyString string);
    int                 writeRouteStrings(int filedes, int indent, 
                                          bool end = false);
    void                copyRoutes(Node *toNode, Node *fromNode);
    void                copyRoutesToScene(Node *node);
    void                changeRoutes(Node *toNode, int toField,
                                     Node *fromNode, int fromField);
    void                changeRoutes(Node *toNode, int toField, 
                                     int toOffset,
                                     Node *fromNode, int fromField, 
                                     int fromOffset);
    int                 writeComponents(int filedes);

    void                setHasFocus(void)    {m_hasFocus=true;}
    void                deleteHasFocus(void) {m_hasFocus=false;}
    bool                getHasFocus(void)    
                           {
                           if (TheApp->dontCareFocus())
                               return true;
                           else
                               return m_hasFocus;
                           }  
    void                setMouseNavigationMode(bool flag) 
                           {m_navigationMouseMode=flag;}
    bool                getMouseNavigationMode() 
                           {return m_navigationMouseMode;}
    void                setInputDeviceNavigationMode(bool flag) 
                           {m_navigationInputDeviceMode=flag;}
    bool                getInputDeviceNavigationMode() 
                           {return m_navigationInputDeviceMode;}

    void                setViewOfLastSelection(SceneView* view);
    SceneView          *getViewOfLastSelection(void);

    void                copyLastSelection(void);
    void                pasteLastSelection(void);
    void                pasteSymetricLastSelection(int direction);
    void                deleteLastSelection(void);

    void                disableMakeSimilarName() { m_similarNameFlag = false; } 
    void                makeSimilarName(Node *node, const char *name);
    void                enableMakeSimilarName() { m_similarNameFlag = true; } 

    void                saveProtoStatus(void);
    void                restoreProtoStatus(void);

    void                setErrorLineNumber(int lineno) 
                           { m_errorLineNumber = lineno ;}
    int                 getErrorLineNumber(void) { return m_errorLineNumber; }

    const char         *getErrors(void) { return m_errors; }

    /* 
     * a extra modifiedflag is needed to signal possible changes 
     * (e.g. from the file -> Textedit menupoint)
     */
    void                setExtraModifiedFlag(void) 
                           { m_extraModifiedFlag = true; }
    StringArray        *getAllNodeNames(void);
    void                draw3dCursor(int x, int y);
    void                use3dCursor(bool flag) { m_use3dCursor = flag; }
    bool                use3dCursor(void);
    bool                isVerticesOrNurbs(void);
    void                draw3dBoundingBox(int xfrom, int yfrom, 
                                          int xto, int yto);
    bool                setPrefix(char* prefix);
    MyString            getNodeWithPrefix(const MyString &nodeType);
    bool                hasExternProtoWarning(void) 
                           { return m_externProtoWarning; }
    void                setExternProtoWarning(bool flag) 
                           { m_externProtoWarning = flag; }
    bool                belongsToNodeWithExternProto(const char *protoName);
    int                 writeIndexedFaceSet(int f, Node* node);
    int                 writeIndexedTriangleSet(int f, Node* node);
    int                 writeTriangleSet(int f, Node* node);
    int                 writeTriangles(int f, Node* node);
    int                 writeIndexedLineSet(int f, Node* node);
    bool                getXSymetricMode() 
                           { return TheApp->GetXSymetricMode(); }
    bool                isParsing(void) { return m_isParsing; }
    int                 getNumberBuildinProtos(void) 
                           { return m_numberBuildinProtos; }
    void                addDelayedWriteNode(Node *node) 
                           { m_delayedWriteNodes.append(node); }
    void                recalibrate(void);
    bool                hasJoints(void) { return m_hasJoints; }
    void                setHasJoints(void) { m_hasJoints = true; }
    bool                showJoints(void) { return m_showJoints; }
    void                setShowJoints(bool flag) 
                           { if (m_hasJoints) m_showJoints = flag; }
    int                 getNumInteractiveProtos(int type)
                           { return getInteractiveProtos(type)->size(); }
    Proto              *getInteractiveProto(int type, int i)
                           { return (*getInteractiveProtos(type))[i]; }
    void                setXrayRendering(bool flag)
                           { m_xrayRendering = flag; }
    int                 getDestField(Node* src, Node *dest, int destField);

    void                setXonly(bool flag) { m_xOnly = flag; } 
    void                setYonly(bool flag) { m_yOnly = flag; } 
    void                setZonly(bool flag) { m_zOnly = flag; } 
    bool                getXonly(void) { return m_xOnly; }
    bool                getYonly(void) { return m_yOnly; }
    bool                getZonly(void) { return m_zOnly; }
    int                 getConstrain(void);
    Vec3f               constrainVec(Vec3f vec);

    void                setTurnPoint(void);

    NodeList            getTimeSensors(void) { return m_timeSensors; }

    void                setImportIntoVrmlScene(bool f) { m_importIntoVrmlScene = f; }
    bool                getImportIntoVrmlScene(void) { return m_importIntoVrmlScene; }

    void                convertProtos2X3d(void);
    void                convertProtos2Vrml(void);

    void                setRigidBodyHandleNode(Node *node) 
                           { m_rigidBodyHandleNode = node; }

    Node               *convertToIndexedFaceSet(Node* node);
    Node               *convertToTriangleSet(Node* node);
    Node               *convertToIndexedTriangleSet(Node* node);
    Node               *convertToIndexedLineSet(Node* node);

    void                branchConvertToTriangleSet(Node *node);
    void                branchConvertToIndexedTriangleSet(Node *node);
    void                branchConvertToIndexedFaceSet(Node *node);
    void                branchConvertToTriangles(Node *node, bool targetHasCcw);
    void                branchCreateNormals(Node *node);
    void                createNormal(Node *node);
    void                branchCreateTextureCoordinates(Node *node);
    void                createTextureCoordinate(Node *node);

    void                branchSetBbox(void);

    bool                isCurrentViewpoint(Node *node)
                            { return node == (Node *)m_currentViewpoint; }
    bool                isDefaultViewpoint(Node *node)
                            { return node == (Node *)m_currentViewpoint; }

    void                addToSensorNodes(Node *node);
    void                removeSensorNodes(void) { m_sensorNodes.resize(0); }

    void                setParsedX3dVersion(int version) 
                           { m_x3dVersion = version; }
    int                 getParsedX3dVersion(void) { return m_x3dVersion; }
    int                 getX3dVersion(void);

    void                warning(const char* string);
    void                warning(int id);
    void                warning(int id, const char *string);

    Proto              *getProto(long i) 
                            {
                            if (i >= m_protoNames.size())
                                return NULL;
                            return m_protos[m_protoNames[i]]; 
                            }
    int                 getNumProtos(void) { return m_protoNames.size(); }
    void                toggleDeselect(void) { m_deselect = !m_deselect; }
    bool                getDeselect(void) { return m_deselect; }
    Node               *searchProtoNodeId(long id);
    Node               *searchProtoNodeIdInNode(Node *node, long id);
    int                 getProtoType(Proto *proto);
    void                setLastSelectedHAnimJoint(Node *n) 
                           { m_lastSelectedHAnimJoint = n; }
    Node               *getLastSelectedHAnimJoint(void) 
                           { return m_lastSelectedHAnimJoint; }
    void                setLastSelectedHAnimJointOrHumanoid(Node *n) 
                           { m_lastSelectedHAnimJointOrHumanoid = n; }
    Node               *getLastSelectedHAnimJointOrHumanoid(void) 
                           { return m_lastSelectedHAnimJointOrHumanoid; }
    double              getCurrentTime(void) { return m_currentTime; }

    MyString            getRibTexureFile(MyString file)
                            { return m_ribTexureFiles[file]; }
    void                addRibTexureFile(MyString file, MyString txFile)
                            {
                            if (m_ribTexureFiles.hasKey(file))
                                m_ribTexureFiles.replace(file, txFile); 
                            else 
                                m_ribTexureFiles.add(file, txFile); 
                            }
     bool               getSavedVrml(void) { return m_saved_vrml; }
     bool               getSavedX3dv(void) { return m_saved_x3dv; }
     bool               getSavedX3dXml(void) { return m_saved_x3dxml; }
     void               setSaved(void) {
                            m_saved_vrml = true;
                            m_saved_x3dv = true;
                            m_saved_x3dxml = true;
                        }
     void               setNotSaved(void) {
                            m_saved_vrml = false;
                            m_saved_x3dv = false;
                            m_saved_x3dxml = false;
                        }
     MyArray<Node *>   *getViewPorts();
     void               setViewPorts(void);
     void               addViewPort(Node *node) {
                           m_viewports.append(node);
                        }

     bool               getViewpointUpdated(void) { return m_viewpointUpdated; }

     bool               getDrawViewports() { return m_drawViewPorts; }

    void                resetPerspective(void);

    int                 getWidth(void)  { return m_width; }
    int                 getHeight(void) { return m_height; }

    bool                hasParticleSystem(void) 
                            { return m_hasParticleSystem; }
    void                setParticleSystem(bool flag) 
                            { m_hasParticleSystem = flag; }

    bool                hasMovieTexture(void) 
                            { return m_hasMovieTexture; }
    void                setMovieTexture(bool flag) 
                            { m_hasMovieTexture = flag; }

    void                setInfoHandles(bool flag) 
                            { m_infoHandles = flag; }
    bool                getInfoHandles(void) { return m_infoHandles; }

    int                 getGlName(void) { return m_glName; }   
    void                increaseGlName(void) { m_glName++; }   

    bool                getScriptTypeWritten(void)
                            { return m_scriptTypeWritten; }
    void                setScriptTypeWritten(void)
                            { m_scriptTypeWritten = true; }

    bool                dynamicNodeCallbackWritten(const char *name)
                            { 
                            return m_writeCDynamicNodeCallback.find(name) > -1;
                            }
    void                dynamicNodeCallbackAppend(const char *name)
                            { m_writeCDynamicNodeCallback.append(name); }
    void                dynamicNodeCallbackRemove(void)
                            { m_writeCDynamicNodeCallback.resize(0); }

    void                addToStore4Convex_hull(void);
    MyArray<Vec3f>     *getStore4Convex_hull(void)
                            { return &m_store4convex_hull; }
    void                removeStore4Convex_hull(void) 
                            { m_store4convex_hull.resize(0); }

    VertexModifier     *getVertexModifier(void) { return m_vertexModifier; }
    void                setVertexModifier(VertexModifier *mod) 
                            { m_vertexModifier = mod; }

    void                storeHtmlData(const char *data);
    void                storeHtmlElementAndAttributes(const char *element,
                                                      const char **attribures,
                                                      int numAttributes,
                                                      bool htmlFirstPart);

    void                storeAsHtml(void) { m_storeAsHtml = true; }
    bool                getStoreAsHtml(void) { return m_htmlData.size() > 2; }

    bool                hasPROTONodes(void);

    MyArray<Node *>     searchTimeSensors(void);
    MyArray<Node *>     searchInterpolators(void);
    MyArray<Node *>     searchTimeSensorInInterpolator(Node *interpolator);

protected:
    int                 writeExtensionProtos(int f, int flag);
    ProtoArray         *getInteractiveProtos(int type); 
    void                buildInteractiveProtos(void);
    void                updateViewpoint(void);
    DownloadPathData    downloadPathIntern(const URL &url);
protected:
    NodeMap             m_nodeMap;
    Node               *m_root;
    int                 m_rootField;
    StringMap           m_symbols;
    StringArray         m_symbolList;
    StringArray         m_protoNames;
    int                 m_numProtoNames;
    int                 m_statusNumProtoNames;
    StringArray         m_protoDefinitions;
    MyArray<bool>       m_isNestedProto;
    int                 m_numProtoDefinitions;
    int                 m_statusNumProtoDefinitions;
    ProtoMap            m_protos;

    MyArray<MyString>   m_htmlBegin;
    MyArray<MyString>   m_htmlData;
    MyArray<MyString>   m_htmlEnd;
    MyArray<bool>       m_htmlFirstPart;

    int                 m_numDraw;

    CommandStack        m_undoStack;
    CommandStack        m_redoStack;

    CommandList        *m_backupCommandList;
    int                 m_numLights;
    int                 m_numClipPlanes;
    NodeList            m_viewpoints;
    NodeList            m_navigationinfos;
    NodeList            m_backgrounds;
    NodeList            m_fogs;
    MyStack<Node *>     m_fogStack;

    NodeList            m_timeSensors;

    int                 m_headlight;
    bool                m_headlightIsSet;

    NodeList            m_nodes;

    Path               *m_selection;
    IntArray            m_selectedHandles;
    int                 m_lastSelectedHandle;
    int                 m_oldLastSelectedHandle;
    bool                m_isNewSelectedHandle;

    Node               *m_oldSelection;
    IntArray            m_oldSelectedHandles;

    bool                m_singleSelectedHandle;

    int                 m_selectionMode;

    TransformMode      *m_transformMode;

    int                 m_selectionLevel;

    MyString            m_URL;
    const char         *m_newURL;
    int                 m_writeFlags;
    int                 m_x3dVersion;

    int                 m_numSymbols;

    bool                m_running;
    double              m_timeStart;
    bool                m_recording;

    Command            *m_unmodified;
    bool                m_extraModifiedFlag;

    bool                m_setViewpoint;
    Node               *m_defaultViewpoint;
    Node               *m_currentViewpoint;

    bool                m_setNavigationInfo;
    NodeNavigationInfo *m_defaultNavigationInfo;
    NodeNavigationInfo *m_currentNavigationInfo;

    NodeFog            *m_currentFog;
    NodeBackground     *m_currentBackground;

    MyArray<Node *>     m_viewports;

    bool                m_canUpdateViewsSelection;
    List<SceneView *>   m_views;

    MyString            m_errors;
    MyArray<FontInfo *> m_fonts;

    MyString            m_path;
    MyString            m_pathIntern;

    List<MyString>      m_routeList;
    List<MyString>      m_endRouteList;

    /* when picking several objects which one click, which one is selected ? */
    int                 m_selectlevel;

    /* current scene is in mouse focus ? */
    bool                m_hasFocus;

    bool                m_xOnly;
    bool                m_yOnly;
    bool                m_zOnly;

    bool                m_navigationMouseMode;
    bool                m_navigationInputDeviceMode;

    SceneView          *m_viewOfLastSelection;
    bool                m_selection_is_in_scene;

    GLUquadricObj      *m_obj3dCursor;
    bool                m_use3dCursor;
    int                 m_errorLineNumber;

    MyArray<const char *> m_nodesWithExternProto;
    MyArray<const char *> m_nodesForceExternProtoWrite;
    bool                m_externProtoWarning;
    int                 m_numberBuildinProtos;

    bool                m_isParsing;

    // temporary nodes to write (e.g. Interpolators for PureVRML Export)
    MyArray<Node *>     m_delayedWriteNodes;

    bool                m_hasJoints;
    bool                m_showJoints;

    ProtoArray          m_interactiveSFBoolProtos;
    ProtoArray          m_interactiveSFRotationProtos;
    ProtoArray          m_interactiveSFTimeProtos;
    ProtoArray          m_interactiveSFVec3fProtos; 
    ProtoArray          m_emptyProtoArray; 

    bool                m_xrayRendering;

    Vec3d               m_turnPointPos;
    Quaternion          m_turnPointRot;

    double              m_oldWalkTime;

    Node               *m_defNode;
    MyArray<Proto *>    m_writtenExtensionProtos;
    bool                m_importIntoVrmlScene;

    int                 m_numDataClasses;
    int                 m_numDataFunctions;
    int                 m_startNumDataFunctions;
    int                 m_numDataFunctionsPerClass;
    bool                m_writeKanimNow;

    StringMap           m_ac3dMaterialIndexMap;
    NodeArray           m_ac3dMaterialNodeArray;
    StringArray         m_ac3dMaterialNameArray;
    int                 m_ac3dEmptyMaterial;

    int                 m_cattGeoFileCounter;
    int                 m_cattGeoCornerCounter;
    int                 m_cattGeoPlaneCounter;

    bool                m_cattRecIsWritten;
    bool                m_cattSrcIsWritten;

    int                 m_currentLdrawColor;

    int                 m_variableCounter;

    NodeList            m_nodeList;

    Node               *m_rigidBodyHandleNode;

    MyArray<const char*>  m_metaKeys;
    MyArray<const char*>  m_metaValues;

    MyArray<const char*>  m_unitCategory;
    MyArray<const char*>  m_unitName;
    MyArray<double>       m_unitConversionFactor;

    double              m_unitLength;
    double              m_unitAngle;

    MyStack<double>     m_unitLengthStack;
    MyStack<double>     m_unitAngleStack;

    NodeArray           m_sensorNodes;

    StringArray         m_warnings;

    double              m_currentTime;
    double              m_oldTime;

    bool                m_deselect;

    int                 m_firstSelectionRangeHandle;

    IntArray            m_hiddenVertices;

    Node               *m_lastSelectedHAnimJoint;
    Node               *m_lastSelectedHAnimJointOrHumanoid;

    bool                m_downloaded;

    Map<MyString, MyString> m_ribTexureFiles;
    
    bool                m_saved_vrml;
    bool                m_saved_x3dv;
    bool                m_saved_x3dxml;

    bool                m_viewpointUpdated;

    bool                m_drawViewPorts;

    int                 m_width;
    int                 m_height;

    bool                m_hasParticleSystem;
    bool                m_hasMovieTexture;

    Node               *m_orthoViewpoint;

    bool                m_infoHandles;

    int                 m_glName;               
   
    bool                m_scriptTypeWritten;

    MyArray<const char *> m_writeCDynamicNodeCallback;

    MyArray<Vec3f>      m_store4convex_hull;

    VertexModifier     *m_vertexModifier;
 
    bool                m_storeAsHtml;

    bool                m_similarNameFlag; 
public:
    MyArray<CGlNameData> m_glNameData;               
};

bool writeCNodeData(Node *node, void *data);

enum {
    UPDATE_ALL = 0,
    UPDATE_SELECTION,
    UPDATE_FIELD,
    UPDATE_ADD_NODE,
    UPDATE_REMOVE_NODE,
    UPDATE_CHANGE_INTERFACE_NODE,
    UPDATE_ADD_ROUTE,
    UPDATE_DELETE_ROUTE,
    UPDATE_MODE,
    UPDATE_PROTO,
    UPDATE_TIME,
    UPDATE_START_FIELD_EDIT,
    UPDATE_STOP_FIELD_EDIT,
    UPDATE_ENABLE_COLOR_CIRCLE,
    UPDATE_DISABLE_COLOR_CIRCLE,
    UPDATE_CLOSE_COLOR_CIRCLE,
    UPDATE_CLOSE_VERTEX_MODIFIER,
    UPDATE_SELECTED_FIELD,
    UPDATE_SELECTION_NAME,
    UPDATE_NODE_NAME,
    UPDATE_ADD_NODE_SCENE_GRAPH_VIEW,
    UPDATE_REDRAW,
    UPDATE_REDRAW_3D,
    UPDATE_PREVIEW,
    UPDATE_SOLID_CHANGED,
    UPDATE_TOOLBAR,
    UPDATE_SELF
};

enum {
    RENDER_PASS_GEOMETRY,
    RENDER_PASS_NON_TRANSPARENT,
    RENDER_PASS_TRANSPARENT
};

class Hint {
};

class FieldUpdate : public Hint {
public:
                    FieldUpdate(Node *n = NULL, int f = -1, int i = -1);


    Node           *node;
    int             field;
    int             index;
};

class NodeUpdate : public Hint {
public:
                    NodeUpdate(Node *n, Node *p, int f)
                    { node = n; parent = p, field = f; }

    Node           *node;
    Node           *parent;
    int             field;
};

class RouteUpdate : public Hint {
public:
                    RouteUpdate(Node *s, int out, Node *d, int in)
                    { src = s; eventOut = out; dest = d; eventIn = in; }

    Node           *src;
    Node           *dest;
    int             eventOut;
    int             eventIn;
};

class ProtoUpdate : public Hint {
public:
                    ProtoUpdate(Proto *p) { proto = p; }

    Proto          *proto;
};

void BackupRoutesRec(Node *node, CommandList *list);

#endif // _SCENE_H

