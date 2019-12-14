/*
 * NurbsCurve.h
 *
 * Copyright (C) 2003 Th. Rothermel, 2019 J. "MUFTI" Scheurich
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

#ifndef _NURBS_CURVE_H
#define _NURBS_CURVE_H

#ifndef _GEOMETRY_NODE_H
# include "GeometryNode.h"
#endif
#ifndef _PROTO_MACROS_H
# include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
# include "Proto.h"
#endif
#ifndef _VEC3F_H
# include "Vec3f.h"
#endif
#ifndef _SFMFTYPES_H
# include "SFMFTypes.h"
#endif
#ifndef _VEC3F
# include "Vec3f.h"
#endif
#include "ChainBasedNode.h"

#include "MFVec3f.h"

class NurbsCurve {
protected:
    static int      findSpan(int dimension, int order, float u, 
                             const float knots[]);
    static void     basisFuns(int span, float u, int order,
                              const float knots[], float basis[], 
                              float deriv[]);
    static Vec3f    curvePoint(int dimension, int order, 
                               const float knots[],
                               const Vec3f controlPoints[],
                               const float weight[], float u);
};

#endif
