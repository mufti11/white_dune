/*
 * NodeList.cpp
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

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifndef _WIN32
# include <fcntl.h>
#endif

#include "swt.h"
#include "NodeList.h"
#include "Node.h"
#include "Scene.h"

void
NodeList::clearFlag(int flag) const
{
    for (size_t i = 0; i < size(); i++)
        if (m_data[i] != NULL)
            m_data[i]->clearFlag(flag);
}

void
NodeList::setFlag(int flag) const
{
    for (size_t i = 0; i < size(); i++)
        if (m_data[i] != NULL)
            m_data[i]->setFlag(flag);
}

void            
NodeList::setZeroCounter4SceneTreeView(void) const
{
    for (size_t i = 0; i < size(); i++)
        if (m_data[i] != NULL)
            m_data[i]->setCounter4SceneTreeViewToZero();
}


bool
NodeList::canWriteAc3d()
{
    for (size_t i = 0; i < size(); i++) {
        Node *node = get(i);
        if (node != NULL)
            if (node->canWriteAc3d())
                return true;
    }
    return false;
}

int 
NodeList::writeAc3d(int f, int indent) const
{
    int kids = 0;
    for (size_t i = 0; i < size(); i++)
        if (get(i) && get(i)->canWriteAc3d())
            kids++;

    if (kids == 0)
        return 0;

    RET_ONERROR( mywritef(f, "OBJECT group\n") )

    RET_ONERROR( mywritef(f, "kids %d\n", kids) )        

    for (size_t i = 0; i < size(); i++)
        if (get(i))
            RET_ONERROR( get(i)->writeAc3d(f, indent) )
    return 0; 
}

int 
NodeList::writeRib(int f, int indent) const
{
    for (size_t i = 0; i < size(); i++)
        if (get(i))
            RET_ONERROR( get(i)->writeRib(f, indent) )

    return 0; 
}

bool    
NodeList::canWriteCattGeo()
{
    for (size_t i = 0; i < size(); i++) {
        Node *node = get(i);
        if (node != NULL)
            if (node->canWriteCattGeo())
                return true;
    }
    return false;
}

// see dangerous contruct in NodeSwitch::writeCattGeo, 
// if node should deliver other things like name, scene etc. 

int
NodeList::writeCattGeo(Node *node, int filedes, int indent)
{ 
    int children = 0;
    for (size_t i = 0; i < size(); i++)
        if (get(i) && get(i)->canWriteCattGeo())
            children++;

    if (children == 0)
        return 0;

    Scene *scene = node->getScene();
    const char *path = TheApp->GetCattExportPath();
    int len = strlen(path)+256;
    char *filename = new char[len];
    if (node == scene->getRoot())
        mysnprintf(filename, len - 1, "%sMaster.geo", path);
    else
        mysnprintf(filename, len - 1, "%s%s%d.geo", path, 
                   (const char *) node->getProto()->getName(scene->isX3d()),
                   scene->getAndIncCattGeoFileCounter());
    int f = open(filename, O_WRONLY | O_CREAT, 00666);
    if (f == -1)
        return -1;

    RET_ONERROR( mywritef(f, "OBJECT") )
    RET_ONERROR( mywritef(f, swGetLinefeedString()) )

    if (node->hasName())
        RET_ONERROR( mywritef(f, "; grouping node with vrml97 DEF name: %s", 
                              (const char *)node->getName()) )
    RET_ONERROR( mywritef(f, swGetLinefeedString()) )

    if (node == scene->getRoot()) {
        RET_ONERROR( mywritef(f, "INCLUDE ") )
        RET_ONERROR( mywritef(f, "material.geo") )
        RET_ONERROR( mywritef(f, swGetLinefeedString()) )
    }

    for (size_t i = 0; i < size(); i++) {
        if (get(i) && get(i)->canWriteCattGeo()) {
            if ((get(i)->getType() == DUNE_CATT_EXPORT_REC) ||
                (get(i)->getType() == DUNE_CATT_EXPORT_SRC)) {
                RET_ONERROR(get(i)->writeCattGeo(filedes, indent) )
                continue;
            }

            const char *protoName = get(i)->getProto()->getName(false);
            mysnprintf(filename, len - 1, "%s%d.geo", protoName, 
                       scene->getCattGeoFileCounter());
            RET_ONERROR( mywritef(f, "INCLUDE ") )
            RET_ONERROR( mywritef(f, filename) )
            RET_ONERROR( mywritef(f, swGetLinefeedString()) )            
            RET_ONERROR(get(i)->writeCattGeo(filedes, indent) )
        }
    }
    RET_ONERROR( mywritef(f, "CORNERS") )
    RET_ONERROR( mywritef(f, swGetLinefeedString()) )    
    RET_ONERROR( mywritef(f, "PLANES") )
    RET_ONERROR( mywritef(f, swGetLinefeedString()) )    

    delete [] filename;

    return swTruncateClose(f);
}

bool
NodeList::canWriteLdrawDat()
{
    for (size_t i = 0; i < size(); i++) {
        Node *node = get(i);
        if (node != NULL)
            if (node->canWriteLdrawDat())
                return true;
    }
    return false;
}

int 
NodeList::writeLdrawDat(int f, int indent) const
{
    for (size_t i = 0; i < size(); i++)
        if (get(i) != NULL)
            RET_ONERROR( get(i)->writeLdrawDat(f, indent) )
    return 0; 
}

bool 
NodeList::doWithBranch(DoWithNodeCallback callback, void *data,
                       bool searchInRest, bool skipBranch, bool skipProto,
                       bool callSelf, bool skipInline)
{
    for (size_t i = 0; i < size(); i++)
        if (get(i) != NULL)
            if (!get(i)->doWithBranch(callback, data,
                                      searchInRest, skipBranch, skipProto,
                                      callSelf, skipInline))
                return false; 
    return true;
}

void
NodeList::update()
{
    for (size_t i = 0; i < size(); i++)
        if (get(i) != NULL)
            get(i)->update();
}

