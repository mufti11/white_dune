/*
 * Path.h
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

#ifndef _PATH_H
#define _PATH_H

#ifndef _FIELDVALUE_H
#include "FieldValue.h"
#endif
#ifndef _NODELIST_H
#include "NodeList.h"
#endif

/*
    A "Path" is a array of Nodes together with a array of fields pointing
    to the next node
 */

class Path {
public:
                        Path(const int *path, int len, Scene *scene,
                             bool protoInTree = true, 
                             bool ignoreStrangePath = false);
                       ~Path();

    const int          *getPath() const { return m_path; }
    int                 getPathLen() const { return m_len; }
    const NodeList     *getNodes() const { return &m_nodes; }
    Node               *getNode() const { return m_node; }
    Proto              *getProto(Scene *scene) const;
    Node               *getParent() const { return m_parent; }
    int                 getField() const { return m_field; }
    int                 getParentField() const { return m_parentField; }
    Node               *getNextNode(Node *node, int field, int i) const;
    void                printStrangePath(Node *node, int field, int depth) 
                            const;
    bool                isProto(Scene *scene) const
                            { return (getProto(scene) && (getPathLen() == 3)); }
    void                dump() const;
    int                *copyPathInt(void);
protected:
    int                *m_path;
    int                 m_len;
    NodeList            m_nodes;
    bool                m_protoInTree;
    bool                m_ignoreStrangePath;
    Node               *m_node;
    Node               *m_parent;
    int                 m_field;
    int                 m_parentField;
    bool                m_needExtraUpdateSelection;
};

#endif // _PATH_H
