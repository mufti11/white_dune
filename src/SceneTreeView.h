/*
 * SceneTreeView.h
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

#include "SceneView.h"
#include "swttypedef.h"

class Node;
class NodeList;
class Path;
class Hint;
class Scene;
class RouteUpdate;

class SceneTreeView : public SceneView
{
public:
                        SceneTreeView(Scene *scene, SWND parent);
        virtual        ~SceneTreeView();

        virtual void    OnUpdate(SceneView *sender, int type, Hint *hint);
        virtual void    OnSize(int width, int height);
        virtual int     OnDragEnter(int x, int y, int modifiers);
        virtual int     OnDragOver(int x, int y, int modifiers);
        virtual void    OnDragLeave();
        virtual int     OnDrop(int x, int y, int modifiers);

        const char     *getName(void) const { return "SceneTreeView"; }

        void            OnSelectionChanged(STREEITEM item);
        void            OnBeginDrag(STREEITEM item);

        void            UpdateNode(Path *updatePath);

protected:
        void            InsertNodeListRec(NodeList *list, int field,
                                          STREEITEM parent);
        STREEITEM       InsertNodeRec(Node *node, int field,
                                      int position, STREEITEM relative);
        STREEITEM       InsertProtoRec(Proto *proto, int i, STREEITEM relative);
        void            InsertChildren(STREEITEM item, Node *node);
        bool            isNull(STREEITEM item);
        void            UpdateSelection();
        bool            ScrollIsValid(int scrollCode, int which);
        void            DeleteItemRec(STREEITEM item);
        bool            UpdateNode(STREEITEM item, Path *updatePath);
        bool            UpdateAddNode(STREEITEM item, Node *node,
                                      Node *parent, int field);
        bool            UpdateRemoveNode(STREEITEM, Node *node, Node *parent,
                                         int field, Node *curParent);
        void            UpdateNodeName(Node *node);
        Path           *MakePath(STREEITEM item);
        int             GetIndex(STREEITEM item);
        void            UpdateOverlay();
        void            UpdateOverlayRec(STREEITEM item);
        STREEITEM       TreeInsertItem(STREE tree, int position, 
                                       STREEITEM relative,Node *node);
        void            renameNode(STREEITEM item, RouteUpdate *routeUpdate);
        void            renameNode(STREEITEM item, Node *node);

        int             getItemPositionOffset(Node *node);
        void            print(const char *msg, STREEITEM item);

protected:
        STREE           m_tree;
        SBITMAP         m_bitmap;
        int             m_bitmapItems;
        SBITMAP         m_mask;
        Node           *m_currentDragSource;
        Node           *m_currentDragParent;
        int             m_currentDragField;
};
