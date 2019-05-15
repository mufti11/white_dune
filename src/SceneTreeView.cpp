/*
 * SceneTreeView.cpp
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

#include "SceneTreeView.h"
#include "IconSize.h"

#include "swt.h"

#include "Scene.h"
#include "DuneApp.h"
#include "Node.h"
#include "Proto.h"
#include "Field.h"
#include "MFNode.h"
#include "SFNode.h"
#include "Path.h"
#include "resource.h"

class TreeNode {
public:
                TreeNode (int f, Node *n, Proto *p = NULL)
                { field = f; node = n; proto = p;}
    int         field;  // which field of parent is this node in
    Node       *node;   // the node, or NULL if a field or a PROTO
    Proto      *proto;  // or Proto
};

static void
treeCallback(void *data, int type, STREEITEM item)
{
    switch (type) {
      case SW_TREE_SELECT:
        ((SceneTreeView *) data)->OnSelectionChanged(item);
        break;
      case SW_TREE_BEGIN_DRAG:
        ((SceneTreeView *) data)->OnBeginDrag(item);
        break;
    }
}

SceneTreeView::SceneTreeView(Scene *scene, SWND parent)
  : SceneView(scene, parent)
{
    int width, height;

    swGetSize(parent, &width, &height);
    m_tree = swCreateTree(0, 0, width, height, parent);
    swTreeSetClientData(m_tree, this);
    swTreeSetCallback(m_tree, treeCallback);
    int idb_node_icons = IDB_NODE_ICONS;
    m_bitmap = swLoadBitmap(parent, idb_node_icons);
    m_bitmapItems = scene->getNumberBuildinProtos() + 4;
    m_mask = swCreateMask(m_bitmap, ICON_SIZE_X * m_bitmapItems, ICON_SIZE_Y, 
                          0x808000);
    swTreeSetImageList(m_tree, m_bitmap, m_mask, ICON_SIZE_X, ICON_SIZE_Y,
                       m_bitmapItems);
    swTreeSetOverlayImage(m_tree, m_bitmapItems - 2);
    m_currentDragSource = NULL;
    m_currentDragParent = NULL;
    m_currentDragField = -1;
    RegisterDropTarget();
}

SceneTreeView::~SceneTreeView()
{
    UnregisterDropTarget();
    swTreeSetCurrentItem(m_tree, NULL);
    DeleteItemRec(swTreeGetRootItem(m_tree));
    swDestroyTree(m_tree);
    swDestroyBitmap(m_bitmap);
    swDestroyBitmap(m_mask);
}

void SceneTreeView::renameNode(STREEITEM item, Node *node)
{
    STREEITEM child;
    for (child = swTreeGetFirstChild(m_tree,item); child != NULL;
         child = swTreeGetNextItem(m_tree, child)) {
        TreeNode *cinfo = (TreeNode *) swTreeGetItemData(m_tree, child);
        if (cinfo && cinfo->node == node) {
            if (TheApp->is4Catt())
                node->generateTreeLabel();
            const char *name;            
            if (node->hasName() || TheApp->is4Catt()) 
                name = node->getTreeLabel();
            else {
                bool x3d = m_scene->isX3d() && !m_scene->converts2VRML();
                x3d = x3d || m_scene->converts2X3d();
                name = node->getProto()->getName(x3d);
            }
            swTreeSetItemName(m_tree, child, name);
        }        
        renameNode(child, node);
    }        
}

void SceneTreeView::renameNode(STREEITEM item, RouteUpdate *routeUpdate)
{
    STREEITEM child;
    for (child = swTreeGetFirstChild(m_tree,item); child != NULL;
         child = swTreeGetNextItem(m_tree, child)) {
        TreeNode *cinfo = (TreeNode *) swTreeGetItemData(m_tree, child);
        if (cinfo && cinfo->node == routeUpdate->src) {
            Node *node = routeUpdate->src;
            node->getName();
            swTreeSetItemName(m_tree, child, (const char*)node->getTreeLabel());
        }
        if (cinfo && cinfo->node == routeUpdate->dest) {
            Node *node = routeUpdate->dest;
            node->getName();
            swTreeSetItemName(m_tree, child, (const char*)node->getTreeLabel());
        }
        renameNode(child, routeUpdate);
    }        
}


void SceneTreeView::OnUpdate(SceneView* sender, int type, Hint *hint) 
{
    Node *root = m_scene->getRoot();
    NodeUpdate *nodeUpdate;
    RouteUpdate *routeUpdate;
    STREEITEM item;

    switch (type) {
      CASE_UPDATE(UPDATE_ADD_ROUTE)
        routeUpdate = (RouteUpdate *) hint;
        for (item = swTreeGetRootItem(m_tree); item != NULL;
             item = swTreeGetNextItem(m_tree, item)) 
            renameNode(item,routeUpdate);
        break;
      CASE_UPDATE(UPDATE_ALL)
        swTreeSetCurrentItem(m_tree, NULL);
        DeleteItemRec(swTreeGetRootItem(m_tree));
        m_scene->getNodes()->clearFlag(NODE_FLAG_TOUCHED);
        m_scene->getNodes()->setZeroCounter4SceneTreeView();
        InsertNodeRec(root, -1, SW_INSERT_ROOT, NULL);
        UpdateOverlay();
        if (root != NULL) {
            // insert PROTOs at SW_INSERT_FIRST_CHILD so flip loop
            for (int i = m_scene->getNumProtos() - 1; i >= 0; i--) {
                item = swTreeGetRootItem(m_tree);
                InsertProtoRec(m_scene->getProto(i), i, item);
                swTreeSetCurrentItem(m_tree, item);
            }
        }
        item = swTreeGetFirstChild(m_tree, swTreeGetRootItem(m_tree));
        // open always the first level of scenegraph on UPDATE_ALL
        item = swTreeGetFirstChild(m_tree, swTreeGetRootItem(m_tree));
        if (item) {
            swTreeSetCurrentItem(m_tree, item);
            swTreeSetCurrentItem(m_tree, swTreeGetRootItem(m_tree));
        }
        UpdateSelection();
        break;
      CASE_UPDATE(UPDATE_SELECTION_NAME)
        UpdateNodeName(m_scene->getSelection()->getNode());
        break;
      CASE_UPDATE(UPDATE_SELECTION)
        UpdateSelection();
        break;
      CASE_UPDATE(UPDATE_ADD_NODE)
        nodeUpdate = (NodeUpdate *) hint;
        UpdateAddNode(swTreeGetRootItem(m_tree), nodeUpdate->node, 
                      nodeUpdate->parent, nodeUpdate->field);
        UpdateOverlay();
        break;
      CASE_UPDATE(UPDATE_REMOVE_NODE)
        nodeUpdate = (NodeUpdate *) hint;
        if ((nodeUpdate->node == NULL) || (nodeUpdate->field == -1))
            break;
        UpdateRemoveNode(swTreeGetRootItem(m_tree), nodeUpdate->node,
                         nodeUpdate->parent, nodeUpdate->field, 
                         nodeUpdate->parent);
        UpdateOverlay();
        break;
      CASE_UPDATE(UPDATE_SOLID_CHANGED)
      CASE_UPDATE(UPDATE_NODE_NAME)
        nodeUpdate = (NodeUpdate *) hint;
        UpdateNodeName(nodeUpdate->node);
        break;
    }
}

void SceneTreeView::OnSize(int width, int height)
{
    swSetSize(swTreeGetWindow(m_tree), width, height);
}

bool SceneTreeView::isNull(STREEITEM item)
{
    if (item == NULL)
        return true;
    TreeNode *info = (TreeNode *) swTreeGetItemData(m_tree, item);
    if (info == NULL)
        return true;
    if (info->node == NULL)
        return true;
    return false;    
}

void SceneTreeView::print(const char *msg, STREEITEM item)
{
    printf("%s", msg);
    if (item == NULL) {
        printf("item NULL\n");
        return;
    }
    TreeNode *info = (TreeNode *) swTreeGetItemData(m_tree, item);
    if (info == NULL) {
        printf("info NULL\n");
        return;
    } else if (info->node == NULL) {
        printf("info->node NULL field %d\n", info->field);
        return;
    }
    printf("item %s %s\n", (const char *)info->node->getName(),
                           (const char *)info->node->getProto()->getName(true));
}

bool SceneTreeView::UpdateAddNode(STREEITEM item, Node *node,
                                  Node *parent, int field)
{
    STREEITEM child, next, sib;
    bool found = false;

    TreeNode *info = (TreeNode *) swTreeGetItemData(m_tree, item);
    if (info && info->node == parent) {
        int pos = parent->findChild(node, field);
        int offset = getItemPositionOffset(node);
        if (offset > 0) 
            pos += offset - 1;
        if (TheApp->GetShowAllFields() || parent->showFields()) {
            for (child = swTreeGetFirstChild(m_tree, item); child != NULL;
                 child = swTreeGetNextItem(m_tree, child)) {
                TreeNode *cinfo = (TreeNode *) swTreeGetItemData(m_tree, child);
                if (!cinfo->node && cinfo->field == field) {
                    item = child;
                    break;
                }
            }
        } else {
            for (child = swTreeGetFirstChild(m_tree, item); child != NULL;
                 child = swTreeGetNextItem(m_tree, child)) {
                TreeNode *cinfo = (TreeNode *) swTreeGetItemData(m_tree, child);
                if (cinfo && cinfo->field >= field) break;
                pos++;
            }
        }
        int vpos;
        if (pos == 0) {
            vpos = SW_INSERT_FIRST_CHILD;
            sib = item;
        } else {
            vpos = SW_INSERT_AFTER;
            sib = swTreeGetFirstChild(m_tree, item);
            while (--pos > 0) {
                sib = swTreeGetNextItem(m_tree, sib);
            }
        }
        item = InsertNodeRec(node, field, vpos, sib);
        swTreeDeselectAll(m_tree);
        swTreeSetCurrentItem(m_tree, item);
        swTreeSelectItem(m_tree, item);
        found = true;
    } else {
        for (child = swTreeGetFirstChild(m_tree, item); child != NULL;
             child = next) {
            next = swTreeGetNextItem(m_tree, child);
            found = UpdateAddNode(child, node, parent, field);
        }
    }
    return found;
}

bool SceneTreeView::UpdateRemoveNode(STREEITEM item,
                                     Node *node, Node *parent, int field,
                                     Node *curParent)
{
    bool ret = false;
    STREEITEM child, next;

    TreeNode *info = (TreeNode *) swTreeGetItemData(m_tree, item);
    bool isTheRightParent = (curParent == parent);
    if (curParent == NULL)
        isTheRightParent = true;
    bool deleteItem = false;
    if (info && info->node == node &&
        (info->field == field) && 
        isTheRightParent) {
        deleteItem = true;
        ret = true;
    } 
    if ((curParent != NULL) && (info->node != NULL))
        curParent = info->node;
    if ((info->node != NULL) && info->node->hasParent() && 
        (info->node->getParent()->getNumParents() > 1)) {
        // remove all USEd nodes
        curParent = NULL;
    }
    for (child = swTreeGetFirstChild(m_tree, item); child != NULL;
         child = next) {
        next = swTreeGetNextItem(m_tree, child);
        if (UpdateRemoveNode(child, node, parent, field, curParent))
            ret = true;
    }
    if  (deleteItem)
        DeleteItemRec(item);
    return ret;
}

void SceneTreeView::DeleteItemRec(STREEITEM item)
{
    if (!item) return;

    STREEITEM child, next;

    for (child = swTreeGetFirstChild(m_tree, item); child != NULL;
         child = next) {
        next = swTreeGetNextItem(m_tree, child);
        DeleteItemRec(child);
    }

    delete (TreeNode *) swTreeGetItemData(m_tree, item);
    swTreeDeleteItem(m_tree, item);
}

void SceneTreeView::UpdateNode(const Path *updatePath)
{
    Node *node = m_scene->getRoot();
    STREEITEM item = swTreeGetRootItem(m_tree);
    if (updatePath == NULL) return;

    int len = updatePath->getPathLen();

//#ifdef DEBUG
//    updatePath->dump();
//#endif

    const int *path = updatePath->getPath();

    int start = 0;
    if ((len >= 3) && (path[0] < 0)) {
        start = 4;
        Proto *proto = m_scene->getProto(-path[0] - 1); 
        item = swTreeGetFirstChild(m_tree, item);
        for (int i = 0; i < -path[0] - 1; i++)
            item = swTreeGetNextItem(m_tree, item);             
        if (proto != NULL) {
            if (path[1] != -1)
                node = proto->getNode(path[1]);
            else
                node = NULL;
        }
        if (node != NULL)
            item = swTreeGetFirstChild(m_tree, item);
        for (int j = 0; j < path[1]; j++)
            item = swTreeGetNextItem(m_tree, item);             
    }

    bool endloops = false;
    for (int i = start; i < len;) {
        int field = path[i++];
        if (node && node->isPROTO()) {
            node = node->getProto()->getNode(0);
        }
        if (node == NULL) {
            item = NULL;
            break;
        }
        FieldValue *value = NULL;
        if (field > -1)
            value = node->getField(field);
        if (value == NULL)
            return;
        if (node->getProto()->getField(field) == NULL)
            return;
        if (i > len) {
            item = NULL;
            break;
        }
        bool isNode = false;
        bool isNotSFNode = true;
        STREEITEM tmpItem = item;
        STREEITEM oldTmpItem = NULL;
        Node *oldNode = node;
        if (i < len) {
            int pos = path[i++]; 
            if (value->getType() == SFNODE) {
                node = ((SFNode *) value)->getValue();
                item = swTreeGetFirstChild(m_tree, item);
                isNotSFNode = false;
            } else if ((value->getType() == MFNODE) && (pos > -1)) {
                node = ((MFNode *) value)->getValue(pos);
                item = swTreeGetFirstChild(m_tree, item);
                if (item && strcmp(swTreeGetItemName(m_tree, item), 
                                   "metadata") == 0)
                    item = swTreeGetNextItem(m_tree, item);
            } else
               break;
        }
        bool showAllFields = isNotSFNode && (TheApp->GetShowAllFields() || 
                                             oldNode->showFields());
        if (showAllFields) {
            Proto *proto = oldNode->getProto();
            {
                STREEITEM tmp = swTreeGetFirstChild(m_tree, item);
                if (!isNull(tmp)) {
                    item = tmp;
                    if (value->getType() == MFNODE) {
                        MFNode* mfNode = (MFNode *)value;
                        for (int n = 0; n < mfNode->getSize(); n++) { 
                            if (n == 0) {
                                TreeNode *t = (TreeNode *) 
                                              swTreeGetItemData(m_tree, item);
                                if (t && t->node && t->node->isEqual(node))
                                   break;
                                continue;
                            }    
                            for (; tmpItem != NULL;
                                oldTmpItem = tmpItem,
                                tmpItem = swTreeGetNextItem(m_tree, item)) {
                                if (tmpItem) {
                                    if (tmpItem == oldTmpItem)
                                        break;
                                    TreeNode *t = (TreeNode *) 
                                        swTreeGetItemData(m_tree, item);
                                    if (t && t->node && t->node->isEqual(node))
                                    {
                                        item = tmpItem;
                                        isNode = true;
                                    }
                                    if (t->node == updatePath->getNode()) {
                                        item = tmpItem;
                                        endloops=true;
                                        break;
                                    }
                                    if (isNode && t && t->field == field) {
                                        item = tmpItem;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if (endloops)
            break;
        // search for field
        for (; item != NULL; item = swTreeGetNextItem(m_tree, item)) {
            TreeNode *t = (TreeNode *) swTreeGetItemData(m_tree, item);
            if (t && t->node && t->node->isEqual(node))
                isNode = true;
            if (isNode && t && t->field == field) {
                tmpItem = item;
                break;
            }
        }
        if ((tmpItem != NULL) && (tmpItem == item))
            item = tmpItem;
        if ((item == NULL) && (tmpItem != NULL))
            item = tmpItem;
    }

    if (item != NULL && item != swTreeGetCurrentItem(m_tree)) {
        swTreeDeselectAll(m_tree);
        swTreeSetCurrentItem(m_tree, item);
        swTreeSelectItem(m_tree, item);
    }
}

void SceneTreeView::UpdateSelection()
{
    const Path *selection = m_scene->getSelection();
    UpdateNode(selection);
}

void SceneTreeView::InsertNodeListRec(NodeList *list, int field, STREEITEM parent)
{
    if (list == NULL) return;
    for (int i = 0; i < list->size(); i++) {
        InsertNodeRec(list->get(i), field, SW_INSERT_LAST_CHILD, parent);
    }
}

void SceneTreeView::InsertChildren(STREEITEM item, Node *node)
{
    assert(node != NULL);

    STREEITEM fieldItem;
    Proto *def = node->getProto();
    bool x3d = m_scene->isX3d();
    bool showAllFields = TheApp->GetShowAllFields() || node->showFields();
    if (node == m_scene->getRoot())
        showAllFields = false;

    for (int i = 0; i < def->getNumFields(); i++) {
        Field *field = def->getField(i);
        if (m_scene->isInvalidElement(field))
            continue;
        if (field->getType() == MFNODE) {
            MFNode *value = (MFNode *) node->getField(i);
            const char *name = (const char *) field->getName(x3d);
            if (showAllFields) {
                fieldItem = swTreeInsertItem(m_tree, SW_INSERT_LAST_CHILD,
                                             item, name);
                swTreeSetItemData(m_tree, fieldItem, new TreeNode(i, NULL));
                swTreeSetItemImage(m_tree, fieldItem, m_bitmapItems - 2, 
                                   m_bitmapItems - 2);
                InsertNodeListRec(value->getValues(), i, fieldItem);
            } else
                InsertNodeListRec(value->getValues(), i, item);
        } else if (field->getType() == SFNODE) {
            SFNode *value = (SFNode *) node->getField(i);
            const char *name = (const char *) field->getName(x3d);
            if (showAllFields) {
                fieldItem = swTreeInsertItem(m_tree, SW_INSERT_LAST_CHILD,
                                             item, name);
                swTreeSetItemData(m_tree, fieldItem, new TreeNode(i, NULL));
                swTreeSetItemImage(m_tree, fieldItem, m_bitmapItems - 2, 
                                   m_bitmapItems - 2);
                InsertNodeRec(value->getValue(), i, SW_INSERT_LAST_CHILD,
                              fieldItem);
            } else
                InsertNodeRec(value->getValue(), i, SW_INSERT_LAST_CHILD, item);
        }
    }
}

STREEITEM SceneTreeView::InsertProtoRec(Proto *proto, int i, STREEITEM relative)
{
    if (proto == NULL) return NULL;

    MyString name = "";
    name += "PROTO ";
    const char *protoName = proto->getName(m_scene->isX3d());
    name += protoName;

    bool nestedProto = m_scene->isNestedProto(protoName);
    if (nestedProto)
        name += " nested (editing not supported)";

    STREEITEM item;
    item = swTreeInsertItem(m_tree, SW_INSERT_FIRST_CHILD, relative, name);

    int img = m_bitmapItems - 1;
    swTreeSetItemImage(m_tree, item, img, img);

    swTreeSetItemData(m_tree, item, new TreeNode(i, NULL, proto));
    swTreeSetItemCollapsed(m_tree, item, false);

    STREEITEM parentItem = swTreeGetParentItem(m_tree, item);
    if (parentItem != NULL) {
        parentItem = swTreeGetParentItem(m_tree, parentItem);
    }

    if (!nestedProto)
        for (int j = 0; j < proto->getNumNodes(); j++)
            InsertNodeRec(proto->getNode(j), j, SW_INSERT_LAST_CHILD, item);

    return item;
}

STREEITEM SceneTreeView::InsertNodeRec(Node *node, int field, int position,
                                       STREEITEM relative)
{
    STREEITEM item;
    const char *name;

    if (node == NULL) return NULL;
    if (node == m_scene->getRoot()) {
        name = "Scene";
    } else if (node->hasName() || TheApp->is4Catt()) {
        name = node->getTreeLabel();
    } else {
        name = node->getProto()->getName(m_scene->isX3d());
    }
    item = swTreeInsertItem(m_tree, position, relative, name);

    int img = node->getType();
    if (node == m_scene->getRoot())
        img = m_bitmapItems - 1;
    swTreeSetItemImage(m_tree, item, img, img);

    swTreeSetItemData(m_tree, item, new TreeNode(field, node));
    swTreeSetItemCollapsed(m_tree, item, node->isCollapsed());

    if (node->getCounter4SceneTreeView() <= TheApp->GetMaxInlinesToLoad()) {
        node->addCounter4SceneTreeView();
        InsertChildren(item, node);
    }

    return item;
}

void SceneTreeView::OnSelectionChanged(STREEITEM item) 
{
    if (item) {
        Path *path = MakePath(item);
        m_scene->setSelection(path);
        m_scene->UpdateViews(this, UPDATE_SELECTION);
        m_scene->removeSelectedHandles();
    }
}

void SceneTreeView::OnBeginDrag(STREEITEM item) 
{
    TreeNode *treeNode = (TreeNode *) swTreeGetItemData(m_tree, item);
    Node *node = treeNode->node;

    if (node && treeNode->node) {
        STREEITEM parentItem = swTreeGetParentItem(m_tree, item);

        if (parentItem) {
            Node *parent = ((TreeNode *) 
                            swTreeGetItemData(m_tree, parentItem))->node;

            if (!parent) {
                parentItem = swTreeGetParentItem(m_tree, parentItem);
                parent = ((TreeNode *) 
                          swTreeGetItemData(m_tree, parentItem))->node;
            }
            m_currentDragSource = node;
            m_currentDragParent = parent;
            m_currentDragField = treeNode->field;
            swDragDrop(m_wnd, SW_DRAG_MOVE | SW_DRAG_LINK | SW_DRAG_COPY,
                       m_bitmap, m_mask, ICON_SIZE_X * node->getType(), 0, 
                       ICON_SIZE_X, ICON_SIZE_Y);
        }
    }
}

int SceneTreeView::OnDragEnter(int x, int y, int modifiers)
{
    return OnDragOver(x, y, modifiers);
}

int SceneTreeView::OnDragOver(int x, int y, int modifiers) 
{
    int rc = 0;

    STREEITEM target = swTreeHitTest(m_tree, x, y);
    if (target) {
        TreeNode *treeNode = (TreeNode *) swTreeGetItemData(m_tree, target);
        Node *node = treeNode->node;
        int field = -1;
        if (!node && !treeNode->proto) {
            // dragging onto a field, so we know what field
            field = treeNode->field;
            treeNode = (TreeNode *) swTreeGetItemData(m_tree, 
                            swTreeGetParentItem(m_tree, target));
            node = treeNode->node;
        }
        if (m_currentDragSource) {
            rc = m_scene->OnDragOver(m_currentDragSource, m_currentDragParent,
                                    m_currentDragField, node, field, modifiers);
            if (rc != 0) {
                swTreeSelectDropTarget(m_tree, target);
            } else {
                swTreeSelectDropTarget(m_tree, NULL);
            }
        } else {
            // the data came from another app
            // eventually, get the actual data object through drag & drop
        }
    }
    return rc;
}

void SceneTreeView::OnDragLeave()
{
    swTreeSelectDropTarget(m_tree, NULL);
}

int SceneTreeView::OnDrop(int x, int y, int effect) 
{
    int rc = 0;

    swTreeSelectDropTarget(m_tree, NULL);
    STREEITEM target = swTreeHitTest(m_tree, x, y);
    if (target) {
        TreeNode *treeNode = (TreeNode *) swTreeGetItemData(m_tree, target);
        Node *node = treeNode->node;
        int field = -1;
        if (!node) {
            // dragging onto a field, so we know what field
            field = treeNode->field;
            treeNode = (TreeNode *) swTreeGetItemData(m_tree, 
                  swTreeGetParentItem(m_tree, target));
            node = treeNode->node;
        }
        rc = m_scene->OnDrop(m_currentDragSource, m_currentDragParent, m_currentDragField,
                            node, field, effect);
        m_currentDragSource = NULL;
        m_currentDragParent = NULL;
        m_currentDragField = -1;
    }
    return rc;
}

Path *SceneTreeView::MakePath(STREEITEM item)
{
    int len;
    STREEITEM p;
    TreeNode *t, *t1;
    Node *root = m_scene->getRoot();

    t1 = (TreeNode *) swTreeGetItemData(m_tree, item);
    len = ((t1 && t1->node) || (t1 == NULL)) ? 0 : 1;

    for (p = item; p != NULL; p = swTreeGetParentItem(m_tree, p)) {
        t = (TreeNode *) swTreeGetItemData(m_tree, p);
        if (t && t->proto != NULL)
            len += 2;
        else if (t && t->node && t->node != root)
            len += 2;
    }
    int *list = new int[len];

    int i = len-1;
    if (t1 && !t1->node) {
        list[i--] = t1->field;
    }
    
    TreeNode *tOld = NULL;
    for (p = item; p != NULL; p = swTreeGetParentItem(m_tree, p)) {
        t = (TreeNode *) swTreeGetItemData(m_tree, p);
        if (t && t->proto != NULL) {
            list[i--] = tOld ? tOld->field : 0; 
            int protoNum = 0;
            for (int j = 0; j < m_scene->getNumProtos(); j++) {
                if (m_scene->getProto(j) == t->proto) {
                    protoNum = j;
                    break;
                }
            }
            list[i--] = -protoNum - 1;
        } else if (t && t->node && t->node != root) {
            list[i--] = GetIndex(p);
            list[i--] = t->field;
        }
        tOld = t;
    }

    Path *path = new Path(list, len, m_scene, false);
    delete [] list;
    return path;
}

int
SceneTreeView::GetIndex(STREEITEM item)
{
    assert(item != NULL);
    TreeNode *t = (TreeNode *) swTreeGetItemData(m_tree, item);
    STREEITEM parent = swTreeGetParentItem(m_tree, item);

    if (parent) {
        TreeNode *p = (TreeNode *) swTreeGetItemData(m_tree, parent);

        if (p == NULL)
            return 0;
        if (!p->node) {
            if (p->proto != NULL) {
                for (int i = 0; i < m_scene->getNumProtos(); i++)
                    if (p->proto == m_scene->getProto(i))
                        return i;
            }
            // parent is a field, look up
            parent = swTreeGetParentItem(m_tree, parent);
            p = (TreeNode *) swTreeGetItemData(m_tree, parent);
        }
        if (p->node == NULL) {
            return 0;
        }
        FieldValue *value = p->node->getField(t->field);
        if (value->getType() == MFNODE) {
            int pos = ((MFNode *) value)->getValues()->find(t->node);
            return pos;
        } else if (value->getType() == SFNODE)  {
            return 0;
        } else {
            assert(0);
            return -1;
        }
    } else {
        return 0;
    }
}

void SceneTreeView::UpdateOverlay()
{
    m_scene->getNodes()->clearFlag(NODE_FLAG_TOUCHED);
    m_scene->getNodes()->setZeroCounter4SceneTreeView();
    UpdateOverlayRec(swTreeGetRootItem(m_tree));
}

void SceneTreeView::UpdateOverlayRec(STREEITEM item)
{
    TreeNode *treeNode = ((TreeNode *) swTreeGetItemData(m_tree, item));
    Node *node = NULL;
    if (treeNode)
        node = treeNode->node;

    if (node) {
        if (node->getFlag(NODE_FLAG_TOUCHED)) {
            swTreeSetFlags(m_tree, item, SW_TREE_ITEM_OVERLAY,
                           SW_TREE_ITEM_OVERLAY);
        } else {
            swTreeSetFlags(m_tree, item, SW_TREE_ITEM_OVERLAY, 0);
            node->setFlag(NODE_FLAG_TOUCHED);
        }
    }

    for (STREEITEM child = swTreeGetFirstChild(m_tree, item); child != NULL;
        child = swTreeGetNextItem(m_tree, child)) {
        UpdateOverlayRec(child);
    }
}

void  
SceneTreeView::UpdateNodeName(Node *node)
{
    for (STREEITEM item = swTreeGetRootItem(m_tree); item != NULL;
        item = swTreeGetNextItem(m_tree, item)) 
    renameNode(item, node);
}

int             
SceneTreeView::getItemPositionOffset(Node *node) {
    if (!node)
        return 0;
    if (node->hasParent() && (node->getParent() == m_scene->getRoot()))
        return m_scene->getNumProtos();
    return 0;
}


#if 0
void SceneTreeView::OnItemExpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_TREEVIEW *pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    STREEITEM item = pNMTreeView->itemNew.hItem;
    TreeNode *treeNode = (TreeNode *) GetTreeCtrl().GetItemData(item);

    if (treeNode->node) {
        if (pNMTreeView->action == TVE_COLLAPSE) {
            treeNode->node->setFlag(NODE_FLAG_COLLAPSED);
        } else if (pNMTreeView->action == TVE_EXPAND) {
            treeNode->node->clearFlag(NODE_FLAG_COLLAPSED);
        }
    }
    *pResult = 0;
}
#endif


