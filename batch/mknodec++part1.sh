#!/bin/sh 

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

if test $# != 1 ;then
   echo usage: $0 nodename 1>&2
   exit 1
fi
NODENAME=$1

cat << EOT
/*
 * Node$NODENAME.cpp
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

#include <stdio.h>
#include "stdafx.h"

#include "Node$NODENAME.h"
#include "Proto.h"
#include "FieldValue.h"
EOT
