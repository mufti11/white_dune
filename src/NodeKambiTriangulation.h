/*
 * NodeKambiTriangulation.h
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

#ifndef _NODE_KAMBI_TRIANGULATION_H
#define _NODE_KAMBI_TRIANGULATION_H

#ifndef _NODE_H
#include "ExternTheApp.h"
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "KambiCommonFunctions.h"

#include "SFMFTypes.h"

class ProtoKambiTriangulation : public Proto {
public:
                    ProtoKambiTriangulation(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return KAMBI_KAMBI_TRIANGULATION; }

    FieldIndex quadricSlices;
    FieldIndex quadricStacks;
    FieldIndex rectDivisions;

    virtual bool          isKambiProto(void) { return true; }
};

class NodeKambiTriangulation : public Node {
public:
                    NodeKambiTriangulation(Scene *scene, Proto *proto);

    virtual Node   *copy() { return new NodeKambiTriangulation( 
                             m_scene, m_proto); }

    kambiCommonFunctions()

    fieldMacros(SFInt32, quadricSlices, ProtoKambiTriangulation)
    fieldMacros(SFInt32, quadricStacks, ProtoKambiTriangulation)
    fieldMacros(SFInt32, rectDivisions, ProtoKambiTriangulation)
};

#endif 
