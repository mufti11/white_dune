/*
 * SceneGraphView.cpp
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

#include "stdafx.h"

#include "SceneGraphView.h"
#include "Scene.h"
#include "Node.h"
#include "Proto.h"
#include "Path.h"
#include "FieldValue.h"
#include "Field.h"
#include "ExposedField.h"
#include "EventIn.h"
#include "EventOut.h"
#include "SFNode.h"
#include "MFNode.h"
#include "MoveCommand.h"
#include "RouteCommand.h"
#include "UnRouteCommand.h"
#include "CommandList.h"
#include "resource.h"
#include "Util.h"

#include "IconSize.h"

#define SOCKET_WIDTH 6
#define SOCKET_HEIGHT 6
#define SOCKET_SPACING 2

#define BORDER_WIDTH 2 
#define NODE_WIDTH 190

#define ADD_WIDTH 8
#define ADD_HEIGHT 4

#ifdef _WIN32
// near windows canvas limit 
# define MAX_X 8000
# define MAX_Y 8000
#else
// near motif canvas limit
# define MAX_X 32000
# define MAX_Y 32000
#endif

#define MINIMAL_SIZE (2 * BORDER_WIDTH + 1 + ICON_SIZE_X)   

// position from left of node to icon
#define ICON_OFFSET (BORDER_WIDTH + SOCKET_WIDTH + ICON_SIZE_X)   

// position from left of node to text
#define TEXT_OFFSET (ICON_OFFSET + ICON_SIZE_X + 4)   

#define LINE_STUB (SOCKET_WIDTH + BORDER_WIDTH + 2)

#define SHADOW_OFFSET 1

#define AUTOSCROLL_MARGIN 10
#define AUTOSCROLL_AMOUNT 20

static int currentX, currentY;

#define MoveTo(dc, x, y) { currentX = x; currentY = y; }
#define LineTo(dc, x, y) { swDrawLine(dc, currentX, currentY, x, y); \
                           currentX = x; currentY = y; }
#define InvalidateTo(wnd, x, y) { int minX = MIN(currentX, x); \
                                  int minY = MIN(currentY, y); \
                                  int maxX = MAX(currentX, x); \
                                  int maxY = MAX(currentY, y); \
swInvalidateRect(wnd, minX, minY, maxX - minX + 2, maxY - minY + 2); \
                 currentX = x; currentY = y; }

static void
expose(void *data, int x, int y, int width, int height)
{
    ((SceneGraphView *) data)->OnDraw(x, y, width, height);
}

static void
keyCB(void *data, int key, int value, int x, int y, int modifiers)
{
    if (key == SW_MOUSE1) {
        if (value) {
            ((SceneView *) data)->OnLButtonDown(x, y, modifiers);
        } else {
            ((SceneView *) data)->OnLButtonUp(x, y, modifiers);
        }
    } else if (value) {
        ((SceneView *) data)->OnKeyDown(key, x, y, modifiers);
    } else {
        ((SceneView *) data)->OnKeyUp(key, x, y, modifiers);
    }
}

static void
mouseCB(void *data, int x, int y, int modifiers)
{
    ((SceneView *) data)->OnMouseMove(x, y, modifiers);
}

static void
mouseEnterCB(void *data, int value)
{
    if (value) {
        ((SceneView *) data)->OnMouseEnter();
    } else {
        ((SceneView *) data)->OnMouseLeave();
    }
}

SceneGraphView::SceneGraphView(Scene *scene, SWND parent)
  : SceneView(scene, parent)
{
    int width = 1, height = 1;
    swGetSize(parent, &width, &height);

    m_lastXPosition = 0;
    m_lastYPosition = 0;
    m_maxYPosition = 0;
    m_mode = NONE;
    m_currentProto = NULL;
    m_dstNode = NULL;
    m_dstProto = NULL;
    m_dstSocket = -1;
    m_dstSide = 0;
    m_srcNode = NULL;
    m_srcProto = NULL;
    m_srcSocket = -1;
    m_srcSide = 0;
    m_scroller = swCreateScrolledWindow(0, 0, width, height, parent);
    m_window = swCreateCanvas("", 0, 0, width, height, m_scroller);
    swScrolledWindowSetChild(m_scroller, m_window);
    swSetExposeCallback(m_window, expose);
    swSetKeyCallback(m_window, keyCB);
    swSetMouseCallback(m_window, mouseCB);
    swSetEnterCallback(m_window, mouseEnterCB);
    swSetClientData(m_window, this);
    m_face = swGetWindowColor(m_window, SW_COLOR_FACE);
    m_highlight = swGetWindowColor(m_window, SW_COLOR_TSHADOW);
    m_shadow = swGetWindowColor(m_window, SW_COLOR_BSHADOW);
    int mapFrom[4] = { 0x808000, 0x000000, 0xC0C0C0, 0x808080 };
    int mapTo[4] = { m_face, m_shadow, m_highlight, m_face };
    int idb_node_icons = IDB_NODE_ICONS;
    m_nodeBitmap = swLoadMappedBitmap(m_window, idb_node_icons, mapFrom, mapTo, 1);
    m_nodeBitmaps = scene->getNumberBuildinProtos() + 4;
    m_socketBitmap = swLoadMappedBitmap(m_window, IDB_EMPTY_SOCKET, mapFrom, mapTo, 4); 
    m_socketBitmapRecommended = swLoadMappedBitmap(m_window, IDB_RECOMMENDED_SOCKET, mapFrom, mapTo, 4); 
    m_socketBitmapRecommendedWonderland = swLoadMappedBitmap(m_window, IDB_RECOMMENDED_WONDERLAND_SOCKET, mapFrom, mapTo, 4); 
    m_socketBitmapWonderland = swLoadMappedBitmap(m_window, IDB_WONDERLAND_SOCKET, mapFrom, mapTo, 4); 
    BuildSocketBitmaps();
    m_autoScrolling = false;
    m_zoom = 1.0;
    m_floodedWarning = false;

    m_XNodesOnlyEventOuts = ADD_WIDTH;
    m_YNodesOnlyEventOuts = ADD_HEIGHT;

    m_XNodesBothEvents = NODE_WIDTH + 2 * ADD_WIDTH;  
    m_YNodesBothEvents = ADD_HEIGHT;
    m_XNodesOnlyEventIns = 2 * NODE_WIDTH + 3 * ADD_WIDTH;  
    m_YNodesOnlyEventIns = ADD_HEIGHT;
}

SceneGraphView::~SceneGraphView()
{
    for (int i = 0; i < 20; i++) {
        swDestroyBitmap(m_socketBitmaps[i]);
    }
    swDestroyBitmap(m_nodeBitmap);
    swDestroyBitmap(m_socketBitmap);
    swDestroyWindow(m_window);
    swDestroyWindow(m_scroller);
}

bool collectNode(Node *node, void *data)
{
    MyArray<NodeOrProto> *nodes = (MyArray<NodeOrProto> *)data;
    NodeOrProto np(node);
    nodes->append(np); 
    return true;
}

void
SceneGraphView::InitializeNodes(bool isInsideProto)
{
    for (int i = 0; i < m_nodesOrProtos.size(); i++) {
        Node *node = m_nodesOrProtos.get(i).getNode();
        if (node && ((isInsideProto && node->isInsideProto()) ||
                     ((!isInsideProto) && (!node->isInsideProto()))))  {
            int width = 1, height = 1;
            getZoomGraphSize(width, height, node);
            if (node->hasOutputsOrIs() && !node->hasInputsOrIs()) {
                setZoomGraphPosition(node, 
                                     m_XNodesOnlyEventOuts, 
                                     m_YNodesOnlyEventOuts);
                m_YNodesOnlyEventOuts += height + ADD_HEIGHT;
                m_XNodesOnlyEventOuts += TheApp->GetRoutedNodeIndent();
                m_XNodesBothEvents += TheApp->GetRoutedNodeIndent();
                m_XNodesOnlyEventIns += TheApp->GetRoutedNodeIndent();
            } else if (node->hasOutputsOrIs() && node->hasInputsOrIs()) {
                setZoomGraphPosition(node, 
                                     m_XNodesBothEvents, 
                                     m_YNodesBothEvents);
                m_YNodesBothEvents += height + ADD_HEIGHT;
                m_XNodesBothEvents += TheApp->GetRoutedNodeIndent();
                m_XNodesOnlyEventIns += TheApp->GetRoutedNodeIndent();
            } else if (node->hasInputsOrIs() && !node->hasOutputsOrIs()) {
                setZoomGraphPosition(node, 
                                     m_XNodesOnlyEventIns,
                                     m_YNodesOnlyEventIns);
                m_XNodesOnlyEventIns += TheApp->GetRoutedNodeIndent();
                m_YNodesOnlyEventIns += height + ADD_HEIGHT;
            }
        }
    }
}

void 
SceneGraphView::Initialize()
{
    m_nodesOrProtos.resize(0);

    m_XNodesOnlyEventOuts = ADD_WIDTH;
    m_YNodesOnlyEventOuts = ADD_HEIGHT;

    m_XNodesBothEvents = NODE_WIDTH + 2 * ADD_WIDTH;  
    m_YNodesBothEvents = ADD_HEIGHT;

    m_XNodesOnlyEventIns = 2 * NODE_WIDTH + 4 * ADD_WIDTH;  
    m_YNodesOnlyEventIns = ADD_HEIGHT;

    m_lastXPosition = m_lastYPosition = 0;

    if (m_currentProto != NULL) {
        Proto *proto = m_currentProto;
        NodeOrProto np(proto);
        m_nodesOrProtos.append(np);
        if (proto) {
            setZoomGraphPosition(proto, 0, 0);
            accountGraphSize(proto);
        }
        for (int j = 0; j < proto->getNumNodes(); j++)
            proto->getNode(j)->doWithBranch(collectNode, (void *)
                                                         &m_nodesOrProtos);
    }

    const NodeList *nodes = m_scene->getNodes();

    for (int i = 0; i < nodes->size(); i++) {
        Node *node = nodes->get(i);
        if (node && (!node->isInsideProto()) && (node->isInScene(m_scene)) &&
            node != m_scene->getRoot()) {
            NodeOrProto np(node);
            m_nodesOrProtos.append(np);
        }
    }

    for (int i = 0; i < m_nodesOrProtos.size(); i++) {
        Node *node = m_nodesOrProtos.get(i).getNode();
        Proto *proto = m_nodesOrProtos.get(i).getProto();
        if (node) {
            setZoomGraphPosition(node, 0, 0);
            accountGraphSize(node);
        } else if (proto) {
            setZoomGraphPosition(proto, 0, 0);
            accountGraphSize(proto);
        } 
    }

    if (TheApp->GetShowRoutesAtBegin()) {
        for (int i = 0; i < m_nodesOrProtos.size(); i++) {
            Node *node = m_nodesOrProtos.get(i).getNode();
            Proto *proto = m_nodesOrProtos.get(i).getProto();
            if (proto && !node) {
                int width = 1, height = 1;
                getZoomGraphSize(width, height, proto);
                setZoomGraphPosition(proto, 
                                     m_XNodesBothEvents, m_YNodesBothEvents);
                m_YNodesBothEvents += height + ADD_HEIGHT;
                m_XNodesBothEvents += TheApp->GetRoutedNodeIndent();
                m_XNodesOnlyEventIns += TheApp->GetRoutedNodeIndent();
            }
        }
        InitializeNodes(true);
        InitializeNodes(false);
        m_lastYPosition = getFirstYPosition();
    }

    for (int i = 0; i < m_nodesOrProtos.size(); i++) {
        Node *node = m_nodesOrProtos.get(i).getNode();
        if (node) {
            Position(node);
        }
    }

    if (!TheApp->GetRouteViewShowAll())
        m_maxYPosition = GetYEndRouteBlock();

    swInvalidateWindow(m_window);
    SetScrollSizes(m_lastXPosition, m_maxYPosition);
    swSetScrollPosition(m_scroller, 0, 0);
}

void SceneGraphView::moveRoutesToTop(Node *moveNode)
{
    int eventIns = moveNode->getProto()->getNumEventIns();
    int eventOuts = moveNode->getProto()->getNumEventOuts();

    NodeList movingNodes;
  
    // collect nodes of eventOuts 
    for (int i = 0; i < eventOuts; i++) {
        for (SocketList::Iterator *j = moveNode->getOutput(i).first(); 
             j != NULL; j = j->next()) {
            Node *outputNode = j->item().getNode();
            bool isInList= false;
            for (int n = 0; n < movingNodes.size(); n++)
                if (outputNode == movingNodes[n]) {
                    isInList = true;
                    break;
                }    
            if (!isInList)
                movingNodes.append(outputNode);
        }
    }
    movingNodes.append(moveNode);
    // collect nodes of eventIns 
    for (int i = 0; i < eventIns; i++) {
        for (SocketList::Iterator *j = moveNode->getInput(i).first(); j != NULL;
             j = j->next()) {
            Node *inputNode = j->item().getNode();
            bool isInList= false;
            for (int n = 0; n < movingNodes.size(); n++)
                if (inputNode == movingNodes[n]) {
                    isInList = true;
                    break;
                }    
            if (!isInList)
                movingNodes.append(inputNode);
        }
    }

    for (int i = 0; i < movingNodes.size(); i++) {
        int x;
        int y;
        getZoomGraphPosition(x, y, movingNodes[i]);
        moveToTop(movingNodes[i], x);
    }

    SetScrollSizes(m_lastXPosition, m_maxYPosition);
    swSetScrollPosition(m_scroller, 0, 0);
    swInvalidateWindow(m_window);
}

void SceneGraphView::moveToTop(Node *moveNode, Node* ignoreNode)
{
    moveToTop(moveNode, NODE_WIDTH + 2 * ADD_WIDTH, ignoreNode);

    SetScrollSizes(m_lastXPosition, m_maxYPosition);
    swSetScrollPosition(m_scroller, 0, 0);
    swInvalidateWindow(m_window);
}

void SceneGraphView::moveToTop(Node *moveNode, int x, Node* ignoreNode)
{
    if (moveNode != m_scene->getRoot()) {
        setZoomGraphPosition(moveNode, x, ADD_HEIGHT);
        accountGraphSize(moveNode);
    } else
        return;

    int moveWidth = 1, moveHeight = 1;
    getZoomGraphSize(moveWidth, moveHeight, moveNode);

    m_YNodesOnlyEventIns += moveHeight;

    for (int i = 0; i < m_nodesOrProtos.size(); i++) {
        Proto *proto = m_nodesOrProtos.get(i).getProto();
        if (proto) {
            int width = 1, height = 1;
            getZoomGraphPosition(width, height, proto);
            setZoomGraphPosition(proto, width, height + moveHeight + 
                                 ADD_HEIGHT);
        }
        Node *node = m_nodesOrProtos.get(i).getNode();
        if (node && (node != moveNode)) {
            int width = 1, height = 1;
            getZoomGraphPosition(width, height, node);
            setZoomGraphPosition(node, width, height + moveHeight + ADD_HEIGHT);
            Position(node);

            getZoomGraphPosition(width, height, node);
            if (node != ignoreNode) {
                bool hasInputs = node->hasInputsOrIs();
                bool hasOutputs = node->hasOutputsOrIs();
                if (hasOutputs && !hasInputs)
                    if (m_YNodesOnlyEventOuts < height)
                        m_YNodesOnlyEventOuts = height;
                if (hasOutputs && hasInputs)
                    if (m_YNodesBothEvents < height)
                        m_YNodesBothEvents = height;
                if (!hasOutputs && hasInputs)
                    if (m_YNodesOnlyEventIns < height)
                        m_YNodesOnlyEventIns = height;
            }
            if ((GetYEndRouteBlock() + moveHeight) > m_maxYPosition)
                m_maxYPosition = GetYEndRouteBlock() + moveHeight;
        }
    }
    if (!TheApp->GetRouteViewShowAll())
        m_maxYPosition = MAX(m_maxYPosition, getFirstYPosition());
}

void SceneGraphView::OnDraw(int x, int y, int width, int height, bool update)
{
    if (!swIsVisible(m_wnd))
        return;

    SDC dc, frontDC;

    const Path *sel = m_scene->getSelection();
    Node *currentNode = sel ? sel->getNode() : NULL;
    Proto *currentProto = NULL;
    if (sel->isProto(m_scene))
        currentProto = sel->getProto(m_scene);

    if ((width == 0 || height == 0) || (width == 1 || height == 1)) {
        swGetSize(m_window, &width, &height);
    }

    frontDC = swCreateDC(m_window);
    dc = swCreateBitmapDC(frontDC, x + width, y + height);

    if (dc == NULL) {
        swDestroyDC(frontDC);
        return;
    }

    swSetFGColor(dc, m_shadow);
    swFillRect(dc, x, y, width, height);

    for (int i = 0; i < m_nodesOrProtos.size(); i++) {
        Node *node = m_nodesOrProtos.get(i).getNode();
        Proto *proto = m_nodesOrProtos.get(i).getProto();
        if (node) {
            int nx, ny, nw, nh;
            getZoomGraphPosition(nx, ny, node);
            getZoomGraphSize(nw, nh, node);
            if (node != currentNode
                && node != m_scene->getRoot()
                && nx + nw >= x && nx <= x + width
                && ny + nh >= y && ny <= y + height) {
                if (nx > MAX_X) {
                    warnFlooded();
                } else                   
                    if (TheApp->GetRouteViewShowAll() || 
                        (ny <= GetYEndRouteBlock()))
                    DrawNode(dc, node, nx, ny);
            }
        } else if (proto) {
            int nx, ny, nw, nh;
            getZoomGraphPosition(nx, ny, proto);
            getZoomGraphSize(nw, nh, proto);
            if ((proto != currentProto)
                && nx + nw >= x && nx <= x + width
                && ny + nh >= y && ny <= y + height) {
                if (nx > MAX_X) {
                    warnFlooded();
                } else                   
                    if (TheApp->GetRouteViewShowAll() || 
                        (ny <= GetYEndRouteBlock()))
                    DrawProto(dc, proto, nx, ny);
            }
        }
    }

    if (currentNode) {
        int nx, ny, nw, nh;
        getZoomGraphPosition(nx, ny, currentNode);
        getZoomGraphSize(nw, nh, currentNode);
        if (currentNode != m_scene->getRoot()
            && nx + nw >= x && nx <= x + width
            && ny + nh >= y && ny <= y + height) {
            if (TheApp->GetRouteViewShowAll() || (ny <= GetYEndRouteBlock()))
                DrawNode(dc, currentNode, nx, ny);
        }
    } 
    if (currentProto) {
        int nx = 0, ny = 0, nw = 0, nh = 0;
        getZoomGraphPosition(nx, ny, currentProto);
        getZoomGraphSize(nw, nh, currentProto);
        if (nx + nw >= x && nx <= x + width
            && ny + nh >= y && ny <= y + height) {
            if (TheApp->GetRouteViewShowAll() || (ny <= GetYEndRouteBlock()))
                DrawProto(dc, currentProto, nx, ny);
        }
    }

    for (int i = 0; i < m_nodesOrProtos.size(); i++) {
        Node *node = m_nodesOrProtos.get(i).getNode();
        Proto *proto = m_nodesOrProtos.get(i).getProto();
        if (node) {
            DrawRoutes(dc, node);
        } else if (proto) {
            DrawIs(dc, proto);
        }
    }
    
    int cindex = -1;

    bool isField = false;
    if (m_mode == ROUTING) {
        Proto *proto = m_srcProto;
        if (m_srcNode)
            proto = m_srcNode->getProto();
        int eventIns = proto->getNumEventIns();
        int eventOuts = proto->getNumEventOuts();
        int sockets = MAX(eventIns, eventOuts);
        if (m_srcSide == 1) {
            isField = false;
            if (m_srcNode && (m_srcNode->isNodeWithAdditionalEvents()) &&
                (proto->getNumEventOuts() == m_srcSocket)) {
                cindex = LAST_TYPE + 1;
                if ((m_dstNode != NULL) && (m_srcNode != m_dstNode)) {
                    Proto *dstProto = m_dstNode->getProto();
                    if ((m_dstSocket > 0) 
                        && (m_dstSocket < dstProto->getNumEventIns()))
                        cindex = dstProto->getEventIn(m_dstSocket)->getType();
                }
            } else if (m_srcSocket < eventOuts) {
                cindex = proto->getEventOut(m_srcSocket)->getType();
            } else if (m_srcSocket - sockets - 1 < 
                       proto->getNumExposedFields()) {
                cindex = proto->getExposedField(m_srcSocket - sockets - 1)->
                             getType();
                isField = true;
            }
        } else {
            isField = false;
            if (m_srcNode && (m_srcNode->isNodeWithAdditionalEvents()) &&
                (proto->getNumEventIns() == m_srcSocket)) {
                cindex = LAST_TYPE + 1;
                if ((m_dstNode != NULL) && (m_srcNode != m_dstNode)) {
                    Proto *dstProto = m_dstNode->getProto();
                    if ((m_dstSocket > 0) 
                        && (m_dstSocket < dstProto->getNumEventOuts()))
                        cindex = dstProto->getEventOut(m_dstSocket)->getType();
                }
            } else if (m_srcSocket < eventIns) {
                cindex = proto->getEventIn(m_srcSocket)->getType();
            } else if (m_srcSocket - sockets - 1 < proto->getNumFields()) {
                cindex = proto->getField(getFieldOfSocket(proto, m_srcSocket -
                                sockets - 1))->getType();
                isField = true;
            }
        }

        if (cindex > -1) {
            if ((m_srcNode == NULL) && m_srcProto)
                DrawRoute(dc, GetSocketPosition(m_srcProto, 
                                                m_srcSocket - 
                                                (isField ? sockets : 0),
                                                m_srcSide, isField), m_point1, 
                                                m_typeColors[cindex]);
            else
                DrawRoute(dc, GetSocketPosition(m_srcNode, m_srcSocket, 
                                                m_srcSide), m_point1, 
                                                m_typeColors[cindex]);
        }
    } else if (m_mode == CUTTING) {
        swSetFGColor(dc, 0x0000ff);
        swDrawLine(dc, m_point1.x, m_point1.y, m_point2.x, m_point2.y);
    }

    swCopyRect(dc, frontDC, x, y, x, y, width, height);
    swDestroyDC(frontDC);
    swDestroyDC(dc);
}

void SceneGraphView::OnSize(int width, int height)
{
    SetSize(width, height);
    SetScrollSizes(m_lastXPosition, m_maxYPosition);
}

void SceneGraphView::OnMouseLeave()
{
#ifdef HAVE_OLD_MOTIF
//
// don't ask me, why some versions of lesstif/openmotif see
// highlighting of a node socket in the route view as a leaving of window...
//
    if (m_dstNode)
    {
        InvalidateNode(m_dstNode);
        m_dstNode = NULL;
        m_dstSocket = -1;
    }
#endif
}

void SceneGraphView::YPosition(int width, int height)
{
    if ((m_lastYPosition + height + ADD_HEIGHT) < MAX_Y) {
        setLastYPosition(m_lastYPosition + height + ADD_HEIGHT);
        if (m_maxYPosition < MAX_Y)
           m_maxYPosition = m_lastYPosition;
    } else {
        setLastYPosition(height + ADD_HEIGHT + getFirstYPosition());
        setLastXPosition(m_lastXPosition + width + ADD_WIDTH); 
        m_maxYPosition = MAX_Y;
    }
}

void SceneGraphView::accountGraphSize(Node *node)
{
    int height = 0;
    int eventIns = node->getProto()->getNumEventIns();
    int eventOuts = node->getProto()->getNumEventOuts();
    int sockets = MAX(eventIns, eventOuts);
    if (node->isInsideProto()) {
        int fields = node->getProto()->getNumFields();
        int exposedFields = node->getProto()->getNumExposedFields();
        int numFields = 0;
        for (int i = 0; i < fields; i++) {
            Field *field = node->getProto()->getField(i);
            if (!(field->getFlags() & FF_HIDDEN))
                numFields++;
        }
        sockets += MAX(numFields, exposedFields);
    }
    if (node->isNodeWithAdditionalEvents())
         sockets++;
    height = sockets * SOCKET_HEIGHT + 
             (sockets + 1) * SOCKET_SPACING + BORDER_WIDTH * 2;
    if (node->isInsideProto())
        height += 2 * SOCKET_SPACING;
    height = MAX(height, ICON_SIZE_Y + BORDER_WIDTH * 6);

    setGraphSize(node, NODE_WIDTH, height);
}

void SceneGraphView::accountGraphSize(Proto *proto)
{
    int height = 0;
    int eventIns = proto->getNumEventIns();
    int eventOuts = proto->getNumEventOuts();
    int fields = proto->getNumFields();
    int exposedFields = proto->getNumExposedFields();
    int sockets = MAX(eventIns, eventOuts);
    sockets++;
    sockets += MAX(fields, exposedFields);
     
    height = sockets * SOCKET_HEIGHT + 
             (sockets + 1) * SOCKET_SPACING + BORDER_WIDTH * 2;
    height = MAX(height, ICON_SIZE_Y + BORDER_WIDTH * 6);

    setGraphSize(proto, NODE_WIDTH, height);
}

void SceneGraphView::accountGraphPosition(Node *node)
{
    int width = 1, height = 1;
    getZoomGraphSize(width, height, node);
    width = NODE_WIDTH + LINE_STUB;
    setLastXPosition(MAX(m_lastXPosition, ADD_WIDTH + width));
    YPosition(width, height);
    setZoomGraphPosition(node, m_lastXPosition - width , 
                               m_lastYPosition - height);
}

void SceneGraphView::accountGraphPosition(Proto *proto)
{
    int width = 1, height = 1;
    getZoomGraphSize(width, height, proto);
    width = NODE_WIDTH + LINE_STUB;
    setLastXPosition(MAX(m_lastXPosition, ADD_WIDTH + width));
    YPosition(width, height);
    setZoomGraphPosition(proto, m_lastXPosition - width , 
                               m_lastYPosition - height);
}

void SceneGraphView::Position(Node *node)
{
    int width = 1, height = 1;
    getZoomGraphPosition(width, height, node);
    if (height == 0) {
        accountGraphSize(node);
        accountGraphPosition(node);
    }
}

void SceneGraphView::DrawNode(SDC dc, Node *node, int xPos, int yPos)
{
    int eventIns = node->getProto()->getNumEventIns();
    int eventOuts = node->getProto()->getNumEventOuts();
    int fields = node->getProto()->getNumFields();
    int exposedFields = node->getProto()->getNumExposedFields();
    int eventSockets = MAX(eventIns, eventOuts);
    int width = 1, height = 1;

    if ((m_scene->isX3d() && (node->getFlag(NODE_FLAG_VRML_ONLY))) ||
        ((!m_scene->isX3d()) && (node->getFlag(NODE_FLAG_X3D_ONLY))))
        return;

    getZoomGraphSize(width, height, node);

    int y = 0;

    SBITMAP sbitmap;

    for (int i = 0; i < BORDER_WIDTH; i++) {
        swDraw3DRect(dc, m_window, xPos + i, yPos + i,
                     width - i * 2, height - i * 2);
    }

    const Path *sel = m_scene->getSelection();
    Node *currentNode = sel ? sel->getNode() : NULL;
    if (currentNode == node)
        swSetFGColor(dc, 0xFF0000);
    else
        swSetFGColor(dc, m_face);
    int rectY = yPos + BORDER_WIDTH; 
    int rectHeight = height - BORDER_WIDTH * 2;
    swFillRect(dc, xPos + BORDER_WIDTH, rectY,
               width - BORDER_WIDTH * 2, rectHeight);

    if (height > ICON_SIZE_Y) {
        swSetFGColor(dc, m_face);
        int yMin = MIN(ICON_SIZE_Y - BORDER_WIDTH, height / 2);
        swFillRect(dc, xPos + ICON_OFFSET - BORDER_WIDTH, 
                   yPos + yMin - 8 - BORDER_WIDTH,
                   ICON_SIZE_X + BORDER_WIDTH * 2, 
                   ICON_SIZE_Y + BORDER_WIDTH * 2);
        swDrawBitmap(dc, m_nodeBitmap, node->getType() * ICON_SIZE_X, 0,
                     xPos + ICON_OFFSET, yPos + yMin - 8, 
                     ICON_SIZE_X, ICON_SIZE_Y);
    }

    if (m_zoom != 1.0)
        return;

    if (currentNode == node) {
        swSetFGColor(dc, m_face);
        int rectWidth = SOCKET_WIDTH + 2 * SOCKET_SPACING;
        swFillRect(dc, xPos + BORDER_WIDTH, rectY, rectWidth, rectHeight);
        swFillRect(dc, xPos + width - rectWidth - BORDER_WIDTH, rectY,
                   rectWidth, rectHeight);
    }
    bool x3d = m_scene->isX3d();
    MyString name;
    name = node->getName();
    if (name[0] == 0)
       name = node->getProto()->getName(x3d);
    if (currentNode == node)
        swSetFGColor(dc, m_face);
    else
        swSetFGColor(dc, 0x000000);
    int yMin = MIN(ICON_SIZE_Y - BORDER_WIDTH, height / 2);
    swDrawText(dc, xPos + TEXT_OFFSET, yPos + yMin + 6, name);

    y = yPos + BORDER_WIDTH + SOCKET_SPACING;
    for (int i = 0; i < eventIns; i++) {
        EventIn *eventIn = node->getProto()->getEventIn(i);
        if (m_scene->isInvalidElement(eventIn)) {
            y += SOCKET_HEIGHT + SOCKET_SPACING;
            continue;
        }
        SBITMAP selectedSocketBitmap = m_socketBitmaps[eventIn->getType()];
        const char *name = eventIn->getName(node->getScene()->isX3d());
        bool drawName = false;
        if (node == m_dstNode && m_dstSide == 0 && m_dstSocket == i && 
            (isTracking() || (m_srcSide == 1 && 
             Scene::validRoute(m_srcNode, m_srcSocket, m_dstNode, m_dstSocket)))) {
            sbitmap = selectedSocketBitmap;
            drawName = true;
        } else if (node == m_dstNode && m_dstSocket == i && 
                   validIs(m_srcProto, m_srcSocket, m_srcSide, 
                           m_dstNode, m_dstSocket, m_dstSide)) {
            sbitmap = selectedSocketBitmap;
            drawName = true;
        } else if (m_mode == ROUTING && node == m_srcNode && m_srcSide == 0 
                   && m_srcSocket == i) {
            sbitmap = selectedSocketBitmap;
            drawName = true;
        } else if (node->getInput(i).size() > 0) {
            sbitmap = selectedSocketBitmap;
         } else if (node->getOutsideProto() &&
                    node->getOutsideProto()->lookupIsEventIn(node, i) != -1) {
            sbitmap = selectedSocketBitmap;
        } else if ((eventIn->getFlags() & EIF_RECOMMENDED) &&
                   (eventIn->getFlags() & EIF_WONDERLAND_SUPPORTED)) {
            sbitmap = m_socketBitmapRecommendedWonderland;
        } else if (eventIn->getFlags() & EIF_WONDERLAND_SUPPORTED) {
            sbitmap = m_socketBitmapWonderland;
        } else if (eventIn->getFlags() & EIF_RECOMMENDED) {
            sbitmap = m_socketBitmapRecommended;
        } else {
            sbitmap = m_socketBitmap;
        }
        if (drawName)
            DrawSocketName(dc, xPos + BORDER_WIDTH + SOCKET_WIDTH 
                               + SOCKET_SPACING * 2, y + SOCKET_HEIGHT / 2, 
                           name, false);
        swDrawBitmap(dc, sbitmap, 0, 0, 
                xPos + BORDER_WIDTH + SOCKET_SPACING, y,
                SOCKET_WIDTH, SOCKET_HEIGHT);
        y += SOCKET_HEIGHT + SOCKET_SPACING;
    }
    if (node->isNodeWithAdditionalEvents()) {
        sbitmap = m_socketBitmap;
        bool drawName = false;
        if (m_mode == ROUTING && node == m_srcNode && m_srcSide == 0 
            && m_srcSocket == eventIns) {
            if (m_dstSocket >= 0) {
                Proto *proto = m_dstNode->getProto();
                if (m_dstSocket < proto->getNumEventOuts()) {
                    int type = proto->getEventOut(m_dstSocket)->getType();
                    sbitmap = m_socketBitmaps[type];
                }
            }
            drawName = true;
        } else if (node == m_dstNode && m_dstSide == 0 && 
                   m_dstSocket == eventIns && 
                   (isTracking() || (m_srcSide == 1 && 
             Scene::validRoute(m_srcNode, m_srcSocket, m_dstNode, m_dstSocket))))
             {
            drawName = true; 
        } else if (node == m_dstNode && m_dstSocket == eventIns && 
                   validIs(m_srcProto, m_srcSocket, m_srcSide, 
                           m_dstNode, m_dstSocket, m_dstSide)) {
            drawName = true;
        } else if ((node == m_dstNode) && (m_mode == ROUTING) && 
                   (m_srcSocket >= 0) && (m_srcNode != NULL)) {
            Proto *proto = m_srcNode->getProto();
            if (m_srcSocket < proto->getNumEventOuts()) {
                int type = proto->getEventOut(m_srcSocket)->getType();
                sbitmap = m_socketBitmaps[type];
            }
        }
        if (drawName)
            DrawSocketName(dc, xPos + BORDER_WIDTH + SOCKET_WIDTH 
                               + SOCKET_SPACING * 2, y + SOCKET_HEIGHT / 2, 
                           "(connect anything)", false);
        swDrawBitmap(dc, sbitmap, 0, 0,
                         xPos + BORDER_WIDTH + SOCKET_SPACING, y,
                         SOCKET_WIDTH, SOCKET_HEIGHT);
    }
    int yLeft = y;
    y = yPos + BORDER_WIDTH + SOCKET_SPACING;
    for (int i = 0; i < eventOuts; i++) {
        EventOut *eventOut = node->getProto()->getEventOut(i);
        if (m_scene->isInvalidElement(eventOut)) {
            y += SOCKET_HEIGHT + SOCKET_SPACING;
            continue;
        }
        SBITMAP selectedSocketBitmap = m_socketBitmaps[eventOut->getType()];
        const char *name = eventOut->getName(x3d);
        bool drawName = false;
        if (node == m_dstNode && m_dstSide == 1 && m_dstSocket == i && 
            (isTracking() || (m_srcSide == 0 && 
             Scene::validRoute(m_dstNode, m_dstSocket, m_srcNode, m_srcSocket)))) {
            sbitmap = selectedSocketBitmap;
            drawName = true;
        } else if (node == m_dstNode && m_dstSocket == i && 
                   validIs(m_srcProto, m_srcSocket, m_srcSide, 
                           m_dstNode, m_dstSocket, m_dstSide)) {
            sbitmap = selectedSocketBitmap;
            drawName = true;
        } else if (m_mode == ROUTING && node == m_srcNode && m_srcSide == 1 && 
                   m_srcSocket == i) {
            sbitmap = selectedSocketBitmap;
            drawName = true;
        } else if (node->getOutput(i).size() > 0)  { 
            sbitmap = selectedSocketBitmap;
         } else if (node->getOutsideProto() &&
                    node->getOutsideProto()->lookupIsEventOut(node, i) != -1) {
            sbitmap = selectedSocketBitmap;
        } else if ((eventOut->getFlags() & EOF_RECOMMENDED) &&
                   (eventOut->getFlags() & EOF_WONDERLAND_SUPPORTED)) {
            sbitmap = m_socketBitmapRecommendedWonderland;
        } else if (eventOut->getFlags() & EOF_WONDERLAND_SUPPORTED) {
            sbitmap = m_socketBitmapWonderland;
        } else if (eventOut->getFlags() & EOF_RECOMMENDED) {
            sbitmap = m_socketBitmapRecommended;
        } else {
            sbitmap = m_socketBitmap;
        }
        if (drawName)
            DrawSocketName(dc, xPos + width - BORDER_WIDTH - SOCKET_SPACING*2 
                               - SOCKET_WIDTH, y + SOCKET_HEIGHT / 2, 
                           name, true);
        swDrawBitmap(dc, sbitmap, 0, 0,
                xPos + width - BORDER_WIDTH - SOCKET_SPACING - SOCKET_WIDTH, y,
                SOCKET_WIDTH, SOCKET_HEIGHT);
        y += SOCKET_HEIGHT + SOCKET_SPACING;
    }
    if (node->isNodeWithAdditionalEvents()) {
        sbitmap = m_socketBitmap;
        bool drawName = false;
        if (m_mode == ROUTING && node == m_srcNode && m_srcSide == 1 && 
            m_srcSocket == eventOuts) {
            if (m_dstSocket >= 0) {
                Proto *proto = m_dstNode->getProto();
                if (m_dstSocket < proto->getNumEventIns()) {
                    int type = proto->getEventIn(m_dstSocket)->getType();
                    sbitmap = m_socketBitmaps[type];
                }
            }
            drawName = true;
        } else if (node == m_dstNode && m_dstSide == 1 &&
                   m_dstSocket == eventOuts && 
                   (isTracking() || m_srcSide == 0)) {
            drawName = true; 
            if ((m_mode == ROUTING) && (m_srcSocket >= 0)) {
                Proto *proto = m_srcNode->getProto();
                if (m_srcSocket < proto->getNumEventIns()) {
                    int type = proto->getEventIn(m_srcSocket)->getType();
                    sbitmap = m_socketBitmaps[type];
                }
            }
        }    
        if (drawName)
            DrawSocketName(dc, 
                xPos + width - BORDER_WIDTH - SOCKET_SPACING*2 - SOCKET_WIDTH,
                y + SOCKET_HEIGHT / 2, "(connect anything)", true);
        swDrawBitmap(dc, sbitmap, 0, 0, 
                xPos + width - BORDER_WIDTH - SOCKET_SPACING - SOCKET_WIDTH, y,
                SOCKET_WIDTH, SOCKET_HEIGHT);
        y += SOCKET_HEIGHT + SOCKET_SPACING;
    }

    if (node->isInsideProto()) {
        swSetFGColor(dc, 0x000000);
        int yMax = MAX(y, yLeft);
        swDrawLine(dc, xPos + BORDER_WIDTH, yMax, 
                       xPos + width - BORDER_WIDTH, yMax); 

        y = yMax + BORDER_WIDTH + SOCKET_SPACING;

        MyArray<int> fieldArray;
        for (int i = 0; i < fields; i++) {
            Field *field = node->getProto()->getField(i);
            if (!(field->getFlags() & FF_HIDDEN))
                fieldArray.append(i);
        }
        for (int i = 0; i < fieldArray.size(); i++) {
            Field *field = node->getProto()->getField(fieldArray[i]);
            if (m_scene->isInvalidElement(field)) {
                y += SOCKET_HEIGHT + SOCKET_SPACING;
                continue;
            }
            SBITMAP selectedSocketBitmap = m_socketBitmaps[field->getType()];
            const char *name = field->getName(node->getScene()->isX3d());
            bool drawName = false;
            if (node == m_dstNode && m_dstSide == 0 && 
                m_dstSocket == i + eventSockets && 
                (isTracking() || (m_srcSide == 1 && 
                 Scene::validRoute(m_srcNode, m_srcSocket, m_dstNode, m_dstSocket)))) {
                sbitmap = selectedSocketBitmap;
                drawName = true;
            } else if (node == m_dstNode && (m_dstSocket == i + eventSockets) &&
                validIs(m_srcProto, m_srcSocket, m_srcSide, 
                        m_dstNode, m_dstSocket, m_dstSide)) {
                sbitmap = selectedSocketBitmap;
                drawName = true;
            } else if (m_mode == ROUTING && node == m_srcNode && m_srcSide == 0 
                       && m_srcSocket == i + eventSockets) {
                sbitmap = selectedSocketBitmap;
                drawName = true;
            } else if (node->getOutsideProto()->lookupIsField(node, 
                             fieldArray[i]) != -1) {
                sbitmap = selectedSocketBitmap;
            } else if (node->getOutsideProto()->lookupIsExposedField(node, 
                             fieldArray[i]) != -1) {
                sbitmap = selectedSocketBitmap;
            } else {
                sbitmap = m_socketBitmap;
            }
            if (drawName)
                DrawSocketName(dc, xPos + BORDER_WIDTH + SOCKET_WIDTH 
                                   + SOCKET_SPACING * 2, 
                               y + SOCKET_HEIGHT / 2 + SOCKET_SPACING, 
                               name, false);
            swDrawBitmap(dc, sbitmap, 0, 0, 
                    xPos + BORDER_WIDTH + SOCKET_SPACING, 
                    y - SOCKET_HEIGHT / 2 - 1,
                    SOCKET_WIDTH, SOCKET_HEIGHT);
            y += SOCKET_HEIGHT + SOCKET_SPACING;
        }
    
        y = yMax + BORDER_WIDTH + SOCKET_SPACING;

        for (int i = 0; i < exposedFields; i++) {
            ExposedField *exposedField = node->getProto()->getExposedField(i);
            if (m_scene->isInvalidElement(exposedField)) {
                y += SOCKET_HEIGHT + SOCKET_SPACING;
                continue;
            }
            SBITMAP selectedSocketBitmap = m_socketBitmaps[exposedField->getType()];
            const char *name = exposedField->getName(x3d);
            bool drawName = false;
            if ((node == m_dstNode) && (m_dstSide == 1) && 
                (m_dstSocket == (i + eventSockets)) && 
                (isTracking() || ((m_srcSide == 0) && 
                 Scene::validRoute(m_dstNode, m_dstSocket, m_srcNode, 
                                   m_srcSocket)))) {
                sbitmap = selectedSocketBitmap;
                drawName = true;
            } else if (node == m_dstNode && m_dstSocket == 
                       (i + eventSockets) && 
                       validIs(m_srcProto, m_srcSocket, m_srcSide, 
                               m_dstNode, m_dstSocket, m_dstSide)) {
                sbitmap = selectedSocketBitmap;
                drawName = true;
            } else if (m_mode == ROUTING && node == m_srcNode && 
                       m_srcSide == 1 && m_srcSocket == (i + eventSockets)) {
                sbitmap = selectedSocketBitmap;
                drawName = true;
            } else if (node->getOutsideProto()->lookupIsExposedField(node, 
                             exposedField->getFieldIndex()) != -1) {
                sbitmap = selectedSocketBitmap;
            } else if (node->getOutsideProto()->lookupIsField(node, 
                             exposedField->getFieldIndex()) != -1) {
                sbitmap = selectedSocketBitmap;
            } else {
                sbitmap = m_socketBitmap;
            }
            if (drawName)
                DrawSocketName(dc, xPos + width - BORDER_WIDTH - SOCKET_SPACING*2 
                                   - SOCKET_WIDTH, 
                               y + SOCKET_HEIGHT / 2 + SOCKET_SPACING, 
                               name, true);
            swDrawBitmap(dc, sbitmap, 0, 0,
                    xPos + width - BORDER_WIDTH - SOCKET_SPACING - SOCKET_WIDTH, 
                    y - SOCKET_HEIGHT / 2 - 1,
                    SOCKET_WIDTH, SOCKET_HEIGHT);
            y += SOCKET_HEIGHT + SOCKET_SPACING;
        }        
    }
}

void SceneGraphView::DrawProto(SDC dc, Proto *proto, int xPos, int yPos)
{
    int eventIns = proto->getNumEventIns();
    int eventOuts = proto->getNumEventOuts();
    int fields = proto->getNumFields();
    int exposedFields = proto->getNumExposedFields();
    int eventSockets = MAX(eventIns, eventOuts);
    int width = 1, height = 1;

    getZoomGraphSize(width, height, proto);

    int y = 0;

    SBITMAP sbitmap;

    for (int i = 0; i < BORDER_WIDTH; i++) {
        swDraw3DRect(dc, m_window, xPos + i, yPos + i,
                     width - i * 2, height - i * 2);
    }

    const Path *sel = m_scene->getSelection();
    Proto *current = NULL;
    if (sel && sel->isProto(m_scene))
        current = sel->getProto(m_scene);
    if (current == proto)
        swSetFGColor(dc, 0xFF0000);
    else
        swSetFGColor(dc, m_face);
    int rectY = yPos + BORDER_WIDTH; 
    int rectHeight = height - BORDER_WIDTH * 2;
    swFillRect(dc, xPos + BORDER_WIDTH, rectY,
               width - BORDER_WIDTH * 2, rectHeight);

    if (height > ICON_SIZE_Y) {
        swSetFGColor(dc, m_face);
        int yMin = MIN(ICON_SIZE_Y - BORDER_WIDTH, height / 2);
        swFillRect(dc, xPos + ICON_OFFSET - BORDER_WIDTH, 
                   yPos + yMin - 8 - BORDER_WIDTH,
                   ICON_SIZE_X + BORDER_WIDTH * 2, 
                   ICON_SIZE_Y + BORDER_WIDTH * 2);
        swDrawBitmap(dc, m_nodeBitmap, (m_nodeBitmaps - 1) * ICON_SIZE_X, 0,
                     xPos + ICON_OFFSET, yPos + yMin - 8, 
                     ICON_SIZE_X, ICON_SIZE_Y);
    }

    if (m_zoom != 1.0)
        return;

    if (current == proto) {
        swSetFGColor(dc, m_face);
        int rectWidth = SOCKET_WIDTH + 2 * SOCKET_SPACING;
        swFillRect(dc, xPos + BORDER_WIDTH, rectY, rectWidth, rectHeight);
        swFillRect(dc, xPos + width - rectWidth - BORDER_WIDTH, rectY,
                   rectWidth, rectHeight);
    }
    bool x3d = m_scene->isX3d();
    MyString name = "";
    name += "PROTO ";
    name += proto->getName(x3d);
    if (current == proto)
        swSetFGColor(dc, m_face);
    else
        swSetFGColor(dc, 0x000000);
    int yMin = MIN(ICON_SIZE_Y - BORDER_WIDTH, height / 2);
    swDrawText(dc, xPos + TEXT_OFFSET, yPos + yMin + 6, name);

    y = yPos + BORDER_WIDTH + SOCKET_SPACING;
    for (int i = 0; i < eventIns; i++) {
        EventIn *eventIn = proto->getEventIn(i);
        if (m_scene->isInvalidElement(eventIn)) {
            y += SOCKET_HEIGHT + SOCKET_SPACING;
            continue;
        }
        SBITMAP selectedSocketBitmap = m_socketBitmaps[eventIn->getType()];
        const char *name = eventIn->getName(x3d);
        bool drawName = false;
        if (proto == m_srcProto && m_srcSocket == i && (isTracking() || 
            validIs(m_srcProto, m_srcSocket, m_srcSide,
                    m_dstNode, m_dstSocket, m_dstSide))) {
            sbitmap = selectedSocketBitmap;
            drawName = true;
        } else if (proto == m_dstProto && m_dstSocket == i && 
                   (m_dstSide == 0) &&
                   (isTracking() || 
                    validIs(m_dstProto, m_dstSocket, m_dstSide,
                            m_srcNode, m_srcSocket, m_srcSide))) {
            sbitmap = selectedSocketBitmap;
            drawName = true;
        } else if (m_mode == ROUTING && proto == m_srcProto && m_srcSide == 0 
                   && m_srcSocket == i) {
            sbitmap = selectedSocketBitmap;
            drawName = true;
        } else if (proto->getEventIn(i)->getNumIs() > 0) {
            sbitmap = selectedSocketBitmap;
        } else {
            sbitmap = m_socketBitmap;
        }
        if (drawName)
            DrawSocketName(dc, xPos + BORDER_WIDTH + SOCKET_WIDTH 
                               + SOCKET_SPACING * 2, y + SOCKET_HEIGHT / 2, 
                           name, false);
        swDrawBitmap(dc, sbitmap, 0, 0, 
                xPos + BORDER_WIDTH + SOCKET_SPACING, y,
                SOCKET_WIDTH, SOCKET_HEIGHT);
        y += SOCKET_HEIGHT + SOCKET_SPACING;
    }
    int yLeft = y;
    y = yPos + BORDER_WIDTH + SOCKET_SPACING;
    for (int i = 0; i < eventOuts; i++) {
        EventOut *eventOut = proto->getEventOut(i);
        if (m_scene->isInvalidElement(eventOut)) {
            y += SOCKET_HEIGHT + SOCKET_SPACING;
            continue;
        }
        SBITMAP selectedSocketBitmap = m_socketBitmaps[eventOut->getType()];
        const char *name = eventOut->getName(x3d);
        bool drawName = false;
        if (proto == m_srcProto && m_srcSocket == i && (isTracking() || 
            validIs(m_srcProto, m_srcSocket, m_srcSide,
                    m_dstNode, m_dstSocket, m_dstSide))) {
            sbitmap = selectedSocketBitmap;
            drawName = true;
        } else if (proto == m_dstProto && m_dstSocket == i &&
                   (m_dstSide == 1) && 
                   (isTracking() ||
                    validIs(m_dstProto, m_dstSocket, m_dstSide,
                            m_srcNode, m_srcSocket, m_srcSide))) {
            sbitmap = selectedSocketBitmap;
            drawName = true;
        } else if (m_mode == ROUTING && proto == m_srcProto && m_srcSide == 1 &&
                   m_srcSocket == i) {
            sbitmap = selectedSocketBitmap;
            drawName = true;
        } else if (proto->getEventOut(i)->getNumIs() > 0)  { 
            sbitmap = selectedSocketBitmap;
        } else {
            sbitmap = m_socketBitmap;
        }
        if (drawName)
            DrawSocketName(dc, xPos + width - BORDER_WIDTH - SOCKET_SPACING*2 
                               - SOCKET_WIDTH, y + SOCKET_HEIGHT / 2, 
                           name, true);
        swDrawBitmap(dc, sbitmap, 0, 0,
                xPos + width - BORDER_WIDTH - SOCKET_SPACING - SOCKET_WIDTH, y,
                SOCKET_WIDTH, SOCKET_HEIGHT);
        y += SOCKET_HEIGHT + SOCKET_SPACING;
    }

    swSetFGColor(dc, 0x000000);
    int yMax = MAX(y, yLeft);
    swDrawLine(dc, xPos + BORDER_WIDTH, yMax, 
                   xPos + width - BORDER_WIDTH, yMax); 

    y = yMax + BORDER_WIDTH + SOCKET_SPACING;

    MyArray<int> fieldArray;
    for (int i = 0; i < fields; i++) {
        Field *field = proto->getField(i);
        if (!(field->getFlags() & FF_HIDDEN))
            fieldArray.append(i);
    }
    for (int i = 0; i < fieldArray.size(); i++) {
        Field *field = proto->getField(fieldArray[i]);
        if (m_scene->isInvalidElement(field)) {
            y += SOCKET_HEIGHT + SOCKET_SPACING;
            continue;
        }
        SBITMAP selectedSocketBitmap = m_socketBitmaps[field->getType()];
        const char *name = field->getName(proto->getScene()->isX3d());
        bool drawName = false;
        if (proto == m_srcProto && m_srcSocket == i + eventSockets + 1 && 
            (isTracking() || 
             validIs(m_srcProto, m_srcSocket, m_srcSide,
                     m_dstNode, m_dstSocket, m_dstSide))) {
            sbitmap = selectedSocketBitmap;
            drawName = true;
        } else if (proto == m_dstProto && 
                   m_dstSocket == i + eventSockets + 1 && (m_dstSide == 0) &&
                   (isTracking() ||
                    validIs(m_dstProto, m_dstSocket, m_dstSide,
                            m_srcNode, m_srcSocket, m_srcSide))) {
            sbitmap = selectedSocketBitmap;
            drawName = true;
        } else if (m_mode == ROUTING && proto == m_srcProto &&
                   m_srcSide == 0 && m_srcSocket == i + eventSockets + 1) {
            sbitmap = selectedSocketBitmap;
            drawName = true;
        } else if (proto->getField(fieldArray[i])->getNumIs() > 0) {
            sbitmap = selectedSocketBitmap;
        } else {
            sbitmap = m_socketBitmap;
        }
        if (drawName)
            DrawSocketName(dc, xPos + BORDER_WIDTH + SOCKET_WIDTH 
                               + SOCKET_SPACING * 2, 
                           y + SOCKET_SPACING + SOCKET_HEIGHT, 
                           name, false);
        swDrawBitmap(dc, sbitmap, 0, 0, 
                     xPos + BORDER_WIDTH + SOCKET_SPACING, 
                     y + SOCKET_HEIGHT / 2 + 1,
                     SOCKET_WIDTH, SOCKET_HEIGHT);
        y += SOCKET_HEIGHT + SOCKET_SPACING;
    }

    y = yMax + BORDER_WIDTH + SOCKET_SPACING;

    for (int i = 0; i < exposedFields; i++) {
        ExposedField *exposedField = proto->getExposedField(i);
        if (m_scene->isInvalidElement(exposedField)) {
            y += SOCKET_HEIGHT + SOCKET_SPACING;
            continue;
        }
        SBITMAP selectedSocketBitmap = m_socketBitmaps[exposedField->getType()];
        const char *name = exposedField->getName(x3d);
        bool drawName = false;
        int fieldIndex = proto->getExposedField(i)->getFieldIndex();
        if (proto == m_srcProto && m_srcSocket == i + eventSockets + 1 && 
            (isTracking() || 
             validIs(m_srcProto, m_srcSocket, m_srcSide,
                     m_dstNode, m_dstSocket, m_dstSide))) {
            sbitmap = selectedSocketBitmap;
            drawName = true;
        } else if (proto == m_dstProto && m_dstSocket == i + eventSockets + 1 &&
                   (m_dstSide == 1) &&
                   (isTracking() ||
                    validIs(m_dstProto, m_dstSocket, m_dstSide,
                            m_srcNode, m_srcSocket, m_srcSide))) {
            sbitmap = selectedSocketBitmap;
            drawName = true;
        } else if (m_mode == ROUTING && proto == m_srcProto && m_srcSide == 1 &&
                   m_srcSocket == (i + eventSockets + 1)) {
            sbitmap = selectedSocketBitmap;
            drawName = true;
        } else if ((proto->getExposedField(i)->getNumIs() > 0) ||
                   (proto->getField(fieldIndex)->getNumIs() > 0)) {
            sbitmap = selectedSocketBitmap;
        } else {
            sbitmap = m_socketBitmap;
        }
        if (drawName)
            DrawSocketName(dc, xPos + width - BORDER_WIDTH - 
                               SOCKET_SPACING * 2 - SOCKET_WIDTH, 
                           y + SOCKET_SPACING + SOCKET_HEIGHT, 
                           name, true);
        swDrawBitmap(dc, sbitmap, 0, 0,
                     xPos + width - BORDER_WIDTH - SOCKET_SPACING - 
                     SOCKET_WIDTH, y + SOCKET_HEIGHT / 2 + 1,
                     SOCKET_WIDTH, SOCKET_HEIGHT);
        y += SOCKET_HEIGHT + SOCKET_SPACING;
    }        
}

void SceneGraphView::DrawRoute(SDC dc, Point start, Point end, int color)
{
    int midx = (start.x + end.x) / 2;
    int midy = (start.y + end.y) / 2;

    if (end.x - start.x < LINE_STUB * 2) {
        swSetFGColor(dc, 0x000000);
        MoveTo(dc, start.x + SHADOW_OFFSET, start.y + SHADOW_OFFSET);
        LineTo(dc, start.x + LINE_STUB + SHADOW_OFFSET, start.y + SHADOW_OFFSET);
        LineTo(dc, start.x + LINE_STUB + SHADOW_OFFSET, midy + SHADOW_OFFSET);
        LineTo(dc, end.x - LINE_STUB + SHADOW_OFFSET, midy + SHADOW_OFFSET);
        LineTo(dc, end.x - LINE_STUB + SHADOW_OFFSET, end.y + SHADOW_OFFSET);
        LineTo(dc, end.x + SHADOW_OFFSET, end.y + SHADOW_OFFSET);
        swSetFGColor(dc, color);
        MoveTo(dc, start.x, start.y);
        LineTo(dc, start.x + LINE_STUB, start.y);
        LineTo(dc, start.x + LINE_STUB, midy);
        LineTo(dc, end.x - LINE_STUB, midy);
        LineTo(dc, end.x - LINE_STUB, end.y);
        LineTo(dc, end.x, end.y);
    } else {
        swSetFGColor(dc, 0x000000);
        MoveTo(dc, start.x + SHADOW_OFFSET, start.y + SHADOW_OFFSET);
        LineTo(dc, midx + SHADOW_OFFSET, start.y + SHADOW_OFFSET);
        LineTo(dc, midx + SHADOW_OFFSET, end.y + SHADOW_OFFSET);
        LineTo(dc, end.x + SHADOW_OFFSET, end.y + SHADOW_OFFSET);
        swSetFGColor(dc, color);
        MoveTo(dc, start.x, start.y);
        LineTo(dc, midx, start.y);
        LineTo(dc, midx, end.y);
        LineTo(dc, end.x, end.y);
    }
}

bool SceneGraphView::IntersectRoute(Point start, Point end, 
                                    Point rStart, Point rEnd)
{
    int midx = (rStart.x + rEnd.x) / 2;
    int midy = (rStart.y + rEnd.y) / 2;
    bool rc;

    if (rEnd.x - rStart.x < LINE_STUB * 2) {
        rc = Util::IntersectLines(start.x, start.y, end.x, end.y,
                                  rStart.x, rStart.y,
                                  rStart.x + LINE_STUB, rStart.y)
          || Util::IntersectLines(start.x, start.y, end.x, end.y,
                                  rStart.x + LINE_STUB, rStart.y,
                                  rStart.x + LINE_STUB, midy)
          || Util::IntersectLines(start.x, start.y, end.x, end.y,
                                  rStart.x + LINE_STUB, midy,
                                  rEnd.x - LINE_STUB, midy)
          || Util::IntersectLines(start.x, start.y, end.x, end.y,
                                  rEnd.x - LINE_STUB, midy,
                                  rEnd.x - LINE_STUB, rEnd.y)
          || Util::IntersectLines(start.x, start.y, end.x, end.y,
                                  rEnd.x - LINE_STUB, rEnd.y,
                                  rEnd.x, rEnd.y);
    } else {
        rc = Util::IntersectLines(start.x, start.y, end.x, end.y,
                                  rStart.x, rStart.y,
                                  midx, rStart.y)
          || Util::IntersectLines(start.x, start.y, end.x, end.y,
                                  midx, rStart.y,
                                  midx, rEnd.y)
          || Util::IntersectLines(start.x, start.y, end.x, end.y,
                                  midx, rEnd.y,
                                  rEnd.x, rEnd.y);
    }
    return rc;
}

void SceneGraphView::InvalidateRoute(Point start, Point end)
{
    int midx = (start.x + end.x) / 2;
    int midy = (start.y + end.y) / 2;

    swInvalidateRect(m_window, start.x - 3, start.y - 2, 6, 6);
    swInvalidateRect(m_window, end.x, end.y - 2, 6, 6);
    if (end.x - start.x < LINE_STUB * 2) {
        MoveTo(m_window, start.x, start.y);
        InvalidateTo(m_window, start.x + LINE_STUB, start.y);
        InvalidateTo(m_window, start.x + LINE_STUB, midy);
        InvalidateTo(m_window, end.x - LINE_STUB, midy);
        InvalidateTo(m_window, end.x - LINE_STUB, end.y);
        InvalidateTo(m_window, end.x, end.y);
    } else {
        MoveTo(m_window, start.x, start.y);
        InvalidateTo(m_window, midx, start.y);
        InvalidateTo(m_window, midx, end.y);
        InvalidateTo(m_window, end.x, end.y);
    }
}

void SceneGraphView::DrawRoutes(SDC dc, Node *node)
{
    int eventOuts = node->getProto()->getNumEventOuts();

    for (int i = 0; i < eventOuts; i++) {
        for (SocketList::Iterator *j = node->getOutput(i).first(); j != NULL;
             j = j->next()) {
            RouteSocket s = j->item();
            int c = m_typeColors[node->getProto()->getEventOut(i)->getType()];
            Point dest = GetSocketPosition(s.getNode(), s.getField(), 0);
            DrawRoute(dc, GetSocketPosition(node, i, 1), 
                      Point(dest.x - SOCKET_WIDTH / 2, dest.y), c);
        }
    }
}

int 
SceneGraphView::destSide(int elementType)
{
    switch(elementType) {
      case EL_EVENT_OUT:
        return 1;
      case EL_EXPOSED_FIELD:
        return 1;
    }
    return 0;       
}

void SceneGraphView::DrawIs(SDC dc, Proto *proto)
{
    int eventOuts = proto->getNumEventOuts();
    for (int i = 0; i < eventOuts; i++) {
        for (int j = 0; j < proto->getEventOut(i)->getNumIs(); j++) {
            int c = m_typeColors[proto->getEventOut(i)->getType()];
            Point dest = GetSocketPosition(
                             proto->getEventOut(i)->getIsNode(j), 
                             proto->getEventOut(i)->getIsField(j), 1);
            DrawRoute(dc, GetSocketPosition(proto, i, 1, false), 
                      Point(dest.x - SOCKET_WIDTH / 2, dest.y), c);
        }
    }

    int eventIns = proto->getNumEventIns();
    for (int i = 0; i < eventIns; i++) {
        for (int j = 0; j < proto->getEventIn(i)->getNumIs(); j++) {
            int c = m_typeColors[proto->getEventIn(i)->getType()];
            Point dest = GetSocketPosition(
                             proto->getEventIn(i)->getIsNode(j), 
                             proto->getEventIn(i)->getIsField(j), 0);
            DrawRoute(dc, GetSocketPosition(proto, i, 0, false), 
                      Point(dest.x - SOCKET_WIDTH / 2, dest.y), c);
        }
    }

    for (int i = 0; i < proto->getNumFields(); i++) {
        Field *field = proto->getField(i);
        for (int j = 0; j < field->getNumIs(); j++) {
            int c = m_typeColors[field->getType()];
            int dSide = destSide(field->getIsElementType(j));
            int sSide = 0;
            int sField = i;
            if (field->getExposedField()) {
                sSide = 1;
                sField = proto->getExposedOfField(field);
                sField++;
            }
            Point dest = GetSocketPosition(field->getIsNode(j), 
                                           field->getIsField(j), dSide, true);
            DrawRoute(dc, GetSocketPosition(proto, sField, sSide, true), 
                      Point(dest.x - SOCKET_WIDTH / 2, dest.y), c);
        }
    }

    int exposedFields = proto->getNumExposedFields();
    for (int i = 0; i < exposedFields; i++) {
        ExposedField *exposedField = proto->getExposedField(i);
        for (int j = 0; j < exposedField->getNumIs(); j++) {
            int c = m_typeColors[exposedField->getType()];
            int side = destSide(exposedField->getIsElementType(j));
            Point dest = GetSocketPosition(
                             exposedField->getIsNode(j), 
                             exposedField->getIsField(j), 
                             side, true);
            DrawRoute(dc, GetSocketPosition(proto, i + 1, 1, true), 
                      Point(dest.x - SOCKET_WIDTH / 2, dest.y), c);
        }
    }
}

void SceneGraphView::DrawSocketName(SDC dc, int x, int y, const char *name,
                                    bool rightAlign)
{
    const int margin = 3;
    int width = swGetStringWidth(swGetDefaultFont(), name) + margin * 2;
    int descent = swGetFontDescent(swGetDefaultFont());
    int ascent = swGetFontAscent(swGetDefaultFont());
    int height = ascent + descent + margin * 2;
    if (rightAlign) x -= width;
    y -= height / 2;

    swSetFGColor(dc, m_face);
    swFillRect(dc, x, y, width, height);
    swDraw3DRect(dc, m_window, x, y, width, height);
    swSetFGColor(dc, 0x000000);
    swDrawText(dc, x + margin, y + ascent + margin, name);
}

void SceneGraphView::InvalidateNode(Node *node)
{
    if (!node) return;

    int eventIns = node->getProto()->getNumEventIns();
    int eventOuts = node->getProto()->getNumEventOuts();
    int x = 0, y = 0, width = 1, height = 1;

    getZoomGraphPosition(x, y, node);
    getZoomGraphSize(width, height, node);
    swInvalidateRect(m_window, x, y, width + 1, height + 1);
    for (int i = 0; i < eventOuts; i++) {
        for (SocketList::Iterator *j = node->getOutput(i).first(); j != NULL;
             j = j->next()) {
            RouteSocket s = j->item();
            InvalidateRoute(GetSocketPosition(node, i, 1), 
                            GetSocketPosition(s.getNode(), s.getField(), 0) - 
                            Point(SOCKET_WIDTH / 2, 0));
        }
    }
    for (int i = 0; i < eventIns; i++) {
        for (SocketList::Iterator *j = node->getInput(i).first(); j != NULL;
             j = j->next()) {
            RouteSocket s = j->item();
            InvalidateRoute(GetSocketPosition(s.getNode(), s.getField(), 1),
                            GetSocketPosition(node, i, 0) - 
                            Point(SOCKET_WIDTH / 2, 0));
        }
    }
    if (node->isInsideProto()) {
        Proto *proto = node->getOutsideProto();
        InvalidateProto(proto);
    }
}


void SceneGraphView::InvalidateProto(Proto *proto)
{
    if (!proto) return;

    int x = 0, y = 0, width = 1, height = 1;

    getZoomGraphPosition(x, y, proto);
    getZoomGraphSize(width, height, proto);
    swInvalidateRect(m_window, x, y, width + 1, height + 1);

    int eventOuts = proto->getNumEventOuts();
    for (int i = 0; i < eventOuts; i++) {
        for (int j = 0; j < proto->getEventOut(i)->getNumIs(); j++) {
            Point dest = GetSocketPosition(
                             proto->getEventOut(i)->getIsNode(j), 
                             proto->getEventOut(i)->getIsField(j), 1);
            InvalidateRoute(GetSocketPosition(proto, i, 1, false), 
                            Point(dest.x - SOCKET_WIDTH / 2, dest.y));
        }
    }

    int eventIns = proto->getNumEventIns();
    for (int i = 0; i < eventIns; i++) {
        for (int j = 0; j < proto->getEventIn(i)->getNumIs(); j++) {
            Point dest = GetSocketPosition(
                             proto->getEventIn(i)->getIsNode(j), 
                             proto->getEventIn(i)->getIsField(j), 0);
            InvalidateRoute(GetSocketPosition(proto, i, 0, false), 
                            Point(dest.x - SOCKET_WIDTH / 2, dest.y));
        }
    }

    int fields = proto->getNumFields();
    for (int i = 0; i < fields; i++) {
        for (int j = 0; j < proto->getField(i)->getNumIs(); j++) {
            Point dest = GetSocketPosition(
                             proto->getField(i)->getIsNode(j), 
                             proto->getField(i)->getIsField(j), 0, true);
            InvalidateRoute(GetSocketPosition(proto, i, 0, true), 
                            Point(dest.x - SOCKET_WIDTH / 2, dest.y));
        }
    }

    int exposedFields = proto->getNumExposedFields();
    for (int i = 0; i < exposedFields; i++) {
        for (int j = 0; j < proto->getExposedField(i)->getNumIs(); j++) {
            Point dest = GetSocketPosition(
                             proto->getExposedField(i)->getIsNode(j), 
                             proto->getExposedField(i)->getIsField(j), 1, true);
            InvalidateRoute(GetSocketPosition(proto, i, 1, true), 
                            Point(dest.x - SOCKET_WIDTH / 2, dest.y));
        }
    }
}

void SceneGraphView::InvalidateNodeRec(Node *node)
{
    Position(node);
    InvalidateNode(node);
    for (int i = 0; i < node->getProto()->getNumFields(); i++) {
        if (node->getField(i) != NULL) {
            if (node->getField(i)->getType() == SFNODE) {
                Node *child = ((SFNode *) node->getField(i))->getValue();
                if (child != NULL) InvalidateNodeRec(child);
            } else if (node->getField(i)->getType() == MFNODE) {
                NodeList *value = ((MFNode *) node->getField(i))->getValues();
                for (int j = 0; j < value->size(); j++) {
                    InvalidateNodeRec(value->get(j));
                }
            }
        }
    }
}

Node *SceneGraphView::HitTestNode(int x, int y) const
{
    for (int i = 0; i < m_nodesOrProtos.size(); i++) {
        Node *node = m_nodesOrProtos.get(i).getNode();
        if (node) {
            int nx = 0, ny = 0, width = 1, height = 1;
            getZoomGraphPosition(nx, ny, node);
            getZoomGraphSize(width, height, node);
            if (x >= nx && y >= ny && x < nx + width && y < ny + height)
                return node;
        }
    }
    return NULL;
}

int 
SceneGraphView::SocketHitTestNode(int x, int y, Node *node, int *side) const
{
    int nx = 0, ny = 0, width = 1, height = 1;

    if (m_zoom != 1.0)
        return -1;
    getZoomGraphPosition(nx, ny, node);
    getZoomGraphSize(width, height, node);
    int px = x - nx;
    int py = y - ny;
    int sock = (py - BORDER_WIDTH - SOCKET_SPACING) 
               / (SOCKET_HEIGHT + SOCKET_SPACING);
    int socketsIn = node->getProto()->getNumEventIns();
    int socketsOut = node->getProto()->getNumEventOuts();
    if (node->isNodeWithAdditionalEvents()) {
        socketsIn++;
        socketsOut++;
    }
    int sockets = MAX(socketsIn, socketsOut);
    int fields = node->getProto()->getNumFields();
    int exposedFields = node->getProto()->getNumExposedFields();
    if (node->isInsideProto()) {
        socketsIn = sockets + fields;
        socketsOut = sockets + exposedFields;
    }

    if (px >= BORDER_WIDTH + SOCKET_SPACING
        && px < BORDER_WIDTH + SOCKET_SPACING + SOCKET_WIDTH 
        && sock < socketsIn) {
        *side = 0;  // left side, inputs
        return sock;
    } else if (px >= width - BORDER_WIDTH - SOCKET_WIDTH - SOCKET_SPACING
               && px < width - BORDER_WIDTH - SOCKET_SPACING
               && sock < socketsOut) {
        *side = 1;  // right side, outputs
        return sock;
    } else {
        return -1;
    }
}

Proto *SceneGraphView::HitTestProto(int x, int y) const
{
    for (int i = 0; i < m_nodesOrProtos.size(); i++) {
        Proto *proto = m_nodesOrProtos.get(i).getProto();
        if (proto) {
            int nx = 0, ny = 0, width = 1, height = 1;
            getZoomGraphPosition(nx, ny, proto);
            getZoomGraphSize(width, height, proto);
            if (x >= nx && y >= ny && x < nx + width && y < ny + height)
                return proto;
        }
    }
    return NULL;
}

int 
SceneGraphView::SocketHitTestProto(int x, int y, Proto *proto, int *side) 
        const 
{
    int nx = 0, ny = 0, width = 1, height = 1;

    if (m_zoom != 1.0)
        return -1;
    getZoomGraphPosition(nx, ny, proto);
    getZoomGraphSize(width, height, proto);
    int px = x - nx;
    int py = y - ny;
    int sock = (py - BORDER_WIDTH - SOCKET_SPACING) 
               / (SOCKET_HEIGHT + SOCKET_SPACING);
    int socketsIn = proto->getNumEventIns();
    int socketsOut = proto->getNumEventOuts();
    int sockets = MAX(socketsIn, socketsOut);
    int fields = proto->getNumFields();
    int exposedFields = proto->getNumExposedFields();
    socketsIn = sockets + fields + 1;
    socketsOut = sockets + exposedFields + 1;

    if (px >= BORDER_WIDTH + SOCKET_SPACING
        && px < BORDER_WIDTH + SOCKET_SPACING + SOCKET_WIDTH 
        && sock < socketsIn) {
        *side = 0;  // left side, inputs
        return sock;
    } else if (px >= width - BORDER_WIDTH - SOCKET_WIDTH - SOCKET_SPACING
               && px < width - BORDER_WIDTH - SOCKET_SPACING
               && sock < socketsOut) {
        *side = 1;  // right side, outputs
        return sock;
    } else {
        return -1;
    }
}

void SceneGraphView::OnLButtonDown(int x, int y, int modifiers) 
{
    Node *node = HitTestNode(x, y);
    const Path *sel = m_scene->getSelection();
    Node *currentNode = sel ? sel->getNode() : NULL;

    Proto *proto = HitTestProto(x, y);
    Proto *currentProto = NULL;
    if (sel->isProto(m_scene))
        currentProto = sel->getProto(m_scene);

    swSetFocus(m_wnd);
    if (node) {
        if (currentNode != node) {
            m_scene->setSelection(node);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        }
        if (m_dstSocket != -1) {
            m_mode = ROUTING;
            m_srcNode = node;
            m_srcProto = NULL;
            m_srcSocket = m_dstSocket;
            m_srcSide = m_dstSide;
            m_point1 = m_point2 = Point(x, y);
        } else {
            m_mode = TRACKING;
            m_dstNode = node;
            m_dstProto = NULL;
            int nx, ny;
            getZoomGraphPosition(nx, ny, node);
            m_point2 = Point(x - nx, y - ny);
        }
    } else if (proto) {
        if (currentProto != proto) {
            m_scene->setSelection(proto);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        }
        if (m_dstSocket != -1) {
            m_mode = ROUTING;
            m_srcProto = proto;
            m_srcNode = NULL;
            m_srcSocket = m_dstSocket;
            m_srcSide = m_dstSide;
            m_point1 = m_point2 = Point(x, y);
        } else {
            m_mode = TRACKING;
            m_dstProto = proto;
            m_dstNode = NULL;
            int nx, ny;
            getZoomGraphPosition(nx, ny, proto);
            m_point2 = Point(x - nx, y - ny);
        }
    } else {
        m_mode = CUTTING;
        m_point1 = m_point2 = Point(x, y);
    }
    swSetCapture(m_window);
}

int
SceneGraphView::getType(Proto *proto, int elementType, int field)
{
    int type = -1;
    switch (elementType) {
      case EL_EVENT_IN:
        type = proto->getEventIn(field)->getType();
        break;
      case EL_EVENT_OUT:
        type = proto->getEventOut(field)->getType();
        break;
      case EL_FIELD:
        type = proto->getField(field)->getType();
        break;
      case EL_EXPOSED_FIELD:
        type = proto->getExposedField(field)->getType();
        break;
    }
    return type;
}

bool
SceneGraphView::validIs(Proto *src, int srcSocket, int srcSide,
                        Node *dst, int dstSocket, int dstSide)
{
    return addIs(src, srcSocket, srcSide, dst, dstSocket, dstSide, true);
}

bool
SceneGraphView::addIs(Proto *src, int srcSocket, int srcSide,
                      Node *dst, int dstSocket, int dstSide, bool askAddIs)
{
    if ((src == NULL) || (dst == NULL))
        return false;
    if (dst->getOutsideProto() != src)
        return false;
    if ((srcSocket == -1) || (dstSocket == -1))
        return false;
    if ((srcSide == -1) || (dstSide == -1))
        return false;

    int srcEventIns = src->getNumEventIns();
    int srcEventOuts = src->getNumEventOuts(); 
    int srcSockets = MAX(srcEventIns, srcEventOuts);

    int dstEventIns = dst->getProto()->getNumEventIns();
    int dstEventOuts = dst->getProto()->getNumEventOuts();
    int dstSockets = MAX(dstEventIns, dstEventOuts);

    int dstElementType = -1;
    int dstField = -1;    
    int dstExposedField = -1;    
    if (dstSide == 0)
        if (dstSocket < dstEventIns) {
            dstElementType = EL_EVENT_IN;
            dstField = dstSocket;
        } else {
            dstElementType = EL_FIELD;
            dstField = getFieldOfSocket(dst->getProto(), dstSocket - dstSockets);
        }
    else
        if (dstSocket < dstEventOuts) {
            dstElementType = EL_EVENT_OUT;
            dstField = dstSocket;
        } else {
            dstElementType = EL_EXPOSED_FIELD;
            dstField = dstSocket - dstSockets;            
            if (dstField < 0)
                return false;
            dstExposedField = dst->getProto()->getFieldOfExposed(
                                    dst->getProto()->getExposedField(dstField));
        }

    if (dstElementType == -1)
        return false;
    if (dstField < 0)
        return false;
    int dstType = getType(dst->getProto(), dstElementType, dstField);
    int srcField = -1;
    if (srcSide == 0)
        if (srcSocket < srcEventIns) {
            srcField = srcSocket;
            EventIn *eventIn = src->getEventIn(srcField);
            if (eventIn->getType() != dstType)
                return false;
            if (askAddIs)
                return true;
            eventIn->addIs(dst, dstField, dstElementType, src, srcField);
            src->setIsNodeIndex();
        } else {
            srcField = getFieldOfSocket(src, srcSocket - srcSockets - 1);
            Field *field = src->getField(srcField);
            if (field->getType() != dstType)
                return false;
            if (askAddIs)
                return true;
            dstField = dstExposedField != -1 ? dstExposedField : dstField;
            field->addIs(dst, dstField, dstElementType, src, srcField);
            src->setIsNodeIndex();
        }
    else
        if (srcSocket < srcEventOuts) {
            srcField = srcSocket;
            EventOut *eventOut = src->getEventOut(srcField);
            if (eventOut->getType() != dstType)
                return false;
            if (askAddIs)
                return true;
            eventOut->addIs(dst, dstField, dstElementType, src, srcField);
            src->setIsNodeIndex();
        } else {
            srcField = srcSocket - srcSockets - 1;            
            if (srcField  < 0)
                return false;
            ExposedField *field = src->getExposedField(srcField);
            if (field)
                if (field->getType() != dstType)
                    return false;
            if (askAddIs)
                return true;
            if (field) {
                dstField = dstExposedField != -1 ? dstExposedField : dstField;
                field->addIs(dst, dstField, dstElementType, 
                             src, srcField);
            }
            src->setIsNodeIndex();
        }
    m_scene->updateNodePROTOs(src);
    return true;
}    

void SceneGraphView::OnLButtonUp(int x, int y, int modifiers) 
{
    swReleaseCapture(m_window);
    if (m_autoScrolling) {
        swKillTimer(m_timer);
        m_autoScrolling = false;
    }
    if (m_mode == TRACKING) {
        m_dstNode = NULL;
    } else if (m_mode == ROUTING) {
        if (m_srcNode && m_dstNode != NULL && m_dstSocket != -1 && 
            m_dstSide != m_srcSide) {
            // got a route, try to assign it
            Node *src, *dst;
            int srcSocket, dstSocket;

            if (m_srcSide == 0) {
                // routing backwards
                src = m_dstNode;
                dst = m_srcNode;
                srcSocket = m_dstSocket;
                dstSocket = m_srcSocket;
            } else {
                src = m_srcNode;
                dst = m_dstNode;
                srcSocket = m_srcSocket;
                dstSocket = m_dstSocket;
            }
            if (Scene::validRoute(src, srcSocket, dst, dstSocket)) {
                m_scene->execute(
                    new RouteCommand(src, srcSocket, dst, dstSocket), this);
            }   
        } else if (m_srcProto != NULL && m_dstNode != NULL &&
                   m_srcSocket != -1 && m_dstSocket != -1) {
            // got a IS, try to assign it
            Proto *src;
            Node *dst;
            int srcSocket, dstSocket;
            int srcSide, dstSide;

            src = m_srcProto;
            dst = m_dstNode;
            srcSocket = m_srcSocket;
            dstSocket = m_dstSocket;
            srcSide = m_srcSide;            
            dstSide = m_dstSide;

            addIs(src, srcSocket, srcSide, dst, dstSocket, dstSide);
        } else if (m_srcNode != NULL && m_dstProto != NULL && 
                   m_srcSocket != -1 && m_dstSocket != -1) {
            // got a IS, try to assign it
            Proto *src;
            Node *dst;
            int srcSocket, dstSocket;
            int srcSide, dstSide;

            src = m_dstProto;
            dst = m_srcNode;
            srcSocket = m_dstSocket;
            dstSocket = m_srcSocket;
            srcSide = m_dstSide;            
            dstSide = m_srcSide;

            addIs(src, srcSocket, srcSide, dst, dstSocket, dstSide);
        }
        InvalidateNode(m_srcNode);
        InvalidateProto(m_srcProto);
        InvalidateNode(m_dstNode);
        InvalidateProto(m_dstProto);
    } else if (m_mode == CUTTING) {
        CutRoutes(m_point1, m_point2);
        int top = MIN(m_point1.y, m_point2.y);
        int bottom = MAX(m_point1.y, m_point2.y);
        int left = MIN(m_point1.x, m_point2.x);
        int right = MAX(m_point1.x, m_point2.x);
        swInvalidateRect(m_window, left, top, right - left + 1, bottom - top + 1);
    }
    m_mode = NONE;
}

void SceneGraphView::CutRoutes(const Point &start, const Point &end)
{
    CommandList *list = NULL;

    for (int i = 0; i < m_nodesOrProtos.size(); i++) {
        Node *node = m_nodesOrProtos.get(i).getNode();
        Proto *proto = m_nodesOrProtos.get(i).getProto();
        if (node) {
            int eventOuts = node->getProto()->getNumEventOuts();
            for (int j = 0; j < eventOuts; j++) {
                for (SocketList::Iterator *k = node->getOutput(j).first(); 
                     k != NULL; k = k->next()) {
                    RouteSocket  s = k->item();
                    if (IntersectRoute(start, end, 
                                       GetSocketPosition(node, j, 1), 
                                       GetSocketPosition(s.getNode(), 
                                                         s.getField(), 0) - 
                                       Point(SOCKET_WIDTH / 2, 0))) {
                        if (!list) list = new CommandList();
                        list->append(new UnRouteCommand(node, j, 
                                                        s.getNode(), 
                                                        s.getField()));
                    }
                }
            }
        }
        if (list) m_scene->execute(list);
        if (proto) {
            int eventOuts = proto->getNumEventOuts();
            for (int j = 0; j < eventOuts; j++) {
                EventOut *eventOut = proto->getEventOut(j);
                for (int k = 0; k < eventOut->getNumIs(); k++) {
                     Point dest = GetSocketPosition(
                                     eventOut->getIsNode(k), 
                                     eventOut->getIsField(k), 1);
                     Point src = GetSocketPosition(proto, j, 1, false);
                     if (IntersectRoute(start, end, src, dest  - 
                                        Point(SOCKET_WIDTH / 2, 0))) {
                         eventOut->removeIs(k);
                         swInvalidateWindow(m_window);
                     }
                }
            }
            int eventIns = proto->getNumEventIns();
            for (int j = 0; j < eventIns; j++) {
                EventIn *eventIn = proto->getEventIn(j);
                for (int k = 0; k < eventIn->getNumIs(); k++) {
                    Point dest = GetSocketPosition(eventIn->getIsNode(k), 
                                                   eventIn->getIsField(k), 0);
                    Point src = GetSocketPosition(proto, j, 0, false); 
                    if (IntersectRoute(start, end, src,
                                       Point(dest.x - SOCKET_WIDTH / 2, 
                                             dest.y))) {
                        eventIn->removeIs(k);
                        swInvalidateWindow(m_window);
                    }
                }
            }
            for (int j = 0; j < proto->getNumFields(); j++) {
                Field *field = proto->getField(j);
                for (int k = 0; k < field->getNumIs(); k++) {
                    int dSide = destSide(field->getIsElementType(k));
                    int sSide = 0;
                    int sField = j;
                    if (field->getExposedField()) {
                        sSide = 1;
                        sField = proto->getExposedOfField(field);
                    }
                    Point dest = GetSocketPosition(field->getIsNode(k), 
                                                   field->getIsField(k), dSide, 
                                                   true);
                    Point src = GetSocketPosition(proto, sField, sSide, true);
                    if (IntersectRoute(start, end, src,
                                       Point(dest.x - SOCKET_WIDTH / 2, 
                                             dest.y))) {
                        field->removeIs(k);
                        swInvalidateWindow(m_window);
                    }
                }
            }
            int exposedFields = proto->getNumExposedFields();
            for (int j = 0; j < exposedFields; j++) {
                ExposedField *exposedField = proto->getExposedField(j);
                for (int k = 0; k < exposedField->getNumIs(); k++) {
                    int side = destSide(exposedField->getIsElementType(k));
                    Point dest = GetSocketPosition(
                                     exposedField->getIsNode(k), 
                                     exposedField->getIsField(k), side, true);
                    Point src = GetSocketPosition(proto, j + 1, 1, true);
                    if (IntersectRoute(start, end, src,
                                       Point(dest.x - SOCKET_WIDTH / 2, 
                                             dest.y))) {
                        exposedField->removeIs(k);
                        swInvalidateWindow(m_window);
                    }
                }
            }
        }
    }
}
     
void SceneGraphView::OnMouseMove(int x, int y, int modifiers) 
{
    if (m_mode != NONE) {
        CheckAutoScroll(x, y);
        setLastXPosition(MAX(m_lastXPosition, x));
        setLastYPosition(MAX(m_lastYPosition, y));
        if (TheApp->GetRouteViewShowAll())
            m_maxYPosition = MAX(m_maxYPosition, m_lastYPosition);
        else
            m_maxYPosition = MAX(getFirstYPosition(), y);
        SetScrollSizes(m_lastXPosition, m_maxYPosition);
    }
    DoMouseMove(x, y, modifiers);
}

void SceneGraphView::DoMouseMove(int px, int py, int modifiers)
{
    if ((m_mode == TRACKING) && (m_dstNode != NULL)) {
        int oldX = 0, oldY = 0, width = 1, height = 1;
        getZoomGraphPosition(oldX, oldY, m_dstNode);
        getZoomGraphSize(width, height, m_dstNode);
        InvalidateNode(m_dstNode);
        int newX = MAX(px - m_point2.x, 0);
        int newY = MAX(py - m_point2.y, 0);
        if (newX != oldX || newY != oldY) {
            InvalidateNode(m_dstNode);
            setZoomGraphPosition(m_dstNode, newX, newY);
            InvalidateNode(m_dstNode);
            setLastXPosition(MAX(m_lastXPosition, newX + width + LINE_STUB));
            setLastYPosition(MAX(m_lastYPosition, newY + height));
            if (TheApp->GetRouteViewShowAll())
                m_maxYPosition = MAX(m_maxYPosition, m_lastYPosition);
            else
                m_maxYPosition = MAX(getFirstYPosition(), newY + height);
            if (m_maxYPosition > GetYEndRouteBlock())
                m_YNodesOnlyEventOuts = m_maxYPosition;
            int sx, sy;
            swGetScrollPosition(m_scroller, &sx, &sy);
            SetScrollSizes(m_lastXPosition, m_maxYPosition);
            swSetScrollPosition(m_scroller, sx, sy);
        }
    } else if ((m_mode == TRACKING) && (m_dstProto != NULL)) {
        int oldX = 0, oldY = 0, width = 1, height = 1;
        getZoomGraphPosition(oldX, oldY, m_dstProto);
        getZoomGraphSize(width, height, m_dstProto);
        InvalidateProto(m_dstProto);
        int newX = MAX(px - m_point2.x, 0);
        int newY = MAX(py - m_point2.y, 0);
        if (newX != oldX || newY != oldY) {
            InvalidateProto(m_dstProto);
            setZoomGraphPosition(m_dstProto, newX, newY);
            InvalidateProto(m_dstProto);
            setLastXPosition(MAX(m_lastXPosition, newX + width + LINE_STUB));
            setLastYPosition(MAX(m_lastYPosition, newY + height));
            if (TheApp->GetRouteViewShowAll())
                m_maxYPosition = MAX(m_maxYPosition, m_lastYPosition);
            else
                m_maxYPosition = MAX(getFirstYPosition(), newY + height);
            if (m_maxYPosition > GetYEndRouteBlock())
                m_YNodesOnlyEventOuts = m_maxYPosition;
            int sx, sy;
            swGetScrollPosition(m_scroller, &sx, &sy);
            SetScrollSizes(m_lastXPosition, m_maxYPosition);
            swSetScrollPosition(m_scroller, sx, sy);
        }
    } else if (m_mode == ROUTING) {
        if (m_srcNode == NULL) {
            InvalidateRoute(GetSocketPosition(m_srcProto, m_srcSocket, 
                                              m_srcSide, true), m_point1);
            InvalidateRoute(GetSocketPosition(m_srcProto, m_srcSocket, 
                                              m_srcSide, false), m_point1);
        } else
            InvalidateRoute(GetSocketPosition(m_srcNode, m_srcSocket, 
                                              m_srcSide), m_point1);
        m_point1 = Point(px, py);
        if (m_srcNode == NULL) {
            InvalidateRoute(GetSocketPosition(m_srcProto, m_srcSocket, 
                                              m_srcSide, true), m_point1);
            InvalidateRoute(GetSocketPosition(m_srcProto, m_srcSocket, 
                                              m_srcSide, false), m_point1);
        } else
            InvalidateRoute(GetSocketPosition(m_srcNode, m_srcSocket, 
                                              m_srcSide), m_point1);

        HighlightSocket(m_point1);
    } else if (m_mode == CUTTING) {
        int top, left, right, bottom;
        top = MIN(m_point1.y, m_point2.y);
        bottom = MAX(m_point1.y, m_point2.y);
        left = MIN(m_point1.x, m_point2.x);
        right = MAX(m_point1.x, m_point2.x);
        swInvalidateRect(m_window, left, top, right - left + 1, bottom - top + 1);
        m_point2 = Point(px, py);
        top = MIN(m_point1.y, m_point2.y);
        bottom = MAX(m_point1.y, m_point2.y);
        left = MIN(m_point1.x, m_point2.x);
        right = MAX(m_point1.x, m_point2.x);
        swInvalidateRect(m_window, left, top, right - left + 1, bottom - top + 1);
    } else {
        HighlightSocket(Point(px, py));
    }
}

static int
timerCB(void *data)
{
    return ((SceneGraphView *) data)->OnTimer();
}

void SceneGraphView::CheckAutoScroll(int px, int py)
{
    bool autoScroll = true;
    int x = 0, y = 0, width = 1, height = 1, sx = 0, sy = 0;
    int dx = 0, dy = 0;

    swGetPosition(m_scroller, &x, &y);
    swGetScrollViewportSize(m_scroller, &width, &height);
    swGetScrollPosition(m_scroller, &sx, &sy);
    px -= sx;
    py -= sy;
    m_autoScrollPX = px;
    m_autoScrollPY = py;

    if (px < AUTOSCROLL_MARGIN) {
        dx = -AUTOSCROLL_AMOUNT;
    } else if (px > x + width - AUTOSCROLL_MARGIN) {
        dx = AUTOSCROLL_AMOUNT;
    } else if (py < AUTOSCROLL_MARGIN) {
        dy = -AUTOSCROLL_AMOUNT;
    } else if (py > y + height - AUTOSCROLL_MARGIN) {
        dy = AUTOSCROLL_AMOUNT;
    } else {
        autoScroll = false;
    }

    if (autoScroll && !m_autoScrolling) {   // start scrolling
        m_timer = swSetTimer(m_window, 70, timerCB, this);
        m_autoScrollDX = dx;
        m_autoScrollDY = dy;
        m_autoScrolling = true;
    }
    if (!autoScroll && m_autoScrolling) {   // stop scrolling
        swKillTimer(m_timer);
        m_autoScrolling = false;
    }
}

void SceneGraphView::HighlightSocket(Point point)
{
    // check if we're over a socket
    Node *node = HitTestNode(point.x, point.y);
    Proto *proto = HitTestProto(point.x, point.y);
    int socket = -1;
    int side = -1;

    if (node) {
        socket = SocketHitTestNode(point.x, point.y, node, &side);
    } else if (proto) {
        socket = SocketHitTestProto(point.x, point.y, proto, &side);
    }

    if (m_dstNode && node != m_dstNode) {
        InvalidateNode(m_dstNode);
    } else if (node && (socket != m_dstSocket || side != m_dstSide)) {
        InvalidateNode(node);
    }

    if (m_dstProto && proto != m_dstProto) {
        InvalidateProto(m_dstProto);
    } else if (proto && (socket != m_dstSocket || side != m_dstSide)) {
        InvalidateProto(proto);
    }
    m_dstNode = node;
    m_dstProto = proto;
    m_dstSocket = socket;
    m_dstSide = side;
}

void SceneGraphView::OnUpdate(SceneView *sender, int type, Hint *hint) 
{
    NodeUpdate *nodeUpdate;
    RouteUpdate *routeUpdate;
    ProtoUpdate *protoUpdate;

    switch (type) {
      CASE_UPDATE(UPDATE_ALL)
        Initialize();
        break;
      CASE_UPDATE(UPDATE_SELECTION)
      CASE_UPDATE(UPDATE_SELECTION_NAME)
        if (swIsVisible(m_wnd)) {
            if (m_currentProto != m_scene->getSelection()->getProto(m_scene)) {
                m_currentProto = m_scene->getSelection()->getProto(m_scene);
                Initialize();
            }
            InvalidateNode(m_scene->getSelection()->getNode());
            InvalidateProto(m_scene->getSelection()->getProto(m_scene));
            swInvalidateWindow(m_window);
        }
        break;
      CASE_UPDATE(UPDATE_CHANGE_INTERFACE_NODE)
        nodeUpdate = (NodeUpdate *) hint;
        // when the number of events change, size can change
        accountGraphSize(nodeUpdate->node);
      CASE_UPDATE(UPDATE_ADD_NODE)
      CASE_UPDATE(UPDATE_ADD_NODE_SCENE_GRAPH_VIEW)
        nodeUpdate = (NodeUpdate *) hint;
        accountGraphSize(nodeUpdate->node);
        InvalidateNodeRec(nodeUpdate->node);
        Initialize();
        if (!TheApp->GetRouteViewShowAll()) {
            m_maxYPosition = getFirstYPosition();
            // automatically show new created event processing nodes on top 
            if (nodeUpdate->node != NULL) {
                if ((!nodeUpdate->node->hasInputs()) && 
                    (!nodeUpdate->node->hasOutputs()))
                     if ((nodeUpdate->node->getNodeClass() & SENSOR_NODE) ||
                         nodeUpdate->node->matchNodeClass(INTERPOLATOR_NODE) ||
                         nodeUpdate->node->isNodeWithAdditionalEvents())
                          moveToTop(nodeUpdate->node);
            }
        }
        SetScrollSizes(m_lastXPosition, m_maxYPosition);
        break;
      CASE_UPDATE(UPDATE_REMOVE_NODE)
        nodeUpdate = (NodeUpdate *) hint;
        accountGraphSize(nodeUpdate->node);
        InvalidateNodeRec(nodeUpdate->node);
        if (!TheApp->GetRouteViewShowAll())
            m_maxYPosition = getFirstYPosition();
        SetScrollSizes(m_lastXPosition, m_maxYPosition);
        Initialize();
        swInvalidateWindow(m_window);
        break;
      CASE_UPDATE(UPDATE_ADD_ROUTE)
        routeUpdate = (RouteUpdate *) hint;
        if (!TheApp->GetRouteViewShowAll() && (sender != this)) {
            int width = 1, height = 1;

            getZoomGraphPosition(width, height, routeUpdate->src);
            if (routeUpdate->src->hasOutputsOrIs() && 
                !routeUpdate->src->hasInputsOrIs()) {
                setZoomGraphPosition(routeUpdate->src, 
                                     m_XNodesOnlyEventOuts, height); 
                m_XNodesOnlyEventOuts += TheApp->GetRoutedNodeIndent();
                m_XNodesBothEvents += TheApp->GetRoutedNodeIndent();
                m_XNodesOnlyEventIns += TheApp->GetRoutedNodeIndent();
            } else if (!routeUpdate->src->hasOutputsOrIs() && 
                     routeUpdate->src->hasInputsOrIs()) {
                setZoomGraphPosition(routeUpdate->src, 
                                         m_XNodesOnlyEventIns, height);
                m_XNodesOnlyEventIns += TheApp->GetRoutedNodeIndent();
            } else {
                setZoomGraphPosition(routeUpdate->src, 
                                     m_XNodesBothEvents, height);
                m_XNodesBothEvents += TheApp->GetRoutedNodeIndent();
                m_XNodesOnlyEventIns += TheApp->GetRoutedNodeIndent();
            }
            setLastXPosition(MAX(m_lastXPosition, ADD_WIDTH + width));

            getZoomGraphPosition(width, height, routeUpdate->dest);
            if (routeUpdate->dest->hasOutputs() && 
                !routeUpdate->dest->hasInputs()) {
                setZoomGraphPosition(routeUpdate->dest, 
                                     m_XNodesOnlyEventOuts, height); 
                m_XNodesOnlyEventOuts += TheApp->GetRoutedNodeIndent();
                m_XNodesBothEvents += TheApp->GetRoutedNodeIndent();
                m_XNodesOnlyEventIns += TheApp->GetRoutedNodeIndent();
            } else if (!routeUpdate->dest->hasOutputs() &&  
                routeUpdate->dest->hasInputs()) {
                setZoomGraphPosition(routeUpdate->dest, 
                                     m_XNodesOnlyEventIns, height);
                m_XNodesOnlyEventIns += TheApp->GetRoutedNodeIndent();
            } else {
                setZoomGraphPosition(routeUpdate->dest, 
                                     m_XNodesBothEvents, height);
                m_XNodesBothEvents += TheApp->GetRoutedNodeIndent();
                m_XNodesOnlyEventIns += TheApp->GetRoutedNodeIndent();
            }
            setLastXPosition(MAX(m_lastXPosition, ADD_WIDTH + width));

            SetScrollSizes(m_lastXPosition, m_maxYPosition);
            swInvalidateWindow(m_window);
        }
      CASE_UPDATE(UPDATE_DELETE_ROUTE)
        routeUpdate = (RouteUpdate *) hint;
        InvalidateRoute(GetSocketPosition(routeUpdate->src,
                                          routeUpdate->eventOut, 1), 
                        GetSocketPosition(routeUpdate->dest,
                                          routeUpdate->eventIn, 0)
                          - Point(SOCKET_WIDTH / 2, 0));
        break;
      CASE_UPDATE(UPDATE_PROTO)
        protoUpdate = (ProtoUpdate*) hint;
        accountGraphSize(protoUpdate->proto);
        swInvalidateWindow(m_window);
        break;              
      CASE_UPDATE(UPDATE_REDRAW)
        swInvalidateWindow(m_window);
        break;      
    }
}

int 
SceneGraphView::getFieldOfSocket(Proto *proto, int sock) const
{
    int count = sock + 1;
    for (int i = 0; i < proto->getNumFields(); i++) {
        if (!(proto->getField(i)->getFlags() & FF_HIDDEN))
            count--;
        if (count <= 0)
            return i;                       
    }
    return proto->getNumFields() - 1;
}

int 
SceneGraphView::getSocketOfField(Proto *proto, int sock) const
{
    Field *field = proto->getField(sock);
    const char *fieldName = field->getName(m_scene->isX3d());
    int socket = -1;
    for (int i = 0; i < proto->getNumFields(); i++) {
        if (!(proto->getField(i)->getFlags() & FF_HIDDEN))
            socket++;
        if (strcmp(proto->getField(i)->getName(m_scene->isX3d()), fieldName) 
            == 0)
            return socket;                       
    }
    return -1;
}

int 
SceneGraphView::getSocketOfExposedField(Proto *proto, int sock) const
{
    Field *field = proto->getField(sock);
    const char *fieldName = field->getName(m_scene->isX3d());
    int socket = -1;
    for (int i = 0; i < proto->getNumFields(); i++) {
        if (proto->getField(i)->getFlags() & FF_HIDDEN)
            socket++;
        if (strcmp(proto->getField(i)->getName(m_scene->isX3d()),
                   fieldName) == 0)
            return socket;                       
    }
    return -1;
}

Point SceneGraphView::GetSocketPosition(Node *node, int sock, int side, 
                                        bool isField) const
{
    int x = 0, y = 0, width = 1, height = 1;
    
    int socketsIn = node->getProto()->getNumEventIns();
    int socketsOut = node->getProto()->getNumEventOuts();
    int sockets = MAX(socketsIn, socketsOut);
    
    int socket = sock;
    if (isField && (side == 0)) {
        socket = getSocketOfField(node->getProto(), sock);
    } else if (isField && (side == 1)) {
        socket = getSocketOfExposedField(node->getProto(), sock);
    }
    if (isField)
        socket += sockets;

    getZoomGraphPosition(x, y, node);
    getZoomGraphSize(width, height, node);
    y += (BORDER_WIDTH + ((SOCKET_SPACING + SOCKET_HEIGHT) * 
                          (2 * socket + 1)) / 2) * m_zoom;
    if (side == 0) { // left
        x += (BORDER_WIDTH + SOCKET_SPACING + SOCKET_WIDTH / 2) * m_zoom;
    } else {
        x += width - (BORDER_WIDTH + SOCKET_SPACING  + SOCKET_WIDTH / 2) * 
                     m_zoom;
    }
    return Point(x, y);
}

Point SceneGraphView::GetSocketPosition(Proto *proto, int sock, int side, 
                                        bool isField) const
{
    int x = 0, y = 0, width = 1, height = 1;

    int socketsIn = proto->getNumEventIns();
    int socketsOut = proto->getNumEventOuts();
    int sockets = MAX(socketsIn, socketsOut);
    
    int socket = sock;
    if (isField)
        socket += sockets;
    
    getZoomGraphPosition(x, y, proto);
    getZoomGraphSize(width, height, proto);
    y += (BORDER_WIDTH + ((SOCKET_SPACING + SOCKET_HEIGHT) * 
                          (2 * socket + 1)) / 2) * m_zoom;
    if (side == 0) { // left
        x += (BORDER_WIDTH + SOCKET_SPACING + SOCKET_WIDTH / 2) * m_zoom;
    } else {
        x += width - (BORDER_WIDTH + SOCKET_SPACING  + SOCKET_WIDTH / 2) * 
                     m_zoom;
    }
    return Point(x, y);
}

SBITMAP
SceneGraphView::LoadSocketBitmap(int id, int color)
{
    int mapFrom[4] = { 0x808000, 0x000000, 0xC0C0C0, 0x808080 };
    int mapTo[4] = { m_face,
      SW_RGB(SW_RED(color) / 3, SW_GREEN(color) / 3, SW_BLUE(color) / 3),
      SW_RGB(MIN(SW_RED(color) + 128, 255),
             MIN(SW_GREEN(color) + 128, 255),
             MIN(SW_BLUE(color) + 128, 255)),
             color
    };

    return swLoadMappedBitmap(m_window, id, mapFrom, mapTo, 4);
}

void
SceneGraphView::BuildSocketBitmaps()
{
    for (int i = 0; i < LAST_TYPE + 2 ; i++) {
        m_typeColors[i] = 0xFFFFFF;
    }

    int numColors = 0;

    m_typeColors[SFBOOL]      = 0x7F7F7F;
    numColors++;
    m_typeColors[MFBOOL]      = 0x1F1F1F;
    numColors++;

    m_typeColors[SFCOLOR]     = 0x2F4FCF;
    numColors++;
    m_typeColors[MFCOLOR]     = 0x3F3F7F;
    numColors++;

    m_typeColors[SFCOLORRGBA] = 0x1FCFCF;
    numColors++;
    m_typeColors[MFCOLORRGBA] = 0x3F7F7F;
    numColors++;

    m_typeColors[SFDOUBLE]    = 0xCFFFFF;
    numColors++;
    m_typeColors[MFDOUBLE]    = 0x5F7F7F;
    numColors++;

    m_typeColors[SFFLOAT]     = 0xCFCFFF;
    numColors++;
    m_typeColors[MFFLOAT]     = 0x5F5F7F;
    numColors++;

    m_typeColors[SFIMAGE]     = 0xCF3FCF;
    numColors++;
    
    m_typeColors[SFINT32]     = 0x00FFFF;
    numColors++;
    m_typeColors[MFINT32]     = 0x007F7F; 
    numColors++;

    m_typeColors[SFNODE]      = 0xFF1F7F;
    numColors++;
    m_typeColors[MFNODE]      = 0xCF005F;
    numColors++;

    m_typeColors[SFROTATION]  = 0xCF00CF;
    numColors++;
    m_typeColors[MFROTATION]  = 0x7F007F;
    numColors++;

    m_typeColors[SFSTRING]    = 0xCFCF00;
    numColors++;
    m_typeColors[MFSTRING]    = 0x7F7F00;
    numColors++;

    m_typeColors[SFTIME]      = 0xCF5F00;
    numColors++;
    m_typeColors[MFTIME]      = 0x7F5F00;
    numColors++;

    m_typeColors[SFVEC2F]     = 0x1F7F7F;
    numColors++;
    m_typeColors[MFVEC2F]     = 0x005F5F;
    numColors++;

    m_typeColors[SFVEC2D]     = 0x1F7F00;
    numColors++;
    m_typeColors[MFVEC2D]     = 0x005F00;
    numColors++;

    m_typeColors[SFVEC3F]     = 0xFF0000;
    numColors++;
    m_typeColors[MFVEC3F]     = 0x7F4F4F;
    numColors++;

    m_typeColors[SFVEC3D]     = 0xFF3737;
    numColors++;
    m_typeColors[MFVEC3D]     = 0x7F7777;
    numColors++;

    m_typeColors[SFVEC4F]     = 0xFF1717;
    numColors++;
    m_typeColors[SFVEC4F]     = 0xFF5757;
    numColors++;

    m_typeColors[SFMATRIX3F]  = 0xCFFFCF;
    numColors++;
    m_typeColors[SFMATRIX4F]  = 0xCF77CF;
    numColors++;

    m_typeColors[MFMATRIX3F]  = 0x1FFF1F;
    numColors++;
    m_typeColors[MFMATRIX4F]  = 0x1F001F;
    numColors++;

    m_typeColors[LAST_TYPE + 1] = 0xFFFFFF;
    numColors++;

    // warn, if a new added type got no color
    if (numColors != (LAST_TYPE + 2))
        m_scene->errorf("Internal error: not enough colors (%d of %d) in RouteView\n",
                       numColors, (LAST_TYPE + 2));

    for (int i = 0; i < LAST_TYPE + 2 ; i++) {
        m_socketBitmaps[i] = LoadSocketBitmap(IDB_FULL_SOCKET, m_typeColors[i]);
    }
}

int 
SceneGraphView::OnTimer() 
{
    int sx, sy;
    int w, h, vw, vh;

    swGetScrollPosition(m_scroller, &sx, &sy);
    swGetSize(m_window, &w, &h);
    swGetScrollViewportSize(m_scroller, &vw, &vh);
    sx += m_autoScrollDX;
    if (sx >= w - vw) sx = w - vw;
    if (sx < 0) sx = 0;
    sy += m_autoScrollDY;
    if (sy >= h - vh) 
        sy = h - vh;
    if (sy < 0) 
        sy = 0;
    if (sy > MAX_Y) 
        sy = MAX_Y;
    if (sy > m_maxYPosition) 
        sy = m_maxYPosition;
    swSetScrollPosition(m_scroller, sx, sy);
    DoMouseMove(m_autoScrollPX + sx, m_autoScrollPY + sy, 0);
    return TRUE;  // continue timer
}

void
SceneGraphView::getZoomGraphPosition(int &width, int &height, Node *node) const
{
    float w = 1, h = 1;
    node->getGraphPosition(&w, &h);
    width = (int) w * m_zoom; 
    height = (int) h * m_zoom; 
}

void
SceneGraphView::setZoomGraphPosition(Node *node, int x, int y)
{
    node->setGraphPosition(x / m_zoom, y / m_zoom);
}

void
SceneGraphView::getZoomGraphSize(int &width, int &height, Node *node) const
{
    node->getGraphSize(&width, &height);
    width *= m_zoom; 
    if (width == 0)
        width = 1;
    height *= m_zoom; 
    if (height == 0)
        height = 1;
}

void
SceneGraphView::setGraphSize(Node *node, int width, int height)
{
    node->setGraphSize(width, height);
}

void
SceneGraphView::getZoomGraphPosition(int &width, int &height, Proto *proto) const
{
    float w = 1, h = 1;
    proto->getGraphPosition(&w, &h);
    width = (int) w * m_zoom; 
    height = (int) h * m_zoom; 
}

void
SceneGraphView::setZoomGraphPosition(Proto *proto, int x, int y)
{
    proto->setGraphPosition(x / m_zoom, y / m_zoom);
}

void
SceneGraphView::getZoomGraphSize(int &width, int &height, Proto *proto) const
{
    proto->getGraphSize(&width, &height);
    width *= m_zoom; 
    if (width == 0)
        width = 1;
    height *= m_zoom; 
    if (height == 0)
        height = 1;
}

void
SceneGraphView::setGraphSize(Proto *proto, int width, int height)
{
    proto->setGraphSize(width, height);
}

void
SceneGraphView::zoomIn(void)
{ 
    if (m_zoom >= 1.0) { 
        m_zoom = 1.0;
        return;
    }
    setLastXPosition(m_lastXPosition * 2);
    m_maxYPosition *= 2;
    m_zoom *= 2.0; 
    int sx, sy;
    swGetScrollPosition(m_scroller, &sx, &sy);
    SetScrollSizes(m_lastXPosition, m_maxYPosition);
    swSetScrollPosition(m_scroller, sx * 2, sy * 2);
    swInvalidateWindow(m_window);
}

void
SceneGraphView::zoomOut(void)
{ 
    setLastXPosition(m_lastXPosition / 2);
    m_maxYPosition /= 2;
    m_zoom /= 2.0; 
    int sx, sy;
    swGetScrollPosition(m_scroller, &sx, &sy);
    SetScrollSizes(m_lastXPosition, m_maxYPosition);
    swSetScrollPosition(m_scroller, sx / 2, sy / 2);
    swInvalidateWindow(m_window);
}

void
SceneGraphView::unZoom(void)
{ 
    int lastPosition = m_lastXPosition / m_zoom;
    setLastXPosition(lastPosition);
    m_maxYPosition /= m_zoom;
    m_zoom = 1.0; 
    jumpToSelection();
    SetScrollSizes(m_lastXPosition, m_maxYPosition);
    swInvalidateWindow(m_window);
}

void
SceneGraphView::jumpToSelection(void)
{
    const Path *sel = m_scene->getSelection();
    Node *current = sel ? sel->getNode() : NULL;
    if (current == NULL) 
        return;
    if (current == m_scene->getRoot())
        return;
    int sx, sy;
    getZoomGraphPosition(sx, sy, current);
    int width = 1, height = 1;
    swGetScrollViewportSize(m_scroller, &width, &height);
    sx = sx - width / 2;
    if (sx < 0)
        sx = 0;
    sy = sy - height  / 2;
    if (sy < 0)
        sy = 0;
    swSetScrollPosition(m_scroller, sx, sy);
}

void
SceneGraphView::warnFlooded(void)
{
    if (!m_floodedWarning) {
        m_floodedWarning = true;
        TheApp->PrintMessageWindowsId(IDS_ROUTE_VIEW_FLOODED);
    }
}


void
SceneGraphView::setLastXPosition(int x)
{
    if (x < MAX_X)
        m_lastXPosition = x;
}

void
SceneGraphView::setLastYPosition(int y)
{
    m_lastYPosition = y;
}

int
SceneGraphView::getFirstYPosition(void)
{
    int ret = MAX(m_YNodesOnlyEventOuts, m_YNodesBothEvents);
    return MAX(ret, m_YNodesOnlyEventIns);
}

void 
SceneGraphView::SetScrollSizes(int width, int height) 
{
    int x = MAX(width, m_XNodesOnlyEventIns + NODE_WIDTH);
    swSetScrollSizes(m_scroller, x, height);
}
        
void 
SceneGraphView::SetSize(int width, int height)
{
    swSetSize(m_scroller, width, height);
}
           
int
SceneGraphView::GetYEndRouteBlock(void)
{
    int ret = 0;
    if (m_YNodesOnlyEventOuts > ret)
        ret = m_YNodesOnlyEventOuts;
    if (m_YNodesBothEvents > ret)
        ret = m_YNodesBothEvents;
    if (m_YNodesOnlyEventIns > ret)
        ret = m_YNodesOnlyEventIns;
    return ret;
}




