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

#ifndef _ARRAY_H
#include "Array.h"
#endif

#include "NodeArray.h"
#include "ExternTheApp.h"
#include "Node.h"

class NodeList : public NodeArray {
public:
                    NodeList(Node *node) : NodeArray() 
                        { 
                        if (node->isValid())
                            append(node);
                        }
                    NodeList() : NodeArray() {}
    virtual        ~NodeList() {}

    bool            isValid(void) 
                        {
                        for (int i = 0; i < m_size; i++)
                            if (!(get(i)->isValid()))
                                return false;
                            return true;
                        } 
    void            clearFlag(int flag) const;
    void            setFlag(int flag) const;
    void            setZeroCounter4SceneTreeView(void) const;
    NodeList*       copy() 
                        { 
                        return (NodeList *)new 
                               NodeArray(this->getData(), this->size());
                        }
    virtual void    update();
    virtual bool    canWriteAc3d();
    virtual int     writeAc3d(int filedes, int indent);
    virtual int     writeRib(int filedes, int indent);
    virtual bool    canWriteCattGeo();
    virtual int     writeCattGeo(Node *node, int filedes, int indent);
    virtual bool    canWriteLdrawDat();
    virtual int     writeLdrawDat(int filedes, int indent);
    bool            doWithBranch(DoWithNodeCallback callback, void *data,
                                 bool searchInRest = true, 
                                 bool skipBranch = false,
                                 bool skipProto = false,
                                 bool callSelf = true,
                                 bool skipInline = true);

};


#ifndef _NODE_LIST_H
#define _NODE_LIST_H
#endif


