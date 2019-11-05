/*
 * NodeProximitySensor.h
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

#ifndef _NODE_PROXIMITYSENSOR_H
#define _NODE_PROXIMITYSENSOR_H

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

class ProtoProximitySensor : public WonderlandExportProto {
public:
                    ProtoProximitySensor(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_PROXIMITY_SENSOR; }
    virtual int     getNodeClass() const { return SENSOR_NODE | CHILD_NODE; }

    virtual bool    isDeclaredInRwd_h() { return true; }      

    virtual bool    isCRouteSource(void) { return true; }

    FieldIndex center;
    FieldIndex size;
    FieldIndex enabled;
};

class NodeProximitySensor : public Node {
public:
                    NodeProximitySensor(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const;
    virtual int     getX3dVersion(void) const { return 0; } 
    virtual Node   *copy() const { return new NodeProximitySensor(*this); }

    virtual void    draw(int pass);

    virtual void    transformForHandle(int handle);
    virtual void    drawHandles(void);
    virtual Vec3f   getHandle(int handle, int *constraint, int *field);
    virtual void    setHandle(int handle, const Vec3f &v);

    virtual bool    hasX3domOnOutputChange(void) { return true; } 

    fieldMacros(SFVec3f, center,  ProtoProximitySensor)
    fieldMacros(SFVec3f, size,    ProtoProximitySensor)
    fieldMacros(SFBool,  enabled, ProtoProximitySensor)

protected:
    void drawLines(bool handleDraw);

protected:
    float             m_handleScale;
    int               m_handleOffset;
};

#endif // _NODE_PROXIMITYSENSOR_H
