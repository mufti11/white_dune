/*
 * NodeShape.cpp
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

#include <stdio.h>
#include "stdafx.h"

#include "NodeShape.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFColor.h"
#include "SFFloat.h"
#include "SFNode.h"
#include "Node.h"
#include "NodeAppearance.h"
#include "NodeMaterial.h"
#include "NodeNurbsGroup.h"
#include "NodeNurbsSet.h"
#include "Util.h"
#include "Scene.h"

ProtoShape::ProtoShape(Scene *scene)
  : WonderlandExportProto(scene, "Shape")
{
    appearance.set(
          addExposedField(SFNODE, "appearance", new SFNode(NULL), 
                          VRML_APPEARANCE));
    geometry.set(
          addExposedField(SFNODE, "geometry", new SFNode(NULL), GEOMETRY_NODE));
    if (TheApp->getCoverMode()) {
        // non standard Covise/COVER extensions
        effect.set(
          addExposedField(SFNODE, "effect", new SFNode(NULL), COVER_WAVE));
        setFieldFlags(effect, FF_COVER_ONLY);
    }
    bboxCenter.set(
        addField(SFVEC3F, "bboxCenter", new SFVec3f(0.0f, 0.0f, 0.0f)));
    setFieldFlags(bboxCenter, FF_X3D_ONLY);
    bboxSize.set(
        addField(SFVEC3F, "bboxSize", new SFVec3f(-1.0f, -1.0f, -1.0f), 
                 new SFFloat(-1.0f)));
    setFieldFlags(bboxSize, FF_X3D_ONLY);
    octreeTriangles.set(
        addField(SFNODE, "octreeTriangles", new SFNode(),
                 KAMBI_KAMBI_OCTREE_PROPERTIES));
    setFieldFlags(octreeTriangles, FF_KAMBI_ONLY);
    shading.set(
        addExposedField(SFSTRING, "shading", new SFString("DEFAULT")));
    setFieldFlags(shading, FF_KAMBI_ONLY);

    isPickable.set(
        addExposedField(SFBOOL, "isPickable", new SFBool(true)));
    setFieldFlags(isPickable, FF_X3DOM_ONLY);

    render.set(
        addExposedField(SFBOOL, "render", new SFBool(true)));
    setFieldFlags(render, FF_X3DOM_ONLY);
}

Node *
ProtoShape::create(Scene *scene)
{ 
    return new NodeShape(scene, this);
}

NodeShape::NodeShape(Scene *scene, Proto *def)
  : Node(scene, def)
{
    m_appearance.set(((ProtoShape *)def)->appearance);
    m_geometry.set(((ProtoShape *)def)->geometry);
}

void
NodeShape::createNewAppearance(bool emissiveDefaultColor) 
{
    NodeAppearance *nAppearance = (NodeAppearance *)
                                  m_scene->createNode("Appearance");
    NodeMaterial *nMaterial = (NodeMaterial *)m_scene->createNode("Material");
    if (emissiveDefaultColor)
        nMaterial->emissiveColor(new SFColor(0.8, 0.8, 0.8));
    nAppearance->material(new SFNode(nMaterial));
    appearance(new SFNode(nAppearance));
}

void
NodeShape::draw(int pass)
{
    Node *appearance = ((SFNode *) getField(m_appearance))->getValue();
    bool transparent = false;
    if (appearance)
        transparent = appearance->isTransparent();
    if (transparent && (pass == RENDER_PASS_NON_TRANSPARENT))
        return;
    if (!transparent && (pass == RENDER_PASS_TRANSPARENT))
        return;

    Node *geometry = ((SFNode *) getField(m_geometry))->getValue();

    if (isLit()) {
        glEnable(GL_LIGHTING);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    } else {
        glDisable(GL_LIGHTING);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        Util::myGlColor3f(1.0f, 1.0f, 1.0f);
        float c[4];
        c[0] = c[1] = c[2] = c[3] = 1;
        Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);
        Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);
    }

    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glAlphaFunc(GL_NOTEQUAL, 0);

    if (appearance) appearance->bind();
    if (geometry) {
        glPushName(geometry_Field());  // field geometry
        glPushName(0);                 // index 0
        geometry->draw();
        doSpecularPass(appearance, geometry);
        glPopName();
        glPopName();
    }
    if (appearance) appearance->unbind();
    glEnable(GL_LIGHTING);
}

bool
NodeShape::isLit()
{
    NodeAppearance *appearance = (NodeAppearance *)
                                 ((SFNode *) getField(m_appearance))->getValue();
    if (appearance == NULL)
        return false;
    if (appearance->material() && appearance->material()->getValue())
        return true;
    if (appearance->texture() && appearance->texture()->getValue())
        return true;
    return false;
}

void
NodeShape::doSpecularPass(Node *appearance, Node *geometry)
{
//    if (!glIsEnabled(GL_TEXTURE_2D)) return;

    if ((appearance != NULL)) {
        Node *nMaterial = ((NodeAppearance *)appearance)->material() ?
            ((NodeAppearance *)appearance)->material()->getValue() : NULL;
        if ((nMaterial != NULL) && (nMaterial->getType() == VRML_MATERIAL)) {
            if (((NodeMaterial *) nMaterial)->specularColor() == NULL)
                return;
            if (((NodeMaterial *) nMaterial)->transparency() == NULL)
                return;
            const float *specular = 
                  ((NodeMaterial *) nMaterial)->specularColor()->getValue();
            float transparency = 
                  ((NodeMaterial *) nMaterial)->transparency()->getValue();
            if (specular[0] != 0.0f || specular[1] != 0.0f || 
                specular[2] != 0.0f) {
                float s[4];
                // premultiply by the transparency
                s[0] = specular[0] * (1.0f - transparency);
                s[1] = specular[1] * (1.0f - transparency);
                s[2] = specular[2] * (1.0f - transparency);
                s[3] = 1.0f;
                glDisable(GL_TEXTURE_2D);
                static float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
                Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, black);
                Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, black);
                Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
                Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, s);
                glBlendFunc(GL_ONE, GL_ONE);
                glEnable(GL_BLEND);
                geometry->draw();
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glDisable(GL_BLEND);
            }
        }
    }
}

void
NodeShape::flip(int index)
{
    Node *ngeometry = geometry()->getValue();

    if (ngeometry != NULL)
        ngeometry->flip(index);
}

void
NodeShape::swap(int fromTo)
{
    Node *ngeometry = geometry()->getValue();

    if (ngeometry != NULL)
        ngeometry->swap(fromTo);
}

int         
NodeShape::countPolygons(void)
{
    int ret = 0;    
    Node *ngeometry = geometry()->getValue();
    
    if (ngeometry != NULL)
        ret += ngeometry->countPolygons();
    return ret;
}

int         
NodeShape::countPrimitives(void)
{
    int ret = 0;    
    Node *ngeometry = geometry()->getValue();
    
    if (ngeometry != NULL)
        ret += ngeometry->countPrimitives();
    return ret;
}

int         
NodeShape::countPolygons1Sided(void)
{
    int ret = 0;    
    Node *ngeometry = geometry()->getValue();
    
    if (ngeometry != NULL)
        ret += ngeometry->countPolygons1Sided();
    return ret;
}

int         
NodeShape::countPolygons2Sided(void)
{
    int ret = 0;    
    Node *ngeometry = geometry()->getValue();
    
    if (ngeometry != NULL)
        ret += ngeometry->countPolygons2Sided();
    return ret;
}

float   
NodeShape::getTransparency(void)
{
    float ret = 0;    
    Node *appearance = ((SFNode *) getField(m_appearance))->getValue();
    if (appearance)
        ret = appearance->getTransparency();
    return ret;
}

int
NodeShape::write(int filedes, int indent, bool avoidUse)
{
    Node *geo = geometry()->getValue();
    bool vrml97 = false;
    if (m_scene->isPureVRML() && (geo != NULL))
        vrml97 = true;
    if (vrml97 && (geo->getType() == VRML_NURBS_GROUP)) {
        NodeNurbsGroup *nurbsGroup = (NodeNurbsGroup *)geo;
        NodeGroup group(nurbsGroup);
        RET_ONERROR( group.write(filedes, indent, avoidUse) )                
    } else if (vrml97 && (geo->getType() == X3D_NURBS_SET)) {
        NodeNurbsSet *nurbsSet = (NodeNurbsSet *)geo;
        NodeGroup group(nurbsSet);
        RET_ONERROR( group.write(filedes, indent, avoidUse) )
    } else
        RET_ONERROR( Node::write(filedes, indent, avoidUse) )
    return 0;
}

Node *
NodeShape::convert2Vrml(void) 
{
    Node *geo = geometry()->getValue();
    if (geo != NULL) {
        if (geo->getType() == VRML_NURBS_GROUP)
            return geo->convert2Vrml();
        else if (geo->getType() == X3D_NURBS_SET)
            geometry(new SFNode(geo->convert2Vrml()));
        else
            return geo->convert2Vrml();
    }
    return NULL;
}

bool
NodeShape::canWriteAc3d()
{
    Node *ngeometry = geometry()->getValue();
    if (ngeometry)
        return ngeometry->canWriteAc3d();
    return false;
}

bool
NodeShape::canWriteCattGeo()
{
    Node *ngeometry = geometry()->getValue();
    if (ngeometry)
        return ngeometry->canWriteCattGeo();
    return false;
}


