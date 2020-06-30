/*
 * NodeList.h
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

class Node;

#include "Array.h"
#include "NodeArray.h"
#include "Node.h"

class NodeList : public NodeArray {
public:
                    NodeList(Node *node) : NodeArray() { append(node); }
                    NodeList() : NodeArray() {}
    virtual        ~NodeList() {}

    void            clearFlag(int flag) const;
    void            setFlag(int flag) const;
    void            setZeroCounter4SceneTreeView(void) const;
    NodeList*       copy() 
                       { return (NodeList  *)new NodeArray((NodeArray)*this); }
    virtual void    update();
    virtual bool    canWriteAc3d();
    virtual int     writeAc3d(int filedes, int indent) const;
    virtual int     writeRib(int filedes, int indent) const;
    virtual int     writePovray(int filedes, int indent) const;
    virtual bool    canWriteCattGeo();
    virtual int     writeCattGeo(Node *node, int filedes, int indent);
    virtual bool    canWriteLdrawDat();
    virtual int     writeLdrawDat(int filedes, int indent) const;
    bool            doWithBranch(DoWithNodeCallback callback, void *data,
                                 bool searchInRest = true, 
                                 bool skipBranch = false,
                                 bool skipProto = false,
                                 bool callSelf = true,
                                 bool skipInline = true);
};
