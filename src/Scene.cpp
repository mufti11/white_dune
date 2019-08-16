/*
 * Scene.cpp
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

/***************************************************************************
 * Scene::Download() based on ftpget example of Curl library
 *
 * Copyright (C) 1998 - 2017, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include "stdafx.h"
#ifndef _WIN32
# include <unistd.h>
# include <fcntl.h>
#endif
#include "Util.h"
#include "resource.h"

#ifdef HAVE_LIBZ
extern "C" {
# include "zlib.h"
}
#endif

#ifdef HAVE_LIBCURL
# include <curl/curl.h>
#endif

#include "swt.h"

#include "Scene.h"
#include "SceneProtoMap.h"
#include "SceneView.h"
#include "WonderlandModuleExport.h"
#include "Matrix.h"
#include "FieldValue.h"
#include "FieldCommand.h"
#include "MFieldCommand.h"
#include "CommandList.h"
#include "RouteCommand.h"
#include "UnRouteCommand.h"
#include "MoveCommand.h"
#include "Node.h"
#include "SFNode.h"
#include "SFRotation.h"
#include "SFTime.h"
#include "MFNode.h"
#include "Proto.h"
#include "parser.h"
#include "Path.h"
#include "Field.h"
#include "EventIn.h"
#include "EventOut.h"
#include "ExposedField.h"
#include "URL.h"
#include "FontInfo.h"
#include "DuneApp.h"
#include "WriteFlags.h"
#include "RenderState.h"
#include "Interpolator.h"
#include "NextCommand.h"
#include "FaceData.h"

#include "NodeTimeSensor.h"
#include "NodeVrmlCut.h"
#include "NodeNavigationInfo.h"
#include "NodeViewpoint.h"
#include "NodeOrthoViewpoint.h"
#include "NodeGeoViewpoint.h"
#include "NodeBackground.h"
#include "NodeCurveAnimation.h"
#include "NodeFog.h"
#include "NodeInline.h"
#include "NodeGroup.h"
#include "NodeShape.h"
#include "NodeIndexedFaceSet.h"
#include "NodeIndexedLineSet.h"
#include "NodeIndexedTriangleSet.h"
#include "NodeTriangleSet.h"
#include "NodeNurbsCurve.h"
#include "NodeNurbsSurface.h"
#include "NodeCattExportRec.h"
#include "NodeLdrawDatExport.h"
#include "NodeViewport.h"

#define ARRAY_SIZE(v)  ((int) (sizeof(v) / sizeof(v[0])))

#define PICK_BUFFER_SIZE 65536
#define PICK_REGION_SIZE 2.5

#define MAX_JAVA_CALLS 8000

enum {
    PICKED_NODE,
    PICKED_HANDLE,
    PICKED_RIGID_BODY_HANDLE,
    PICKED_3DCURSOR
};

Scene::Scene()
{
    m_writeFlags = 0;

    m_numSymbols = 0;

    m_headlight = true;
    m_numLights = 0;
    m_numClipPlanes = 0;
    m_selection = NULL;
    m_selectedHandles.resize(0);
    m_selectionMode = TheApp->GetIntPreference("SelectionMode",
                                               SELECTION_MODE_VERTICES);
    m_lastSelectedHandle = -1;
    m_isNewSelectedHandle = false;
    m_singleSelectedHandle = true;
    m_transformMode = new TransformMode(
          (TMode) TheApp->GetIntPreference("TransformMode",TM_HOVER),
          (TDimension) TheApp->GetIntPreference("TransformDimension",TM_3D),
          (T2axes) TheApp->GetIntPreference("Transform2Axes",TM_NEAR_FAR));
    if (TheApp->getMaxNumberAxesInputDevices()<=4)
        if ((m_transformMode->tmode == TM_6D) || (m_transformMode->tmode == TM_6D))
            m_transformMode->tmode = TM_HOVER;
    if (TheApp->getMaxNumberAxesInputDevices()<3)
        if (m_transformMode->tmode == TM_ROCKET)
            m_transformMode->tmode = TM_HOVER;
    if (TheApp->getMaxNumberAxesInputDevices()<2) 
        if (m_transformMode->tmode >= TM_6D)
            m_transformMode->tmode = TM_TRANSLATE;
 
    m_root = NULL;
    m_unmodified = NULL;
    resetWriteFlags(0);
    m_extraModifiedFlag = false;

    m_x3dVersion = -1;

    m_unitLength = 1;
    m_unitAngle = 1;

    SceneProtoMap::createProtoMap(&m_protos, this);

    StringArray *allNodeNames = getAllNodeNames();
    m_numberBuildinProtos = allNodeNames->size();
    delete allNodeNames;

    m_root = createNode("Group");
    m_root->ref();
    m_rootField = ((NodeGroup *) m_root)->children_Field();
    m_running = false;
    m_recording = false;

    m_setViewpoint = false;
    
    m_defaultViewpoint = NULL;
    m_currentViewpoint = NULL;
    updateViewpoint();

    m_setNavigationInfo = false;
    m_defaultNavigationInfo = (NodeNavigationInfo *) 
                             createNode("NavigationInfo");
    m_defaultNavigationInfo->ref();

    m_currentNavigationInfo = m_defaultNavigationInfo;
    m_currentNavigationInfo->ref();

    m_currentFog = NULL;
    m_currentBackground = NULL;

    m_numProtoNames = 0; 
    m_numProtoDefinitions = 0; 

    m_navigationMouseMode = true;
    m_navigationInputDeviceMode = true;

    m_routeList.Init();

    m_selectlevel=1;
    m_hasFocus=false;

    m_viewOfLastSelection=NULL;
    m_selection_is_in_scene=false;
    m_URL="";
    m_newURL="";
    m_errorLineNumber = -1;

    m_obj3dCursor = gluNewQuadric();

    m_backupCommandList = NULL;

    m_use3dCursor = false;

    m_nodesWithExternProto.append("Contour2D");
    m_nodesWithExternProto.append("CoordinateDeformer");
    m_nodesWithExternProto.append("InlineLoadControl");
    m_nodesWithExternProto.append("LoadSensor");

    m_nodesWithExternProto.append("NurbsCurve");
    m_nodesWithExternProto.append("NurbsCurve2D");
    m_nodesWithExternProto.append("NurbsGroup");
    m_nodesWithExternProto.append("NurbsPositionInterpolator");
    m_nodesWithExternProto.append("NurbsSurface");
    m_nodesWithExternProto.append("NurbsTextureSurface");
    m_nodesWithExternProto.append("TrimmedSurface");

    m_nodesWithExternProto.append("Arc2D");
    m_nodesWithExternProto.append("ArcClose2D");
    m_nodesWithExternProto.append("BooleanFilter");
    m_nodesWithExternProto.append("BooleanSequencer");
    m_nodesWithExternProto.append("BooleanToggle");
    m_nodesWithExternProto.append("BooleanTrigger");
    m_nodesWithExternProto.append("Circle2D");
    m_nodesWithExternProto.append("ColorRGBA");
    m_nodesWithExternProto.append("CoordinateInterpolator2D");
    m_nodesWithExternProto.append("Disk2D");
    m_nodesWithExternProto.append("FillProperties");
    m_nodesWithExternProto.append("IntegerSequencer");
    m_nodesWithExternProto.append("IntegerTrigger");
    m_nodesWithExternProto.append("KeySensor");
    m_nodesWithExternProto.append("LineProperties");
    m_nodesWithExternProto.append("Metadata");
    m_nodesWithExternProto.append("MetadataBoolean");
    m_nodesWithExternProto.append("MetadataDouble");
    m_nodesWithExternProto.append("MetadataFloat");
    m_nodesWithExternProto.append("MetadataInteger");
    m_nodesWithExternProto.append("MetadataSet");
    m_nodesWithExternProto.append("MetadataString");
    m_nodesWithExternProto.append("MultiTexture");
    m_nodesWithExternProto.append("Polyline2D");
    m_nodesWithExternProto.append("Polypoint2D");
    m_nodesWithExternProto.append("PositionInterpolator2D");
    m_nodesWithExternProto.append("Rectangle2D");
    m_nodesWithExternProto.append("StringSensor");
    m_nodesWithExternProto.append("TimeTrigger");
    m_nodesWithExternProto.append("TriangleSet2D");

    m_nodesWithExternProto.append("SuperEllipsoid");
    m_nodesWithExternProto.append("SuperExtrusion");
    m_nodesWithExternProto.append("SuperShape");
    m_nodesWithExternProto.append("SuperRevolver");

    m_nodesWithExternProto.append("CattExportRec");
    m_nodesWithExternProto.append("CattExportSrc");

    m_nodesWithExternProto.append("LdrawDatExport");
    m_nodesWithExternProto.append("WonderlandImportJava");

    m_nodesWithExternProto.append("VrmlCut");
    m_nodesForceExternProtoWrite.append("VrmlCut");
    m_nodesWithExternProto.append("VrmlScene");
    m_nodesForceExternProtoWrite.append("VrmlScene");

    m_nodesWithExternProto.append("CurveAnimation");

    m_nodesWithExternProto.append("ARSensor");
    m_nodesWithExternProto.append("COVER");
    m_nodesWithExternProto.append("CubeTexture");
    m_nodesWithExternProto.append("JoystickSensor");
    m_nodesWithExternProto.append("LabView");
    m_nodesWithExternProto.append("SpaceSensor");
    m_nodesWithExternProto.append("Sky");
    m_nodesWithExternProto.append("SteeringWheel");
    m_nodesWithExternProto.append("TUIButton");
    m_nodesWithExternProto.append("TUIComboBox");
    m_nodesWithExternProto.append("TUIFloatSlider");
    m_nodesWithExternProto.append("TUIFrame");
    m_nodesWithExternProto.append("TUILabel");
    m_nodesWithExternProto.append("TUIListBox");
    m_nodesWithExternProto.append("TUIMap");
    m_nodesWithExternProto.append("TUIProgressBar");
    m_nodesWithExternProto.append("TUISlider");
    m_nodesWithExternProto.append("TUISplitter");
    m_nodesWithExternProto.append("TUITab");
    m_nodesWithExternProto.append("TUITabFolder");
    m_nodesWithExternProto.append("TUIToggleButton");
    m_nodesWithExternProto.append("Vehicle");
    m_nodesWithExternProto.append("VirtualAcoustics");
    m_nodesWithExternProto.append("VirtualSoundSource");
    m_nodesWithExternProto.append("Wave");

    m_nodesWithExternProto.append("BlendMode");
    m_nodesWithExternProto.append("ColorSetInterpolator");
    m_nodesWithExternProto.append("CubicBezier2DOrientationInterpolator");
    m_nodesWithExternProto.append("CubicBezierPositionInterpolator");
    m_nodesWithExternProto.append("GeneratedShadowMap");
    m_nodesWithExternProto.append("MatrixTransform");
    m_nodesWithExternProto.append("KambiAppearance");
    m_nodesWithExternProto.append("KambiHeadLight");
    m_nodesWithExternProto.append("KambiInline");
    m_nodesWithExternProto.append("KambiNavigationInfo");
    m_nodesWithExternProto.append("KambiTriangulation");
    m_nodesWithExternProto.append("OctreeProperties");
    m_nodesWithExternProto.append("ProjectedTextureCoordinate");
    m_nodesWithExternProto.append("RenderedTexture");
    m_nodesWithExternProto.append("Teapot");
    m_nodesWithExternProto.append("Text3D");
    m_nodesWithExternProto.append("KambiTriangulation");
    m_nodesWithExternProto.append("VectorInterpolator");

    m_externProtoWarning = true;
    TheApp->readProtoLibrary(this);
    m_isParsing = false;
    m_hasJoints = false;
    m_showJoints = false;
    m_headlightIsSet = false;
    m_xrayRendering = false;
    m_defNode = NULL;
    buildInteractiveProtos();
    m_importIntoVrmlScene = false;
    m_rigidBodyHandleNode = NULL;
    m_writeKanimNow = false;
    m_ac3dEmptyMaterial = 0;
    m_selectionLevel = 1;
    m_canUpdateViewsSelection = true;
    m_variableCounter = 0;
    m_deselect = false;
    zeroNumDataClasses();
    zeroNumDataFunctions();
    m_startNumDataFunctions = 0;
    m_numDataFunctionsPerClass = 0;
    m_numDraw = 0;
    m_lastSelectedHAnimJoint = NULL;
    m_downloaded = false;
    m_path = "";
    m_firstSelectionRangeHandle = -1;
    m_saved_vrml = true;
    m_saved_x3dv = true;
    m_saved_x3dxml = true;
    setSelection(getRoot());
    setViewPorts();
    updateTime();
    m_viewpointUpdated = false;
    m_drawViewPorts = true;
    m_width = 1024;
    m_height = 768;
    m_hasParticleSystem = false;
    m_hasMovieTexture = false;
    m_infoHandles = false;
    m_glName = 0;
    m_vertexModifier = NULL;
}

void
Scene::updateViewpoint(void)
{
    if (m_defaultViewpoint)
        m_defaultViewpoint->unref();
    m_defaultViewpoint = (NodeViewpoint *) createNode("Viewpoint");
    m_defaultViewpoint->ref();

    if (m_currentViewpoint)
        m_currentViewpoint->unref();
    m_currentViewpoint = m_defaultViewpoint;
    m_currentViewpoint->ref();

    m_viewpointUpdated = true;
}

Scene::~Scene()
{
    TheApp->SetIntPreference("TransformMode",m_transformMode->tmode);
    TheApp->SetIntPreference("TransformDimension",m_transformMode->tdimension);
    TheApp->SetIntPreference("Transform2Axes",m_transformMode->t2axes);

    TheApp->SetIntPreference("SelectionMode", m_selectionMode);
/*
    while (!m_undoStack.empty()) delete m_undoStack.pop();

    while (!m_redoStack.empty()) delete m_redoStack.pop();
*/

    m_defaultViewpoint->unref();
    m_currentViewpoint->unref();

    delete m_selection;
    m_selection = NULL;    

    m_root->unref();

    ProtoMap::Chain::Iterator *j;

    for (int i = 0; i < m_protos.width(); i++) {
        for (j = m_protos.chain(i).first(); j != NULL; j = j->next()) {
            delete j->item()->getData();
        }
    }
    for (int i = 0; i < m_fonts.size(); i++) {
        delete m_fonts[i];
    }
    gluDeleteQuadric(m_obj3dCursor);
}

void Scene::updateSceneMap(void)
{
    SceneProtoMap::updateProtoMap(&m_protos, this);
}

void Scene::def(const char *nodeName, Node *value)
{
    if (value && nodeName && (strlen(nodeName) > 0)) {
        value->setName(nodeName);
        m_nodeMap[nodeName] = value;
        Proto *proto = value->getProto();
        if (proto && (proto->isDynamicProto()))
            proto->buildExportNames(nodeName);
    }
}

void Scene::undef(MyString nodeName)
{
    if (nodeName && nodeName.length() > 0) {
       m_defNode = m_nodeMap[nodeName];
       if (m_defNode)
           m_nodeMap.remove(nodeName);
    }
}

Node *Scene::use(const char *nodeName)
{
    return m_nodeMap[nodeName];
}

bool Scene::canUse(Node *parentNode, int parentField)
{
    if (m_defNode == NULL)
        return false;
    // avoid to create recursive scenegraphs
    if (m_defNode == parentNode)
        return false; 
    return getDestField(m_defNode, parentNode, parentField) >= 0;
}

bool Scene::use(Node *parentNode, int parentField)
{
    // avoid to create recursive scenegraphs
    Node *testRecursive = parentNode;
    while (testRecursive->hasParent()) {
       if (testRecursive == m_defNode)
           return false;
       testRecursive = testRecursive->getParent();
    }

    int destField = getDestField(m_defNode, parentNode, parentField);
    if (destField >= 0)
        execute(new MoveCommand(m_defNode, NULL, -1, parentNode, destField));
    return true;
}

int Scene::getDestField(Node* src, Node *dest, int destField)
{
    int field = destField;
    if (field == -1) 
        field = dest->findValidField(src);
    if (dest->validChildType(field, src->getNodeClass()) ||
        dest->validChildType(field, src->getType())) {
        if (src->isInvalidChildNode()) {
            if (dest->getType() == VRML_NURBS_GROUP)
                return field;
            if (dest->getProto()->getField(field)->getNodeType() & CHILD_NODE)
                return -1; 
        }
        return field;
    }
    return -1;    
}

int
Scene::addSymbol(MyString s)
{
    int &id = m_symbols[s];

    if (id == 0) {
        id = m_numSymbols++;
        m_symbolList[id] = s;
    }
    return id;
}

const MyString &
Scene::getSymbol(int id) const
{
    return m_symbolList[id];
}

void
Scene::setNodes(NodeList *nodes)
{
    ((NodeGroup *)m_root)->children(new MFNode(nodes));
}

static bool searchViewPortOrParticles(Node *node, void *data)
{
    Scene *scene = (Scene *)data;
    if (node->getType() == X3D_VIEWPORT) {
        scene->addViewPort(node);
    } else if (node->getType() == X3D_LAYOUT_GROUP) {
        scene->addViewPort(node);
    } else if (node->getType() == X3D_LAYOUT_LAYER) {
        scene->addViewPort(node);
    } else if (node->getType() == X3D_PARTICLE_SYSTEM) {
        scene->setParticleSystem(true);
    }
    return true;
}     

static bool searchMovieTexture(Node *node, void *data)
{
    Scene *scene = (Scene *)data;
    if (node->getType() == VRML_MOVIE_TEXTURE) {
        scene->setMovieTexture(true);
    }
    return true;
}     

void
Scene::setViewPorts(void)
{
    m_viewports.resize(0);
    m_hasParticleSystem = false;
    m_hasMovieTexture = false;
    m_root->doWithBranch(searchViewPortOrParticles, this);
    m_root->doWithBranch(searchMovieTexture, this);
}

void
Scene::addNodes(Node *targetNode, int targetField, NodeList *nodes, int scanFor)
{
    if (targetNode == NULL)
        m_root->addFieldNodeList(m_rootField, nodes);
    else if (targetField == -1)
        targetNode->addFieldNodeList(m_rootField, nodes);
    else if (targetNode->getField(targetField)->getType() == MFNODE) {
        targetNode->setField(targetField, new MFNode(nodes));        
    } else if (targetNode->getField(targetField)->getType() == SFNODE) {
        SFNode *oldSFNode = (SFNode *)targetNode->getField(targetField);
        MoveCommand *removeCommand = new MoveCommand(oldSFNode->getValue(), 
                                                     targetNode, targetField,
                                                     NULL, -1);
        removeCommand->execute();
        if (nodes->size() > 0) {
            Node *lastNode = nodes->get(nodes->size() - 1);
            MoveCommand *addCommand = new MoveCommand(lastNode, NULL, -1,
                                                      targetNode, targetField);
           addCommand->execute();
       }
    } else {
        // wrong targetField
        assert(0);
    }
    if ((scanFor == SCAN_FOR_BOTH) || (scanFor == SCAN_FOR_EXTERN_PROTO))
        scanForExternProtos(nodes);
    if ((scanFor == SCAN_FOR_BOTH) || (scanFor == SCAN_FOR_INLINE))
        scanForInlines(nodes);
    scanForMultimedia(nodes);
    nodes->clearFlag(NODE_FLAG_TOUCHED);
    for (int i = 0; i < nodes->size(); i++)
        if (nodes->get(i))
            nodes->get(i)->doWithBranch(searchViewPortOrParticles, this, 
                                        false);
}

static bool loadInline(Node *node, void *data)
{
    if ((node->getType() == VRML_INLINE) ||
        (node->getType() == VRML_INLINE_LOAD_CONTROL))
        if (TheApp->loadNewInline()) {
            NodeInline *nodeInline =(NodeInline *)node;
            node->getScene()->readInline(nodeInline);
            if (node->getLoadedNodes() != NULL)
                for (int j = 0; j < node->getLoadedNodes()->size(); j++) {
                    Node *inlinedNode = node->getLoadedNodes()->get(j);
                    inlinedNode->doWithBranch(loadInline, NULL);
                }
        }
    return true;
}

static bool readInlines(Node *node, void *data)
{
    if ((node->getType() == VRML_INLINE) ||
        (node->getType() == VRML_INLINE_LOAD_CONTROL)) 
        loadInline(node, NULL);
    return true;
}     

void 
Scene::scanForInlines(NodeList *nodes)
{
     nodes->doWithBranch(readInlines, NULL, false, true, false, true, false);
}

static bool loadMultimedia(Node *node, void *data)
{
    if ((node->getType() == VRML_IMAGE_TEXTURE) ||
        (node->getType() == VRML_AUDIO_CLIP) ||
        (node->getType() == VRML_MOVIE_TEXTURE) ||
        (node->getType() == VRML_GEO_LOD) || 
        (node->getType() == X3DOM_BINARY_GEOMETRY) ||
        (node->getType() == X3DOM_EXTERNAL_GEOMETRY) ||
        (node->getType() == X3DOM_POP_GEOMETRY_LEVEL) ||
        (node->getType() == X3DOM_MULTI_PART) ||
        (node->getType() == X3D_IMAGE_TEXTURE_3D) ||
        (node->getType() == X3D_DIS_ENTITY_TYPE_MAPPING))
        node->load();
    return true;
}


void                
Scene::scanForMultimedia(NodeList *nodes)
{
     nodes->doWithBranch(loadMultimedia, NULL, false, true, false, true, false);
}


static bool recreateNodePROTO(Node *node, void *data)
{
    if (node->isPROTO() && (node->getProto()->isLoaded())) {
        NodePROTO *protoNode = (NodePROTO *)node;
        if (!(protoNode->isLoaded())) {
            protoNode->handleIs();
            protoNode->createPROTO();
            protoNode->reInit();
        }
    }
    return true;
}     

void 
Scene::scanForExternProtos(NodeList *nodes)
{
    ProtoMap::Chain::Iterator *j;
    for (int i = 0; i < m_protos.width(); i++) {
        for (j = m_protos.chain(i).first(); j != NULL; j = j->next()) {
            Proto *proto = j->item()->getData();
            if (proto == NULL)
                continue;
            if (belongsToNodeWithExternProto(proto->getName(isX3d())))
                continue;
            if (proto->isCoverProto() || 
                proto->isKambiProto() || 
                proto->isX3domProto())
                continue;
            if (proto->isX3dInternalProto())
                continue;
            if (proto == NULL)
                continue;
            if (proto->isLoaded())
                continue;
            if (proto->isLoading())
                continue;
#ifdef NURBS_CURVE_ANIMATION_COMPATIBILTY
            if (strcmp(proto->getName(true), "NurbsCurveAnimation") == 0) {
                readExternProto(proto);
                continue;
            }                        
#endif
            if ((proto->getUrls() != NULL) && (!proto->isInternUrl())) {
                if (!readExternProto(proto)) {
                    MFString *urls = (MFString *)proto->getUrls();
                    MyString files = "";
                    for (int i = 0; i < urls->getSize(); i++) {
                         files += urls->getValue(i);
                         files += " ";
                    }     
                    TheApp->MessageBox(IDS_EXTERNPROTO_FILE_FAILED, 
                                       proto->getName(isX3d()), files);
                }
            }
        }
    }
    nodes->doWithBranch(recreateNodePROTO, NULL, false, true, false, true, 
                                                 false);
}

void 
Scene::addToNodeList(Node *node)
{
    m_nodeList.append(node);
}

static bool checkNodeType(Node *node, void *data)
{
    int *nodeType = (int *) data;
    if (node->getType() == *nodeType)
        node->getScene()->addToNodeList(node);
    return true;
}     

NodeList *
Scene::searchNodeType(int nodeType)
{
    m_nodeList.resize(0);
    m_root->doWithBranch(checkNodeType, &nodeType);
    NodeList *nodeList = new NodeList();
    for (int i = 0; i < m_nodeList.size(); i++)
        nodeList->append(m_nodeList[i]);  
        // delete returned NodeList after usage
    return nodeList;
}

void 
Scene::readInline(NodeInline *node)
{
    if (node->alreadyLoaded()) {
        return;
    }
    MyString oldDir = "";
    oldDir += TheApp->getImportURL();
    MFString *urls = node->url();
    if (urls == NULL)
        return;
    for (int j = 0; j < urls->getSize(); j++) {
        if (urls->getValue(j).length() == 0) 
            continue;
        URL url(oldDir, urls->getValue(j));
        MyString path;
        if (Download(url, &path)) {
            TheApp->setImportURL(url.GetPath());
            struct stat fileStat;
            MyString filename = "";
            filename += path;
            if (stat(filename, &fileStat) == 0) {
                if (S_ISREG(fileStat.st_mode)) {
                    bool oldX3d = isX3d();
                    FILE *file = fopen(filename, "rb");
                    if (file == NULL) {
                        TheApp->MessageBoxPerror(IDS_INLINE_FILE_FAILED,
                                                 filename);
                        continue;
                    }
                    double oldUnitLength = getUnitLength();
                    pushUnitLength();
                    TheApp->setImportFile(filename);
                    parse(file, node, -1, SCAN_FOR_BOTH);
                    node->setUnitLength(oldUnitLength / getUnitLength());
                    popUnitLength();
                    fclose(file);
                    if (oldX3d && (!isX3d()))
                        setX3d();
                    else if ((!oldX3d) && isX3d())             
                        setVrml();
                    break;
                }
            }
        }
    }    
    TheApp->setImportURL(oldDir);
}

bool                
Scene::readExternProto(Proto *proto)
{
    if (proto->isInternUrl())
        return true;
    if (proto->isDefined() && proto->isLoaded())
        return true;
    MyString oldDir = "";
    oldDir += TheApp->getImportURL();
    FILE* file = NULL;
    bool oldX3d = isX3d();
    MFString *urls = (MFString *)proto->getUrls();
    if (urls == NULL)
        return false;
    for (int j = 0; j < urls->getSize(); j++) {
        if (urls->getValue(j).length() == 0) 
            continue;
        URL url(oldDir, urls->getValue(j));
        url.TrimTopic();
        MyString path;
        if (Download(url, &path)) {
            struct stat fileStat;
            const char *filename = path;
            if (stat(filename, &fileStat) == 0) {
                if (S_ISREG(fileStat.st_mode)) {
                    file = fopen(filename, "rb");
                    if (file == NULL)
                        continue;
                    Node *oldTargetNode = targetNode;
                    int oldTargetField = targetField;
                    // fake root node, will be ignored later
                    NodeGroup *node = (NodeGroup *)createNode("Group");
                    if (proto->isLoading()) {
                        fclose(file);
                        return true;
                    }
                    proto->setLoading(true);
                    double oldUnitLength = getUnitLength();
                    pushUnitLength();
                    pushUnitAngle();
                    TheApp->setImportURL(oldDir);
                    TheApp->setImportFile(filename);
                    parse(file, node, node->children_Field(), 
                          SCAN_FOR_EXTERN_PROTO);
                    proto->setLoaded(true);
                    proto->setUnitLength(oldUnitLength / getUnitLength());
                    proto->setUnitAngle(getUnitAngle());
                    popUnitAngle();
                    popUnitLength();
                    proto->setLoading(false);
                    targetNode = oldTargetNode;
                    targetField = oldTargetField;
                    fclose(file);
                    if (oldX3d && (!isX3d()))
                        setX3d();
                    else if ((!oldX3d) && isX3d())             
                        setVrml();
                    updateNodePROTOs(proto);
                    break;
                }
            }
        }
    }
    if (file == NULL)
        return false;
    return true;
}


static bool getProfileCallback(Node *node, void *data)
{
    int *currentProfile = (int *)data;
    if (node->getProfile() > *currentProfile)
        *currentProfile = node->getProfile();
    return true;     
}


int
Scene::writeRouteStrings(int filedes, int indent, bool end)
{
    static bool alreadyIn = false;
    // avoid recursive call of writeRouteStrings via Node::write
    if (alreadyIn)
        return 0;
    alreadyIn = true;
    // write tempory nodes first
    for (int i = 0; i < m_delayedWriteNodes.size(); i++) {
        int ret = 0;
        if (isX3dXml()) 
            ret = m_delayedWriteNodes[i]->writeXml(filedes, indent);
        else
            ret = m_delayedWriteNodes[i]->write(filedes, indent);
        if (ret < 0) {
            alreadyIn = false;
            return ret;
        }
        removeNode(m_delayedWriteNodes[i]);
    }
    if (m_delayedWriteNodes.size() > 0)
        m_delayedWriteNodes.resize(0);
    alreadyIn = false;

    if (m_routeList.size() != 0) {
       for (List<MyString>::Iterator* routepointer = m_routeList.first();
            routepointer != NULL; routepointer = routepointer->next() ) 
          {
          RET_ONERROR( indentf(filedes, indent) )
          RET_ONERROR( mywritestr(filedes, (const char*) routepointer->item()) )
          RET_ONERROR( mywritestr(filedes, "\n") )
          TheApp->incSelectionLinenumber();
          }
       RET_ONERROR( mywritestr(filedes, "\n") )
       TheApp->incSelectionLinenumber();
       m_routeList.removeAll();
    }
    if (end && m_endRouteList.size() != 0) {
       for (List<MyString>::Iterator* routepointer = m_endRouteList.first();
            routepointer != NULL; routepointer = routepointer->next() ) 
          {
          RET_ONERROR( indentf(filedes, indent) )
          RET_ONERROR( mywritestr(filedes, (const char*) routepointer->item()) )
          RET_ONERROR( mywritestr(filedes, "\n") )
          TheApp->incSelectionLinenumber();
          }
       RET_ONERROR( mywritestr(filedes, "\n") )
       TheApp->incSelectionLinenumber();
       m_endRouteList.removeAll();
    }
    return 0;
}

int Scene::writeExternProto(int f, const char* protoName)
{
    // search if EXTERNPROTO already exist
    bool foundProto = false;
    for (int i = 0; i < m_numProtoNames; i++)
        if (strcmp((const char*)m_protoNames[i], protoName)==0) {
            foundProto = true;   
            // force writing of some nodes despite EXTERNPROTO already exist
            for (int j = 0; j < m_nodesForceExternProtoWrite.size(); j++)
                if (strcmp(m_nodesForceExternProtoWrite[j], protoName) == 0)
                    foundProto = false;
        }        

    if (!foundProto) {
        // write EXTERNPROTO
        const NodeList *nodes = getNodes();
        for (int i = 0; i < nodes->size(); i++) {
            Node *node = nodes->get(i);
            if (node->isInScene(this)) {
                const char *nodeName = node->getProto()->getName(isX3d());
                if (strcmp(nodeName, protoName)==0) {
                    RET_ONERROR( node->writeProto(f) )
                    RET_ONERROR( mywritestr(f ,"\n\n") )
                    TheApp->incSelectionLinenumber(2);
                    break;
                }
            }
        }
    }
    return 0;
}


static bool avoidProtoOnPureVrml(MyString name)
{
    if (strcmp(name, "SuperEllipsoid") == 0)
        return true;
    if (strcmp(name, "SuperExtrusion") == 0)
        return true;
    if (strcmp(name, "SuperRevolver") == 0)
        return true;
    if (strcmp(name, "SuperShape") == 0)
        return true;
    if (strcmp(name, "CurveAnimation") == 0)
        return true;
    if (strcmp(name, "NurbsCurve") == 0)
        return true;
    if (strcmp(name, "NurbsCurve2D") == 0)
        return true;
    if (strcmp(name, "NurbsGroup") == 0)
        return true;
    if (strcmp(name, "NurbsOrientationInterpolator") == 0)
        return true;
    if (strcmp(name, "NurbsPositionInterpolator") == 0)
        return true;
    if (strcmp(name, "NurbsSet") == 0)
        return true;
    if (strcmp(name, "NurbsSurface") == 0)
        return true;
    if (strcmp(name, "NurbsSurfaceInterpolator") == 0)
        return true;
    if (strcmp(name, "NurbsSweptSurface") == 0)
        return true;
    if (strcmp(name, "NurbsSwungSurface") == 0)
        return true;
    if (strcmp(name, "NurbsTextureCoordinate") == 0)
        return true;
    if (strcmp(name, "NurbsTextureSurface") == 0)
        return true;
    if (strcmp(name, "NurbsTrimmedSurface") == 0)
        return true;
    if (strcmp(name, "Contour2D") == 0)
        return true;
    if (strcmp(name, "CoordinateDeformer") == 0)
        return true;
    if (strcmp(name, "Polyline2D") == 0)
        return true;
    if (strcmp(name, "TrimmedSurface") == 0)
        return true;
    if (strcmp(name, "KambiAppearance") == 0)
        return true;
    if (strcmp(name, "KambiHeadLight") == 0)
        return true;
    if (strcmp(name, "KambiInline") == 0)
        return true;
    if (strcmp(name, "KambiNavigationInfo") == 0)
        return true;
    if (strcmp(name, "KambiOctreeProperties") == 0)
        return true;
    if (strcmp(name, "KambiTriangulation") == 0)
        return true;
    if (strcmp(name, "Text3D") == 0)
        return true;

    return false;
}

static bool avoidProtoOnX3dom(MyString name)
{
    return avoidProtoOnPureVrml(name);
}

static bool avoidProtoOnX3d(MyString name)
{
    if (strcmp(name, "NurbsCurve") == 0)
        return true;
    if (strcmp(name, "NurbsCurve2D") == 0)
        return true;
    if (strcmp(name, "NurbsGroup") == 0)
        return true;
    if (strcmp(name, "NurbsOrientationInterpolator") == 0)
        return true;
    if (strcmp(name, "NurbsPositionInterpolator") == 0)
        return true;
    if (strcmp(name, "NurbsSet") == 0)
        return true;
    if (strcmp(name, "NurbsSurface") == 0)
        return true;
    if (strcmp(name, "NurbsSurfaceInterpolator") == 0)
        return true;
    if (strcmp(name, "NurbsTextureSurface") == 0)
        return true;
    if (strcmp(name, "NurbsSweptSurface") == 0)
        return true;
    if (strcmp(name, "NurbsSwungSurface") == 0)
        return true;
    if (strcmp(name, "MultiTexture") == 0)
        return true;
    if (strcmp(name, "MultiTextureCoordinate") == 0)
        return true;
    if (strcmp(name, "MultiTextureTransform") == 0)
        return true;
    if (strcmp(name, "Contour2D") == 0)
        return true;
    if (strcmp(name, "LoadSensor") == 0)
        return true;
    if (strcmp(name, "Arc2D") == 0)
        return true;
    if (strcmp(name, "ArcClose2D") == 0)
        return true;
    if (strcmp(name, "BooleanFilter") == 0)
        return true;
    if (strcmp(name, "BooleanToggle") == 0)
        return true;
    if (strcmp(name, "BooleanTrigger") == 0)
        return true;
    if (strcmp(name, "Circle2D") == 0)
        return true;
    if (strcmp(name, "ColorRGBA") == 0)
        return true;
    if (strcmp(name, "ContourPolyline2D") == 0)
        return true;
    if (strcmp(name, "CoordinateInterpolator2D") == 0)
        return true;
    if (strcmp(name, "Disk2D") == 0)
        return true;
    if (strcmp(name, "FillProperties") == 0)
        return true;
    if (strcmp(name, "IntegerSequencer") == 0)
        return true;
    if (strcmp(name, "IntegerTrigger") == 0)
        return true;
    if (strcmp(name, "KeySensor") == 0)
        return true;
    if (strcmp(name, "LineProperties") == 0)
        return true;
    if (strcmp(name, "Polypoint2D") == 0)
        return true;
    if (strcmp(name, "Polyline2D") == 0)
        return true;
    if (strcmp(name, "PositionInterpolator2D") == 0)
        return true;
    if (strcmp(name, "Rectangle2D") == 0)
        return true;
    if (strcmp(name, "StringSensor") == 0)
        return true;
    if (strcmp(name, "TimeTrigger") == 0)
        return true;
    if (strcmp(name, "TriangleSet2D") == 0)
        return true;
    if (strcmp(name, "InlineLoadControl") == 0)
        return true;
    return false;
}

static bool avoidComponentOnPureX3dv(MyString name)
{
    if (strcmp(name, "NURBS") == 0)
        return true;
    return false;
}

static bool getComponents(Node *node, void *data)
{
    StringMap *components = (StringMap *) data;
    if (node->isPROTO())
        ((NodePROTO *)node)->getComponentsInBranch(getComponents, data);
    else {   
        int level = node->getComponentLevel();
        if (level != -1) {
            const char* name = node->getComponentName();
            StringMap::Chain::Iterator *j;
            bool hasAlreadyName = false;
            for (int i = 0; i < components->width(); i++)
                for (j = components->chain(i).first(); j != NULL; j = j->next())
                    if (strcmp(name, j->item()->getKey()) == 0)
                        hasAlreadyName = true;
            bool x3d = node->getScene()->isX3d();
            if (node->getScene()->isPureX3dv() && 
                avoidComponentOnPureX3dv(node->getProto()->getName(x3d)))
                return true;
            if (!hasAlreadyName)
                (*components)[name] = level;
            else if (level > (*components)[name])
                (*components)[name] = level;
        }
    }
    return true;
}

int
Scene::writeComponents(int f)
{
    StringMap components;
    m_root->doWithBranch(getComponents, &components);

    if (components.width() == 0)
        return 0;

    StringMap::Chain::Iterator *j;
    for (int i = 0; i < components.width(); i++)
        for (j = components.chain(i).first(); j != NULL; j = j->next())
            if (j->item()->getData() != -1) {
                if (isX3dv())
                    RET_ONERROR( mywritestr(f, "COMPONENT ") )
                else if (isX3dXml()) {
                    RET_ONERROR( indentf(f, TheApp->GetIndent()) )
                    RET_ONERROR( mywritestr(f, "<component name='") )
                }
                RET_ONERROR( mywritestr(f, (const char *) j->item()->getKey()) )
                if (isX3dv())
                    RET_ONERROR( mywritestr(f, ":") )
                else if (isX3dXml())
                    RET_ONERROR( mywritestr(f, "' level='") )
                RET_ONERROR( mywritef(f, "%d", j->item()->getData()) ) 
                if (isX3dXml())
                    RET_ONERROR( mywritestr(f, "'/>") )
                RET_ONERROR( mywritestr(f, "\n") )
                TheApp->incSelectionLinenumber();
            }

    return 0;            
}

#define RET_RESET_FLAGS_ONERROR(x) RET_AND_RESET_ONERROR(x, \
{    \
    TheApp->disableEFloatWriteFormat(m_writeFlags); \
    if (m_writeFlags & TEMP_EXPORT) \
        m_writeFlags = oldWriteFlags; \
}) 


static bool getVariableNames(Node *node, void *data)
{
    if (node != NULL) {
        node->getVariableName();
        if (node->isPROTO()) {
            NodePROTO *protoNode = (NodePROTO *)node;
            for (int i = 0; i < protoNode->getNumProtoNodes(); i++)
                protoNode->getProtoNode(i)->doWithBranch(getVariableNames, 
                                                         data);
            }
    }
    return true;
}

static bool generateConvertedNodes(Node *node, void *data)
{
    int *writeFlags = (int *)data;
    if (node != NULL) {
        node->addToConvertedNodes(*writeFlags);
    }
    return true;
}

static bool convertBackAndDeleteConvertedNodes(Node *node, void *data)
{
    Scene *scene = (Scene *)data;
    node->deleteConvertedNodes();
    if ((node->getType() != VRML_NURBS_GROUP) && !scene->isX3d())
        node->convert2Vrml();
    return true;
}

static bool markUsedProto(Node *node, void *data)
{
    Scene *scene = (Scene *)data;
    if (node == NULL)
        return true;
    Proto *proto = scene->getProto(node->getProto()->getName(false));
    if (proto == NULL)
        return true;
    proto->setInUse(true);
    proto->setIsInScene(true);
    if ((node->getType() == VRML_INLINE) || 
        (node->getType() == VRML_INLINE_LOAD_CONTROL)) {
        NodeList *nodelist = node->getLoadedNodes();
        if (nodelist != NULL)
            for (int i = 0; i < nodelist->size(); i++)
                nodelist->get(i)->doWithBranch(markUsedProto, data);
    } 

    return true;
}

int Scene::write(int f, const char *url, int writeFlags, char *wrlFile)
{
    if (!(writeFlags & SKIP_SAVED_TEST)) {
        if (writeFlags & X3DV)
            if (m_saved_x3dv == true)
                return 0;
        if (writeFlags & X3D_XML)
            if (m_saved_x3dxml == true)
                return 0;
    }    

    TheApp->setWriteUrl(url);
    ProtoMap::Chain::Iterator *j;
    for (int i = 0; i < m_protos.width(); i++)
        for (j = m_protos.chain(i).first(); j != NULL; j = j->next())
            if (j->item()->getData() != NULL)
                j->item()->getData()->setInUse(false);
    m_root->doWithBranch(markUsedProto, this);

    int oldWriteFlags = m_writeFlags;
    bool done = false;
    int ret = 0;
    bool x3dv = ::isX3dv(writeFlags);
    if (writeFlags & (TRIANGULATE | (C_SOURCE | CC_SOURCE | JAVA_SOURCE))) {
        m_root->doWithBranch(generateConvertedNodes, &writeFlags);
    }
    if (writeFlags & (C_SOURCE | CC_SOURCE | JAVA_SOURCE))
        if (isX3d())
            x3dv = true;
    writeFlags = writeFlags & (~(TRIANGULATE));
    bool oldXml = isX3dXml();
    if (x3dv)
        setX3dv();
    else if (::isX3dXml(writeFlags))
        setX3dXml();
    else
        setVrml();
    if (writeFlags & (C_SOURCE | CC_SOURCE | JAVA_SOURCE)) {
        m_root->doWithBranch(getVariableNames, &writeFlags);
    }
    if (writeFlags & AC3D) {
        ret = writeAc3d(f, writeFlags & AC3D_4_RAVEN);
        done = true;
    } else if (writeFlags & RIB) {
        ret = writeRib(f, url);
        done = true;
    } else if (writeFlags & KANIM) {
        ret = writeKanim(f, url);
        done = true;
    } else if (writeFlags & LDRAW_DAT) {
        ret = writeLdrawDat(f, url);
        done = true;
    } else if (writeFlags & C_SOURCE) {
        ret = writeC(f, writeFlags);
        done = true;
    } else if (writeFlags & CC_SOURCE) {
        ret = writeC(f, writeFlags);
        done = true;
    } else if (writeFlags & JAVA_SOURCE) {
        ret = writeC(f, writeFlags);
        done = true;
    } else if (writeFlags & WONDERLAND) {
        URL *fileUrl = new URL(getURL());
        const char *fileName = fileUrl->GetFileNameWithoutExtension();
        ret = TheApp->writeWonderlandModule(url, fileName, this, 
                                            writeFlags & MANY_JAVA_CLASSES);
        done = true;
    }
    if (done) {
        m_writeFlags = oldWriteFlags;
        m_root->doWithBranch(convertBackAndDeleteConvertedNodes, this);
        return ret;
    }
    TheApp->disableEFloatWriteFormat(writeFlags);
    m_newURL = url;

    bool convertFromXml = false;
    if (oldXml && !::isX3dXml(writeFlags))
        convertFromXml=true;
    bool convertToXml = false;
    if (!oldXml && ::isX3dXml(writeFlags))
        convertToXml=true;
    m_writeFlags = writeFlags;
    if (convertToXml)
        m_writeFlags |= CONVERT_TO_XML;
    if (convertFromXml)
        m_writeFlags |= CONVERT_FROM_XML;

    getNodes()->clearFlag(NODE_FLAG_DEFED);
    getNodes()->clearFlag(NODE_FLAG_TOUCHED);

    // remove multiple identical ProtoDefinitions smuggled in by Inline nodes
    for (int i = 0;i < m_numProtoDefinitions;i++) {
        for (int j = 0;j < m_numProtoDefinitions;j++) 
           if (i != j)
               if (strcmp((const char*)m_protoDefinitions[i],
                         (const char*)m_protoDefinitions[j]) == 0)
                   m_protoDefinitions[j] = "";
    }

    int indent = 0;
    if (writeFlags & X3DOM) {
        RET_RESET_FLAGS_ONERROR( mywritestr(f,"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\n") )
        RET_RESET_FLAGS_ONERROR( mywritestr(f,"  \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n") )
        RET_RESET_FLAGS_ONERROR( mywritestr(f,"<html xmlns=\"http://www.w3.org/1999/xhtml\">\n") )
        RET_RESET_FLAGS_ONERROR( mywritestr(f,"  <head>\n") )
        RET_RESET_FLAGS_ONERROR( mywritestr(f,"    <title></title>\n") )
        RET_RESET_FLAGS_ONERROR( mywritef(f,
                                          "    <link rel=\"stylesheet\" type=\"text/css\" href=\"%sx3dom.css\" />\n", 
                                          TheApp->GetX3domPath()) )
        RET_RESET_FLAGS_ONERROR( mywritef(f,
                                          "    <script type=\"text/javascript\" src=\"%sx3dom-full.js\"></script>\n",
                                          TheApp->GetX3domPath()) )
        RET_RESET_FLAGS_ONERROR( writeHead(f, writeFlags) )
        RET_RESET_FLAGS_ONERROR( mywritestr(f,"  </head>\n") )
        RET_RESET_FLAGS_ONERROR( mywritestr(f,"  <body>\n") )
        RET_RESET_FLAGS_ONERROR( mywritef(f,"    <x3d %s>\n",
                                              TheApp->GetX3domParameter()) )
        RET_RESET_FLAGS_ONERROR( mywritestr(f ,"      <Scene>\n") )
        indent = 2 * TheApp->GetIndent();
    } else if (writeFlags & XITE) {
        RET_RESET_FLAGS_ONERROR( mywritestr(f,"<!DOCTYPE html>\n") )
        RET_RESET_FLAGS_ONERROR( mywritestr(f,"<html>\n") )
        RET_RESET_FLAGS_ONERROR( mywritestr(f,"  <head>\n") )
        RET_RESET_FLAGS_ONERROR( mywritestr(f,"    <title></title>\n") )
        RET_RESET_FLAGS_ONERROR( mywritef(f,
                                          "    <link rel=\"stylesheet\" type=\"text/css\" href=\"%sx_ite.css\" />\n", 
                                          TheApp->GetXitePath()) )
        RET_RESET_FLAGS_ONERROR( mywritef(f,
                                          "    <script type=\"text/javascript\" src=\"%sx_ite.min.js\"></script>\n",
                                          TheApp->GetXitePath()) )
        RET_RESET_FLAGS_ONERROR( mywritef(f,
                                          "    <script type=\"text/javascript\" src=\"%srigid-body-physics.min.js\"></script>\n",
                                          TheApp->GetXitePath()) )
        RET_RESET_FLAGS_ONERROR( mywritestr(f,"  <style>\n") )
        RET_RESET_FLAGS_ONERROR( mywritestr(f,"X3DCanvas {\n") )
        RET_RESET_FLAGS_ONERROR( mywritestr(f,"  width: 100%;\n") )
        RET_RESET_FLAGS_ONERROR( mywritestr(f,"  height: 432px;\n") )
        RET_RESET_FLAGS_ONERROR( mywritestr(f,"}\n") )
        RET_RESET_FLAGS_ONERROR( mywritestr(f,"  </style>\n") )
        RET_RESET_FLAGS_ONERROR( mywritestr(f,"  \n") )
        RET_RESET_FLAGS_ONERROR( mywritestr(f,"  </head>\n") )
        RET_RESET_FLAGS_ONERROR( mywritestr(f,"  <body>\n") )
        RET_RESET_FLAGS_ONERROR( mywritestr(f,"  <p margin: 1px 0;>") )
        RET_RESET_FLAGS_ONERROR( mywritef(f,"<X3DCanvas url='\"%s\"'/></p>\n", 
                                          wrlFile ? wrlFile : url) )
        RET_RESET_FLAGS_ONERROR( mywritestr(f,"  </body>\n") )
        RET_RESET_FLAGS_ONERROR( mywritestr(f ,"</html>\n") )
        return 0;
    } else if (::isX3d(writeFlags)) {
        if (::isX3dXml(writeFlags)) {
            RET_RESET_FLAGS_ONERROR( mywritestr(f, 
                  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n") )
            TheApp->incSelectionLinenumber();
            RET_RESET_FLAGS_ONERROR( mywritestr(f,"<!DOCTYPE X3D PUBLIC ") )
            RET_RESET_FLAGS_ONERROR( mywritestr(f,"\"ISO//Web3D//DTD ") )
            RET_RESET_FLAGS_ONERROR( mywritef(f,"X3D 3.%d//EN\" ", 
                                              getX3dVersion() ) )
            RET_RESET_FLAGS_ONERROR( mywritef(f, 
                  "\"http://www.web3d.org/specifications/x3d-3.%d.dtd\">\n",
                  getX3dVersion()) )
            TheApp->incSelectionLinenumber();
            RET_RESET_FLAGS_ONERROR( mywritestr(f ,"<X3D profile='") )
        } else {
            RET_RESET_FLAGS_ONERROR( mywritef(f ,"#X3D V3.%d utf8\n",
                                              getX3dVersion()) )
            TheApp->incSelectionLinenumber();
            RET_RESET_FLAGS_ONERROR( mywritestr(f ,"PROFILE ") )
        }
        int profile = PROFILE_CORE;
        getRoot()->doWithBranch(getProfileCallback, &profile);
        switch (profile) {
          case PROFILE_CORE:
            RET_RESET_FLAGS_ONERROR( mywritestr(f ,"Core") )
            break;
          case PROFILE_INTERCHANGE:
            RET_RESET_FLAGS_ONERROR( mywritestr(f ,"Interchange") )
            break;
          case PROFILE_INTERACTIVE:
            RET_RESET_FLAGS_ONERROR( mywritestr(f ,"Interactive") )
            break;
          case PROFILE_MPEG4_INTERACTIVE:
            RET_RESET_FLAGS_ONERROR( mywritestr(f ,"MPEG-4 interactive") )
            break;
          case PROFILE_IMMERSIVE:
            RET_RESET_FLAGS_ONERROR( mywritestr(f ,"Immersive") )
            break;
          default:  //  PROFILE_FULL
            RET_RESET_FLAGS_ONERROR( mywritestr(f ,"Full") )
            break;
        }
        if (::isX3dXml(writeFlags)) {
            RET_RESET_FLAGS_ONERROR( mywritef(f ,"' Version='3.%d>",
                                              getX3dVersion()) )
            RET_RESET_FLAGS_ONERROR( mywritestr(f ,"'>") )
        }
        RET_RESET_FLAGS_ONERROR( mywritestr(f ,"\n") )
        TheApp->incSelectionLinenumber();    

        if (::isX3dXml(writeFlags)) {
            RET_ONERROR( mywritestr(f, "<head>\n") )
            TheApp->incSelectionLinenumber();
        }
        RET_RESET_FLAGS_ONERROR( writeComponents(f) )
        TheApp->incSelectionLinenumber();
        RET_RESET_FLAGS_ONERROR( mywritestr(f ,"\n") )
        TheApp->incSelectionLinenumber();

        RET_RESET_FLAGS_ONERROR( writeHead(f, writeFlags) )

        if (::isX3dXml(writeFlags)) {
            RET_ONERROR( mywritestr(f, "</head>\n") )
            TheApp->incSelectionLinenumber();
        }
    } else {
        RET_RESET_FLAGS_ONERROR( mywritestr(f ,"#VRML V2.0 utf8\n\n") )
        TheApp->incSelectionLinenumber(2);
    }
    if (isX3dXml()) {
    }
    if (::isX3dXml(m_writeFlags) && !(writeFlags & X3DOM)) {
        RET_RESET_FLAGS_ONERROR( mywritestr(f ,"<Scene>\n") )
        TheApp->incSelectionLinenumber();
    }
    if (writeFlags & (PURE_VRML97 | PURE_X3DV | X3DOM)) {
        if (TheApp->getCoverMode())
            writeExtensionProtos(f, FF_COVER_ONLY);
        if (TheApp->getKambiMode())
            writeExtensionProtos(f, FF_KAMBI_ONLY);
    }
    for (int i = 0;i < m_numProtoNames;i++) {
        MyString protoName = m_protoNames[i];
        if ((isPureVRML() && avoidProtoOnPureVrml(protoName)) ||
            (isX3d() && avoidProtoOnX3d(protoName)) ||
            ((writeFlags & X3DOM) && avoidProtoOnX3dom(protoName))) {
            if (belongsToNodeWithExternProto(protoName))
                continue;
        }
        bool writeProto = true;
        // avoid writing of Protos, which are forced written via ExternProto
        for (int j = 0; j < m_nodesForceExternProtoWrite.size(); j++)
            if (strcmp(m_nodesForceExternProtoWrite[j], protoName) == 0)
                writeProto = false;
        if (!writeProto)
            continue;

        if (TheApp->getPrefix() != NULL) {
            MyString prefix = TheApp->getPrefix();
            bool isPrefixProto = false;
            for (int j = 0; j < m_numProtoNames; j++) {
                if (strncmp(m_protoNames[j], prefix, prefix.length()) == 0)
                    if (strcmp(m_protoNames[j], protoName) == 0)
                        isPrefixProto = true;
            }
            if (!isPrefixProto) {
                for (int j = 0; j < m_numProtoNames; j++) {
                    if (strncmp(m_protoNames[j], prefix, prefix.length()) == 0)
                        m_protoDefinitions[i].gsubOnce(
                              m_protoNames[j] + prefix.length(), 
                              m_protoNames[j]);
                }
            }
        }   
        // avoid PROTOs from Proto Library, which are not used
        if (m_protos[protoName] != NULL)
            if (m_protos[protoName]->fromProtoLibrary() &&
                !m_protos[protoName]->isInUse())
                continue;   
        if (m_isNestedProto[i]) {
            if (isX3dXml()) {
                static char format[256];
                swLoadString(IDS_X3D_NESTED_PROTO_NOT_SUPPORTED, format, 255);
                errorf(format, (const char *)protoName);
            }
            RET_RESET_FLAGS_ONERROR( m_protos[protoName]->write(f, 0,
                                                                writeFlags) )
            // count end of line characters in protodefinitions
            const char* string = m_protoDefinitions[i];
            while ((string = strchr(string, '\n')) !=NULL) {
                TheApp->incSelectionLinenumber();
                string++;
            }
            RET_RESET_FLAGS_ONERROR( mywritestr(f ,"\n\n") )
            TheApp->incSelectionLinenumber(2);
        } else if (strlen(m_protoNames[i]) > 0)  {
            RET_RESET_FLAGS_ONERROR( m_protos[protoName]->write(f, indent, 
                                                                m_writeFlags) )
        }
    }

    for (int k = 0; k < m_nodesWithExternProto.size(); k++) {
        // do not write EXTERN PROTOs for some (e.g. Nurbs Nodes) when using 
        // pureVRML97 cause this nodes are written converted to pure VRML97
        bool doWriteExternProto = true;
        if (isPureVRML() && avoidProtoOnPureVrml(m_nodesWithExternProto[k]))
            doWriteExternProto = false;
        if (isX3d() && avoidProtoOnX3d(m_nodesWithExternProto[k]))
            doWriteExternProto = false;
        if (doWriteExternProto)
            RET_RESET_FLAGS_ONERROR( 
                writeExternProto(f, m_nodesWithExternProto[k]) 
            )
    }

    getNodes()->clearFlag(NODE_FLAG_TOUCHED);

    NodeList *childList = ((NodeGroup *)getRoot())->children()->getValues();
    for (int i = 0; i < childList->size(); i++) {
        if (::isX3dXml(m_writeFlags)) {
            int rootIndent = indent + TheApp->GetIndent();
            RET_RESET_FLAGS_ONERROR( childList->get(i)->writeXml(f, rootIndent))
        } else
            RET_RESET_FLAGS_ONERROR( childList->get(i)->write(f, 0) )
    }

    if ((!isTempSave()) && (!isPureVRML())) {
        m_unmodified = m_undoStack.empty() ? (Command *) NULL : m_undoStack.peek();
        m_extraModifiedFlag = false;
        MyString newURL = "";
        newURL += url;
        TheApp->setImportURL(newURL);
        m_URL = newURL;
    }

    RET_RESET_FLAGS_ONERROR( writeRouteStrings(f, indent + TheApp->GetIndent(),
                                               true) )

    if (::isX3dXml(m_writeFlags)) {
        RET_RESET_FLAGS_ONERROR( indentf(f, indent) )
        RET_RESET_FLAGS_ONERROR( mywritestr(f ,"</Scene>\n") )
        TheApp->incSelectionLinenumber();
        RET_RESET_FLAGS_ONERROR( indentf(f, indent) )
        if (writeFlags & X3DOM)
            RET_RESET_FLAGS_ONERROR( mywritestr(f ,"</x3d>\n") )
        else
            RET_RESET_FLAGS_ONERROR( mywritestr(f ,"</X3D>\n") )
        TheApp->incSelectionLinenumber();
    }
    if (writeFlags & X3DOM) {
        RET_RESET_FLAGS_ONERROR( indentf(f, indent > 0 ? 2 : 0) )
        RET_RESET_FLAGS_ONERROR( mywritestr(f ,"</body>\n") )
        TheApp->incSelectionLinenumber();
        RET_RESET_FLAGS_ONERROR( mywritestr(f ,"</html>\n") )
        TheApp->incSelectionLinenumber();
    }

    if (writeFlags & (PURE_VRML97 | PURE_X3DV | X3DOM))
        if (TheApp->getCoverMode())
            deleteExtensionProtos();
    TheApp->enableEFloatWriteFormat(writeFlags);
    if ((oldWriteFlags & (TEMP_EXPORT | TEMP_SAVE)) ||
        (writeFlags & (TEMP_EXPORT | TEMP_SAVE))) {
        resetWriteFlags(oldWriteFlags);
    }

    if (writeFlags & X3DV) 
        m_saved_x3dv = true;
    else if (writeFlags & X3D_XML)
        m_saved_x3dxml = true;

    return 0;
}

int Scene::writeHead(int f, int writeFlags)
{
    int oldWriteFlags = m_writeFlags;
    int maxMetas = m_metaKeys.size();
    if (m_metaValues.size() < maxMetas)
        maxMetas = m_metaValues.size(); 
    if (::isX3dXml(writeFlags))
        for (int i = 0; i < maxMetas; i++) {
            RET_RESET_FLAGS_ONERROR( indentf(f, TheApp->GetIndent()) )
            RET_RESET_FLAGS_ONERROR( mywritestr(f , "<meta name='") )
            RET_RESET_FLAGS_ONERROR( mywritestr(f , m_metaKeys[i]) )
            RET_RESET_FLAGS_ONERROR( mywritestr(f , "' content='") )
            RET_RESET_FLAGS_ONERROR( mywritestr(f , m_metaValues[i]) )
            RET_RESET_FLAGS_ONERROR( mywritestr(f , "' />\n") )
            TheApp->incSelectionLinenumber();
        }
    else
        for (int i = 0; i < maxMetas; i++) {
            RET_RESET_FLAGS_ONERROR( mywritestr(f , "META \"") )
            RET_RESET_FLAGS_ONERROR( mywritestr(f , m_metaKeys[i]) )
            RET_RESET_FLAGS_ONERROR( mywritestr(f , "\" \"") )
            RET_RESET_FLAGS_ONERROR( mywritestr(f , m_metaValues[i]) )
            RET_RESET_FLAGS_ONERROR( mywritestr(f , "\"\n") )
            TheApp->incSelectionLinenumber();
        }    
    if (maxMetas > 0) {
        RET_RESET_FLAGS_ONERROR( mywritestr(f , "\n") )
        TheApp->incSelectionLinenumber(2);
    }
    int maxUnits = m_unitCategory.size();
    if (m_unitName.size() < maxUnits)
        maxUnits = m_unitName.size(); 
    if (m_unitConversionFactor.size() < maxUnits)
        maxUnits = m_unitConversionFactor.size(); 
    if (::isX3dXml(writeFlags))
        for (int i = 0; i < maxUnits; i++) {
            RET_RESET_FLAGS_ONERROR( indentf(f, TheApp->GetIndent()) )
            RET_RESET_FLAGS_ONERROR( mywritestr(f , "<unit category='") )
            RET_RESET_FLAGS_ONERROR( mywritestr(f , m_unitCategory[i]) )
            RET_RESET_FLAGS_ONERROR( mywritestr(f , "' name='") )
            RET_RESET_FLAGS_ONERROR( mywritestr(f , m_unitName[i]) )
            RET_RESET_FLAGS_ONERROR( mywritestr(f , "' conversionFactor='") )
            if (strcmp(m_unitCategory[i], "length") == 0)
                RET_RESET_FLAGS_ONERROR( mywritef(f , "%f", m_unitLength) )
            else if (strcmp(m_unitCategory[i], "angle") == 0)
                RET_RESET_FLAGS_ONERROR( mywritef(f , "%f", m_unitAngle) )
            else    
                RET_RESET_FLAGS_ONERROR( mywritef(f , "%f", 
                                                  m_unitConversionFactor[i]) )
            RET_RESET_FLAGS_ONERROR( mywritestr(f , "' />\n") )
            TheApp->incSelectionLinenumber();
        }
    else
        for (int i = 0; i < maxUnits; i++) {
            RET_RESET_FLAGS_ONERROR( mywritestr(f , "UNIT ") )
            RET_RESET_FLAGS_ONERROR( mywritestr(f , m_unitCategory[i]) )
            RET_RESET_FLAGS_ONERROR( mywritestr(f , " ") )
            RET_RESET_FLAGS_ONERROR( mywritestr(f , m_unitName[i]) )
            RET_RESET_FLAGS_ONERROR( mywritestr(f , " ") )
            if (strcmp(m_unitCategory[i], "length") == 0)
                RET_RESET_FLAGS_ONERROR( mywritef(f , "%f", m_unitLength) )
            else if (strcmp(m_unitCategory[i], "angle") == 0)
                RET_RESET_FLAGS_ONERROR( mywritef(f , "%f", m_unitAngle) )
            else    
                RET_RESET_FLAGS_ONERROR( mywritef(f , "%f", 
                                                  m_unitConversionFactor[i]) )
            RET_RESET_FLAGS_ONERROR( mywritestr(f , "\n") )
            TheApp->incSelectionLinenumber();
        }    
    if (maxUnits > 0) {
        RET_RESET_FLAGS_ONERROR( mywritestr(f , "\n") )
        TheApp->incSelectionLinenumber(2);
    }
    return 0;
}

bool
Scene::belongsToNodeWithExternProto(const char *protoName) 
{
    bool found = false;
    for (int i = 0; i < m_nodesWithExternProto.size(); i++)
        if (strcmp(protoName, m_nodesWithExternProto[i]) == 0) {
            found = true;
            break;
        }
    return found;
} 

#define KANIM_RET_ONERROR(x) RET_AND_RESET_ONERROR(x, delete [] name;m_writeKanimNow = false)

int 
Scene::writeKanim(int f, const char *url)
{
    m_writeKanimNow = true;
    char* name = new char[strlen(url) + 1 + strlen(".wrl") + 1 + LEN_DEZIMAL_INT_MAX + 1];
    strcpy(name, url);
    char *nameBaseEnd = strrchr(name, '.');
    if (nameBaseEnd == NULL)
        nameBaseEnd = name + strlen(url);
    FloatArray keyTimes;
    bool allInterpolatorsLoop = true;
    for (int i = 0; i < m_timeSensors.size(); i++) {
        NodeTimeSensor *timer = (NodeTimeSensor *)m_timeSensors[i];
        if (!timer->loop()->getValue())
            allInterpolatorsLoop = false;
        int frac = timer->fraction_changed_Field();
        float interval = timer->cycleInterval()->getValue();
        for (SocketList::Iterator *j = timer->getOutput(frac).first(); 
               j != NULL; j = j->next()) {
            Interpolator *interpolator = (Interpolator *)(j->item().getNode());
            MFFloat *keys = interpolator->key();
            for (int k = 0; k < keys->getSize(); k++) {
                float time = keys->getValue(k) * interval;
                if (keyTimes.size() == 0)
                    keyTimes.append(time);
                else if (time > keyTimes[keyTimes.size() - 1])
                    keyTimes.append(time);                    
                else for (int l = 0 ; l < keyTimes.size(); l++) {
                    if (time == keyTimes[l])
                        break;
                    if (time < keyTimes[l]) {
                        keyTimes.insert(time, l);
                        break;
                    }
                }
            }        
        }           
    }
    KANIM_RET_ONERROR( mywritestr(f ,"<?xml version=\"1.0\"?>\n") ) 
    KANIM_RET_ONERROR( mywritestr(f ,"<animation ") )
    if (allInterpolatorsLoop)
        KANIM_RET_ONERROR( mywritestr(f ,"loop=\"true\"") )
    else
        KANIM_RET_ONERROR( mywritestr(f ,"loop=\"false\"") )
    KANIM_RET_ONERROR( mywritestr(f ,">\n") )     
    double t = swGetCurrentTime();
    m_timeStart = t;
    for (int i = 0; i < m_timeSensors.size(); i++)
        ((NodeTimeSensor *) m_timeSensors[i])->start(t);
    for (int i = 0; i < keyTimes.size(); i++) {
        updateTimeAt(t + keyTimes[i]);
        sprintf(nameBaseEnd, "_%d.wrl", i);
        int filedes = open(name, O_WRONLY | O_CREAT,00666);
        if (filedes == -1) {
            delete [] name;
            m_writeKanimNow = false;
            return -1;
        }
        KANIM_RET_ONERROR( write(filedes, name, PURE_VRML97) )
        if (swTruncateClose(filedes)) {
            delete [] name;
            m_writeKanimNow = false;
            return -1;
        }
        KANIM_RET_ONERROR( mywritestr(f ,"    <frame file_name=\"") )
        URL fileUrl(name);
        MyString filename = fileUrl.GetFileName();
        if (filename.length() == 0)
            filename = name;
        KANIM_RET_ONERROR( mywritestr(f, (const char *)filename) )
        KANIM_RET_ONERROR( mywritestr(f ,"\" time=\"") )
        KANIM_RET_ONERROR( mywritef(f ,"%f", keyTimes[i]) )
        KANIM_RET_ONERROR( mywritestr(f ,"\"/>\n") )
    }
    KANIM_RET_ONERROR( mywritestr(f ,"</animation>\n") )
    delete [] name;
    if (swTruncateClose(f)) {
        m_writeKanimNow = false;
        return -1;
    }
    updateTime();
    m_writeKanimNow = false;    
    return 0;
}

static bool hasEmptyMaterialCallback(Node *node, void *data)
{
    bool *emptyMaterial = (bool *) data;
    if (node->getType() == VRML_SHAPE)
        if (((NodeShape *)node)->appearance()->getValue() == NULL)
            *emptyMaterial = true;
    if (node->getType() == VRML_APPEARANCE)
        if (((NodeAppearance *)node)->material()->getValue() == NULL)
            *emptyMaterial = true;
    return true;
}

bool Scene::hasEmptyMaterial()
{
    bool emptyMaterial = false;
    getRoot()->doWithBranch(hasEmptyMaterialCallback, &emptyMaterial);
    return emptyMaterial;
}

void 
Scene::collectAc3dMaterialInfo(char *name, Node *node)
{
    m_ac3dMaterialNameArray.append(name);
    m_ac3dMaterialNodeArray.append(node);
    int materialIndex = m_ac3dMaterialNodeArray.size() - 1;

    StringMap::Chain::Iterator *j;
    bool alreadyInMap = false;
    for (int i = 0; i < m_ac3dMaterialIndexMap.width(); i++)
        for (j = m_ac3dMaterialIndexMap.chain(i).first(); j != NULL; j = j->next())
            if (strcmp(name, j->item()->getKey()) == 0)
                alreadyInMap = true;
    if (alreadyInMap)
        TheApp->PrintMessageWindowsString(IDS_AC3D_EXPORT_COLOR_IGNORED, name);
    else
        m_ac3dMaterialIndexMap[name] = materialIndex;
}

static void handleMaterial(Scene *scene, char *name, Node *node)
{
    if (name != NULL) {
        scene->collectAc3dMaterialInfo(name, node);
        //free(name);? later !
    }
}

int 
Scene::writeAc3d(int f, bool raven) 
{
    RET_ONERROR( mywritestr(f, "AC3Db\n") ) 

    m_ac3dMaterialNameArray.resize(0);
    m_ac3dMaterialNodeArray.resize(0);
    m_ac3dMaterialIndexMap.removeAll();

    NodeList *childList = ((NodeGroup *)getRoot())->children()->getValues();

    for (int i = 0; i < childList->size(); i++)
        childList->get(i)->handleAc3dMaterial(handleMaterial ,this);

    if (raven) {
        int materialIndex = 0;
        StringMap::Chain::Iterator *j;
        for (int i = 0; i < m_ac3dMaterialIndexMap.width(); i++) {
            for (j = m_ac3dMaterialIndexMap.chain(i).first(); j != NULL; 
                 j = j->next()) {
                const char *materialName = j->item()->getKey();
                Node *materialNode = NULL;
                for (int k = 0; k < m_ac3dMaterialNameArray.size(); k++) {
                     if (strcmp(materialName, m_ac3dMaterialNameArray[k]) == 0) {
                         materialNode = m_ac3dMaterialNodeArray[k];
                         materialNode->setAc3dMaterialIndex(materialIndex);
                     } 
                }
                if (materialNode != NULL) {
                    materialNode->writeAc3dMaterial(f, 0, materialName);
                    materialIndex += materialNode->getIncAc3dMaterialIndex();
                }
            }
        }
        m_ac3dEmptyMaterial = materialIndex;
    } else {
        int materialIndex = 0;
        for (int i = 0; i < m_ac3dMaterialNameArray.size(); i++) {
            const char *materialName = m_ac3dMaterialNameArray[i];
            Node *materialNode = m_ac3dMaterialNodeArray[i]; 
            materialNode->setAc3dMaterialIndex(materialIndex); 
            materialNode->writeAc3dMaterial(f, 0, materialName);
            materialIndex += materialNode->getIncAc3dMaterialIndex();
        }
        m_ac3dEmptyMaterial = materialIndex;
    }
    
    // write ac3d equivalent of empty material
    RET_ONERROR( mywritestr(f, "MATERIAL \"") )
    if (raven) 
        RET_ONERROR( mywritestr(f, "__") ) 
    RET_ONERROR( mywritestr(f, TheApp->GetDefaultAc3dMaterialName()) )
    RET_ONERROR( mywritestr(f, "\" ") )
    RET_ONERROR( mywritestr(f, "rgb 1 1 1  amb 0.2 0.2 0.2  emis 0 0 0  ") ) 
    RET_ONERROR( mywritestr(f, "spec 0 0 0  shi 2  trans 0\n") )

    int kids = 0;
    for (int i = 0; i < childList->size(); i++)
        if (childList->get(i)->canWriteAc3d())
            kids++;
    RET_ONERROR( mywritef(f, "OBJECT world\nkids %d\n", kids) )
    for (int i = 0; i < childList->size(); i++)
        RET_ONERROR( childList->get(i)->writeAc3d(f, 0) )
    return(0);
}

static bool searchLongestTime(Node *node, void *data)
{
    double *time = (double *)data;
    if (node->getType() == VRML_TIME_SENSOR) {
        NodeTimeSensor *sensor = (NodeTimeSensor *)node;
        node->getScene()->addTimeSensor(sensor);
        if (sensor->cycleInterval()->getValue() > *time)
            *time = sensor->cycleInterval()->getValue();
    }
    if (node->getType() == DUNE_VRML_CUT) {
        NodeVrmlCut *cut = (NodeVrmlCut *)node;
        float sceneLength = 0;
        MFTime *lengths = cut->sceneLengths();
        for (int i = 0; i < lengths->getSize(); i++)
            sceneLength += lengths->getValue(i);
        if (sceneLength > *time)
            *time = sceneLength;
    }
    return true;
}     

#define FRAME_RATE 24

int 
Scene::writeRib(int filedes, const char *file) 
{
    const char *url = file;
    int f = filedes;

    m_ribTexureFiles.removeAll();

    double longestTime = 0;
    getRoot()->doWithBranch(searchLongestTime, &longestTime);

    m_viewpoints.resize(0);

    findBindableNodes();

    int frames = longestTime * FRAME_RATE;
    if (longestTime == 0)
        frames = 1;

    bool running = isRunning();
    start();
    m_currentTime = 0;
   
    int framesPerFile = frames / TheApp->getNumExportFiles();
    if (framesPerFile == 0) {
        fprintf(stderr, "Warning: only %d frames, not all files are written\n", 
               frames); 
        framesPerFile = 1;
    }
        
    char filename[4096];
    snprintf(filename, 4095, "%s", url); 
    int numFramesPerFile = 0;
    int currentfile = 0;
    for (int j = 0; j < frames; j++) {
        if (((numFramesPerFile >= framesPerFile) && (framesPerFile > 0)) || 
            (((currentfile == 0) && (frames > 1)) && 
             (TheApp->getNumExportFiles() > 1)))  {
            URL file(url);
            const char *ext = strstr(url, ".");
            if (ext != NULL)
                ext++;
            snprintf(filename, 4095, "%s%d.%s", 
                     file.GetFileNameWithoutExtension(), ++currentfile, 
                     ext != NULL ? ext : "");
            if ((TheApp->getNumExportFiles() > 1) || 
                ((TheApp->getNumExportFiles() == 1) && (f != 1))) {
                swTruncateClose(f);
                f = open(filename, O_WRONLY | O_CREAT,00666);
            }

            RET_ONERROR( mywritestr(f, "##RenderMan RIB-Structure 1.0\n") ) 
            RET_ONERROR( mywritestr(f, "version 3.03\n\n") ) 
            numFramesPerFile = 0;
        }
        if (frames == 1) {
            f = open(file, O_WRONLY | O_CREAT,00666);
            
            RET_ONERROR( mywritestr(f, "##RenderMan RIB-Structure 1.0\n") ) 
            RET_ONERROR( mywritestr(f, "version 3.03\n\n") ) 
        }
        RET_ONERROR( writeRibNextFrame(f, file, j) )
        numFramesPerFile++;
    }
        
    swTruncateClose(f);

    if (!running)
        stop();
    updateTime();
  
    return(0);
}

int                 
Scene::writeRibNextFrame(int f, const char *url, int frame)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    updateTimeAt(m_currentTime);
    
    RET_ONERROR( mywritef(f, "FrameBegin %d\n", frame) )
    URL file(url);
    RET_ONERROR( mywritef(f, "Display \"%s%06d.tif\" \"file\" \"rgba\"\n",
                          file.GetFileNameWithoutExtension(), frame) )

    float fov = 30;
    if (m_currentViewpoint)
        fov = m_currentViewpoint->fov()->getValue() * 360.0 / (2 * M_PI);

    RET_ONERROR( mywritestr(f, "ShadingRate 1\n") ) 
    RET_ONERROR( mywritef(f, "Projection \"perspective\" \"fov\" [%f]\n", 
                             fov) )
    RET_ONERROR( mywritestr(f, "Identity\n") ) 

    if (m_currentNavigationInfo && 
        m_currentNavigationInfo->headlight()->getValue())
        RET_ONERROR( mywritestr(f, "LightSource \"ambientlight\" 1\n") )

    RET_ONERROR( mywritef(f, "Rotate 0 0 1 0\n") ) 
    RET_ONERROR( mywritef(f ,"Translate 0 0 0\n") )


    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "WorldBegin\n\n") )
    RET_ONERROR( mywritestr(f, "Identity\n\n") )

    NodeList *childList = ((NodeGroup *)getRoot())->children()->getValues();

    for (int i = 0; i < childList->size(); i++)
        RET_ONERROR( childList->get(i)->writeRib(f, 0) )

    RET_ONERROR( mywritestr(f, "WorldEnd\n\n") )
 
    RET_ONERROR( mywritestr(f, "FrameEnd\n") )

    m_currentTime = (double)frame / FRAME_RATE;  

    return(0);
}

static bool collectCattExportRecNodes(Node *node, void *data)
{
    if (node->getType() == DUNE_CATT_EXPORT_REC) {
        NodeArray *array = (NodeArray *)data;
        array->append(node);
    }
    return true;
}     

int 
Scene::writeCattGeo(void) 
{
    m_cattGeoFileCounter = 0;
    m_cattGeoCornerCounter = 1;
    m_cattGeoPlaneCounter = 1;
    m_cattRecIsWritten = false;
    m_cattSrcIsWritten = false;

    MyArray<NodeCattExportRec *> cattExportRecNodes;
    getRoot()->doWithBranch(collectCattExportRecNodes, &cattExportRecNodes);

    for (int i = 0; i < cattExportRecNodes.size(); i++)
        for (int j = 0; j < i; j++)
           if (cattExportRecNodes[i]->id()->getValue() == 
               cattExportRecNodes[j]->id()->getValue()) {
               char message[1024];
               mysnprintf(message, 1023, "Warning: id %d is used %s\n",
                          cattExportRecNodes[i]->id()->getValue(), 
                          "in more than one CattExportRec nodes\n");
               if (TheApp->mainWnd() == NULL)
                   swDebugf(message);
               else {
                   TheApp->MessageBox(message);
                   setSelection(cattExportRecNodes[j]);
                   UpdateViews(NULL, UPDATE_SELECTION);
               }
               break;
           }
    if (m_root->writeCattGeo(0,0))
        return -1;

    return(0);
}

bool 
Scene::validateLdrawExport()
{
    NodeList *list = searchNodeType(DUNE_LDRAW_DAT_EXPORT);
    if (list->size() == 0) {
        TheApp->MessageBoxId(IDS_LDRAW_DAT_EXPORT_NODE_CREATED, SW_MB_WARNING);
        Node *node = createNode("LdrawDatExport");
        execute(new MoveCommand(node, NULL, -1, m_root, m_rootField));
        UpdateViews(NULL, UPDATE_SELECTION);
        return false;
    }
    if (list->size() > 1) {
        TheApp->MessageBoxId(IDS_TO_MUCH_LDRAW_DAT_EXPORT_NODES);
        return false;
    }
    NodeLdrawDatExport *node = (NodeLdrawDatExport *)list->get(0);
    return node->validate();
}

int                 
Scene::writeLdrawDat(int filedes, const char *path)
{
    m_currentLdrawColor = -1;
    NodeList *list = searchNodeType(DUNE_LDRAW_DAT_EXPORT);
    const char *filename = strrchr(path, swGetPathSeperator());
    if (filename == NULL)
        filename = path;
    else
        filename++;
    if (list->size() == 1) {
        NodeLdrawDatExport *node = (NodeLdrawDatExport *)list->get(0);
        if (node->writeLdrawDatHeader(filedes, filename) != 0)
            return 0;
    };
    m_root->writeLdrawDat(filedes, 0);
    return 0;
}

static bool writeCDynamicNodeDeclaration(Node *node, void *data)
{
    WriteCDynamicNodeData *parameters = (WriteCDynamicNodeData *)data;
    Proto *proto = node->getProto();
    if (proto->isDynamicProto())
        parameters->result = proto->writeCDeclaration(parameters->filedes, 
                                 parameters->languageFlag);
    if (parameters->result != 0)
        return false;

    return true;
}

int 
Scene::writeCDeclaration(int f, int languageFlag) 
{
    ProtoMap::Chain::Iterator *j;
    for (int i = 0; i < m_protos.width(); i++)
        for (j = m_protos.chain(i).first(); j != NULL; j = j->next()) {
            Proto *proto = j->item()->getData();
            if (proto == NULL)
                continue;
            proto->setIsInScene(false);
            proto->setWriteCDeclarationWritten(false);
            proto->setInUse(false);
            if (proto->getType() == VRML_POSITION_INTERPOLATOR)
                proto->setInUse(true);
            if (proto->getType() == VRML_ORIENTATION_INTERPOLATOR)
                proto->setInUse(true);
            if (proto->isDeclaredInRwd_h())
                proto->setInUse(true);
    }
    m_root->doWithBranch(markUsedProto, this);
    for (int i = 0; i < m_protos.width(); i++)
        for (j = m_protos.chain(i).first(); j != NULL; j = j->next()) {
            Proto *proto = j->item()->getData();
            if (proto == NULL)
                continue;
            if (proto->getType() == VRML_COMMENT)
                continue;
            if (proto->isMesh() && !proto->isExportTargetMesh())
                continue;
            if (proto->isMismatchingProto())
                continue;   
            if ((languageFlag & WONDERLAND) && 
                !TheApp->GetWonderlandModuleExportAllX3dClasses())
                if ((!proto->isWonderlandExported()) && (!proto->isInUse()))
                    continue;   
            if (proto->isInUse())
                if (proto->writeCDeclaration(f, languageFlag) != 0)
                    return -1;
        }
    WriteCDynamicNodeData parameters;
    parameters.result = 0;
    parameters.filedes = f;
    parameters.languageFlag = languageFlag;
    m_root->doWithBranch(writeCDynamicNodeDeclaration, &parameters);
    return parameters.result;
}

int
Scene::writeCDataFunctionsCalls(int f, int languageFlag)
{
    for (int i = 0; i < 1; i++) {
        if (languageFlag & MANY_JAVA_CLASSES) {
            RET_ONERROR( mywritef(f, "    }\n") )
            RET_ONERROR( mywritef(f, "}\n") )
            RET_ONERROR( mywritef(f, "class Data%sFunctionClass_%d {\n",
                                  TheApp->getCPrefix(), getNumDataClasses()) )
        }
        RET_ONERROR( mywritef(f, 
            "    public static void data%sFunction%d() {\n",
            TheApp->getCPrefix(), getNumDataClasses() + i) )
        increaseNumDataClasses();
    }

    return(0);
}

struct WriteCStruct {
    int filedes;
    int ret;
    int languageFlag;
    bool outsideJavaClass;
};

bool writeCVariableNameLine(Node *node, const char *variableName,
                            struct WriteCStruct *cWrite, bool extraJavaClass);

bool writeCSetWrittenFalse(Node *node, void *data)
{
    if (node == NULL)
        return true;

    node->setWritten(false);

    return true;
}     

class WriteData {
public:
    int languageFlag;
    int filedes;
};

bool writeCCoordinateFirst(Node *node, void *data)
{
    if (node == NULL)
        return true;

    WriteData *wdata = (WriteData *)data;
    if (node->getType() == VRML_COORDINATE) {
        node->writeCDataFunction(wdata->filedes, wdata->languageFlag, 
                                 true, true);
    }
    return true;
}

bool writeCIndexedFaceSetFirst(Node *node, void *data)
{
    if (node == NULL)
        return true;

    WriteData *wdata = (WriteData *)data;
    if (node->getType() == VRML_INDEXED_FACE_SET) {
        node->writeCDataFunction(wdata->filedes, wdata->languageFlag, 
                                 true, true);
    }   
    return true;
}

bool writeCShapeFirst(Node *node, void *data)
{
    if (node == NULL)
        return true;

    WriteData *wdata = (WriteData *)data;
    if (node->getType() == VRML_SHAPE) {
        node->writeCDataFunction(wdata->filedes, wdata->languageFlag, 
                                 true, true);
    }   
    return true;
}

bool writeCHAnimJointFirst(Node *node, void *data)
{
    if (node == NULL)
        return true;

    WriteData *wdata = (WriteData *)data;
    if (node->getType() == X3D_HANIM_JOINT) {
        node->writeCDataFunction(wdata->filedes, wdata->languageFlag, 
                                 true, true);
    }   
    return true;
}

bool writeCNodeData(Node *node, void *data)
{
    if (node == NULL)
        return true;

    if (node->getNumConvertedNodes() > 0) {
        for (int i = 0; i < node->getNumConvertedNodes(); i++)
            node->getConvertedNode(i)->doWithBranch(writeCNodeData, data,
                                                    false);
        return true;
    }

    Proto *proto = node->getProto();
    if (proto->getType() == VRML_COMMENT)
        return true;
    if (proto->isMesh() && !proto->isExportTargetMesh())
        return true;
    if (proto->isMismatchingProto())
        return true;   

    // do not write multiple declarations for USE'd nodes
    if (node->getFlag(NODE_FLAG_TOUCHED))
        return true;
    node->setFlag(NODE_FLAG_TOUCHED); 

    struct WriteCStruct *cWrite = (struct WriteCStruct *)data;
    if (node->hasProtoNodes()) {
       NodePROTO *protoNode = (NodePROTO *)node;
       for (int i = 0; i < protoNode->getNumProtoNodes(); i++)
           protoNode->getProtoNode(i)->doWithBranch(writeCNodeData, data);
        if (cWrite->ret == -1)
            return false;
    }

    bool extraJavaClass = false;
    if (node->needExtraJavaClass())
        extraJavaClass = true;

    if (cWrite->languageFlag & MANY_JAVA_CLASSES) {
        if (cWrite->outsideJavaClass) {
            if (!extraJavaClass)
                return true;
        } else {
            if (extraJavaClass)
                return true;
        } 
    }
    if (node->getType() != DUNE_CURVE_ANIMATION) {
        writeCVariableNameLine(node, node->getVariableName(), cWrite,
                               extraJavaClass);
        if (cWrite->ret != 0)
            return false;
    } else {
        NodeCurveAnimation *curve = (NodeCurveAnimation *)node;
        Node *inter = curve->getPositionInterpolator();
        const char *variableName = inter->getVariableName();
        writeCVariableNameLine(inter, variableName, cWrite, extraJavaClass);
        if (cWrite->ret != 0)
            return false;
        inter = curve->getOrientationInterpolator();
        variableName = inter->getVariableName();
        writeCVariableNameLine(inter, variableName, cWrite, extraJavaClass);
        if (cWrite->ret != 0)
            return false;
    }
    int f = cWrite->filedes;
    if (cWrite->languageFlag & MANY_JAVA_CLASSES) 
        if (extraJavaClass)
            RET_ONERROR( mywritestr(f, "}\n") )  

    return true;
}

bool writeCVariableNameLine(Node *node, const char *variableName,
                           struct WriteCStruct *cWrite, bool extraJavaClass)
{
    int f = cWrite->filedes;
    int error = 0;
    if (!(extraJavaClass && (cWrite->languageFlag & MANY_JAVA_CLASSES)))
        if (mywritestr(f, "    ") != 0) {
            cWrite->ret = -1;
            return false;
        }
    if (cWrite->languageFlag & C_SOURCE)
        if (mywritestr(f, "struct ") != 0) {
            cWrite->ret = -1;
            return false;
        }
        
    if (cWrite->languageFlag & MANY_JAVA_CLASSES) {
        if (extraJavaClass)
            RET_ONERROR( mywritef(f, "class %s%s {\n    ",
                                  TheApp->getCPrefix(), 
                                  node->getVariableName()) )
        RET_ONERROR( mywritestr(f, "public static ") ) 
    }

    if (mywritestr(f, node->getClassName()) != 0)
        error = -1;
    else if (mywritestr(f, " ") != 0)
        error = -1;
    else if (mywritestr(f, variableName) != 0)
        error = -1;
    else if (mywritestr(f, ";\n") != 0)
        error = -1;
    if (error != 0) {
        cWrite->ret = -1;
    }
    return true;
}

static bool writeCDynamicNodeCallback(Node *node, void *data)
{
    WriteCDynamicNodeData *parameters = (WriteCDynamicNodeData *)data;
    Proto *proto = node->getProto();
    if (proto->isDynamicProto())
        parameters->result = node->writeCDynamicNodeCallback(
                                 parameters->filedes, parameters->languageFlag);
    if (parameters->result != 0)
        return false;
    return true;
}

static bool writeCParentCallback(Node *node, void *data)
{
    WriteCDynamicNodeData *parameters = (WriteCDynamicNodeData *)data;
    parameters->result = node->writeCGetParent(parameters->filedes, parameters->languageFlag);
    if (parameters->result != 0)
        return false;
    return true;
}

static bool writeCInstallDynamicNodeCallback(Node *node, void *data)
{
    WriteCDynamicNodeData *parameters = (WriteCDynamicNodeData *)data;
    Proto *proto = node->getProto();
    if (proto->isDynamicProto() && !node->isPROTO())
        parameters->result = node->writeCInstallDynamicNodeCallback(
                                   parameters->filedes, 
                                   parameters->languageFlag, node->getProto());
    if (parameters->result != 0)
        return false;
    return true;
}

static bool writeCRemoveAlreadyWrittenEventOuts(Node *node, void *data)
{
    node->removeAlreadyWrittenEventOuts();
    return true;
}

int 
Scene::writeC(int f, int languageFlag) 
{
    dynamicNodeCallbackRemove();
    m_root->doWithBranch(writeCRemoveAlreadyWrittenEventOuts, NULL);
    m_scriptTypeWritten = false;
    if (languageFlag & C_SOURCE)
        RET_ONERROR( mywritestr(f, "/*") )
    else
        RET_ONERROR( mywritestr(f, "//") )
    RET_ONERROR( mywritestr(f, " VRML97/X3D file \"") )
    URL *url = new URL(getURL());
    RET_ONERROR( mywritestr(f, url->GetFileName()) )
    delete url;
    RET_ONERROR( mywritestr(f, "\" converted to ") )
    if (languageFlag & C_SOURCE) 
        RET_ONERROR( mywritestr(f, "C") )
    else if (languageFlag & CC_SOURCE)
        RET_ONERROR( mywritestr(f, "C++") )
    else if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritestr(f, "java") )
    RET_ONERROR( mywritestr(f, " with white_dune") )
    if (languageFlag & C_SOURCE)
        RET_ONERROR( mywritestr(f, "*/") )
    RET_ONERROR( mywritestr(f, "\n\n") ) 
    if (languageFlag & C_SOURCE) {
        RET_ONERROR( mywritestr(f, "#ifndef NULL\n") )
        RET_ONERROR( mywritestr(f, "# define NULL (void *)0\n") )
        RET_ONERROR( mywritestr(f, "#endif\n") )
        RET_ONERROR( mywritestr(f, "\n") )
        RET_ONERROR( mywritestr(f, "struct ") )
        RET_ONERROR( mywritestr(f, TheApp->getCSceneGraphName()) ) 
        RET_ONERROR( mywritestr(f, ";\n") )
        RET_ONERROR( mywritestr(f, "\n") )
        RET_ONERROR( mywritestr(f, "typedef void ") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) )
        RET_ONERROR( mywritestr(f, ";\n") )
        RET_ONERROR( mywritestr(f, "\n") )
        RET_ONERROR( mywritestr(f, "struct ") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) )
        RET_ONERROR( mywritestr(f, "Struct;\n") )
        RET_ONERROR( mywritestr(f, "\n") )
        RET_ONERROR( mywritestr(f, "typedef void (*") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "Callback)") )
        RET_ONERROR( mywritestr(f, "(") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) )
        RET_ONERROR( mywritestr(f, " *node, void *data);\n") )
        RET_ONERROR( mywritestr(f, "\n") )
        RET_ONERROR( mywritestr(f, "void ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "TreeRenderCallback(struct ") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) )
        RET_ONERROR( mywritestr(f, "Struct *node, void *data);\n") )
        RET_ONERROR( mywritestr(f, "void ") ) 
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "TreeDoWithDataCallback(struct ") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) )
        RET_ONERROR( mywritestr(f, "Struct *node, void *data);\n") )
        RET_ONERROR( mywritestr(f, "typedef int (*") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "ProcessEventCallback)") )
        RET_ONERROR( mywritestr(f, "(") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) )
        RET_ONERROR( mywritestr(f, " *node, const char *event, ") )
        RET_ONERROR( mywritestr(f, "void *data);\n") )
        RET_ONERROR( mywritestr(f, "\n") )
        RET_ONERROR( mywritestr(f, "\n\n") )
     }
     if (languageFlag & CC_SOURCE) {
        RET_ONERROR( mywritestr(f, "#include <stddef.h> // for NULL\n") )
        RET_ONERROR( mywritestr(f, "\n") )
        RET_ONERROR( mywritestr(f, "class ") )
        RET_ONERROR( mywritestr(f, TheApp->getCSceneGraphName()) ) 
        RET_ONERROR( mywritestr(f, ";\n") )
        RET_ONERROR( mywritestr(f, "\n") )
        RET_ONERROR( mywritestr(f, "class ") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
        RET_ONERROR( mywritestr(f, " {\n") )
        RET_ONERROR( mywritestr(f, "public:\n") )
        RET_ONERROR( mywritestr(f, "    ") )    
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) )
        RET_ONERROR( mywritestr(f, "() {\n") )
        RET_ONERROR( mywritestr(f, "        m_parent = NULL;\n") )
        RET_ONERROR( mywritestr(f, "        m_protoRoot = NULL;\n") )
        RET_ONERROR( mywritestr(f, "        m_data = NULL;\n") )
        RET_ONERROR( mywritestr(f, "    }\n") )
        RET_ONERROR( mywritestr(f, "    virtual void treeRender(") )
        RET_ONERROR( mywritestr(f, "void *dataptr) {\n") )
        RET_ONERROR( mywritestr(f, "        if (m_protoRoot != NULL)\n") )
        RET_ONERROR( mywritestr(f, "            m_protoRoot->") )
        RET_ONERROR( mywritestr(f, "treeRender(dataptr);\n") )
        RET_ONERROR( mywritestr(f, "    }\n") )
        RET_ONERROR( mywritestr(f, "    virtual void treeDoWithData(") )
        RET_ONERROR( mywritestr(f, "void *dataptr) {\n") )
        RET_ONERROR( mywritestr(f, "        if (m_protoRoot != NULL)\n") )
        RET_ONERROR( mywritestr(f, "            m_protoRoot->") )
        RET_ONERROR( mywritestr(f, "treeDoWithData(dataptr);\n") )
        RET_ONERROR( mywritestr(f, "    }\n") )
        RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
        RET_ONERROR( mywritestr(f, " *m_parent;\n") )
        RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
        RET_ONERROR( mywritestr(f, " *m_protoRoot;\n") )
        RET_ONERROR( mywritestr(f, "    void *m_data;\n") )
        RET_ONERROR( mywritestr(f, "    virtual int getType() const = 0;\n") )
        RET_ONERROR( mywritestr(f, "};\n") )
        RET_ONERROR( mywritestr(f, "\n") )
        RET_ONERROR( mywritestr(f, "typedef void (*") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "Callback)") )
        RET_ONERROR( mywritestr(f, "(") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
        RET_ONERROR( mywritestr(f, " *node, void *dataptr);\n") )
        RET_ONERROR( mywritestr(f, "typedef bool (*") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "ProcessEventCallback)") )
        RET_ONERROR( mywritestr(f, "(") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
        RET_ONERROR( mywritestr(f, " *node, const char *event, ") )
        RET_ONERROR( mywritestr(f, "void *dataptr);\n") )
        RET_ONERROR( mywritestr(f, "\n\n") ) 
     }
     if (languageFlag & JAVA_SOURCE) {
        RET_ONERROR( mywritestr(f, "abstract class ") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
        RET_ONERROR( mywritestr(f, " {\n") )
        RET_ONERROR( mywritestr(f, "    void treeRender(") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "Node dataptr, Object object) {\n") )
        RET_ONERROR( mywritestr(f, "        if (m_protoRoot != null)\n") )
        RET_ONERROR( mywritestr(f, "            m_protoRoot.") )
        RET_ONERROR( mywritestr(f, "treeRender(dataptr, object);\n") )
        RET_ONERROR( mywritestr(f, "    }\n") )
        RET_ONERROR( mywritestr(f, "    void treeDoWithData(") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "Node dataptr) {\n") )
        RET_ONERROR( mywritestr(f, "        if (m_protoRoot != null)\n") )
        RET_ONERROR( mywritestr(f, "            m_protoRoot.") )
        RET_ONERROR( mywritestr(f, "treeDoWithData(dataptr);\n") )
        RET_ONERROR( mywritestr(f, "    }\n") )
        RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
        RET_ONERROR( mywritestr(f, " m_parent;\n") )
        RET_ONERROR( mywritestr(f, "    Object m_data;\n") )
        RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
        RET_ONERROR( mywritestr(f, " m_protoRoot;\n") )
        RET_ONERROR( mywritestr(f, "    abstract int getType();\n") )
        RET_ONERROR( mywritestr(f, "};\n") )
        RET_ONERROR( mywritestr(f, "\n") )

        RET_ONERROR( mywritestr(f, "class ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "RenderCallback {\n") )
        RET_ONERROR( mywritestr(f, "   public void render(") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
        RET_ONERROR( mywritestr(f, " node, Object object) {}\n") )
        RET_ONERROR( mywritestr(f, "}\n") )

        RET_ONERROR( mywritestr(f, "class ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "TreeRenderCallback {\n") )
        RET_ONERROR( mywritestr(f, "   public void treeRender(") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
        RET_ONERROR( mywritestr(f, " node, Object object) {}\n") )
        RET_ONERROR( mywritestr(f, "}\n") )
        RET_ONERROR( mywritestr(f, "\n") )

        RET_ONERROR( mywritestr(f, "class ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "DoWithDataCallback {\n") )
        RET_ONERROR( mywritestr(f, "   public void doWithData(") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
        RET_ONERROR( mywritestr(f, " node) ") )
        RET_ONERROR( mywritestr(f, "{}\n") )
        RET_ONERROR( mywritestr(f, "   public void update(") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
        RET_ONERROR( mywritestr(f, " node) ") )
        RET_ONERROR( mywritestr(f, "{}\n") )
        RET_ONERROR( mywritestr(f, "   public Object getNullOrObject(") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
        RET_ONERROR( mywritestr(f, " node) ") )
        RET_ONERROR( mywritestr(f, "{ return null; }\n") )
        RET_ONERROR( mywritestr(f, "}\n") )

        RET_ONERROR( mywritestr(f, "class ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "TreeDoWithDataCallback {\n") )
        RET_ONERROR( mywritestr(f, "   public void treeDoWithData(") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
        RET_ONERROR( mywritestr(f, " node) ") )
        RET_ONERROR( mywritestr(f, "{}\n") )
        RET_ONERROR( mywritestr(f, "}\n") )
        RET_ONERROR( mywritestr(f, "\n") )

        RET_ONERROR( mywritestr(f, "class ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "CreateNormalsCallback {\n") )
        RET_ONERROR( mywritestr(f, "   public void createNormals(") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
        RET_ONERROR( mywritestr(f, " node, Object data) {}\n") )
        RET_ONERROR( mywritestr(f, "}\n") )

        RET_ONERROR( mywritestr(f, "class ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "ProcessEventCallback {\n") )
        RET_ONERROR( mywritestr(f, "   public boolean processEvent(") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
        RET_ONERROR( mywritestr(f, " node, ") )
        RET_ONERROR( mywritestr(f, "String") )
        RET_ONERROR( mywritestr(f, " event) ") )
        RET_ONERROR( mywritestr(f, "{ return false;}\n") )
        RET_ONERROR( mywritestr(f, "}\n") )

        RET_ONERROR( mywritestr(f, "\n\n") )
    }
    RET_ONERROR( writeCDeclaration(f, languageFlag) )
    RET_ONERROR( mywritestr(f, "\n") ) 
    
    int numDataFunctions = 0;        
    MyArray<int> startFunctions;
    MyArray<int> numFunctionsPerClass;

    zeroNumDataFunctions();
    setStartNumDataFunctions(0); 

    WriteData data;
    data.filedes = f;
    data.languageFlag = languageFlag;

    if (languageFlag & MANY_JAVA_CLASSES) {
        struct WriteCStruct cWrite;
        cWrite.filedes = f;
        cWrite.ret = 0;
        cWrite.languageFlag = languageFlag;
        cWrite.outsideJavaClass = true;
        getNodes()->clearFlag(NODE_FLAG_TOUCHED); // to handle DEF/USE
        m_root->doWithBranch(writeCNodeData, &cWrite);
        if (cWrite.ret !=0)
            return -1;
        RET_ONERROR( mywritestr(f, "\n") ) 

        if (m_root->writeC(f, languageFlag | OUTSIDE_JAVA_CLASS))
            return -1;

        getNodes()->clearFlag(NODE_FLAG_TOUCHED); // to handle DEF/USE

        if (m_root->writeCDataAsFunctions(f, languageFlag))
            return -1;

        if (languageFlag & MANY_JAVA_CLASSES) {
            RET_ONERROR( mywritef(f, "class %sScenegraphFunctions%d {\n", 
                                  TheApp->getCPrefix(), getNumDataClasses()) )
            RET_ONERROR( mywritef(f, "    static void data%sFunction0() {",
                                  TheApp->getCPrefix()) )
        }

        m_root->doWithBranch(writeCSetWrittenFalse, NULL);

        m_root->doWithBranch(writeCCoordinateFirst, &data,
                             true, false, false, true, false, true);
        numFunctionsPerClass.append(getNumDataFunctions());
        setNumDataFunctionsPerClass(getNumDataFunctions());
        setStartNumDataFunctions(numDataFunctions);
        startFunctions.append(numDataFunctions);
        numDataFunctions += getNumDataFunctions(); 
        RET_ONERROR( writeCDataFunctionsCalls(f, languageFlag) )
        RET_ONERROR( mywritestr(f, "\n") )        

        m_root->doWithBranch(writeCIndexedFaceSetFirst, &data,
                             true, false, false, true, false, true);
        numFunctionsPerClass.append(getNumDataFunctions());
        setNumDataFunctionsPerClass(getNumDataFunctions());
        startFunctions.append(numDataFunctions);
        setStartNumDataFunctions(numDataFunctions);
        numDataFunctions += getNumDataFunctions(); 
        RET_ONERROR( writeCDataFunctionsCalls(f, languageFlag) )
        RET_ONERROR( mywritestr(f, "\n") )        
        m_root->doWithBranch(writeCShapeFirst, &data);
        numFunctionsPerClass.append(getNumDataFunctions());

        setStartNumDataFunctions(getNumDataFunctions());
        setNumDataFunctionsPerClass(getNumDataFunctions());
        startFunctions.append(numDataFunctions);
        setStartNumDataFunctions(numDataFunctions);
        numDataFunctions += getNumDataFunctions(); 
        RET_ONERROR( writeCDataFunctionsCalls(f, languageFlag) )
        RET_ONERROR( mywritestr(f, "\n") )        

        m_root->doWithBranch(writeCHAnimJointFirst, &data,
                             true, false, false, true, false, true);
        numFunctionsPerClass.append(getNumDataFunctions());
        setNumDataFunctionsPerClass(getNumDataFunctions());
        setStartNumDataFunctions(numDataFunctions);
        startFunctions.append(numDataFunctions);
        numDataFunctions += getNumDataFunctions(); 
        RET_ONERROR( writeCDataFunctionsCalls(f, languageFlag) )
        RET_ONERROR( mywritestr(f, "    }\n") )        
        RET_ONERROR( mywritestr(f, "}\n") )        
        RET_ONERROR( mywritestr(f, "\n") )        
    }

    WriteCDynamicNodeData parameters;
    parameters.result = 0;
    parameters.filedes = f;
    parameters.languageFlag = languageFlag;
    m_root->doWithBranch(writeCDynamicNodeCallback, &parameters);
    if (parameters.result != 0)
        return -1;

    if (languageFlag & C_SOURCE)
        RET_ONERROR( mywritestr(f, "struct ") )
    else
        RET_ONERROR( mywritestr(f, "class ") ) 
    RET_ONERROR( mywritestr(f, TheApp->getCSceneGraphName()) ) 
    RET_ONERROR( mywritestr(f, " {\n") ) 
    if (languageFlag & CC_SOURCE)
        RET_ONERROR( mywritestr(f, "public:\n") )
    struct WriteCStruct cWrite;
    cWrite.filedes = f;
    cWrite.ret = 0;
    cWrite.languageFlag = languageFlag;
    cWrite.outsideJavaClass = false;
    getNodes()->clearFlag(NODE_FLAG_TOUCHED); // to handle DEF/USE
    m_root->doWithBranch(writeCNodeData, &cWrite);
    if (cWrite.ret !=0)
        return -1;

    if (languageFlag & C_SOURCE) {
        RET_ONERROR( mywritestr(f, "};\n\n") )

        RET_ONERROR( mywritestr(f, "struct ") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
        RET_ONERROR( mywritestr(f, "Struct {\n") ) 
        RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
        RET_ONERROR( mywritestr(f, " *m_parent;\n") )
        RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
        RET_ONERROR( mywritestr(f, " *m_protoRoot;\n") )
        RET_ONERROR( mywritestr(f, "    int m_type;\n") )
        RET_ONERROR( mywritestr(f, "    void *m_data;\n") )
        RET_ONERROR( mywritestr(f, "};\n\n") )
     
        RET_ONERROR( mywritestr(f, "void ") )
        RET_ONERROR( mywritestr(f, TheApp->getCSceneGraphName()) ) 
        RET_ONERROR( mywritestr(f, "Init(") )
        RET_ONERROR( mywritestr(f, "struct ") )
        RET_ONERROR( mywritestr(f, TheApp->getCSceneGraphName()) ) 
        RET_ONERROR( mywritestr(f, " *self) {\n") )
        if (m_root->writeC(f, languageFlag))
            return -1;
        parameters.result = 0;
        parameters.filedes = f;
        parameters.languageFlag = languageFlag;
        m_root->doWithBranch(writeCInstallDynamicNodeCallback, &parameters);
        if (parameters.result != 0)
            return -1;
        RET_ONERROR( mywritestr(f, "    }\n\n") ) 
        RET_ONERROR( mywritestr(f, "}\n\n") ) 
        RET_ONERROR( writeCTreeCallback(f, "Render") )
        RET_ONERROR( mywritestr(f, "\n") ) 
        RET_ONERROR( writeCTreeCallback(f, "DoWithData") )
        RET_ONERROR( mywritestr(f, "\n") ) 

        RET_ONERROR( mywritef(f, "%s%s *", TheApp->getCPrefix(), "Node") )
        RET_ONERROR( mywritef(f, "%sGetNodeFromGlName(", TheApp->getCPrefix()) )
        RET_ONERROR( mywritef(f, "struct %sSceneGraph *self, int glName) {\n",
                              TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "    switch (glName) {\n") ) 
        for (int i = 0; i < m_glNameData.size(); i++) {
            RET_ONERROR( mywritef(f, "       case %d:\n", 
                                 m_glNameData[i].glName) )
            RET_ONERROR( mywritestr(f, "         return ") )         
            RET_ONERROR( mywritef(f, "&self->%s;\n", (const char *)
                                               m_glNameData[i].nodeName) )
        }
        RET_ONERROR( mywritestr(f, "    }\n") ) 
        RET_ONERROR( mywritestr(f, "    return NULL;\n") ) 
        RET_ONERROR( mywritestr(f, "}\n") ) 

        for (int i = 0; i < LAST_TYPE; i++) {
            FieldValue *value = typeDefaultValue(i);
            value->writeCSendEventFunction(f, languageFlag);
        }
    }
    getNodes()->clearFlag(NODE_FLAG_TOUCHED); // to handle DEF/USE
    if (languageFlag & CC_SOURCE) {        
        RET_ONERROR( mywritestr(f, "public:\n") ) 
        RET_ONERROR( mywritestr(f, "    ") ) 
        RET_ONERROR( mywritestr(f, TheApp->getCSceneGraphName()) ) 
        RET_ONERROR( mywritestr(f, "();\n") )

        RET_ONERROR( mywritestr(f, "    ") ) 
        RET_ONERROR( mywritestr(f, "void render(void *data) ") )
        RET_ONERROR( mywritestr(f, "{ root.treeRender(data); }\n") ) 

        RET_ONERROR( mywritestr(f, "    ") ) 
        RET_ONERROR( mywritestr(f, "void doWithData(void *data) ") )
        RET_ONERROR( mywritestr(f, "{ root.treeDoWithData(data); }\n") ) 

        RET_ONERROR( mywritestr(f, "    ") ) 
        RET_ONERROR( mywritef(f, "%sNode *", TheApp->getCPrefix()) ) 
        RET_ONERROR( mywritestr(f, "getNodeFromGlName(int glName);\n") ) 
        RET_ONERROR( mywritestr(f, "};\n\n") ) 

        RET_ONERROR( mywritestr(f, TheApp->getCSceneGraphName()) ) 
        RET_ONERROR( mywritestr(f, "::") )
        RET_ONERROR( mywritestr(f, TheApp->getCSceneGraphName()) ) 
        RET_ONERROR( mywritestr(f, "() {\n") )
        if (m_root->writeC(f, languageFlag))
            return -1;
        parameters.result = 0;
        parameters.filedes = f;
        parameters.languageFlag = languageFlag;
        m_root->doWithBranch(writeCInstallDynamicNodeCallback, &parameters);
        if (parameters.result != 0)
            return -1;

        RET_ONERROR( mywritestr(f, "}\n") ) 

        RET_ONERROR( mywritef(f, "%s%s *", TheApp->getCPrefix(), "Node") )
        RET_ONERROR( mywritef(f, "%s::",TheApp->getCSceneGraphName()) ) 
        RET_ONERROR( mywritef(f, "getNodeFromGlName(int glName) {\n") )
        RET_ONERROR( mywritestr(f, "    switch (glName) {\n") ) 
        for (int i = 0; i < m_glNameData.size(); i++) {
            RET_ONERROR( mywritef(f, "       case %d:\n", 
                                 m_glNameData[i].glName) )
            RET_ONERROR( mywritestr(f, "         return ") )         
            RET_ONERROR( mywritef(f, "&%s;\n", (const char *)
                                               m_glNameData[i].nodeName) )
        }
        RET_ONERROR( mywritestr(f, "    }\n") ) 
        RET_ONERROR( mywritestr(f, "    return NULL;\n") ) 
        RET_ONERROR( mywritestr(f, "}\n") ) 
    
        for (int i = 0; i < LAST_TYPE; i++) {
            FieldValue *value = typeDefaultValue(i);
            value->writeCSendEventFunction(f, languageFlag);
        }
    } else if (languageFlag & JAVA_SOURCE) {        
        if (!(languageFlag & MANY_JAVA_CLASSES)) {

            if (m_root->writeCDataAsFunctions(f, languageFlag, true))
                return -1;

            numFunctionsPerClass.append(getNumDataFunctions());
            setNumDataFunctionsPerClass(getNumDataFunctions());
            setStartNumDataFunctions(numDataFunctions);
            startFunctions.append(numDataFunctions);
            numDataFunctions += getNumDataFunctions(); 

            RET_ONERROR(writeCDataFunctionsCalls(f, languageFlag) )

            RET_ONERROR( mywritestr(f, "    }\n") )    
        }

        RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, "public static boolean initThings = true;\n"))
        RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, TheApp->getCSceneGraphName()) ) 
        RET_ONERROR( mywritestr(f, "() {\n") )

        if (m_root->writeC(f, languageFlag))
            return -1;
        for (int j = 0; j < getNumDataClasses(); j++) {
            for (int i = 0; i < 1; i++) {
                if (j == getNumDataClasses() - 1) {
                    RET_ONERROR( mywritestr(f, "        if (initThings)\n") )
                    RET_ONERROR( mywritestr(f, "    ") )
                    RET_ONERROR( mywritestr(f, "        ") )
                    if (languageFlag & MANY_JAVA_CLASSES)
                        RET_ONERROR( mywritef(f, "%sScenegraphFunctions%d.",
                                          TheApp->getCPrefix(), 0) )
                    RET_ONERROR( mywritef(f, "data%sFunction%d();\n",
                                          TheApp->getCPrefix(), 0) ) 
                }
                if (j == 2) {
                    RET_ONERROR( mywritestr(f, "        if (initThings)\n") )
                    RET_ONERROR( mywritestr(f, "    ") )
                }

                RET_ONERROR( mywritestr(f, "        ") )
                if (languageFlag & MANY_JAVA_CLASSES)
                    RET_ONERROR( mywritef(f, "Data%sFunctionClass_%d.",
                                          TheApp->getCPrefix(), j) )
                RET_ONERROR( mywritef(f, "data%sFunction%d();\n",
                                          TheApp->getCPrefix(), j) ) 

                if (j == getNumDataClasses() - 1) 
                    if (languageFlag & MANY_JAVA_CLASSES) {
                        RET_ONERROR(m_root->writeCDataFunction(f, languageFlag,
                                    true, true) )
                    }
            }
        }
        RET_ONERROR( mywritestr(f, "        setParents();\n") )

        RET_ONERROR( mywritestr(f , "    }\n") )

        parameters.result = 0;
        parameters.filedes = f;
        parameters.languageFlag = languageFlag;
        m_root->doWithBranch(writeCInstallDynamicNodeCallback, &parameters);
        if (parameters.result != 0)
            return -1;
            
        RET_ONERROR( mywritestr(f, "    ") ) 
        if (languageFlag & MANY_JAVA_CLASSES) 
            RET_ONERROR( mywritestr(f, "static ") )
        RET_ONERROR( mywritestr(f, "void render() ") )
        RET_ONERROR( mywritestr(f, "{ root.treeRender(null, null); }\n") ) 
        RET_ONERROR( mywritestr(f, "    ") ) 
        if (languageFlag & MANY_JAVA_CLASSES) 
            RET_ONERROR( mywritestr(f, "static ") )
        RET_ONERROR( mywritestr(f, "void doWithData() ") )
        RET_ONERROR( mywritestr(f, "{ root.treeDoWithData(null); }\n") ) 

        RET_ONERROR( mywritef(f, "    static %s%s ", 
                                 TheApp->getCPrefix(), "Node") )
        RET_ONERROR( mywritef(f, "getNodeFromGlName(") )
        RET_ONERROR( mywritef(f, "int glName) {\n") )
        RET_ONERROR( mywritestr(f, "        switch (glName) {\n") ) 
        for (int i = 0; i < m_glNameData.size(); i++) {
            RET_ONERROR( mywritef(f, "           case %d:\n", 
                                  m_glNameData[i].glName) )
            RET_ONERROR( mywritestr(f, "             return ") )
            MyString className = "";
            if (m_glNameData[i].node->hasName()) 
                className += TheApp->getCSceneGraphName();
            else {
                className += TheApp->getCPrefix();
                className += m_glNameData[i].nodeName;
            }
            RET_ONERROR( mywritef(f, "%s.%s;\n", 
                                     (const char *)className,
                                     (const char *)m_glNameData[i].nodeName
                                 ) )
        }
        RET_ONERROR( mywritef(f, "        }\n") )
        RET_ONERROR( mywritef(f, "    return null;\n") )
        RET_ONERROR( mywritef(f, "    }\n") )

        RET_ONERROR( mywritestr(f, "    public void setParents() {\n") )
        parameters.result = 0;
        parameters.filedes = f;
        parameters.languageFlag = languageFlag;
        m_root->doWithBranch(writeCParentCallback, &parameters);
        if (parameters.result != 0)
            return -1;
        RET_ONERROR( mywritestr(f, "    }\n") )

        for (int i = 0; i < LAST_TYPE; i++) {
            FieldValue *value = typeDefaultValue(i);
            value->writeCSendEventFunction(f, languageFlag);
        }
    }

    if (languageFlag & JAVA_SOURCE) {
        RET_ONERROR( mywritestr(f, "\n") )
        RET_ONERROR( mywritestr(f, "    void ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "ShowError(Exception e) {\n") )
        RET_ONERROR( mywritestr(f, "        StackTraceElement stack[] = e.getStackTrace();\n") )
        RET_ONERROR( mywritestr(f, "        ") )
        if (TheApp->isWonderlandModuleExport()) {
            RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
            RET_ONERROR( mywritestr(f, "_Util.logger.warning") )
        } else 
            RET_ONERROR( mywritestr(f, "java.lang.System.err.print") )
        RET_ONERROR( mywritestr(f, "(\"\\n\" + e.getMessage() + \"\\n\");\n") )
        RET_ONERROR( mywritestr(f, "        for (int i = 0; i < stack.length; i++)\n") )
        RET_ONERROR( mywritestr(f, "            ") )
        if (TheApp->isWonderlandModuleExport()) {
            RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
            RET_ONERROR( mywritestr(f, "_Util.logger.warning") )
        } else 
            RET_ONERROR( mywritestr(f, "java.lang.System.err.print") )
        RET_ONERROR( mywritestr(f, "(stack[i].toString() + \"\\n\");\n") )
        RET_ONERROR( mywritestr(f, "    }\n") )
    }
    RET_ONERROR( writeCRoutes(f, languageFlag) )
    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritestr(f, "};\n\n") ) 
    return(0);
}

int 
Scene::writeCTreeCallback(int f, const char *functionName, 
                          bool callTreeCallback)
{
    RET_ONERROR( mywritestr(f, "void ") )
    RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
    RET_ONERROR( mywritestr(f, "Tree") )
    RET_ONERROR( mywritestr(f, functionName) )
    if (callTreeCallback)
        RET_ONERROR( mywritestr(f, "Callback") )
    RET_ONERROR( mywritestr(f, "(struct ") )
    RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
    RET_ONERROR( mywritestr(f, "Struct *node,") )
    RET_ONERROR( mywritestr(f, " void *data) {\n") )
    RET_ONERROR( mywritestr(f, "    switch(node->") ) 
    RET_ONERROR( mywritestr(f, "m_type) {\n") ) 
    ProtoMap::Chain::Iterator *j;
    for (int i = 0; i < m_protos.width(); i++)
        for (j = m_protos.chain(i).first(); j != NULL; j = j->next()) {
            Proto *proto = j->item()->getData();
            if (proto == NULL)
                continue;
            if ((proto->getType() != VRML_INDEXED_FACE_SET) && 
                (!(proto->isInScene())))
                continue;
            if (proto->getType() == VRML_COMMENT)
                continue;
            if (proto->isMesh() && 
                !(proto->isExportTargetMesh()))
                continue;
            if (proto->isMismatchingProto())
                continue;
            RET_ONERROR( mywritestr(f, "      case ") ) 
            if (proto->getNumNodes() > 0)  
                RET_ONERROR( mywritef(f, "%d", getProtoType(proto)) )
            else
                RET_ONERROR( mywritef(f, "%d", proto->getType()) ) 
            RET_ONERROR( mywritestr(f, ":\n") ) 
            if (callTreeCallback) {
                RET_ONERROR( mywritestr(f, "        if (") ) 
                RET_ONERROR( mywritestr(f, proto->getClassName()) )
                RET_ONERROR( mywritestr(f, "Tree") )
                RET_ONERROR( mywritestr(f, functionName) )
                RET_ONERROR( mywritestr(f, "Callback)\n") )
                RET_ONERROR( mywritestr(f, "            ") ) 
                RET_ONERROR( mywritestr(f, proto->getClassName()) )
                RET_ONERROR( mywritestr(f, "Tree") )
                RET_ONERROR( mywritestr(f, functionName) )
                RET_ONERROR( mywritestr(f, "Callback(node, data);\n") )
                RET_ONERROR( mywritestr(f, "        else\n") ) 
                RET_ONERROR( mywritestr(f, "    ") ) 
            }
            RET_ONERROR( mywritestr(f, "        ") ) 
            RET_ONERROR( mywritestr(f, proto->getClassName()) )
            RET_ONERROR( mywritestr(f, "Tree") )
            RET_ONERROR( mywritestr(f, functionName) )
            RET_ONERROR( mywritestr(f, "(node, data);\n") )
            RET_ONERROR( mywritestr(f, "        break;\n") )
        }
    RET_ONERROR( mywritestr(f, "    }\n") ) 
    RET_ONERROR( mywritestr(f, "}\n") ) 
    return 0;
}

class WriteCFunctionCallData {
public:
    int filedes;
    int languageFlag;
    int returnValue;
};

static bool searchNodesOnlyOutputAndWriteCRoutes(Node *node, void *data)
{
    WriteCFunctionCallData *cdata = (WriteCFunctionCallData *)data;
    if (node->hasOutputs() && !(node->hasInputs())) {
        if (node->writeCAndFollowRoutes(cdata->filedes, 4, cdata->languageFlag, 
                                        false, "") != 0) {
            cdata->returnValue = -1;     
            return false;
        }
    }
    return true;
}

int                 
Scene::writeCRoutes(int filedes, int languageFlag)
{
    static bool alreadyIn = false;
    if (alreadyIn)
        return 0;
    alreadyIn = true;
    // write tempory nodes first
    for (int i = 0; i < m_delayedWriteNodes.size(); i++) {
        int ret = m_delayedWriteNodes[i]->writeC(filedes, languageFlag);
        if (ret < 0) {
            alreadyIn = false;
            return ret;
        }
        removeNode(m_delayedWriteNodes[i]);
    }
    if (m_delayedWriteNodes.size() > 0)
        m_delayedWriteNodes.resize(0);
    alreadyIn = false;

    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritestr(filedes, "    ") )
    RET_ONERROR( mywritestr(filedes, "void ") )
    RET_ONERROR( mywritestr(filedes, TheApp->getCProcessEventsName()) )
    RET_ONERROR( mywritestr(filedes, "(") )
    if (languageFlag & C_SOURCE)
        RET_ONERROR( mywritestr(filedes, "struct ") )
    if (languageFlag & (C_SOURCE | CC_SOURCE)) {
        RET_ONERROR( mywritestr(filedes, TheApp->getCSceneGraphName()) )
        RET_ONERROR( mywritestr(filedes, " *self, void *data") ) 
    }

    RET_ONERROR( mywritestr(filedes, ") {\n") )

    if (languageFlag & JAVA_SOURCE) {
        RET_ONERROR( indentf(filedes, 8) )
        RET_ONERROR( mywritestr(filedes, "boolean nextEvent = false;\n") )
    } else {
        RET_ONERROR( indentf(filedes, 4) )
        RET_ONERROR( mywritestr(filedes, "int nextEvent = 0;\n") )
    }
 
    WriteCFunctionCallData cdata;
    cdata.filedes = filedes;
    cdata.languageFlag = languageFlag;
    cdata.returnValue = 0;
    m_root->doWithBranch(searchNodesOnlyOutputAndWriteCRoutes, &cdata);
    for (int i = 0; i < m_sensorNodes.size(); i++) {
        if (languageFlag & JAVA_SOURCE)
            RET_ONERROR( mywritestr(filedes, "    ") )
        RET_ONERROR( mywritestr(filedes, "    ") )
        MyString className = "";
        if (languageFlag & JAVA_SOURCE) {
            if (m_sensorNodes[i]->hasName()) 
                className += TheApp->getCSceneGraphName();
            else {
                className += TheApp->getCPrefix();
                className += m_sensorNodes[i]->getVariableName();
            }
            RET_ONERROR( mywritestr(filedes, "Extra.") )
        }
        RET_ONERROR( mywritestr(filedes, "reInitSensor(") )
        if (languageFlag & JAVA_SOURCE) {
            RET_ONERROR( mywritef(filedes, "%s.", (const char *)className) )
        } else
            RET_ONERROR( mywritestr(filedes, "&(self->") )
        RET_ONERROR( mywritestr(filedes, m_sensorNodes[i]->getVariableName()) )
        if (!(languageFlag & JAVA_SOURCE))
            RET_ONERROR( mywritestr(filedes, ")") )
        RET_ONERROR( mywritestr(filedes, ");\n") )
    }
    removeSensorNodes();
    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritestr(filedes, "    ") )
    RET_ONERROR( mywritestr(filedes, "}\n") ) 
    return cdata.returnValue;

    return 0;
}

static bool searchExtensionProto(int extension, Node *node, void *data)
{
    MyArray<Proto *> *protoArrayPtr = (MyArray<Proto *> *) data;
    if (!node->hasDefault(extension) /* && !node->hasRoute(extension)*/) {
        for (int i = 0; i < (*protoArrayPtr).size(); i++)
            if ((*protoArrayPtr)[i] == node->getProto())
                return true;        
        (*protoArrayPtr).append(node->getProto());
    }
    // Todo: also check if Route to extension event
    return true;     
}

static bool searchCoverExtensionProto(Node *node, void *data)
{
    return searchExtensionProto(FF_COVER_ONLY, node, data);
}

static bool searchKambiExtensionProto(Node *node, void *data)
{
    return searchExtensionProto(FF_KAMBI_ONLY, node, data);
}

int
Scene::writeExtensionProtos(int f, int flag)
{
    int x3d = false;
    if (flag == FF_X3D_ONLY)
        x3d = true;
    MyArray<Proto *> protoArray;
    NodeList *nodes = ((NodeGroup *)getRoot())->children()->getValues();
    for (int i = 0; i < nodes->size(); i++) {
        if (flag == FF_COVER_ONLY)
            nodes->get(i)->doWithBranch(searchCoverExtensionProto, 
                                        &protoArray, false);
        if (flag == FF_KAMBI_ONLY)
            nodes->get(i)->doWithBranch(searchKambiExtensionProto, 
                                        &protoArray, false);
    }
    for (int i = 0; i < protoArray.size(); i++) {
        addProtoName(protoArray[i]->getName(false));
        Proto *newProto = new Proto(this, protoArray[i], flag);
        if (newProto->write(f, 0, x3d) != 0) {
            deleteExtensionProtos();
            return -1;
        }
        if (mywritestr(f, "\n") != 0) {
            deleteExtensionProtos();
            return -1;
        }

        m_writtenExtensionProtos.append(newProto);
    }
    return 0;
}

void
Scene::deleteExtensionProtos(void)
{
    for (int i = 0; i < m_writtenExtensionProtos.size(); i++) {
        deleteProtoName(m_writtenExtensionProtos[i]->getName(false));
        deleteProto(m_writtenExtensionProtos[i]->getName(false));
        delete(m_writtenExtensionProtos[i]);
    }
    m_writtenExtensionProtos.resize(0);
}

Proto *
Scene::getExtensionProto(Proto *proto)
{
    for (int i = 0; i < m_writtenExtensionProtos.size(); i++)
        if (m_writtenExtensionProtos[i]->getNode(0)->getProto() == proto)
            return m_writtenExtensionProtos[i];
    return NULL;
}


void
Scene::addProto(MyString name, Proto *value)
{
    if (belongsToNodeWithExternProto(name))
       return;
    Proto *proto = m_protos[name];
    if (proto && proto->isX3dInternalProto())
        return;
    m_protos[name] = value;
}

void Scene::deleteProto(MyString name)
{
    if (m_protos.hasKey(name)) {
        m_protos.remove(name);
        deleteProtoName(name);
    }
}

Proto *
Scene::getProto(MyString name)
{
    return m_protos[name];
}

Proto *
Scene::getExtensionProto(MyString name)
{
    Proto *proto = NULL;
    MyString protoName = name;
    // TODO: handle X3D node names like the following in a better way
    if (strcmp(protoName, "NurbsPatchSurface") == 0) {
        protoName = "";
        protoName += "NurbsSurface";
    }
    if (m_protos.hasKey(protoName)) 
        proto = m_protos[protoName];
    else
        return NULL;
    if (TheApp->getCoverMode() && proto->isExtensionProto(FF_COVER_ONLY)) {
        proto = proto->getNode(0)->getProto();
        proto->setScene(this);
    } 
    if (TheApp->getKambiMode() && proto->isExtensionProto(FF_KAMBI_ONLY)) {
        proto = proto->getNode(0)->getProto();
        proto->setScene(this);
    }
    if (TheApp->getX3domMode() && proto->isExtensionProto(FF_X3DOM_ONLY)) {
        proto = proto->getNode(0)->getProto();
        proto->setScene(this);
    } 
    return proto;
}

bool
Scene::validRoute(Node *src, int eventOut, Node *dst, int eventIn)
{
    if ((src == NULL) || (dst == NULL))
        return false;

    bool onlyOneConnectAnything = false;
    // "connect anything" route of Script/ShaderNode (eventOut will be created)
    if (src->isNodeWithAdditionalEvents())
        if (eventOut == src->getProto()->getNumEventOuts())
            onlyOneConnectAnything = true;

    // "connect anything" route of Script/ShaderNode (eventIn will be created)
    if (dst->isNodeWithAdditionalEvents())
        if (eventIn == dst->getProto()->getNumEventIns()) {
            if (onlyOneConnectAnything)
                onlyOneConnectAnything = false;
            else
                onlyOneConnectAnything = true;
        }

    if (onlyOneConnectAnything)
        return true;

    if (eventOut < 0 || eventOut >= src->getProto()->getNumEventOuts())
        return false;

    if (eventIn < 0 || eventIn >= dst->getProto()->getNumEventIns())
        return false;

    if (src->getProto()->getEventOut(eventOut)->getType() != 
        dst->getProto()->getEventIn(eventIn)->getType())
        return false;

    return true;
}

bool
Scene::addRoute(Node *src, int eventOut, Node *dst, int eventIn, 
                SceneView *sender)
{
    bool x3d = isX3d();
    bool ret = true;
    if (!validRoute(src, eventOut, dst, eventIn)) 
        ret = false;
    Proto *srcProto = src->getProto();
    if ((eventOut != srcProto->getNumEventOuts()) &&
        (isInvalidElement(srcProto->getEventOut(eventOut))))
        ret = false; 
    Proto *dstProto = dst->getProto();
    if ((eventIn != dstProto->getNumEventIns()) &&
        (isInvalidElement(dstProto->getEventIn(eventIn))))
        ret = false; 
    if (!ret) {
        swDebugf("invalid ROUTE %s.%s To %s.%s\n",
                 (const char *)src->getProto()->getName(x3d),
                 (const char *)src->getProto()->getEventOut(eventOut)->getName(x3d),
                 (const char *)dst->getProto()->getName(x3d),
                 (const char *)dst->getProto()->getEventIn(eventIn)->getName(x3d));
        return false;
    }

    RouteUpdate hint(src, eventOut, dst, eventIn);

    src->addOutput(eventOut, dst, eventIn);
    dst->addInput(eventIn, src, eventOut);

    // try to copy a exposedField value or field of a eventIn
    // to the first value of a Interpolator

    int field = -1;
    bool isFlag = false;

    // is this dst field an ExposedField?
    ExposedField *e = dst->getProto()->getEventIn(eventIn)->getExposedField();
    if (e) {
        field = e->getFieldIndex();
        isFlag = e->getFlags() & FF_IS;
    } else {
        // is this dst field an EventIn connected to a Field ?
        field = dst->getProto()->getEventIn(eventIn)->getField();
        isFlag = dst->getProto()->getEventIn(eventIn)->getFlags() & FF_IS;
    }
    if ((field != -1) && (!isFlag)) {
        Interpolator *interp = findUpstreamInterpolator(dst, field);
        if (interp && !((Node *)interp)->isPROTO()) {
            bool setValue = false;
            if (interp->getNumKeys() == 0) 
                setValue = true;
            if (interp->getNumKeys() == 1)
                if (interp->getKey(0) == 0.0f)
                    setValue = true;
            if (setValue) {
                FieldValue *value = dst->getField(field);
                if (value)
                    interp->recordKey(value, true);
            }
        }
    }
    UpdateViews(sender, UPDATE_ADD_ROUTE, (Hint *) &hint);
    return true;
}

void
Scene::deleteRoute(Node *src, int eventOut, Node *dst, int eventIn)
{
    if (eventOut < 0 || eventOut >= src->getProto()->getNumEventOuts())
        return;

    if (eventIn < 0 || eventIn >= dst->getProto()->getNumEventIns())
        return;

    RouteUpdate hint(src, eventOut, dst, eventIn);

    src->removeOutput(eventOut, dst, eventIn);
    dst->removeInput(eventIn, src, eventOut);
    UpdateViews(NULL, UPDATE_DELETE_ROUTE, (Hint *) &hint);
}

void
Scene::errorf(const char *fmt, ...)
{
    va_list ap;
    char buf[1024], buf2[1024];
    const char *url = "";  

    va_start(ap, fmt);
    myvsnprintf(buf, 1024, fmt, ap);
    if (TheApp->getImportFile() != NULL)
        url = TheApp->getImportFile();
    mysnprintf(buf2, 1024, "%s %d: %s", url, lineno, buf);
#ifndef _WIN32
    fprintf(stderr, "%s", buf2);
    fflush(stderr);
#endif
    m_errors += buf2;
}

void
Scene::invalidNode(const char *name)
{
    errorf("invalid DEF name \"%s\"\n", name);
}

void 
Scene::invalidField(const char *node, const char *field)
{
    errorf("node \"%s\" has no field \"%s\"\n", node, field);
}

#ifdef NURBS_CURVE_ANIMATION_COMPATIBILTY
static bool replaceNurbsCurveAnimation(Node *node, void *data)
{
    if (node == NULL)
        return true;
    Proto *proto = node->getProto();
    if (proto == NULL)
        return true;
    Scene *scene = node->getScene();
    if (scene == NULL)
        return true;
    bool x3d = scene->isX3d();
    int nurbsCurveField;
    if (strcmp(proto->getName(x3d), "NurbsCurveAnimation") == 0) {
        if ((proto->lookupField("rotationAxesInterpolator", x3d) != -1) &&
            (proto->lookupField("timeWarpScalarInterpolator", x3d) != -1) &&
            (nurbsCurveField = proto->lookupField("nurbsCurve", x3d) != -1)) {
            FieldValue *value = node->getField(nurbsCurveField);
            if (value == NULL)
                return true;
            NodeNurbsCurve *curve = (NodeNurbsCurve *)
                                     ((SFNode *)value)->getValue();
            if (curve == NULL)
                return true;
            Node *curveAnimation = curve->toCurveAnimation();
            if (curveAnimation != NULL) {
                Node *fromNode = node;
                Node *toNode = curveAnimation;
                Proto *fromProto = fromNode->getProto();
                Proto *toProto = toNode->getProto();
                int fromFractionEvent = fromProto->lookupEventIn("set_fraction",
                                                                 x3d);
                int toFractionEvent = toProto->lookupEventIn("set_fraction",
                                                             x3d);
                if ((fromFractionEvent == -1) || (toFractionEvent == -1))
                    return true;
                SocketList::Iterator *i = NULL;
                CommandList *list = NULL;
    
                for (i = fromNode->getInput(fromFractionEvent).first(); 
                     i != NULL; i = i->next()) {
                    if (list == NULL) 
                        list = new CommandList();
                    list->append(new RouteCommand(i->item().getNode(), 
                                 i->item().getField(), toNode, 
                                 toFractionEvent));
                }                                        

                int fromPosEvent = fromProto->lookupEventOut("position_changed",
                                                            x3d);
                int toPosEvent = toProto->lookupEventOut("position_changed",
                                                        x3d);
                if ((fromPosEvent == -1) || (toPosEvent == -1))
                    return true;
                for (i = fromNode->getOutput(fromPosEvent).first(); i != NULL; 
                     i = i->next()) {
                    if (list == NULL) 
                        list = new CommandList();
                    list->append(new RouteCommand(toNode, toPosEvent,
                                                  i->item().getNode(), 
                                                  i->item().getField()));
                }

                int fromOriEvent = fromProto->lookupEventOut("orientation_changed",
                                                             x3d);
                int toOriEvent = toProto->lookupEventOut("orientation_changed",
                                                         x3d);
                if ((fromOriEvent == -1) || (toOriEvent == -1))
                    return true;
                for (i = fromNode->getOutput(fromOriEvent).first(); i != NULL; 
                     i = i->next()) {
                    if (list == NULL) 
                        list = new CommandList();
                    list->append(new RouteCommand(toNode, toOriEvent,
                                                  i->item().getNode(), 
                                                  i->item().getField()));
                }
    
                if (list) scene->execute(list);

                MyString oldName = node->getName();
                if (oldName.length() > 0) {
                    scene->undef(oldName);
                    scene->def(oldName, curveAnimation);
                }
                Node *parent = node->getParent();
                int parentField = node->getParentField();
                MoveCommand *command;
                command = new MoveCommand(node, parent, parentField, NULL, -1);
                command->execute();
                command = new MoveCommand(curveAnimation, NULL, -1, 
                                          parent, parentField);
                command->execute();
            }
        }
    }
    return true;     
}
#endif

const char *
Scene::parse(FILE *f, Node* target, int field, int scan)
{
    bool canRewind = true;
    if (isatty(fileno(f)))
        canRewind = false;
#ifdef HAVE_LIBZ
    inputFile = gzdopen(fileno(f),"rb");
#else
    inputFile = f;   
#endif    
    scene = this;
    targetNode = target;
    targetField = field;
    scanFor = scan;
    lineno = 1;
    m_errors = "";
    m_isParsing = true;
    m_writeFlags = m_writeFlags & ~(X3DV | X3D_XML);
    bool isXml = false;
    bool isOther = false;
    if (canRewind) {
        int firstChar = -1;
        int end;
        do {
#ifdef HAVE_LIBZ
           end = -1;
           firstChar = gzgetc(inputFile);
#else
           end = EOF;
           firstChar = getc(inputFile);
#endif    
           if (firstChar == '<')
               isXml = true;
           else if (!isspace(firstChar))
               isOther = true;
        } while (isspace(firstChar) && (firstChar != end));
#ifdef HAVE_LIBZ
        gzrewind(inputFile);
#else
        rewind(inputFile);
#endif    
    } else {
        // no rewind possible, check current mode
        isXml = isX3dXml();
        isOther = !isXml;
    }
#ifdef HAVE_LIBEXPAT
    if (isXml)
        parseX3d();
    if (isOther) 
#endif
        yyparse();
#ifdef NURBS_CURVE_ANIMATION_COMPATIBILTY
    if (targetNode)
        targetNode->doWithBranch(replaceNurbsCurveAnimation, NULL);
    else if (getRoot())
        getRoot()->doWithBranch(replaceNurbsCurveAnimation, NULL);
#endif
    m_isParsing = false;
    updateViewpoint();
    return m_errors;
}

void
Scene::add(Command *cmd)
{
    m_undoStack.push(cmd);

    while (!m_redoStack.empty()) {
        delete m_redoStack.pop();
    }
}


void
Scene::addNextCommand(void)
{
    m_undoStack.push(new NextCommand());
}

void
Scene::execute(Command *cmd, SceneView *sender)
{
    cmd->execute(sender);
    add(cmd);
}

void
Scene::backupFieldsStart(void)
{
    // check for forbidden recursive usage or not missing backupFieldsDone()
    assert(m_backupCommandList == NULL);
    m_backupCommandList = new CommandList();
}

void
Scene::backupFieldsAppend(Node *node, int field)
{
    if (isRecording()) {
        Interpolator *interp = findUpstreamInterpolator(node, field);

        if (interp) {
            interp->backup(m_backupCommandList);
        }
    }
    m_backupCommandList->append(new FieldCommand(node, field));
}

void 
Scene::backupFieldsDone(void)
{
    add(m_backupCommandList);
    m_backupCommandList = NULL;
}

void
Scene::backupField(Node *node, int field)
{
    backupFieldsStart();
    backupFieldsAppend(node, field);
    backupFieldsDone();
}

Interpolator *
Scene::findUpstreamInterpolator(const Node *node, int field) const
{
    // is this field an ExposedField?
    int eventIn = -1;
    Field *f = node->getProto()->getField(field);
    ExposedField *e = f->getExposedField();
    if (e)
        eventIn = e->getEventIn();
    else if (f->getEventIn() != -1)
        eventIn = f->getEventIn();
    if (eventIn != -1) {
        const SocketList::Iterator *i;

        // check for interpolator routed to the corresponding EventIn;

        for (i = node->getInput(eventIn).first(); i != NULL; i = i->next()) {
            if ((i->item().getNode()->getMaskedNodeClass() == 
                 INTERPOLATOR_NODE) && i->item().getNode()->isInterpolator())
                return ((Interpolator *) i->item().getNode());
        }
    }
    return NULL;

}

void
Scene::setField(Node *node, int field, FieldValue *value)
{
    if (isRecording()) {
        Interpolator *interp = findUpstreamInterpolator(node, field);

        if (interp) {
            interp->recordKey(value,isRunning());
        }
    }
    node->setField(field, value);
    if (TheApp->is4Catt()) {
        if ((field != -1) && (node->getSolidField() == field)) {
            node->generateTreeLabel();
            NodeUpdate *hint= new NodeUpdate(node, NULL, 0);
            UpdateViews(NULL, UPDATE_SOLID_CHANGED, (Hint*) hint);
        }
    }
    if (node->getType() == VRML_TIME_SENSOR) {
       NodeTimeSensor *nodeTimeSensor = (NodeTimeSensor *) node;
       nodeTimeSensor->updateStart(field, value, m_currentTime);
    }
    OnFieldChange(node, field);
}

void 
Scene::deleteLastNextCommand(void)
{
    if (m_undoStack.empty())
        return;
    Command *command = m_undoStack.peek();
    if (command->getType() == NEXT_COMMAND)
        command = m_undoStack.pop();
}

int 
Scene::canUndo() const
{ 
    if (m_undoStack.empty())
        return 0;
    int top = m_undoStack.getTop();
    Command *command = m_undoStack.peek(top);
    if (command->getType() == NEXT_COMMAND)
        return (top > 1);
    return (top > 1);
}

int
Scene::canRedo() const
{ 
    if (m_redoStack.empty())
        return 0;
    int top = m_redoStack.getTop();
    Command *command = m_redoStack.peek(top);
    if (command->getType() == NEXT_COMMAND)
        return (top > 1);
    return (top > 1);
}

void
Scene::undo()
{
    if (m_undoStack.empty()) {
        assert(0);
        return;
    }

    Command *change = m_undoStack.pop();        
    if (change->getType() != NEXT_COMMAND) {
        change->undo();
        m_redoStack.push(change);
    } else if (m_undoStack.peek()->getType() == NEXT_COMMAND) {
        // ignore second NEXT_COMMAND
        change = m_undoStack.pop();
        m_redoStack.push(change);
    }
    while (!m_undoStack.empty()) {
        change = m_undoStack.pop();
        if (change->getType() == NEXT_COMMAND) {
            m_redoStack.push(change);
            break;
        } else {
            change->undo();
            m_redoStack.push(change);
        }
    }
}

void
Scene::redo()
{
    if (m_redoStack.empty()) {
        assert(0);
        return;
    }

    Command *change = m_redoStack.pop();        
    if (change->getType() != NEXT_COMMAND) {
        change->execute();
        m_undoStack.push(change);
    } else if (m_redoStack.peek()->getType() == NEXT_COMMAND) {
        // ignore second NEXT_COMMAND
        change = m_redoStack.pop();
        m_undoStack.push(change);
    }

    while (!m_redoStack.empty()) {
        change = m_redoStack.pop();
        if (change->getType() == NEXT_COMMAND) {
            m_undoStack.push(change);
            break;
        } else {
            change->execute();
            m_undoStack.push(change);
        }
    }
}

void
Scene::drawScene(bool pick, int x, int y, float width, float height, 
                 Node *root, bool useUpdate, float scaleX, float scaleY)
{
    m_width = width;
    m_height = height;

    m_drawViewPorts = (root != getRoot());
    if (root == NULL) {
        root = getRoot();
        m_drawViewPorts = false;
    }
    GLint v[4];
    float aspect;

    glGetIntegerv(GL_VIEWPORT, v);

    if (v[3])
        aspect = (GLfloat)v[2]/v[3];
    else  // don't divide by zero, not that we should ever run into that...
        aspect = 1.0f;

    m_numLights = 0;
    m_numClipPlanes = 0;
    m_headlight = true;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (pick) 
        gluPickMatrix(x, y, width, height, v);
    float fieldOfView = RAD2DEG(getCamera()->fov()->getValue() *
                                getUnitAngle());
    if (TheApp->hasFixFieldOfView())
        fieldOfView = TheApp->getFixFieldOfView();
    gluPerspective(fieldOfView, aspect, TheApp->GetNearClippingPlaneDist(),
                                        TheApp->GetFarClippingPlaneDist());

    if (getSelection()->getNode() &&
        getSelection()->getNode()->getType() == X3D_ORTHO_VIEWPOINT) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float dim = 4; // ???
        glOrtho(-dim * aspect, dim * aspect, -dim, dim, 
                TheApp->GetNearClippingPlaneDist(),
                TheApp->GetFarClippingPlaneDist());
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (!m_drawViewPorts) {
        glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }

    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    if (TheApp->getRenderFaster()) {
        glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
        glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
    } else {
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    }
 
    // no scene ambient light, please
    GLint zero[4] = { 0, 0, 0, 0 };
    glLightModeliv(GL_LIGHT_MODEL_AMBIENT, zero);

    double scale = getUnitLength();
    if (scale > 0)
        glScaled(1 / scale, 1 / scale, 1 / scale);

    glScalef(scaleX, scaleY, 1.0f);

    // first pass:  pre-draw traversal
    // enable PointLights and SpotLights; pick up ViewPoints, Fogs.
    // Backgrounds and TimeSensors

    glPushMatrix();

    bool viewpointFlag = getViewpointUpdated();

    applyCamera();

    m_viewpointUpdated = viewpointFlag; 

    glPopMatrix();

    m_timeSensors.resize(0);

    m_headlightIsSet = false;
    m_defaultViewpoint->preDraw();
    root->preDraw();

    if (m_currentFog)
        m_currentFog->apply();

     if (useUpdate)
         updateTime();

    // second pass:  main drawing traversal

    glPushMatrix();
    if (m_headlight) 
        enableHeadlight();
    glPushName(PICKED_NODE);
    if (m_backgrounds.size() > 0) {
//        glPushName(NO_HANDLE);    // FIXME:  can't pick backgrounds, yet
        if (m_currentBackground)
            ((NodeBackground *) m_currentBackground)->apply();
//        glPopName();
    }
    applyCamera();
    if (m_xrayRendering)
        glDepthMask(GL_FALSE);
    else
        glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    root->draw(RENDER_PASS_NON_TRANSPARENT);
    glEnable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);
    root->draw(RENDER_PASS_TRANSPARENT);

    for (int i = 0; i < m_numLights; i++) {
        glDisable((GLenum) (GL_LIGHT0 + i));
    }
    m_numLights = 0;

    for (int i = 0; i < m_numClipPlanes; i++) {
        glDisable((GLenum) (GL_CLIP_PLANE0 + i));
    }
    m_numClipPlanes = 0;

    glDisable(GL_FOG);
    glPopMatrix();

    // post-draw phase
    // draw handles 

    if (m_headlight)
        enableHeadlight();

    if (getSelection()->getNode() &&
        getSelection()->getNode()->getType() != X3D_ORTHO_VIEWPOINT) {
        glPushMatrix();
        applyCamera();
        glLoadName(PICKED_RIGID_BODY_HANDLE);
        drawHandles(root, true);
        glPopMatrix();

        glPushMatrix();
        applyCamera();
        glLoadName(PICKED_HANDLE);
        drawHandles(root, false);
        glPopMatrix();
    }
    glDisable(GL_LIGHT0);
    glPopName();
    TheApp->printRenderErrors();

    m_numDraw++;
}

void
Scene::resetPerspective(void)
{
    GLint v[4];
    float aspect;

    glGetIntegerv(GL_VIEWPORT, v);

    if (v[3])
        aspect = (GLfloat)v[2]/v[3];
    else  // don't divide by zero, not that we should ever run into that...
        aspect = 1.0f;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float fieldOfView = RAD2DEG(getCamera()->fov()->getValue() *
                                getUnitAngle());
    if (TheApp->hasFixFieldOfView())
        fieldOfView = TheApp->getFixFieldOfView();
    gluPerspective(fieldOfView, aspect, TheApp->GetNearClippingPlaneDist(),
                                        TheApp->GetFarClippingPlaneDist());
}

void
Scene::draw3dCursor(int x, int y)
{
    if (!use3dCursor())
         return;

    float objX;
    float objY;
    float objZ;

    float eyeposition=0; 
    float eyeangle=0; 
    float nearPlane=TheApp->GetNearClippingPlaneDist();
    
    glPushMatrix();

    if (TheApp->useStereo())
       {
       // inexact "toe in" stereo method 
       if (TheApp->getEyeMode()==EM_RIGHT)
          {
          eyeposition= - TheApp->getEyeHalfDist();
          eyeangle= - TheApp->getEyeAngle();
          }
       else if (TheApp->getEyeMode()==EM_LEFT)
          {
          eyeposition= + TheApp->getEyeHalfDist();
          eyeangle= + TheApp->getEyeAngle();
          }
       }
    glTranslatef(eyeposition, 0, 0);
    glRotatef(eyeangle, 0,1,0);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    float xSize = (viewport[2] - viewport[0]) * TheApp->GetEyeAngleFactor() *
                  (viewport[3] - viewport[1]) / (viewport[2] - viewport[0]);

    unProjectPoint(x - eyeposition * xSize, y, nearPlane, &objX, &objY, &objZ);

    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
    glLineWidth(TheApp->Get3dCursorWidth());

    if (TheApp->isAnaglyphStereo())
        Util::myGlColor3f(1, 1, 1);
    else
        Util::myGlColor3f(0, 1, 0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(objX + TheApp->Get3dCursorLength(), -objY, objZ);
    glVertex3f(objX - TheApp->Get3dCursorLength(), -objY, objZ);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex3f(objX, -objY + TheApp->Get3dCursorLength(), objZ);
    glVertex3f(objX, -objY - TheApp->Get3dCursorLength(), objZ);
    glEnd();
    glEnable(GL_LIGHTING);
    glLineWidth(1);
    glPopAttrib();

    glPopMatrix();
}


void
Scene::draw3dBoundingBox(int xfrom, int yfrom, int xto, int yto)
{
    Node *selection = NULL;
    if (getSelection())
        selection = getSelection()->getNode();
    bool isNurbs = selection && ((selection->getType() == VRML_NURBS_SURFACE) || 
                                 (selection->getType() == VRML_NURBS_CURVE) ||
                                 (selection->getType() == VRML_NURBS_CURVE_2D) ||
                                 (selection->getType() == DUNE_CURVE_ANIMATION));
    if ((getSelectionMode() != SELECTION_MODE_VERTICES) && (!isNurbs))
        return;

    float objXfrom;
    float objYfrom;
    float objZfrom;

    float objXto;
    float objYto;
    float objZto;

    float eyeposition = 0; 
    float eyeangle = 0; 
    float nearPlane=TheApp->GetNearClippingPlaneDist();
    
    glPushMatrix();

    if (TheApp->useStereo()) {
       // inexact "toe in" stereo method 
       if (TheApp->getEyeMode()==EM_RIGHT)
          {
          eyeposition= - TheApp->getEyeHalfDist();
          eyeangle= - TheApp->getEyeAngle();
          }
       else if (TheApp->getEyeMode()==EM_LEFT)
          {
          eyeposition= + TheApp->getEyeHalfDist();
          eyeangle= + TheApp->getEyeAngle();
          }
       }
    glTranslatef(eyeposition, 0, 0);
    glRotatef(eyeangle, 0,1,0);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    float xSize = (viewport[2] - viewport[0]) * TheApp->GetEyeAngleFactor() *
                  (viewport[3] - viewport[1]) / (viewport[2] - viewport[0]);

    unProjectPoint(xfrom - eyeposition * xSize, yfrom, nearPlane, 
                   &objXfrom, &objYfrom, &objZfrom);
    unProjectPoint(xto - eyeposition * xSize, yto, nearPlane, 
                   &objXto, &objYto, &objZto);

    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
    glLineWidth(TheApp->Get3dCursorWidth());

    if (TheApp->isAnaglyphStereo())
        Util::myGlColor3f(1, 1, 1);
    else
        Util::myGlColor3f(0, 1, 0);

    glBegin(GL_LINE_STRIP);
    glVertex3f(objXfrom, -objYfrom, objZfrom);
    glVertex3f(objXto, -objYfrom, objZfrom);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex3f(objXto, -objYfrom, objZfrom);
    glVertex3f(objXto, -objYto, objZfrom);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex3f(objXto, -objYto, objZfrom);
    glVertex3f(objXfrom, -objYto, objZfrom);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex3f(objXfrom, -objYto, objZfrom);
    glVertex3f(objXfrom, -objYfrom, objZfrom);
    glEnd();

    glEnable(GL_LIGHTING);
    glLineWidth(1);
    glPopAttrib();

    glPopMatrix();
}

Path *
Scene::pick(int x, int y, float width, float height)
{
    float xwidth = PICK_REGION_SIZE * TheApp->GetHandleSize();
    if (width != 0)
        xwidth = width;
    float yheight = PICK_REGION_SIZE * TheApp->GetHandleSize();
    if (height != 0)
        yheight = height;
    GLuint pickBuffer[PICK_BUFFER_SIZE];
#ifdef DEBUG
    for (GLuint i = 0; i < PICK_BUFFER_SIZE; i++) 
        pickBuffer[i] = 0;
#endif
    glSelectBuffer(PICK_BUFFER_SIZE, pickBuffer);
    glRenderMode(GL_SELECT);
    glInitNames();

    TheApp->setEyeMode(EM_NONE);
    drawScene(true, x, y, xwidth, yheight);

    GLint hits = glRenderMode(GL_RENDER);
    Path *path = NULL;
    if (hits < 0) // overflow flag has been set, ignore
        hits = - hits;
    if (hits)
        path = processHits(hits, pickBuffer, (width != 0) && (height != 0));
    if (path != NULL) {
        return path;
    } else {
        return new Path(NULL, 0, this);
    }
}

Path *
Scene::processHits(GLint hits, GLuint *pickBuffer, bool selectMultipleHandles)
{
    GLuint *glPath = NULL;
    int glPathLen = 0;
    unsigned depth = UINT_MAX;
    unsigned handleDepth = 0;
    bool pickedHandle = false;
    bool pickedNode = false;
    int handle = -1;

    for (int i = 0; i < hits; i++) {
        unsigned numNames = *pickBuffer++;
        unsigned minDepth = *pickBuffer++;
        unsigned maxDepth = *pickBuffer++;
        if (*pickBuffer == PICKED_NODE) {
            if (depth >= maxDepth) {
                pickedNode = true;
                pickedHandle = false;
                glPath = pickBuffer + 1;
                glPathLen = numNames - 1;
                depth = maxDepth;
            }
        } else if (*pickBuffer == PICKED_RIGID_BODY_HANDLE) {
            if (m_rigidBodyHandleNode == NULL)
                return NULL;
            handle = pickBuffer[numNames - 1];
            return m_rigidBodyHandleNode->getPath();
        } else if (*pickBuffer == PICKED_HANDLE) {
            bool isCoord = COORDINATE_NODE ==
                  getSelection()->getNode()->getProto()->getNodeClass();
                           
            bool isVertices = getSelectionMode() == SELECTION_MODE_VERTICES ||
                              getSelectionMode() == 
                              SELECTION_HANIM_JOINT_WEIGHT;
            if ((!isCoord) || isVertices || (handleDepth <= maxDepth)) {
                if (pickBuffer[numNames - 1] != NO_HANDLE) {
                    pickedHandle = true;
                    pickedNode = false;
                    glPath = pickBuffer + 1;
                    glPathLen = numNames - 2;
                    handle = pickBuffer[numNames - 1];
                    handleDepth = maxDepth;
                }
                if (selectMultipleHandles) {
                    if (m_deselect)
                        removeSelectedHandle(handle);
                    else
                        addSelectedHandle(handle);
                }
                depth = maxDepth;
            }
        } else if (*pickBuffer == PICKED_3DCURSOR) {
            return NULL;
        } else {
            return NULL;
        }
        pickBuffer += numNames;
    }
    if (pickedNode)
        m_lastSelectedHandle = -1;
    else if (handle != -1)
        if (!selectMultipleHandles) {
            if (m_deselect)
                removeSelectedHandle(handle);
            else
                setSelectedHandle(handle);
        }

    if (glPath == NULL)
        return NULL;

    bool insideProto = false;
    Path *path = new Path((int *) glPath, glPathLen, this, true, 
                          true);
    if (path->getNode()->isDeepInsideProto()) {
        insideProto = true;
        delete path;
    } 
    return new Path((int *) glPath, glPathLen, this, insideProto);
}

void
Scene::transform(const Path *path)
{
    assert(path != NULL);
    applyCamera();
    const NodeList *nodes = path->getNodes();
    int size = nodes->size() - 1;
    for (int i = 0; i < size; i++) {
        nodes->get(i)->transform();
    }
}

// search for a Transform node in a path
// return new path to Transform node or NULL if not found

Path* Scene::searchTransform(void)
{
    Path* transform = new Path(*m_selection);
    if (transform != NULL) { 
        if (transform->getNode()->getType() == VRML_TRANSFORM)
            return transform;
        while (transform->getNode() != m_root) {
            transform = new Path(*(transform->getNode()->getParent()->getPath()));
            if (transform->getNode()->getType() == VRML_TRANSFORM)
                break;
            // delete old_transform; // bug: deleting a path cause a crash
        }
        if (transform->getNode()==m_root)
            transform = NULL;
    }
    return transform;
}

static bool checkHandleParentsForRigidBody(Node *node, void *data) {
    Scene *scene = (Scene *)data;
    if (node == NULL)
        return true;
    if (node->getType() == X3D_RIGID_BODY)
        if (node->isInScene(scene))
            scene->setRigidBodyHandleNode(node);
    return true;
}

void 
Scene::finishDrawHandles(void)
{
    int glPushCount = 0;
    glGetIntegerv(GL_NAME_STACK_DEPTH, &glPushCount);
    glPushCount--;
    for (int i = 0; i < glPushCount; i++)
        glPopName();
}

void
Scene::drawHandles(Node *root, bool drawRigidBodyHandles)
{
    if (root != getRoot())
        return;
    glDisable(GL_DEPTH_TEST);
    switch (TheApp->GetHandleMode()) {
      case HM_NONE:
        break;
      case HM_TREE:
        // draw handles for all nodes in the current selection path
        if ((m_selection != NULL) && (!drawRigidBodyHandles)) {
            glPushMatrix();
            int len = m_selection->getPathLen();
            const int *path = m_selection->getPath();
            Node *node = root;
            for (int i = 0; i < len;) {
                int field = path[i++];
                if (i >= len)
                    break;
                node = m_selection->getNextNode(node, field, i);
                if (node == NULL)
                    break;
                glPushName(field);
                glPushName(path[i++]);
                node->drawHandles();
                node->transform();
            }
            glPopMatrix();
        }
        break;
      case HM_SELECTED:
        if (m_selection != NULL) {
            glPushMatrix();
            int len = m_selection->getPathLen();
            const int *path = m_selection->getPath();
            Node *node = root;
            Node *handlenode = root;
            Node *lastnode = root;
            m_rigidBodyHandleNode = NULL;
            for (int i = 0; i < len;) {
                int field = path[i++];
                if (i >= len) {
                    break;
                }
                Node *newNode = m_selection->getNextNode(node, field, i++);
                if (newNode == NULL) {
                    finishDrawHandles();
                    break;
                } else
                    node = newNode;
                /* search last transform node in path */
                if (node->getType() == VRML_TRANSFORM)
                   handlenode = node;
                // search for a RigidBody node in the Parents of a 
                // X3DNBodyCollidableNode
                if (node->matchNodeClass(BODY_COLLIDABLE_NODE)) {
                    m_rigidBodyHandleNode = NULL;
                    node->doWithParents(checkHandleParentsForRigidBody, this);
                    if (m_rigidBodyHandleNode != NULL)
                        handlenode = node;
                }
            }
            lastnode = node;
            node = root;
            for (int i = 0; i < len;) {
                int field = path[i++];
                if (i >= len)
                    break;
                node = m_selection->getNextNode(node, field, i);
                if (node == NULL) {
                    finishDrawHandles();
                    break;
                }
                glPushName(field);
                glPushName(path[i++]);
                /* display last transform node in path */
                if ((node == handlenode) || (node == lastnode)) {
                     if ((node == handlenode) && 
                         (m_rigidBodyHandleNode != NULL)) {
                         if (drawRigidBodyHandles)
                             m_rigidBodyHandleNode->drawHandles();
                     } else {
                         if (!drawRigidBodyHandles)
                             node->drawHandles();    
                     }
                }
                if (node->getType() != X3D_RIGID_BODY)
                    if (node->getType() != X3D_HANIM_JOINT)
                        node->transform();                
            }
            glPopMatrix();
        }
        break;
      case HM_ALL:
        if (!drawRigidBodyHandles)
            drawHandlesRec(root);
        break;
    }
    finishDrawHandles();   
}

void
Scene::drawHandlesRec(Node *node, bool drawOnlyJoints) const
{
    int numFields = node->getProto()->getNumFields();

    glPushMatrix();
    bool draw = true;
    if (drawOnlyJoints)
        if (!node->isJoint())
            draw = false;
    if (draw)
        if (node->isJoint() && !node->isFirstUSE())
            draw = false;
    if (draw)
        node->drawHandles();
    node->transform();
    for (int i = 0; i < numFields; i++) {
        FieldValue *value = node->getField(i);
        if (value->getType() == SFNODE) {
            Node *child = ((SFNode *) value)->getValue();
            if (child) {
                glPushName(i);
                glPushName(0);
                drawHandlesRec(child, drawOnlyJoints);
                glPopName();
                glPopName();
            }
        } else if (value->getType() == MFNODE) {
            glPushName(i);
            glPushName(0);
            MFNode  *v = (MFNode *) value;
            int n = v->getSize();
            for (int j = 0; j < n; j++) {
                glLoadName(j);
                drawHandlesRec(v->getValue(j), drawOnlyJoints);
            }
            glPopName();
            glPopName();
        }
    }
    glPopMatrix();
}

void
Scene::enableHeadlight()
{
    GLenum light = (GLenum) allocateLight();
    static float pos[4] = {0.0f, 0.0f, 1.0f, 0.0f};
    static float ambientColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    static float diffuseColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(light, GL_AMBIENT, ambientColor);
    glLightfv(light, GL_DIFFUSE, diffuseColor);
    glLightfv(light, GL_POSITION, pos);
    glLightfv(light, GL_SPECULAR, diffuseColor);
    glLightf(light, GL_SPOT_CUTOFF, 180.0f);
    glLightf(light, GL_SPOT_EXPONENT, 0.0f);
    glLightf(light, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(light, GL_LINEAR_ATTENUATION, 0.0f);
    glLightf(light, GL_QUADRATIC_ATTENUATION, 0.0f);
    glEnable(light);
}

// allocateLight()
//
// reserve an openGL light

int
Scene::allocateLight()
{
    GLint maxLights;
    glGetIntegerv(GL_MAX_LIGHTS, &maxLights);

    if (m_numLights >= maxLights) {
        errorf("too many lights!\n");
        return GL_LIGHT0;
    }
    
    return (GL_LIGHT0 + m_numLights++);
}

int
Scene::freeLight()
{
    return GL_LIGHT0 + --m_numLights;
}

int
Scene::allocateClipPlane()
{
    GLint maxClipPlanes;
    glGetIntegerv(GL_MAX_CLIP_PLANES, &maxClipPlanes);

    if (m_numClipPlanes >= maxClipPlanes) {
        errorf("too many ClipPlanes!\n");
        return GL_CLIP_PLANE0;
    }
    
    return (GL_CLIP_PLANE0 + m_numClipPlanes++);
}

int
Scene::freeClipPlane()
{
    return GL_CLIP_PLANE0 + --m_numClipPlanes;
}

void
Scene::projectPoint(float x, float y, float z, float *wx, float *wy, float *wz)
{
    GLdouble mmat[16], pmat[16];
    GLdouble winx, winy, winz;
    GLint viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, mmat);
    glGetDoublev(GL_PROJECTION_MATRIX, pmat);
    glGetIntegerv(GL_VIEWPORT, viewport);

    gluProject(x, y, z, mmat, pmat, viewport, &winx, &winy, &winz);

    *wx = (float) winx;
    *wy = (float) winy;
    *wz = (float) winz;
}

void
Scene::unProjectPoint(float wx, float wy, float wz, float *x, float *y, float *z)
{
    GLdouble mmat[16], pmat[16];
    GLdouble objx, objy, objz;
    GLint viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, mmat);
    glGetDoublev(GL_PROJECTION_MATRIX, pmat);
    glGetIntegerv(GL_VIEWPORT, viewport);

    gluUnProject(wx, wy, wz, mmat, pmat, viewport, &objx, &objy, &objz);

    *x = (float) objx;
    *y = (float) objy;
    *z = (float) objz;
}

void
Scene::addViewpoint(Node *viewpoint)
{
    m_viewpoints.append(viewpoint);
}

void
Scene::addNavigationInfo(Node *navigationInfo)
{
    m_navigationinfos.append(navigationInfo);
}

void
Scene::addFog(Node *fog)
{
    m_fogs.append(fog);
    setFog(fog);
}

void
Scene::setFog(Node *fog)
{
    m_currentFog = (NodeFog *)fog;
}

void
Scene::addBackground(Node *background)
{
    m_backgrounds.append(background);
    setBackground(background);
}

void
Scene::setBackground(Node *background)
{
    m_currentBackground = (NodeBackground *)background;
}

void
Scene::addTimeSensor(Node *timeSensor)
{
    for (int i = 0; i < m_timeSensors.size(); i++)
        if (m_timeSensors[i] == timeSensor)
            return;
    m_timeSensors.append(timeSensor);
}

void
Scene::startWalking()
{
    m_oldWalkTime = swGetCurrentTime();
}

void
Scene::walkCamera(Vec3f walk, bool forward)
{
    Vec3d pos(m_currentViewpoint->getPosition());
    Quaternion rot(m_currentViewpoint->getOrientation());
    float fspeed = m_currentNavigationInfo->speed()->getValue();
    double dt = swGetCurrentTime() - m_oldWalkTime;

    if (forward || dt > 0) {
        Quaternion around(Vec3f(0.0f, 1.0f, 0.0f), -DEG2RAD(walk.x * 2.0f));
        Quaternion newRot(around * rot);
        newRot.normalize();
        rot.normalize();
        m_currentViewpoint->setOrientation(newRot);
        float z = dt * walk.z * 2.0f;
        m_currentViewpoint->setPosition(pos + rot * fspeed * Vec3d(0, 0, z));
        UpdateViews(NULL, UPDATE_REDRAW_3D);
    } else {
         Vec3f vec(dt * fspeed * walk.x * 2.0f, dt * fspeed * walk.y * 2.0f, 0);
         m_currentViewpoint->setPosition(pos + rot * vec);
    }
    m_viewpointUpdated = true;
}

void               
Scene::changeTurnPointDistance(float factor)
{
    if (m_currentNavigationInfo) {
        MFFloat *mfAvatarSize = m_currentNavigationInfo->avatarSize();
        float fdist = m_currentNavigationInfo->speed()->getValue() * 10;
        if (mfAvatarSize->getSize() > 3)
            fdist = mfAvatarSize->getValue(3) * factor;
        MFFloat *newAvatarSize = new MFFloat();
        for (int i = 0; i < mfAvatarSize->getSize(); i++)
            newAvatarSize->setValue(i, mfAvatarSize->getValue(i));
        float values[] = { 0.25f, 1.6f, 0.75f };
        if (newAvatarSize->getSize() < 3)
            for (int i = newAvatarSize->getSize() - 1; i < 3; i++)
                 newAvatarSize->setValue(i, values[i]);
        newAvatarSize->setValue(3, fdist);
        setField(m_currentNavigationInfo, 
                 m_currentNavigationInfo->avatarSize_Field(), newAvatarSize);
        TheApp->PrintMessageWindowsFloat(IDS_TURN_POINT_DISTANCE, fdist);
    }    
}

void
Scene::moveCamera(float dx, float dy, float dz)
{
    Vec3d pos = m_currentViewpoint->getPosition();
    Quaternion rot = m_currentViewpoint->getOrientation();

    float fspeed = m_currentNavigationInfo->speed()->getValue();
    m_currentViewpoint->setPosition(pos + rot * fspeed * Vec3f(dx, dy, dz));

    m_viewpointUpdated = true;
}

void
Scene::turnCamera(float x, float y, float z, float ang)
{
    Quaternion rot = m_currentViewpoint->getOrientation();
    Quaternion r(Vec3f(x, y, z), ang);

    m_currentViewpoint->setOrientation(r * rot);

    m_viewpointUpdated = true;
}

Quaternion oldRot;

void
Scene::setTurnPoint(void)
{    
    m_turnPointPos = m_currentViewpoint->getPosition();
    m_turnPointRot = m_currentViewpoint->getOrientation();
}

void
Scene::orbitCamera(float dtheta, float dphi)
{    
    Vec3d pos(m_currentViewpoint->getPosition());
    Quaternion rot(m_currentViewpoint->getOrientation());
    Quaternion up(Vec3f(0.0f, 1.0f, 0.0f), dtheta);
    Quaternion around(Vec3f(1.0f, 0.0f, 0.0f), dphi);
    Quaternion newRot(up * around * rot);

    Vec3f zAxis(0, 0, 10);
    Vec3f dist(m_turnPointRot * zAxis);
    dist.normalize();
    float fdist = 10;
    MFFloat *mfAvatarSize = m_currentNavigationInfo->avatarSize();
    if (mfAvatarSize->getSize() > 3)
        fdist = mfAvatarSize->getValue(3);
    else
        fdist =  m_currentNavigationInfo->speed()->getValue() * 10;

    dist = dist * fdist;

    Vec3d newPos = m_turnPointPos - dist + dist * m_turnPointRot.conj() * 
                                                  newRot;
    if (TheApp->GetMouseMode() == MOUSE_EXAMINE)
        m_currentViewpoint->setPosition(newPos);

    m_currentViewpoint->setOrientation(newRot);

    m_viewpointUpdated = true;
}

void
Scene::rollCamera(float dtheta)
{    
    Vec3d pos(m_currentViewpoint->getPosition());
    Quaternion rot(m_currentViewpoint->getOrientation());
    Quaternion roll(Vec3f(0.0f, 0.0f, -1.0f), dtheta);
    Quaternion newRot(roll * rot);
    newRot.normalize();
    rot.normalize();

    Vec3d newPos(rot.conj() * pos * newRot);

    if (TheApp->GetMouseMode() == MOUSE_EXAMINE) {
        m_currentViewpoint->setPosition(newPos);
    }
    m_currentViewpoint->setOrientation(newRot);
    applyCamera();

    m_viewpointUpdated = true;
}

void
Scene::standUpCamera(void)
{
    Quaternion rot(m_currentViewpoint->getOrientation());
    rot.x = 0;
    rot.z = 0;
    m_currentViewpoint->setOrientation(rot);
    UpdateViews(NULL, UPDATE_PREVIEW);

    m_viewpointUpdated = true;
}

Node *
Scene::createNode(const char *nodeType, int flags)
{
    Proto *def = NULL;
    if (isX3d() && strcmp(nodeType, "NurbsPatchSurface") == 0)
        def = m_protos["NurbsSurface"];
    else
        def = m_protos[nodeType];

    Node *node = def ? def->create(this) : (Node *) NULL;
    if (node != NULL)
        node->setFlag(flags);
    return node;
}


Node *
Scene::createNode(int nodeType)
{
    ProtoMap::Chain::Iterator *j;
    // search for proto with correct nodeType
    for (int i = 0; i < m_protos.width(); i++) {
        for ( j = m_protos.chain(i).first(); j != NULL; j = j->next()) {
            if (j->item()->getData()->getType() == nodeType)
                return createNode(j->item()->getKey());
        }
    }
    return (Node *) NULL;
}

void
Scene::addNode(Node *node)
{
    m_nodes.append(node);
    
}

MyString
Scene::getUniqueNodeName(const char *name)
{
    static char buf[512];
    for (int i = 1; ; i++) {
        mysnprintf(buf, 512, "%s%d", name, i);
        if (!hasAlreadyName(buf)) break;
    }
    MyString ret = "";
    ret += buf;
    return ret;
}

MyString
Scene::getUniqueNodeName(Node *node)
{
    const char *name = node->getProto()->getName(isX3d());

    return getUniqueNodeName(name);
}

MyString
Scene::generateUniqueNodeName(Node *node)
{
    MyString name = mystrdup(getUniqueNodeName(node));
    def(name, node);
    return name;
}

MyString Scene::generateVariableName(Node *node)
{
     MyString ret = "";
     ret += node->getProto()->getName(isX3d());
     m_variableCounter++;
     char number[1024];
     mysnprintf(number, 1023, "%d", m_variableCounter);
     ret += "_";
     ret += number;
     return ret;
}

void
Scene::removeNode(Node *node)
{
    int index = m_nodes.findBackward(node);
    if ((index >= 0) && (index < m_nodes.size()))
        m_nodes.remove(index);
}

void
Scene::setSelection(Path *path)
{
    m_selection_is_in_scene = true;
    m_viewOfLastSelection = NULL;
    if (m_selection != path) {
        if (m_selection)
            m_oldSelection = m_selection->getNode();
        else
            m_oldSelection = NULL;
        delete m_selection;        
        m_selection = path;
        Node *node = m_selection->getNode();
        if (node == NULL) {
            return;
        } 
        if (node->getType() == VRML_VIEWPOINT) {
            m_currentViewpoint = (NodeViewpoint *)node;
            applyCamera();
        }
        if (node->getType() == VRML_GEO_VIEWPOINT) {
            m_currentViewpoint = (NodeGeoViewpoint *)node;
            applyCamera();
        }
        if (node->getType() == VRML_NAVIGATION_INFO) {
            m_currentNavigationInfo = (NodeNavigationInfo *)node;
            applyNavigationInfo();
        }
        if (node->getType() == X3D_HANIM_JOINT) {
            m_lastSelectedHAnimJoint = node;
        }
        if (node->getType() == X3D_HANIM_HUMANOID) {
            m_lastSelectedHAnimJoint = NULL;
        }
        if (node == getRoot()) {
            m_lastSelectedHAnimJoint = NULL;
            m_firstSelectionRangeHandle = -1;
            removeSelectedHandles();
        }
    }
}

void
Scene::getProtoList(MyArray<int> *protoList, const Node *node)
{
    if (node != getRoot())
        for (int i = 0; (i < getNumProtos()) && (protoList->size() == 0); i++) {
            Proto *proto = getProto(i);
            for (int j = 0; j < proto->getNumNodes(); j++)
                if ((proto->getNode(j)->isEqualCopy((Node *)node)) ||
                    (proto->getNode(j)->isEqual((Node *)node))) {
                    protoList->append(-i - 1);
                    protoList->append(j);
                    break;
                }
        }
}

Path*
Scene::newPath(Node *node)
{
    Path* ret;
    int len = 0;
    Node *n;

    for (n = node; n->hasParent() ; n = n->getParent()) {
        len += 2;
    }

    MyArray<int> protoList;
    getProtoList(&protoList, n);
    if (protoList.size() > 0)
        len += 4;

    if (len > 0) {
        int *list = new int[len];

        int i = len-1;
    
        for (n = node; n->hasParentOrProtoParent(); 
             n = n->getParentOrProtoParent()) {
            Node *parent = n->getParent();

            int field = n->getParentFieldOrProtoParentField();
            list[i--] = parent->findChild(n, field);
            list[i--] = field;
        }
        if (protoList.size() > 0) {
            list[i--] = 0;
            list[i--] = 0;
            list[i--] = protoList[1];
            list[i--] = protoList[0];
        }
        ret=new Path(list, len, this);
        delete [] list;
    } else {
        // select root node
        ret=new Path(NULL, 0, this);
    }
    return ret;
}

void
Scene::setSelection(Node *node)
{
    if (node != NULL)
        setSelection(node->getPath());
}

void
Scene::setSelection(Proto *proto)
{
    Path* path = NULL;
    int len = 3;
    if (len > 0) {
        int *list = new int[len];
        for (int i = 0; i < getNumProtos(); i++) {
             if (proto == getProto(i)) {
                list[0] = -i - 1;
                list[1] = -1;
                list[2] = 0;                
                path = new Path(list, len, this);
                delete [] list;
                setSelection(path);            
                break;
            }
        }
    }
}

bool
Scene::isModified() const
{
    if (!canUndo()) {
        if (m_unmodified != NULL)
            return TRUE;
        else
            return m_extraModifiedFlag;
    } else {
        return m_unmodified != m_undoStack.peek();
    }
}

void
Scene::applyCamera()
{
    if (getSelection()->getNode() &&
        getSelection()->getNode()->getType() == X3D_ORTHO_VIEWPOINT)
        ((NodeOrthoViewpoint *)getSelection()->getNode())->apply();
    else if (m_currentViewpoint->getType() == VRML_GEO_VIEWPOINT)
        ((NodeGeoViewpoint *)m_currentViewpoint)->apply();
    else
        ((NodeViewpoint *)m_currentViewpoint)->apply(TheApp->useStereo(),
                                                     Vec3d(), SFRotation());

    m_viewpointUpdated = false;
}

void
Scene::setCamera(Node *camera)
{
    m_currentViewpoint = camera;

    m_viewpointUpdated = true;
}

void
Scene::applyNavigationInfo(void)
{
    m_currentNavigationInfo->apply();
}

void
Scene::setFirstNavigationInfo(void)
{
    if (!m_setNavigationInfo) {
        m_setNavigationInfo = true;
        if (m_navigationinfos.size() > 0) {
            m_currentNavigationInfo = (NodeNavigationInfo *) m_navigationinfos[0];
        } else {
            m_currentNavigationInfo = m_defaultNavigationInfo;
        }
    }
    applyNavigationInfo();
}

void
Scene::start()
{
    m_running = true;
    if (!m_setViewpoint) {
        m_setViewpoint = true;
        if (m_viewpoints.size() > 0) {
            m_currentViewpoint = (NodeViewpoint *) m_viewpoints[0];
        } else {
            m_currentViewpoint = m_defaultViewpoint;
        }
    }

    double t = swGetCurrentTime();
    updateTimeAt(t);
}

void
Scene::stop()
{
    m_running = false;
}

double
Scene::timeSinceStart(void)
{
    return m_currentTime - m_timeStart;
}

void
Scene::updateTimeAt(double t)
{
    for (int i = 0; i < m_timeSensors.size(); i++)
        ((NodeTimeSensor *) m_timeSensors[i])->setTime(t);
}

void
Scene::updateTime()
{
    m_currentTime = swGetCurrentTime();
    updateTimeAt(m_currentTime);
    UpdateViews(NULL, UPDATE_TIME);
}

Node *
Scene::getCamera() const
{
    return m_currentViewpoint;
}

NodeNavigationInfo *
Scene::getNavigationInfoNode() const
{
    return m_currentNavigationInfo;
}

void
Scene::AddView(SceneView *view)
{
    m_views.append(view);
}

void
Scene::RemoveView(SceneView *view)
{
    m_views.remove(m_views.find(view));
}

static bool updateNodePROTO(Node *node, void *data)
{
    Proto *proto = (Proto *)data;
    if (node->getProto() == proto) {
        ((NodePROTO *)node)->handleIs();
        ((NodePROTO *)node)->createPROTO();
        ((NodePROTO *)node)->reInit();
    }
    return true;
}

void Scene::updateNodePROTOs(Proto *protoToUpdate)
{    
    if (protoToUpdate != NULL) {
        getRoot()->doWithBranch(updateNodePROTO, protoToUpdate,
                                false, false, false, true, false);
        UpdateViews(NULL, UPDATE_REDRAW_3D);
    }
}

void
Scene::OnFieldChange(Node *node, int field, int index)
{
    static double time = 0;
    if ((node->getType() == VRML_VIEWPOINT) || 
        (node->getType() == VRML_GEO_VIEWPOINT))
        if (node != getSelection()->getNode())
            return;
    if ((!m_running) || (!(time == m_currentTime)))
        updateNodePROTOs(node->getOutsideProto());

    time = m_currentTime;

    FieldUpdate hint(node, field, index);
    UpdateViews(NULL, UPDATE_FIELD, (Hint *) &hint);
}

void
Scene::OnAddNode(Node *node, Node *dest, int field)
{
    if (node->getOutsideProto() != NULL) {
        Proto *protoToUpdate = node->getOutsideProto();
        protoToUpdate->setIsNodeIndex();
        getRoot()->doWithBranch(updateNodePROTO, protoToUpdate);
    }

    NodeUpdate hint(node, dest, field);

    UpdateViews(NULL, UPDATE_ADD_NODE, (Hint *) &hint);
}

void
Scene::OnAddNodeSceneGraph(Node *node, Node *dest, int field)
{
    NodeUpdate hint(node, dest, field);

    UpdateViews(NULL, UPDATE_ADD_NODE_SCENE_GRAPH_VIEW, (Hint *) &hint);
}

void
Scene::OnRemoveNode(Node *node, Node *src, int field)
{
    NodeUpdate hint(node, src, field);
    // a deleted DEF'ed node can not be USE'd
    if (node == m_defNode)
        m_defNode = NULL;
    UpdateViews(NULL, UPDATE_REMOVE_NODE, (Hint *) &hint);
}

void
Scene::UpdateViews(SceneView *sender, int type, Hint *hint)
{
    static bool alreadyInUpdate = false;
    if (alreadyInUpdate) // forbid recursive update chains
        return;
    alreadyInUpdate = true;
    UpdateViewsNow(sender, type, hint);
    alreadyInUpdate = false;
}


void
Scene::UpdateViewsNow(SceneView *sender, int type, Hint *hint)
{
    if (!m_canUpdateViewsSelection)
        if (type == UPDATE_SELECTION)
            return;
    for (List<SceneView *>::Iterator *i = m_views.first(); i != NULL; 
         i = i->next()) {
        SceneView *view = i->item();
        if (view != sender)
            view->OnUpdate(sender, type, hint);
    }
}

void
BackupRoutesRec(Node *node, CommandList *list)
{
    int                         i;
    SocketList::Iterator       *j;

    if (!node) return;

    if (node->getNumParents() > 1) return;

    for (i = 0; i < node->getProto()->getNumEventIns(); i++) {
        for (j = node->getInput(i).first(); j != NULL; j = j->next()) {
            const RouteSocket &s = j->item();
            list->append(new UnRouteCommand(s.getNode(), s.getField(), 
                                            node, i));
        }
    }
    for (i = 0; i < node->getProto()->getNumEventOuts(); i++) {
        for (j = node->getOutput(i).first(); j != NULL; j = j->next()) {
            const RouteSocket &s = j->item();
            list->append(new UnRouteCommand(node, i, 
                                            s.getNode(), s.getField()));
        }
    }

    for (i = 0; i < node->getProto()->getNumFields(); i++) {
        FieldValue  *v = node->getField(i);
        if (v->getType() == SFNODE) {
            BackupRoutesRec(((SFNode *) v)->getValue(), list);
        } else if (v->getType() == MFNODE) {
            int size = ((MFNode *) v)->getSize();
            for (int k = 0; k < size; k++) {
                BackupRoutesRec(((MFNode *) v)->getValue(k), list);
            }
        }
    }
}

void 
Scene::selectNext()
{
    Node *node = m_selection->getNode();
    int newIndex = node->getNextSiblingIndex();
    if (newIndex == -1)
        newIndex = node->getPrevSiblingIndex();
    Node *parent = m_selection->getParent();
    Node *newSelection = parent;
    if (parent == NULL)
        newSelection = m_root;
    else {
        int parentField = m_selection->getParentField();
        newSelection = parent;
        if (newIndex != -1) {
            MFNode *mfnode = (MFNode *)parent->getField(parentField);
            if (newIndex < mfnode->getSize())
                newSelection = mfnode->getValue(newIndex);
        }
    }
    setSelection(newSelection);
}

void
Scene::deleteSelected()
{
    Proto *proto = m_selection->getProto(this); 
    if (proto != NULL) {
        Node *node = m_selection->getNode();
        for (int i = 0; i < proto->getNumNodes(); i++)
            if (node->isEqual(proto->getNode(i))) {
                proto->removeNode(i);
                proto->setIsNodeIndex();
                if (i > 0)
                    setSelection(newPath(proto->getNode(i - 1)));
                else
                    setSelection(getRoot());
                UpdateViews(NULL, UPDATE_SELECTION);
                updateNodePROTOs(proto);
                UpdateViews(NULL, UPDATE_ALL);
                return;
            }
    }
    Node *parent = m_selection->getParent();
    int parentField = m_selection->getParentField();
    if ((parent != NULL) && (parentField != -1)) {
        CommandList *list = new CommandList();
        deleteSelectedAppend(list);
        selectNext();
        execute(list);
        UpdateViews(NULL, UPDATE_SELECTION);
    }    
}

void
Scene::deleteAll()
{
    NodeGroup *parent = (NodeGroup *)getRoot();
    int parentField = parent->children_Field();

    CommandList *list = new CommandList();    
    if ((parent != NULL) && (parentField != -1)) {
        for (int i = parent->children()->getSize() - 1; i >= 0; i--) {
            Node *node = parent->children()->getValue(i);      
            BackupRoutesRec(node, list);
            list->append(new MoveCommand(node, parent, parentField, NULL, -1));
            execute(list);
        }
    }
    UpdateViews(NULL, UPDATE_ALL);
}

void
Scene::deleteSelectedAppend(CommandList* list)
{
    if (m_selection && (m_selection->getNode() != m_root)) {
        if (m_selection->getNode() == m_currentViewpoint) {
            m_defaultViewpoint = (NodeViewpoint *)m_currentViewpoint->copy();
            m_currentViewpoint = m_defaultViewpoint;
        }
        if (m_selection->getNode() &&
            m_selection->getNode()->getType() == X3D_VIEWPORT) {
            m_viewports.remove(m_viewports.find((NodeViewport *)
                                                m_selection->getNode()));
        }
        Node *node = m_selection->getNode();
        Node *parent = m_selection->getParent();
        int parentField = m_selection->getParentField();

        if ((parent != NULL) && (parentField != -1)) {
            if (node->getNumParents() == 1) {
                BackupRoutesRec(node, list);
            }
            list->append(new MoveCommand(node, parent, parentField, NULL, -1));
        }
    }
}

int
Scene::OnDragOver(Node *src, Node *srcParent, int src_field, 
                  Node *dest, int dest_field, int modifiers)
{
    int rc = 0;

    if (src && dest) {
        int destField = getDestField(src, dest, dest_field);
        if (destField >= 0) {
            if ((modifiers & SW_CONTROL) && (modifiers & SW_SHIFT)
                && dest != src && !dest->hasAncestor(src)) {
                rc = SW_DRAG_LINK;
            } else if (modifiers & SW_CONTROL) {
                rc = SW_DRAG_COPY;
            } else if (dest != src
                   && !dest->hasAncestor(src)
                   && dest->findChild(src, destField) == -1) {
                rc = SW_DRAG_MOVE;
            }
        }
    }
    return rc;
}

int
Scene::OnDrop(Node *src, Node *srcParent, int srcField, 
              Node *dest, int destField, int modifiers)
{
    int effect = OnDragOver(src, srcParent, srcField, dest, destField, modifiers);
    NodeUpdate *hint = NULL;
    if (src && dest) {
        if (destField == -1) destField = dest->findValidField(src);
        switch(effect) {
          case SW_DRAG_COPY:
            execute(new MoveCommand(src->copy(), NULL, -1, dest, destField));
            src->reInit();
            break;
          case SW_DRAG_MOVE:
            execute(new MoveCommand(src, srcParent, srcField, dest, destField));
            break;
          case SW_DRAG_LINK:
            execute(new MoveCommand(src, NULL, -1, dest, destField));
            hint = new NodeUpdate(src, NULL, 0);
            UpdateViews(NULL, UPDATE_NODE_NAME, (Hint*) hint);
            break;
        }
        return 1;
    } else {
        return 0;
    }
}

struct HttpFile {
  const char *filename;
  FILE *stream;
};

static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
  struct HttpFile *out = (struct HttpFile *)stream;
  if(out && !out->stream) {
    /* open file for writing */
    out->stream = fopen(out->filename, "wb");
    if(!out->stream)
      return -1; /* failure, can't open file to write */
  }
  return fwrite(buffer, size, nmemb, out->stream);
}

class DownloadPathData {
public:
    MyString string;
    bool isRemote;
};

MyString
Scene::downloadPath(const URL &url)
{
     DownloadPathData data = downloadPathIntern(url);
     return data.string;
}

DownloadPathData
Scene::downloadPathIntern(const URL &url)
{
    DownloadPathData ret; 
    ret.string = "";
    ret.isRemote = false;
    if ((strcasecmp(url.getScheme(), "https") == 0) ||
        (strcasecmp(url.getScheme(), "http") == 0) ||
        (strcasecmp(url.getScheme(), "ftp") == 0)) {
        ret.isRemote = true;

        const char *myPath = TheApp->getDownloadDirectory();
        myPath = replaceHome(myPath);
        ret.string += myPath;
        free((void *)myPath);
        ret.string += swGetPathSeperator();
        ret.string += url.getHostname();
        ret.string += swGetPathSeperator();
        ret.string += '/';
        ret.string += url.ToPath();
        if (TheApp->getVerbose())
            swDebugf("%s\n", (const char *)ret.string);
        mkdir_parents4file(ret.string);
        setPath(ret.string);
        return ret;
    } else if (strcasecmp(url.getScheme(), "file") == 0) {
        ret.string += url.ToPath();
        return ret;
    }
    ret.string += url.ToPath();
    return ret;
}

bool
Scene::Download(const URL &url, MyString *path)
{
#ifdef HAVE_LIBCURL
    DownloadPathData data = downloadPathIntern(url);

    if (data.isRemote) {
        MyString filename = "";
        filename += data.string;
        CURL *curl;
        CURLcode res;
        struct HttpFile httpfile = {
          filename, /* name to store the file as if successful */
          NULL
        };
    
        curl_global_init(CURL_GLOBAL_DEFAULT);
    
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, (const char *)url);
            // Define our callback to get called when there's data to be written
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
            // Set a pointer to our struct to pass to the callback 
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &httpfile);
            // disable decompession
            curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "identity");
            curl_easy_setopt(curl, CURLOPT_FAILONERROR, true);
    
    #ifdef DEBUG
            /* Switch on full protocol/debug output */
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    #endif
      
            res = curl_easy_perform(curl);
      
            /* always cleanup */
            curl_easy_cleanup(curl);
      
            if (CURLE_OK != res) {
                /* we failed */
                fprintf(stderr, "curl: %s\n",  curl_easy_strerror(res));
            } else
                m_downloaded = true;
        } else
            return false; 
      
        if (httpfile.stream)
            fclose(httpfile.stream); /* close the local file */
      
        curl_global_cleanup();
                
        *path = filename;
    } else
#endif
        *path = data.string;

    return true;
}

FontInfo *
Scene::LoadGLFont(const char *fontName, const char *style)
{
    int styleId;

    // handle "special" font names
    if (!strcmp(fontName, "SERIF")) {
        fontName = "Times New Roman";
    } else if (!strcmp(fontName, "SANS")) {
        fontName = "Arial";
    } else if (!strcmp(fontName, "TYPEWRITER")) {
        fontName = "Courier New";
    }
    
    if (!strcmp(style, "BOLD")) {
        styleId = SW_BOLD;
    } else if (!strcmp(style, "ITALIC")) {
        styleId = SW_ITALIC;
    } else if (!strcmp(style, "BOLDITALIC")) {
        styleId = SW_BOLD | SW_ITALIC;
    } else {
        styleId = SW_PLAIN;
    }

    // look for font in cache

    for (int i = 0; i < m_fonts.size(); i++) {
        if (!strcmp(m_fonts[i]->name, fontName) && m_fonts[i]->style == styleId) {
            return m_fonts[i];
        }
    }

    // create some font outlines

    FontInfo *info = new FontInfo();

    info->displayListBase = 
          swLoadGLFont(fontName, styleId, info->kernX, info->kernY);

    if (info->displayListBase == 0) {
        delete info;
        return NULL;
    } else {
        info->name = fontName;
        info->style = styleId;
        m_fonts.append(info);
        return info;
    }
}

bool                
Scene::addProtoName(MyString name)
{
   if (belongsToNodeWithExternProto(name))           
       return true;
   for (int i = 0; i < m_numProtoNames; i++)
       if (m_protos[m_protoNames[i]] && name == m_protoNames[i]) {
           if (m_protos[m_protoNames[i]]->isExternProto())
               return true;  
           return false;
       }
   m_protoNames[m_numProtoNames++] = name;
   return true;
}

void
Scene::deleteProtoName(MyString name)
{
   for (int i = 0; i < m_numProtoNames; i++)
       if (name == m_protoNames[i]) {
           m_protoNames.remove(i);
           m_numProtoNames--;
       }
}

void
Scene::addProtoDefinition(void)
{
   m_protoDefinitions[m_numProtoDefinitions++] = "";
   m_isNestedProto[m_numProtoDefinitions - 1] = false;
}

void 
Scene::addToProtoDefinition(const char* string)
{
    if (m_numProtoDefinitions > 0)
        m_protoDefinitions[m_numProtoDefinitions - 1] += string;
}

bool
Scene::isNestedProto(const char *protoName)
{
    for (int i = 0; i < m_numProtoDefinitions; i++) 
        if (i < getNumProtos())
            if (strcmp(m_protoNames[i], protoName) == 0)
                if (m_isNestedProto[i])
                    return true;
    return false;
}       

void
Scene::setNestedProto(void)
{
    if (m_numProtoDefinitions > 0)
        m_isNestedProto[m_numProtoDefinitions - 1] = true;
}

MyString            
Scene::createRouteString(const char *fromNodeName, const char *fromFieldName,
                         const char *toNodeName, const char *toFieldName)
{
    MyString route = "";
    if (::isX3dXml(m_writeFlags)) {
        route += "<ROUTE fromNode='";
        route += fromNodeName;
        route += "' fromField='";
        route += fromFieldName;
        route += "' toNode='";
        route += toNodeName;
        route += "' toField='";
        route += toFieldName;
        route += "'";
        if (m_writeFlags & X3DOM) {
            // X3DOM do not support <ROUTE /> 
            route += ">";
            route += "</ROUTE>";
        } else 
           route += "/>";
    } else {
        route += "ROUTE ";
        route += fromNodeName;
        route += ".";
        route += fromFieldName;
        route += " TO ";
        route += toNodeName;
        route += ".";
        route += toFieldName;
    }
    return route;
}


void 
Scene::addRouteString(MyString string)
{
   // do not store double route strings
   for (List<MyString>::Iterator* routepointer = m_routeList.first();
        routepointer != NULL; routepointer = routepointer->next() )
       if (strcmp(routepointer->item(),string) == 0)
           return;
      
   m_routeList.append(string);
}

void 
Scene::addEndRouteString(MyString string)
{
   m_endRouteList.append(string);
}

void                
Scene::setViewOfLastSelection(SceneView* view)
{ 
   m_viewOfLastSelection=view; 
   m_selection_is_in_scene=false;
}

SceneView* Scene::getViewOfLastSelection(void)
{ 
   return m_viewOfLastSelection; 
}

void Scene::copyLastSelection(void)
{
   if (!m_selection_is_in_scene) {
      // do copy in View
      SceneView* view = getViewOfLastSelection();
      if (view != NULL)
          view->CopyLastSelection();
   }
}

void Scene::pasteLastSelection(void)
{
   if (!m_selection_is_in_scene) {
      // do paste in View
      SceneView* view = getViewOfLastSelection();
      if (view != NULL)
          view->PasteLastSelection();
   }
}

void Scene::pasteSymetricLastSelection(int direction)
{
   if (!m_selection_is_in_scene) {
      // do paste in View
      SceneView* view = getViewOfLastSelection();
      if (view != NULL)
          view->PasteSymetricLastSelection(direction);
   }
}

void Scene::deleteLastSelection(void)
{
   if (m_selection_is_in_scene) {
      // do delete in scene 
      deleteSelected();      
   } else {
      // do delete in View
      SceneView* view = getViewOfLastSelection();
      if (view != NULL)
          view->DeleteLastSelection();
   }
}

void Scene::saveProtoStatus(void)
{
   m_statusNumProtoNames=m_numProtoNames;
   m_statusNumProtoDefinitions=m_numProtoDefinitions;
}

void Scene::restoreProtoStatus(void)
{
   for (int i = m_statusNumProtoNames + 1; i < m_numProtoNames; i++)
       m_protoNames.remove(i);
   m_numProtoNames=m_statusNumProtoNames;
   for (int i = m_statusNumProtoDefinitions + 1; i < m_numProtoDefinitions; i++)
       m_protoDefinitions.remove(i);
   m_numProtoDefinitions=m_statusNumProtoDefinitions;
}

StringArray *
Scene::getAllNodeNames(void)
{
    StringArray *ret = new StringArray();
    ProtoMap::Chain::Iterator *j;
    for (int i = 0; i < m_protos.width(); i++) {
        for ( j = m_protos.chain(i).first(); j != NULL; j = j->next()) {
            ret->append(j->item()->getKey());
        }
    }
    return ret;
}

bool
Scene::use3dCursor(void) 
{ 
    switch (TheApp->Get3dCursorMode()) {
      case CM_3DCURSOR_ALWAYS:
         return m_use3dCursor;
      case CM_3DCURSOR_RECORDING:
        if (isRecording())
            return m_use3dCursor; 
        break;
      case CM_3DCURSOR_NOT_RUN:
        if (!isRunning())
            return m_use3dCursor; 
        break;
      case CM_3DCURSOR_NONE:
          return false;
    }
    return false;            
}

// the proto PREFIX is also needed for the illegal2vrml program
// undefined nodes named "something" are renamed to "PREFIXsomething"
// when the proto "PREFIXsomething" is defined

bool
Scene::setPrefix(char* prefix) 
{ 
    if (prefix != NULL)
       TheApp->setPrefix(prefix); 
    else {
       // compare all protonames to find out a common prefix
       bool prefixFound = false;
       if (getNumProtoNames() > 1) {
           MyString prefix = "";
           for (int numChar = 0; numChar < m_protoNames[0].length(); 
               numChar++) {
               char character = m_protoNames[0][numChar];
               bool sameCharacter = true; 
               for (int i = 1; i < getNumProtoNames(); i++) {
                   if ((numChar >= m_protoNames[i].length()) ||
                       (m_protoNames[i][numChar] != character)) {
                       sameCharacter = false;
                       break;
                   }
               }
               if (sameCharacter) {
                   prefixFound = true;
                   prefix += character;
               } else 
                   break;
           }
           if (prefixFound) 
               TheApp->setPrefix(mystrdup(prefix));
       } 
       if (!prefixFound) { 
           errorf("can not find out prefix from only one node\n");
           errorf("prefix missing, use \"-prefix\" in commandline\n");
           return false;
       }
    }
    return true;
}

MyString
Scene::getNodeWithPrefix(const MyString &nodeType)
{
    MyString newNodeType = "";
    newNodeType += TheApp->getPrefix();
    newNodeType += nodeType;
    return newNodeType;
}

void
Scene::setPathAllURL(const char *path)
{
    const NodeList *nodes = getNodes();
    for (int i = 0; i < nodes->size(); i++) {
        Node *node = nodes->get(i);
        if (node->isInScene(this))
            for (int j = 0; j < node->getProto()->getNumFields(); j++) {
                Field *field = node->getProto()->getField(j);
                if ((field->getType() == MFSTRING) &&
                    ((field->getFlags() & FF_URL) != 0)) {
                    MFString* urls = (MFString *)node->getField(j);
                    for (int k = 0; k < urls->getSize(); k++) {
                        const char *urlk =  urls->getValue(k);
                        if (!isSortOfEcmascript(urlk) && notURN(urlk)) {
                            URL url(getURL(), urlk);
                            MyString *newURL = new MyString("");
                            if (strlen(path) != 0) {
                                *newURL += path;
                                *newURL += "/";
                            }
                            *newURL += url.GetFileName();
                            urls->setValue(k, *newURL);
                        }
                    }
                } 
            }
    }
}

Node *
Scene::replaceNode(Node *oldNode, Node* newNode)
{
    if (newNode == NULL)
        return NULL;
    if (oldNode->getParent() == NULL)
        return NULL;
    int numParents = oldNode->getNumParents();
    for (int i = 0; i <= numParents; i++) {
        Node *parent = oldNode->getParent(i);
        int field = oldNode->getParentField(i);
        if ((parent != NULL) && (field != -1)) {
            execute(new MoveCommand(oldNode, parent, field, NULL, -1));
            execute(new MoveCommand(newNode, NULL, -1, parent, field));
        }
    }
    if (numParents == 0)
        return NULL;
    return newNode;
}

Node *
Scene::convertToIndexedFaceSet(Node* node)
{
    if (!node->canConvertToIndexedFaceSet())
        return NULL;

    NodeIndexedFaceSet *meshNode = (NodeIndexedFaceSet *)
                                   node->toIndexedFaceSet();

    return replaceNode(node, meshNode);
}

int
Scene::writeIndexedFaceSet(int f, Node* node)
{
    NodeIndexedFaceSet *meshNode = (NodeIndexedFaceSet *)
                                   node->toIndexedFaceSet();

    meshNode->setVariableName(strdup(node->getVariableName()));
    int ret = meshNode->write(f, m_writeFlags);
    meshNode->unref();
    return ret;
}

Node *
Scene::convertToIndexedLineSet(Node* node)
{
    if (!node->canConvertToIndexedLineSet())
        return NULL;

    NodeIndexedLineSet *chainNode = (NodeIndexedLineSet *)
                                    node->toIndexedLineSet();
    return replaceNode(node, chainNode);
}

void
Scene::recalibrate(void)
{
    TheApp->calibrateInputDevices();
    if (m_viewpoints.size() > 0) {
        m_currentViewpoint = (NodeViewpoint *) m_viewpoints[0];
    } else {
        m_currentViewpoint = m_defaultViewpoint;
    }    
    applyCamera();
    UpdateViews(NULL, UPDATE_TIME);
}

void 
Scene::makeSimilarName(Node *node, const char *name)
{
    char* reducedName = mystrdup(name);
    int i;
    // strip _ number (e.g. _0 or _12) constructs at end
    for (i = strlen(name) - 1; i > 0; i--)
        if ((name[i] == '_') || ((name[i] >= '0') && (name[i] <= '9'))) {
            if (name[i] == '_')
                reducedName[i] = 0;
        } else
            break;
    int len = strlen(name) + 512;
    char* buf = (char*) malloc(len);
    while (true) {
        mysnprintf(buf, len, "%s_%d", reducedName, i++);
        if (!hasAlreadyName(buf)) {
            MyString* newName = new MyString(buf);
            def(*newName, node);
            break;
        }
    }
    free(buf);
    free(reducedName);
}

ProtoArray *
Scene::getInteractiveProtos(int type)
{
    switch(type)
      {
        case SFBOOL:
          return &m_interactiveSFBoolProtos;
        case SFROTATION:
          return &m_interactiveSFRotationProtos;
        case SFTIME:
          return &m_interactiveSFTimeProtos;
        case SFVEC3F:
          return &m_interactiveSFVec3fProtos;
      }
    return &m_emptyProtoArray;
}        

void
Scene::buildInteractiveProtos(void)
{
    int i = 0;
    m_interactiveSFBoolProtos[i++] = m_protos["CylinderSensor"];   
    m_interactiveSFBoolProtos[i++] = m_protos["PlaneSensor"];   
    m_interactiveSFBoolProtos[i++] = m_protos["ProximitySensor"];   
    m_interactiveSFBoolProtos[i++] = m_protos["SphereSensor"];   
    m_interactiveSFBoolProtos[i++] = m_protos["TouchSensor"];   
    m_interactiveSFBoolProtos[i++] = m_protos["VisibilitySensor"];   
    i = 0;
    m_interactiveSFRotationProtos[i++] = m_protos["CylinderSensor"];
    m_interactiveSFRotationProtos[i++] = m_protos["ProximitySensor"];
    m_interactiveSFRotationProtos[i++] = m_protos["SphereSensor"];
    if (TheApp->getCoverMode()) {
        m_interactiveSFRotationProtos[i++] = m_protos["COVER"];
        m_interactiveSFRotationProtos[i++] = m_protos["SpaceSensor"];
        m_interactiveSFRotationProtos[i++] = m_protos["ARSensor"];
    }
    i = 0;
    m_interactiveSFTimeProtos[i++] = m_protos["Collision"];
    m_interactiveSFTimeProtos[i++] = m_protos["ProximitySensor"];
    m_interactiveSFTimeProtos[i++] = m_protos["TouchSensor"];
    m_interactiveSFTimeProtos[i++] = m_protos["VisibilitySensor"];
    i = 0;
    m_interactiveSFVec3fProtos[i++] = m_protos["CylinderSensor"];
    m_interactiveSFVec3fProtos[i++] = m_protos["PlaneSensor"];
    m_interactiveSFVec3fProtos[i++] = m_protos["ProximitySensor"];
    m_interactiveSFVec3fProtos[i++] = m_protos["SphereSensor"];
    m_interactiveSFVec3fProtos[i++] = m_protos["TouchSensor"];
    if (TheApp->getCoverMode()) {
        m_interactiveSFVec3fProtos[i++] = m_protos["COVER"];
        m_interactiveSFVec3fProtos[i++] = m_protos["SpaceSensor"];
        m_interactiveSFVec3fProtos[i++] = m_protos["ARSensor"];
    }
}

void
Scene::setHeadlight(int headlight)
{ 
    // only use headlight of first NavigationInfo
    if (m_headlightIsSet == false) {
        m_headlight = headlight;
        m_headlightIsSet = true;
    }
}

int
Scene::getConstrain(void)
{
    int ret = CONSTRAIN_NONE;
    if (!m_xOnly && !m_yOnly && !m_zOnly)
        ret = CONSTRAIN_NONE; 
    else if (m_xOnly && !m_yOnly && !m_zOnly)
        ret = CONSTRAIN_X; 
    else if (!m_xOnly && m_yOnly && !m_zOnly)
        ret = CONSTRAIN_Y; 
    else if (!m_xOnly && !m_yOnly && m_zOnly)
        ret = CONSTRAIN_Z; 
    else if (m_xOnly && m_yOnly && !m_zOnly)
        ret = CONSTRAIN_XY; 
    else if (m_xOnly && !m_yOnly && m_zOnly)
        ret = CONSTRAIN_ZX; 
    else if (!m_xOnly && m_yOnly && m_zOnly)
        ret = CONSTRAIN_YZ; 
    return ret;
}

Vec3f 
Scene::constrainVec(Vec3f vec) 
{
    Vec3f v(vec);    
    if ((!m_xOnly) && (m_yOnly || m_zOnly))
        v.x = 0;
    if ((!m_yOnly) && (m_xOnly || m_zOnly))
        v.y=0;
    if ((!m_zOnly) && (m_xOnly || m_yOnly))
        v.z=0;
    return v;
}

void
Scene::setX3d(void)
{
    m_writeFlags = m_writeFlags & ~(CONVERT2VRML); 
    if ((!::isX3d(m_writeFlags)) && (m_root != NULL)) {
        getNodes()->clearFlag(NODE_FLAG_CONVERTED);
        m_writeFlags |= CONVERT2X3D;
        for (int i = 0; i < getNumProtos(); i++)
            for (int j = 0; j < getProto(i)->getNumNodes(); j++)
                getProto(i)->getNode(j)->convert2X3d();
        m_root->convert2X3d();
    }
}

void
Scene::setX3dv(void) 
{
    setX3d();
    if (!(m_writeFlags & PURE_X3DV))
        m_writeFlags |= X3DV; 
}

void
Scene::setX3dXml(void) 
{
    if (!(m_writeFlags & X3D_XML)) {
        m_writeFlags |= X3D_XML; 
    }
    setX3d();
}

void
Scene::setVrml(void) 
{
    m_writeFlags = m_writeFlags & ~(CONVERT2X3D); 
    if (::isX3d(m_writeFlags) && (m_root != NULL)) {
        getNodes()->clearFlag(NODE_FLAG_CONVERTED);
        m_writeFlags |= CONVERT2VRML;
        m_writeFlags = m_writeFlags & ~(X3DV | X3D_XML);
        for (int i = 0; i < getNumProtos(); i++)
            for (int j = 0; j < getProto(i)->getNumNodes(); j++)
                getProto(i)->getNode(j)->convert2Vrml();
        m_root->convert2Vrml();    
    }
    m_writeFlags = m_writeFlags & ~(X3DV | X3D_XML);
}

void
Scene::convertProtos2X3d(void) {
    ProtoMap::Chain::Iterator *j;
    for (int i = 0; i < m_protos.width(); i++)
        for (j = m_protos.chain(i).first(); j != NULL; j = j->next())
            j->item()->getData()->convert2X3d();
}

void
Scene::convertProtos2Vrml(void) {
    ProtoMap::Chain::Iterator *j;
    for (int i = 0; i < m_protos.width(); i++)
        for (j = m_protos.chain(i).first(); j != NULL; j = j->next())
            j->item()->getData()->convert2Vrml();
}

bool
Scene::isInvalidElement(Element *element)
{
    bool x3d = isX3d();
    return !isValidElement(element, x3d);
}

bool
Scene::isValidElement(Element *element, bool x3d)
{
    if (element == NULL)
        return false;
    int flags = element->getFlags();
    if (flags & FF_ALWAYS)
        return true;
    bool invalidX3d = (flags & FF_X3D_ONLY) && (!x3d);
    bool invalidKambi = (flags & FF_KAMBI_ONLY) && (!TheApp->getKambiMode());
    bool invalidX3dom = (flags & FF_X3DOM_ONLY) && (!TheApp->getX3domMode());
    bool invalidX3dKambi = (flags & (FF_X3D_ONLY | FF_KAMBI_ONLY)) &&
                           ((!x3d) && (!TheApp->getKambiMode()));
    if (
        (flags & FF_NEVER) ||
        (invalidX3d && invalidX3dKambi) ||
        (invalidX3dom) ||
        ((flags & FF_VRML_ONLY) && x3d) ||
        (invalidKambi && invalidX3dKambi) ||
        ((flags & FF_COVER_ONLY) && (!TheApp->getCoverMode())) ||
        ((flags & FF_ROOT_ONLY) && (getSelection()->getNode() != getRoot()))
       )
        return false;
    return true;
}


void
Scene::resetWriteFlags(int flags)
{
    if (::isX3d(m_writeFlags) && (!::isX3d(flags)) && (m_root != NULL)) {
        m_writeFlags = flags & ~(CONVERT2X3D); 
        m_writeFlags |= CONVERT2VRML;
        m_root->clearFlagRec(NODE_FLAG_CONVERTED);    
        m_root->convert2Vrml();    
    }
    if ((!::isX3d(m_writeFlags)) && ::isX3d(flags) && (m_root != NULL)) {
        m_writeFlags = flags & ~(CONVERT2VRML); 
        m_writeFlags |= CONVERT2X3D;
        m_root->clearFlagRec(NODE_FLAG_CONVERTED);    
        m_root->convert2X3d();    
    }
    m_writeFlags = flags & ~(CONVERT2VRML | CONVERT2X3D); 
}

void
Scene::changeRoutes(Node *toNode, int toField,
                    Node *fromNode, int fromField)
{
    changeRoutes(toNode, toField, 0, fromNode, fromField, 0);
}

void
Scene::changeRoutes(Node *toNode, int toField, int toOffset,
                    Node *fromNode, int fromField, int fromOffset)
{
    SocketList::Iterator *i = NULL;
    CommandList *list = NULL;
    
    bool fromX3d = false;
    bool toX3d = false;

    Proto *fProto = fromNode->getProto();
    Field *fField = fProto->getField(fromField);
    int fEventIn = fProto->lookupEventIn(fField->getName(fromX3d), fromX3d);
    int fEventOut = fProto->lookupEventOut(fField->getName(fromX3d), fromX3d);

    Proto *tProto = toNode->getProto();
    Field *tField = tProto->getField(toField);
    int tEventIn = tProto->lookupEventIn(tField->getName(toX3d), toX3d);
    int tEventOut = tProto->lookupEventOut(tField->getName(toX3d), toX3d);

    if (fEventIn != -1)
        for (i = fromNode->getInput(fEventIn).first(); i != NULL; 
             i = i->next()) {
            if (list == NULL) 
                list = new CommandList();
            list->append(new UnRouteCommand(i->item().getNode(), 
                                            i->item().getField(),
                                            fromNode, 
                                            fEventIn + fromOffset));
            list->append(new RouteCommand(i->item().getNode(), 
                                          i->item().getField(),
                                          toNode, tEventIn + toOffset));
        }                                        
    if (fEventOut != -1)
        for (i = fromNode->getOutput(fEventOut).first(); i != NULL; 
             i = i->next()) {
            if (list == NULL) 
                list = new CommandList();
            list->append(new UnRouteCommand(fromNode, fEventOut + fromOffset,
                                            i->item().getNode(), 
                                            i->item().getField()));
                                            
            list->append(new RouteCommand(toNode, tEventOut + toOffset,
                                          i->item().getNode(), 
                                          i->item().getField()));
        }
    if (list) execute(list);
}

void                
Scene::copyRoutes(Node *toNode, Node *fromNode)
{
    int j = -1;
    SocketList::Iterator *i = NULL;
    CommandList *list = NULL;
    
    Proto *fromProto = fromNode->getProto();

    for (j = 0; j < fromProto->getNumEventIns(); j++)
        for (i = fromNode->getInput(j).first(); i != NULL; 
             i = i->next()) {
            if (list == NULL) 
                list = new CommandList();
            list->append(new RouteCommand(i->item().getNode(), 
                                          i->item().getField(),
                                          toNode, j));
        }                                        
    for (j = 0; j < fromProto->getNumEventOuts(); j++)
        for (i = fromNode->getOutput(j).first(); i != NULL; 
             i = i->next()) {
            if (list == NULL) 
                list = new CommandList();
            list->append(new RouteCommand(toNode, j,
                                          i->item().getNode(), 
                                          i->item().getField()));
        }
    if (list) execute(list);
}


static bool searchBindableNodes(Node *node, void *data)
{
    Scene *scene = (Scene *)data;
    if (node->getType() == VRML_VIEWPOINT)
        scene->addViewpoint(node);
    if (node->getType() == VRML_GEO_VIEWPOINT)
        scene->addViewpoint(node);
    else if (node->getType() == X3D_ORTHO_VIEWPOINT)
        scene->addViewpoint(node);
    else if (node->getType() == VRML_NAVIGATION_INFO)
        scene->addNavigationInfo(node);
    else if (node->getType() == VRML_BACKGROUND)
        scene->addBackground(node);
    else if (node->getType() == VRML_FOG)
        scene->addFog(node);
    return true;     
}

void
Scene::findBindableNodes(void)
{
    if (m_root != NULL)
        m_root->doWithBranch(searchBindableNodes, this);
    setFirstNavigationInfo();
}

static bool convertToTriangleSet(Node *node, void *data)
{
    Scene *scene = (Scene *)data;
    if (node->canConvertToTriangleSet())
        scene->convertToTriangleSet(node);
    return true;     
}

Node *
Scene::convertToTriangleSet(Node* node)
{
    if (!node->canConvertToTriangleSet())
        return NULL;

    NodeTriangleSet *triangleNode = (NodeTriangleSet *)node->toTriangleSet();

    return replaceNode(node, triangleNode);
}

void Scene::branchConvertToTriangleSet(Node *node)
{
    node->doWithBranch(::convertToTriangleSet, this);
}

Node *
Scene::convertToIndexedTriangleSet(Node* node)
{
    if (!node->canConvertToIndexedTriangleSet())
        return NULL;

    NodeIndexedTriangleSet *meshNode = (NodeIndexedTriangleSet *)
                                       node->toIndexedTriangleSet();

    return replaceNode(node, meshNode);
}

static bool convertToIndexedTriangleSet(Node *node, void *data)
{
    Scene *scene = (Scene *)data;
    if (node->canConvertToIndexedTriangleSet())
        scene->convertToIndexedTriangleSet(node);
    return true;     
}

void Scene::branchConvertToIndexedTriangleSet(Node *node)
{
    node->doWithBranch(::convertToIndexedTriangleSet, this);
}

static bool convertToIndexedFaceSet(Node *node, void *data)
{
    Scene *scene = (Scene *)data;
    if (node->canConvertToIndexedFaceSet())
        scene->convertToIndexedFaceSet(node);
    return true;     
}

void Scene::branchConvertToIndexedFaceSet(Node *node)
{
    node->doWithBranch(::convertToIndexedFaceSet, this);
}

static bool createNormals(Node *node, void *data)
{
    int normalField = node->getNormalField();
    if (normalField == -1)
        return true; 
    // ignore Nodes with already set normal field
    if (((SFNode *)node->getField(normalField))->getValue() == NULL) {
        Node *nnormal = node->getScene()->createNode("Normal");
        if (nnormal != NULL) {
            MoveCommand *command = new MoveCommand(nnormal, NULL, -1,
                                                   node, normalField);
            command->execute();
            node->setNormalFromMesh(nnormal);
        }
    }
    return true;     
}

void Scene::branchCreateNormals(Node *node)
{
    node->doWithBranch(createNormals, NULL);
}

void Scene::createNormal(Node *node)
{
    ::createNormals(node, NULL);
}

static bool createTextureCoordinates(Node *node, void *data)
{
    int texCoordField = node->getTexCoordField();
    if (texCoordField == -1)
        return true; 
    // ignore Nodes with already set texCoord field
    if (((SFNode *)node->getField(texCoordField))->getValue() == NULL) {
        Node *ntexCoord = node->getScene()->createNode("TextureCoordinate");
        if (ntexCoord != NULL) {
            MoveCommand *command = new MoveCommand(ntexCoord, NULL, -1,
                                                   node, texCoordField); 
            command->execute();
            node->setTexCoordFromMesh(ntexCoord);
        }
    }
    return true;     
}

void
Scene::branchCreateTextureCoordinates(Node *node) 
{
    node->doWithBranch(createTextureCoordinates, NULL);
}

void 
Scene::createTextureCoordinate(Node *node)
{
    ::createTextureCoordinates(node, NULL);
}

bool
Scene::checkXSymetricOrSameHandle(int handle, MFVec3f *points)
{
    if ((handle >= points->getSFSize()) || (handle < 0))
        return true;
    Vec3f vIndex = points->getValue(handle);
    float epsilon = TheApp->GetHandleEpsilon();
    for (int i = 0; i < getSelectedHandlesSize(); i++) {
        int checkedHandle = getSelectedHandle(i);
        if ((checkedHandle >= points->getSFSize()) || (checkedHandle < 0))
            continue;
        if (checkedHandle != handle) {
            Vec3f vPoint = points->getValue(checkedHandle);
            if (   (fabs(vPoint.z - vIndex.z) < epsilon) 
                && (fabs(vPoint.y - vIndex.y) < epsilon)) {
                if (fabs(vPoint.x - vIndex.x) < epsilon) {
                    if (isSingleSelectedHandle()) 
                        return true;
                } else 
                    if (fabs(vPoint.x + vIndex.x) < epsilon)
                        return true;         
            }                 
        }
    }
    return false;
}

void
Scene::addMeta(const char *metaKey, const char* metaValue)
{
    m_metaKeys.append(metaKey);
    m_metaValues.append(metaValue);
}

void
Scene::addUnit(const char *category, const char *name,
               double conversionFactor)
{
    int found = -1;
    for (int i = 0; i < m_unitCategory.size(); i++)
        if (strcmp(m_unitCategory[i], category) == 0) {
            found = i;
            break;
        }
    if (found > -1) {
        m_unitName[found] = name;
        m_unitConversionFactor[found] = conversionFactor;
    } else {
        m_unitCategory.append(category);
        m_unitName.append(name);
        m_unitConversionFactor.append(conversionFactor);
    }
    setUnitLengthInit();
    setUnitAngleInit();
    SceneProtoMap::updateProtoMap(&m_protos, this);
}

void
Scene::setUnitLength(double f)
{
    m_unitLength = f;
}

void
Scene::setUnitLengthInit(void)
{
    for (int i = 0; i < m_unitCategory.size(); i++)
        if (strcmp(m_unitCategory[i], "length") == 0)
            m_unitLength = m_unitConversionFactor[i];
}

void
Scene::setUnitAngleInit(void)
{
    for (int i = 0; i < m_unitCategory.size(); i++)
        if (strcmp(m_unitCategory[i], "angle") == 0)
            m_unitAngle = m_unitConversionFactor[i];
}

void                
Scene::setUnitAngle(double f)
{
    m_unitAngle = f;
//    updateViewpoint();
}

void                
Scene::pushUnitLength(void)
{
   m_unitLengthStack.push(getUnitLength());
}

void                
Scene::popUnitLength(void)
{
   if (m_unitLengthStack.empty())
       setUnitLength(1);
   else
       setUnitLength(m_unitLengthStack.pop());
}

void                
Scene::pushUnitAngle(void)
{
   m_unitAngleStack.push(getUnitAngle());
}

void                
Scene::popUnitAngle(void)
{
   if (m_unitAngleStack.empty())
       setUnitAngle(1);
   else
       setUnitAngle(m_unitAngleStack.pop());
}

void
Scene::addToSensorNodes(Node *node)
{
    for (int i = 0; i < m_sensorNodes.size(); i++)
        if (m_sensorNodes.get(i) == node)
            return;
    m_sensorNodes.append(node);
}

void
Scene::restoreSelectedHandles(void) 
{    
    m_selectedHandles.resize(0); 
    if (m_oldSelectedHandles.size() > 0) {
        for (int i = 0; i < m_oldSelectedHandles.size(); i++)
            m_selectedHandles.append(m_oldSelectedHandles[i]);
        m_lastSelectedHandle = m_oldLastSelectedHandle;
        m_singleSelectedHandle = m_selectedHandles.size() <= 1;
    } else {
        m_lastSelectedHandle = -1;
        m_singleSelectedHandle = true;
    }
    m_isNewSelectedHandle = false;
    setSelection(m_oldSelection->getPath());
    UpdateViews(NULL, UPDATE_SELECTION);
}

int 
Scene::getX3dVersion(void) 
{
    int ret = m_x3dVersion; 
    const NodeList *nodes = getNodes();
    for (int i = 0; i < nodes->size(); i++) {
         Node *node = nodes->get(i);
         if (node->getX3dVersion() > ret)
             ret = node->getX3dVersion();
    }

    if (scene != NULL) {
        scene->setParsedX3dVersion(ret);
        scene->updateSceneMap();
    }

    return ret;
}

static Node *returnNode;

static bool searchNodeById(Node *node, void *data)
{
    int *id = (int *)data;
    if ((node != NULL) && (node->getId() == (*id))) {
        returnNode = node;
        return false;
    }
    return true;     
}


Node *
Scene::searchProtoNodeId(long id)
{
    returnNode = NULL;
    for (int i = 0; i < m_nodes.size(); i++) 
        if (m_nodes.get(i)->isPROTO()) {
            NodePROTO *protoNode = (NodePROTO *)m_nodes.get(i);
            for (int j = 0; j < protoNode->getNumProtoNodes(); j++) {
                Node *rootNode = protoNode->getProtoNode(j);
                rootNode->doWithBranch(searchNodeById, &id);
            }
        }
    return returnNode;
}

int 
Scene::getProtoType(Proto *proto)
{
    int lastNodeType = LAST_NODE;
    ProtoMap::Chain::Iterator *j;
    for (int i = 0; i < m_protos.width(); i++)
        for (j = m_protos.chain(i).first(); j != NULL; j = j->next()) {
            Proto *jproto = j->item()->getData();
            lastNodeType++;
            if (jproto == proto) {
                return lastNodeType;
            }        
        }
    return -1;
}

void                
Scene::setSelectionMode(int mode)
{
    int oldMode = getSelectionMode();
    m_selectionMode = mode; 
    if (oldMode != mode) {
        Node *node = getSelection()->getNode();
        NodeIndexedFaceSet *faceSet = NULL;
        MyMesh *mesh = NULL;
        MFInt32 *ci = NULL;
        if (node->hasParent())
            if (node->getParent()->getType() == VRML_INDEXED_FACE_SET) {
                faceSet = (NodeIndexedFaceSet *)node->getParent();
                mesh = faceSet->getMesh();
                ci = mesh->getCoordIndex();
            }
        if (faceSet == NULL)
            return;
        MyArray<int> oldSelection;
        for (int i = 0; i < getSelectedHandlesSize(); i++)
             oldSelection.append(getSelectedHandle(i));
        removeSelectedHandles();
    
        if ((oldMode == SELECTION_MODE_FACES) && 
            (mode == SELECTION_MODE_VERTICES)) {
            for (int i = 0; i < oldSelection.size(); i++) {
                int handle = oldSelection[i]; 
                for (int j = 0; j < mesh->getNumFaces(); j++) {
                    FaceData *face = mesh->getFace(j);
                    if (handle == j) {
                        int offset = face->getOffset();
                        for (int n = offset; n < offset + 
                                                 face->getNumVertices(); n++)
                            addSelectedHandle(ci->getValue(n));
                    }

                }
            }
            UpdateViews(NULL, UPDATE_REDRAW_3D, NULL);
        } else if ((oldMode == SELECTION_MODE_VERTICES) && 
                   (mode == SELECTION_MODE_FACES)) {
            for (int j = 0; j < mesh->getNumFaces(); j++) {
                FaceData *face = mesh->getFace(j);
                int offset = face->getOffset();
                int vertexCount = 0;
                for (int n = offset; n < offset + face->getNumVertices(); n++) {
                    for (int i = 0; i < oldSelection.size(); i++) {
                        int handle = oldSelection[i]; 
                        int meshVertex = ci->getValue(n);
                        if (meshVertex == handle)
                            vertexCount++;
                    }
                }
                if (vertexCount >= face->getNumVertices())
                    addSelectedHandle(j);
            }
            UpdateViews(NULL, UPDATE_REDRAW_3D, NULL);
        }
    }
}           


void
Scene::addSelectionRange(int firstRangeHandle, int secondRangeHandle)
{
     if ((firstRangeHandle > -1) && (m_selection)) {
         Node *node = m_selection->getNode();
         if (node == NULL)
             return;
         Vec3f first = node->getVertex(firstRangeHandle);
         Vec3f second = node->getVertex(secondRangeHandle);
         float len = (second - first).length();
         bool addedHandle = false;
         for (int i = 0; i < node->getNumVertex(); i++) { 
             if (i == firstRangeHandle)
                 continue;
             if (i == secondRangeHandle)
                 continue;
             if (node->validHandle(i) &&
                 (node->getVertex(i) - first).length() - len <=
                 TheApp->GetHandleEpsilon()) {
                 if (addSelectedHandle(i))
                     addedHandle = true;
             }
         }
         if (addedHandle) {
             UpdateViews(NULL, UPDATE_REDRAW_3D);
             m_firstSelectionRangeHandle = -1;
             m_isNewSelectedHandle = false;
         }        
    }
}

void
Scene::hideSelectedVertices(void)
{
     float epsilon = TheApp->GetHandleEpsilon();
     Node *node = getSelection()->getNode();
     for (int i = 0; i < getSelectedHandlesSize(); i++) {
         if (getXSymetricMode() && node->validHandle(getSelectedHandle(i))) {
             Vec3f vertex = node->getVertex(getSelectedHandle(i));
             for (int j = 0; j < node->getNumVertex(); j++) {
                 Vec3f vec = node->getVertex(j);
                 if ((fabsf(vertex.x + vec.x) < epsilon) &&
                     (fabsf(vertex.y - vec.y) < epsilon) &&
                     (fabsf(vertex.z - vec.z) < epsilon))
                     m_hiddenVertices.append(j);
            }
         }
         if (node->validHandle(getSelectedHandle(i))) {
             Vec3f vertex = node->getVertex(getSelectedHandle(i));
             for (int j = 0; j < node->getNumVertex(); j++) {
                 Vec3f vec = node->getVertex(j);
                 if ((fabsf(vertex.x - vec.x) < epsilon) &&
                     (fabsf(vertex.y - vec.y) < epsilon) &&
                     (fabsf(vertex.z - vec.z) < epsilon))
                     m_hiddenVertices.append(j);
            }
         }
         m_hiddenVertices.append(getSelectedHandle(i));
     }
}

void                
Scene::showOnlySelectedVertices(void)
{
     float epsilon = TheApp->GetHandleEpsilon();
     Node *node = getSelection()->getNode();
     IntArray notHiddenVertices;
     for (int i = 0; i < getSelectedHandlesSize(); i++) {
         if (getXSymetricMode() && node->validHandle(getSelectedHandle(i))) {
             Vec3f vertex = node->getVertex(getSelectedHandle(i));
             for (int j = 0; j < node->getNumVertex(); j++) {
                 Vec3f vec = node->getVertex(j);
                 if ((fabsf(vertex.x + vec.x) < epsilon) &&
                     (fabsf(vertex.y - vec.y) < epsilon) &&
                     (fabsf(vertex.z - vec.z) < epsilon))
                     notHiddenVertices.append(j);
            }
         }
         if (node->validHandle(getSelectedHandle(i))) {
             Vec3f vertex = node->getVertex(getSelectedHandle(i));
             for (int j = 0; j < node->getNumVertex(); j++) {
                 Vec3f vec = node->getVertex(j);
                 if ((fabsf(vertex.x - vec.x) < epsilon) &&
                     (fabsf(vertex.y - vec.y) < epsilon) &&
                     (fabsf(vertex.z - vec.z) < epsilon))
                     notHiddenVertices.append(j);
            }
         }
         notHiddenVertices.append(getSelectedHandle(i));
     }
     for (int i = 0; i < node->getNumVertex(); i++) {
         bool skip = false;
         for (int j = 0; j < notHiddenVertices.size(); j++)
             if (i == notHiddenVertices[j]) {
                 skip = true;
                 break;
             }
         if (!skip)
             m_hiddenVertices.append(i);
     }
}

void
Scene::warning(const char* string)
{
    int newIndex = m_warnings.size();
    m_warnings[newIndex] = "";
    m_warnings[newIndex] += string;  
    swDebugf("%s\n", string);
}

MyArray<Node *> *
Scene::getViewPorts() 
{ 
    return &m_viewports; 
}


void
Scene::warning(int id)
{
    char message[256];
    swLoadString(id, message, 255);
    warning(message);
}

void 
Scene::warning(int id, const char *string)
{
    char idText[256];
    static char text[256];
    swLoadString(id, idText, 255);
    mysnprintf(text, 255, idText, string);
    warning(text);
}

void                
Scene::addToStore4Convex_hull(void) 
{
    if (getSelectionMode() != SELECTION_MODE_VERTICES)
        return;
    Node *node = getSelection()->getNode();
    if (!node->getValidVertex())
        return;
    static Matrix transformMatrix;
    Path *trans = searchTransform();
    Node *transform = NULL;
    if (trans) 
        transform = trans->getNode();
    if (transform) {
        transform->transform();
        transform->getMatrix(transformMatrix);
    }
    float eps = TheApp->GetHandleEpsilon(); 
    for (int i = 0; i < getSelectedHandlesSize(); i++) {
         int handle = getSelectedHandle(i);
         if (handle >= NO_HANDLE)
             continue;
         Vec3f vertex = node->getVertex(handle);
         // also collect symetric handles
         if (getXSymetricMode()) 
             for (int j = 0; j < node->getNumVertex(); j++) {
                 if (j == handle)
                     continue;
                 Vec3f vec = node->getVertex(j);
                 if (vec.x != 0) {
                     if ((fabs(vertex.x + vec.x) < eps) &&
                         (fabs(vertex.y - vec.y) < eps) &&
                         (fabs(vertex.x - vec.z) < eps)) {
                         m_store4convex_hull.append(transformMatrix * vec);
                         break;
                     }   
                 }
             } 
         m_store4convex_hull.append(transformMatrix * vertex);
    }
}


FieldUpdate::FieldUpdate(Node *n, int f, int i)
{
    node = n; 
    field = f;
    index = i;
}

