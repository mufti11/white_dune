#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

if test $# != 3 ;then
   echo usage: $0 nodename nodename_h_for_ifdef nodetype 1>&2
   exit 1
fi
NODENAME=$1
NODENAME_H_FOR_IFDEF=$2
NODETYPE=$3
cat << EOT
/*
 * Node$NODENAME.h
 *
 * Copyright (C) 2011 J. "MUFTI" Scheurich
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

#ifndef $NODENAME_H_FOR_IFDEF
#define $NODENAME_H_FOR_IFDEF

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

class Proto$NODENAME : public Proto {
public:
                    Proto$NODENAME(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return $NODETYPE; }

EOT
