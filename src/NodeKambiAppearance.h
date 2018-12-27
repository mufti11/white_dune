/*
 * NodeKambiAppearance.h
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

#ifndef _NODE_KAMBI_APPEARANCE_H
#define _NODE_KAMBI_APPEARANCE_H

#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif
#ifndef _DUNEAPP_H
#include "DuneApp.h"
#endif

#include "KambiCommonFunctions.h"

#include "NodeAppearance.h"

#include "SFMFTypes.h"

class ProtoKambiAppearance : public ProtoAppearance {
public:
                    ProtoKambiAppearance(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return KAMBI_KAMBI_APPEARANCE; }

    virtual bool    isKambiProto(void) { return true; }
};

class NodeKambiAppearance : public NodeAppearance {
public:
                    NodeKambiAppearance(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeKambiAppearance(*this); }

    virtual bool    showFields() { return true; }

    kambiCommonFunctions()
};

#endif
