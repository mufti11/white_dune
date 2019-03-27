/*
 * NodePositionDamper2D.h
 *
 * Copyright (C) 2009 J. "MUFTI" Scheurich
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

#ifndef _NODE_POSITION_DAMPER_2D_H
#define _NODE_POSITION_DAMPER_2D_H

#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"

class ProtoPositionDamper2D : public Proto {
public:
                    ProtoPositionDamper2D(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_POSITION_DAMPER_2D; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex tau;
    FieldIndex tolerance;
    FieldIndex initialDestination;
    FieldIndex initialValue;
    FieldIndex order;
};

class NodePositionDamper2D : public Node {
public:
                    NodePositionDamper2D(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const { return "Followers"; }
    virtual int         getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 2; } 
    virtual Node   *copy() const { return new NodePositionDamper2D(*this); }

    fieldMacros(SFTime,  tau,                ProtoPositionDamper2D);
    fieldMacros(SFFloat, tolerance,          ProtoPositionDamper2D);
    fieldMacros(SFVec2f, initialDestination, ProtoPositionDamper2D);
    fieldMacros(SFVec2f, initialValue,       ProtoPositionDamper2D);
    fieldMacros(SFInt32, order,              ProtoPositionDamper2D);
};

#endif
