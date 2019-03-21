/*
 * SceneGraphView.h
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

#ifndef _SCENE_GRAPH_VIEW_H
#define _SCENE_GRAPH_VIEW_H

class NodeList;
class Node;
class DuneDoc;
class FieldValue;

#ifndef _SWT_H
#include "swt.h"
#endif

#ifndef _SCENE_VIEW_H
#include "SceneView.h"
#endif

#ifndef _POINT_H
#include "Point.h"
#endif

#include "Types.h"

class NodeOrProto {
protected:
        Node *node;
        Proto *proto;
public:
        NodeOrProto()
            {
            node = NULL;
            proto = NULL;
            }
        NodeOrProto(Node *node) { setNode(node); }
        NodeOrProto(Proto *proto) { setProto(proto); }
        void             setNode(Node *n)
                             {
                             node = n;
                             proto = NULL;
                             }
        Node            *getNode(void) const
                             { return node; }
        void             setProto(Proto *p)
                             {
                             node = NULL;
                             proto = p;
                             }
       Proto             *getProto(void) const
                             { return proto; }
};

class SceneGraphView : public SceneView
{
public:
        SceneGraphView(Scene *scene, SWND parent);
        virtual ~SceneGraphView();

public:
        virtual void    OnUpdate(SceneView *sender, int type, Hint *hint);
        virtual void    OnLButtonDown(int x, int y, int modifiers);
        virtual void    OnLButtonUp(int x, int y, int modifiers);
        virtual void    OnMouseMove(int x, int y, int modifiers);
        void            DoMouseMove(int px, int py, int modifiers);

        virtual void    OnDraw(int x, int y, int width, int height,
                               bool update = false);
        virtual void    OnSize(int width, int height);
        virtual void    OnMouseLeave();

        const char     *getName(void) const { return "SceneGraphView"; }

        void            Initialize();
        void            InitializeNodes(bool isInsideProto);


        typedef enum { 
            NONE, 
            ROUTING, 
            TRACKING, 
            CUTTING 
        } SGMode;
    

        void            Position(Node *node);
        void            accountGraphSize(Node *node);
        void            accountGraphSize(Proto *proto);
        void            accountGraphPosition(Node *node);
        void            accountGraphPosition(Proto *proto);

        void            YPosition(int width, int height);
        int             GetYEndRouteBlock(void);

        void            DrawNode(SDC dc, Node *node, int xPos, int yPos);
        void            DrawProto(SDC dc, Proto *proto, int xPos, int yPos);
        Node           *HitTestNode(int x, int y) const;
        Proto          *HitTestProto(int x, int y) const;
        int             SocketHitTestNode(int x, int y, Node *node, int *side) 
                              const;
        int             SocketHitTestProto(int x, int y, Proto *proto, 
                              int *side) const;
        void            GetItemRect(Node *node, int *x, int *y, int *width, 
                                    int *height);

        void            DrawRoute(SDC dc, Point start, Point end, int color);
        void            InvalidateRoute(Point start, Point end);
        void            DrawRoutes(SDC dc, Node *node);
        int             destSide(int elementType);
        void            DrawIs(SDC dc, Proto *proto);
        void            DrawSocketName(SDC dc, int x, int y, const char *name,
                                       bool rightAlign);
        void            InvalidateNode(Node *node);
        void            InvalidateNodeRec(Node *node);
        void            InvalidateProto(Proto *proto);
        int             getFieldOfSocket(Proto *proto, int sock) const;
        int             getSocketOfField(Proto *proto, int sock) const;
        int             getSocketOfExposedField(Proto *proto, int sock) const;
        Point           GetSocketPosition(Node *node, int socket, int side,
                                          bool isField = false) const;
        Point           GetSocketPosition(Proto *proto, int socket, int side, 
                                          bool isField) const;
        void            HighlightSocket(Point point);
        void            DoMouseMove(int x, int y);
        void            CheckAutoScroll(int x, int y);
        SBITMAP         LoadSocketBitmap(int id, int c);
        void            BuildSocketBitmaps();
        void            CutRoutes(const Point &start, const Point &end);
        bool            IntersectRoute(Point start, Point end,
                                       Point rStart, Point rEnd);
        int             OnTimer();
        void            zoomIn(void);
        void            zoomOut(void);
        void            unZoom(void);
        void            jumpToSelection(void);
        bool            canZoomIn(void) { return (m_zoom < 1.0); }

        void            moveToTop(Node *node, Node* ignoreNode = NULL);
        void            moveRoutesToTop(Node *moveNode);

        void            setZoomGraphPosition(Node *node, int x, int y);
        void            setZoomGraphPosition(Proto *proto, int x, int y);

protected:
        int             getType(Proto *proto, int elementType, int field);
        bool            addIs(Proto *src, int srcSocket, int srcSide, 
                              Node *dst, int dstSocket, int dstSide,
                              bool askAddIs = false);
        bool            validIs(Proto *src, int srcSocket, int srcSide, 
                                Node *dst, int dstSocket, int dstSide);
        void            getZoomGraphPosition(int &x, int &y, Node* node) const;
        void            getZoomGraphSize(int &width, int &height, Node *node) const;
        void            setGraphSize(Node *node, int width, int height);
        void            getZoomGraphPosition(int &x, int &y, Proto* proto) const;
        void            getZoomGraphSize(int &width, int &height, Proto *proto) const;
        void            setGraphSize(Proto *proto, int width, int height);
        void            setLastXPosition(int x);
        void            setLastYPosition(int y);
        void            warnFlooded(void);
        int             getFirstYPosition(void);
        void            SetScrollSizes(int width, int height);
        void            SetSize(int width, int height);
        void            moveToTop(Node *moveNode, int x, 
                                  Node *ignoreNode = NULL);
        bool            isTracking(void) 
                            { return (m_mode == TRACKING) || (m_mode == NONE); }

protected:
        SWND            m_window;
        SWND            m_scroller;

        int             m_lastXPosition;
        int             m_lastYPosition;

        int             m_maxYPosition;

        int             m_XNodesOnlyEventOuts; 
        int             m_YNodesOnlyEventOuts; 

        int             m_XNodesBothEvents; 
        int             m_YNodesBothEvents; 

        int             m_XNodesOnlyEventIns;
        int             m_YNodesOnlyEventIns;

        MyArray<NodeOrProto> m_nodesOrProtos;

        Proto          *m_currentProto;

        SGMode          m_mode;
        Point           m_point1;
        Point           m_point2;
        Node           *m_dstNode;
        Proto          *m_dstProto;
        int             m_dstSocket;
        int             m_dstSide;
        Node           *m_srcNode;
        Proto          *m_srcProto;
        int             m_srcSocket;
        int             m_srcSide;
        SFONT           m_font;
        SBITMAP         m_socketBitmaps[LAST_TYPE + 2];
        SBITMAP         m_nodeBitmap;
        int             m_nodeBitmaps;

        SBITMAP         m_socketBitmap;
        SBITMAP         m_socketBitmapRecommended;
        SBITMAP         m_socketBitmapRecommendedWonderland;
        SBITMAP         m_socketBitmapWonderland;

        int             m_typeColors[LAST_TYPE + 2];
        int             m_autoScrolling;
        int             m_autoScrollPX, m_autoScrollPY;
        int             m_autoScrollDX, m_autoScrollDY;
        int             m_highlight;
        int             m_shadow;
        int             m_face;
        STIMER          m_timer;
        float           m_zoom;
        bool            m_floodedWarning;
};

#endif // _SCENE_GRAPH_VIEW_H
