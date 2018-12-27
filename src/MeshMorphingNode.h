/*
 * MeshMorphingNode.h
 *
 * Copyright (C) 2005 J. "MUFTI" Scheurich
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

#ifndef _MESH_MORPHING_NODE_H
#define _MESH_MORPHING_NODE_H

#include "Array.h"
#include "Interpolator.h"
#include "MeshBasedNode.h"

class NodeGroup;

class InterpolatorInfo {
public:
    Array<Interpolator *> interpolator;
    Array<int> field;
    Array<const Node *> node;
};

class MeshMorphingNode : public MeshBasedNode {
public:
                    MeshMorphingNode(Scene *scene, Proto *proto) : 
                                    MeshBasedNode (scene, proto) {}
                   ~MeshMorphingNode() {}
protected:
    virtual void    findInterpolators(InterpolatorInfo& info);
    bool            addCoordinateInterpolator(Node *node,
                                              bool optimize, 
                                              bool appendToScene);

    virtual int     write(int filedes, int indent);
    virtual int     writeXml(int filedes, int indent, int containerField = -1) 
                        { return write(filedes, indent); }

    virtual void    addToConvertedNodes(int writeFlags);

    virtual void   *initializeData(void) = 0;
    virtual void    loadDataFromInterpolators(void *data, Interpolator *inter,
                                             int field, float key) = 0;
    virtual void    createMeshFromData(void* data, bool optimize) = 0;
    virtual void    finalizeData(void* data) = 0;
};

#endif
