/*
 * NodeExtrusion.h
 *
 * Copyright (C) 1999 Stephen F. White, 2004 Wu qingwei
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

#ifndef _NODE_EXTRUSION_H
#define _NODE_EXTRUSION_H

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

#include "SFMFTypes.h"
#include "MeshBasedNode.h"

class ProtoExtrusion : public GeometryProto {
public:
                    ProtoExtrusion(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_EXTRUSION; }

    virtual bool    isMesh(void) { return true; }

    virtual bool    needUpdate(void) { return true; }

    FieldIndex beginCap;
    FieldIndex ccw;
    FieldIndex convex;
    FieldIndex creaseAngle;
    FieldIndex crossSection;
    FieldIndex endCap;
    FieldIndex orientation;
    FieldIndex scale;
    FieldIndex solid;
    FieldIndex spine;
    FieldIndex texCoord;
    x3domGeometryCommonFieldIndex()
};


class NodeExtrusion : public MeshBasedNode {
public:
                    NodeExtrusion(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_IMMERSIVE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() { return new NodeExtrusion(m_scene, m_proto); }

    void            createMesh(bool cleanDoubleVertices = true,
                               bool triangulate = true);
    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual void    draw() { meshDraw(); }
    virtual void    drawHandles(void);
    virtual Vec3f   getHandle(int handle, int *constraint, int *field);
    virtual void    setHandle(int handle, const Vec3f &v );
    virtual bool    validHandle(int handle);

    virtual void    flip(int index);
    virtual void    swap(int fromTo);
    virtual bool    canMoveTo(int direction);

    virtual bool    isInvalidChildNode(void) { return true; }

    virtual bool    hasTwoSides(void) { return true; }
    virtual bool    isDoubleSided(void) { return !solid()->getValue(); }
    virtual void    toggleDoubleSided(void) 
                       { solid(new SFBool(!solid()->getValue())); }
    virtual int     getConvexField() { return convex_Field(); }
    virtual int     getSolidField() { return solid_Field(); }
    virtual void    flipSide(void) { ccw(new SFBool(!ccw()->getValue())); }

    virtual bool    shouldConvertToIndexedFaceSet(void) { return false; }

    fieldMacros(SFBool,     beginCap,     ProtoExtrusion)
    fieldMacros(SFBool,     ccw,          ProtoExtrusion)
    fieldMacros(SFBool,     convex,       ProtoExtrusion)
    fieldMacros(SFFloat,    creaseAngle,  ProtoExtrusion)
    fieldMacros(MFVec2f,    crossSection, ProtoExtrusion)
    fieldMacros(SFBool,     endCap,       ProtoExtrusion)
    fieldMacros(MFRotation, orientation,  ProtoExtrusion)
    fieldMacros(MFVec2f,    scale,        ProtoExtrusion)
    fieldMacros(SFBool,     solid,        ProtoExtrusion)
    fieldMacros(MFVec3f,    spine,        ProtoExtrusion)
    fieldMacros(SFNode,     texCoord,     ProtoExtrusion)
    x3domGeometryCommonFieldMacros(ProtoExtrusion) 
};

#endif // _NODE_EXTRUSION_H
