/*
 * NodeColor.h
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

#ifndef _NODE_COLOR_H
#define _NODE_COLOR_H

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

class ProtoColor : public WonderlandExportProto {
public:
                    ProtoColor(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_COLOR; }
    virtual int     getNodeClass() const { return COLOR_NODE; }

    virtual bool    isDeclaredInRwd_h() { return true; }

    FieldIndex      color;
};

class NodeColor : public Node {
public:
                    NodeColor(Scene *scene, Proto *proto);
    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() { return new NodeColor(m_scene, m_proto); }

    virtual bool    isInvalidChildNode(void) { return true; }

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    void            drawHandles(void);
    Vec3f           getHandle(int handle, int *constraint, int *field);
    void            setHandle(int handle, const Vec3f &v);

    virtual int     writeAc3dMaterial(int filedes, int indent, 
                                      const char *name);
    virtual void    handleAc3dMaterial(ac3dMaterialCallback callback, 
                                       Scene* scene);

    virtual void    setAc3dMaterialIndex(int index)
                       { m_ac3dMaterialIndex = index; }
    virtual int     getIncAc3dMaterialIndex(void) 
                       { return color()->getSFSize(); }
    int             getAc3dMaterialIndex(int i)
                       { return m_ac3dMaterialIndex + i; } 
    int             getLdrawColor(int index);

    virtual NodeColor *getColorNode() { return this; }
    virtual NodeCoordinate *getCoordinateNode()
            { 
            if (hasParent())
                return getParent()->getCoordinateNode();
            return NULL; 
            }
    virtual bool    hasNumbers4kids(void) { return true; } 

    fieldMacros(MFColor, color, ProtoColor)

protected:
    char           *buildExportMaterialName(const char *name);
protected:
    int             m_ac3dMaterialIndex;
};

#endif // _NODE_COLOR_H
