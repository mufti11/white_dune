/*
 * CoverNode.h
 *
 * 2006 J. "MUFTI" Scheurich
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
 * 
 * Implements common functions for nonstandard extensions of the 
 * Cover/Covise vrml browser 
 */

#ifndef _COVER_NODE_H
#define _COVER_NODE_H

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

class CoverNode : public Node {
public:
                    CoverNode(Scene *scene, Proto *def) : Node(scene, def) {}

    virtual int     writeProto(int filedes) 
                       {
                       return ((Node *)this)->writeProto(filedes, 
                                    "urn:inet:www.hlrs.de:library:COVER",
                                    "coverNodes"
#ifdef HAVE_COVER_NODES_PROTO_URL
                                    , HAVE_COVER_NODES_PROTO_URL
#endif
                                                              );
                       }

    virtual bool    isCoverNode(void) { return true; }
};

#endif // _COVER_NODE_H

