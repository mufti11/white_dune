/*
 * NodeLineSet.cpp
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

#include "NodeLineSet.h"
#include "Proto.h"
#include "FieldValue.h"
#include "Scene.h"
#include "SFNode.h"
#include "MFInt32.h"
#include "SFInt32.h"
#include "SFBool.h"
#include "MFColor.h"
#include "MFVec3f.h"

#include "NodeColor.h"
#include "NodeColorRGBA.h"
#include "NodeAppearance.h"
#include "NodeMaterial.h"
#include "NodeImageTexture.h"
#include "NodeTextureTransform.h"
#include "NodeShape.h"
#include "NodeCoordinate.h"
#include "NodeGeoCoordinate.h"
#include "NodeFogCoordinate.h"
#include "Util.h"
#include "LdrawDefines.h"

ProtoLineSet::ProtoLineSet(Scene *scene)
  : GeometryProto(scene, "LineSet")
{
    attrib.set(
          addExposedField(MFNODE, "attrib", new MFNode(),
                          VERTEX_ATTRIBUTE_NODE));
    setFieldFlags(attrib, FF_X3D_ONLY); 
    color.set(
          addExposedField(SFNODE, "color", new SFNode(NULL), COLOR_NODE));
    coord.set(
          addExposedField(SFNODE, "coord", new SFNode(NULL), COORDINATE_NODE));
    fogCoord.set(
          addExposedField(SFNODE, "fogCoord", new SFNode(NULL), 
                          X3D_FOG_COORDINATE));
    setFieldFlags(fogCoord, FF_X3D_ONLY);
    vertexCount.set(
          addField(MFINT32, "vertexCount", new MFInt32()));

    x3domGeometryCommonFields()

    ccw.set(
          addExposedField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_X3DOM_ONLY);

    solid.set(
          addExposedField(SFBOOL, "solid", new SFBool(true)));
    setFieldFlags(solid, FF_X3DOM_ONLY);
    }

Node *
ProtoLineSet::create(Scene *scene)
{ 
    return new NodeLineSet(scene, this); 
}

NodeLineSet::NodeLineSet(Scene *scene, Proto *def)
  : GeometryNode(scene, def)
{
}

void 
NodeLineSet::draw()
{
    Node *ncoord = coord()->getValue();
    if (ncoord != NULL) {
        glPushName(coord_Field());       // field coord
        glPushName(0);                   // index 0
        if (ncoord->getType() == VRML_COORDINATE)
             ((NodeCoordinate *)ncoord)->draw(this);
        else if (ncoord->getType() == VRML_GEO_COORDINATE) {
            ((NodeGeoCoordinate *)ncoord)->draw(this);
        }
        glPopName();
        glPopName();
    }
}

void
NodeLineSet::lineDraw()
{
    Node *coord = ((SFNode *) getField(coord_Field()))->getValue();
    MFFloat *colors = NULL;
    float lineSize = TheApp->GetPointSetSize();

    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    if (lineSize == 0.0) {
        glEnable(GL_LINE_SMOOTH);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glLineWidth(1.0);
    } else {
        glDisable(GL_LINE_SMOOTH);
        glLineWidth(lineSize);
    }
    if (color()->getValue()) {
        if (color()->getValue()->getType() == VRML_COLOR) {
            colors = ((NodeColor *)(color()->getValue()))->color();
        } else if (color()->getValue()->getType() == X3D_COLOR_RGBA) {
            colors = ((NodeColorRGBA *)(color()->getValue()))->color();
        }
    }
    if (colors == NULL) {
        float c[4];
        glGetMaterialfv(GL_FRONT, GL_EMISSION, c);
        Util::myGlColor4fv(c);
    }

    MFVec3f *coords = NULL;
    MFVec3d *coordsDouble = NULL;
    int coordSize;
    if (coord->getType() == VRML_COORDINATE) {
        coords = ((NodeCoordinate *) coord)->point();
        coordSize = coords->getSize();
    } else {
        coordsDouble = ((NodeGeoCoordinate *) coord)->pointX3D();
        coordSize = coordsDouble->getSize();
    }

    MFFloat *fogDepth = NULL;
    if (fogCoord()->getValue())
        if (fogCoord()->getValue()->getType() == X3D_FOG_COORDINATE)
            fogDepth = ((NodeFogCoordinate *) 
                         (fogCoord()->getValue()))->depth();


    int numVertices = 0;
    for (int i = 0; i < vertexCount()->getSize(); i++) {
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < vertexCount()->getValue(i); j++) {
            if (colors != NULL) {
                if ((numVertices >= 0) && (numVertices < colors->getSFSize())) 
                    Util::myGlColor3fv(colors->getValues() + numVertices * 3);
            }
            if (numVertices < coordSize) {
                if (coords)
                    glVertex3fv(coords->getValue(numVertices));
                else
                    glVertex3dv(coordsDouble->getValue(numVertices));
#ifdef HAVE_GLFOGCOORDF
                if (fogDepth) {
                    int fogIndex = fogDepth->getSize() - 1;
                    if (i < fogDepth->getSize())
                        fogIndex = i;
                    if (fogIndex > -1)
                        glFogCoordf(fogDepth->getValue(fogIndex));
               }
#endif
            }
            numVertices++;    
        }
        glEnd();
    }
    glEnable(GL_LIGHTING);
    glPopAttrib();
}

Node * 
NodeLineSet::toPointSet(void)
{
    NodeLineSet *node = (NodeLineSet *)
                               m_scene->createNode("PointSet");
    NodeCoordinate *lineCoord = (NodeCoordinate *)coord()->getValue();
    if (lineCoord != NULL) {
        NodeCoordinate *ncoord = (NodeCoordinate *)
                                 m_scene->createNode("Coordinate");
        ncoord->point(new MFVec3f(*(lineCoord->point())));
        node->coord(new SFNode(ncoord));
    }
    NodeColor *lineColor = (NodeColor *)color()->getValue();
    if (lineColor != NULL) {
        NodeColor *ncolor = (NodeColor *) m_scene->createNode("Color");
        ncolor->color(new MFColor(*(lineColor->color())));
        node->color(new SFNode(ncolor));
    }
    return node;
}


Vec3f
NodeLineSet::getMinBoundingBox(void)
{
    Vec3f ret(0, 0, 0);
    Node *coord = ((SFNode *) getField(coord_Field()))->getValue();
    if (coord != NULL) {
        MFVec3f *coords = ((NodeCoordinate *)coord)->point();
        if (coords != NULL)
            ret = coords->getMinBoundingBox();
   }
   return ret;
}

Vec3f
NodeLineSet::getMaxBoundingBox(void)
{
    Vec3f ret(0, 0, 0);
    Node *coord = ((SFNode *) getField(coord_Field()))->getValue();
    if (coord != NULL) {
        MFVec3f *coords = ((NodeCoordinate *)coord)->point();
        if (coords != NULL)
            ret = coords->getMaxBoundingBox();
   }
   return ret;
}

void
NodeLineSet::flip(int index)
{
    NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
    if (ncoord)
        if (ncoord->getType() == VRML_COORDINATE)
            ncoord->flip(index);
}

void
NodeLineSet::swap(int fromTo)
{
    NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
    if (ncoord)
        if (ncoord->getType() == VRML_COORDINATE) 
             ncoord->swap(fromTo);
}

int
NodeLineSet::writeAc3d(int f, int indent)
{
    NodeColor *color = (NodeColor *)this->color()->getValue();

    NodeMaterial *nmaterial = GeometryNode::getMaterialNode();

    NodeCoordinate *coordinateNode = (NodeCoordinate *)coord()->getValue();
    MFVec3f *vertices = NULL;
    if (coordinateNode)
        vertices = coordinateNode->point();
    else
        return 0;

    RET_ONERROR( mywritestr(f, "OBJECT poly\n") )

    if (hasName())
        RET_ONERROR( mywritef(f, "name \"%s\"\n", (const char *)getName()) )
        
    MFColor *colors = NULL;
    if (color)
        colors = color->color();
    RET_ONERROR( mywritef(f, "numvert %d\n", vertices->getSFSize()) )
    for (int i = 0; i < vertices->getSFSize(); i++)
        RET_ONERROR( mywritef(f, "%f %f %f\n",
                              vertices->getValue(i)[0],
                              vertices->getValue(i)[1],
                              vertices->getValue(i)[2]) )
    int lineFlags = 0x2; // line
    int sumVertices = 0;
    RET_ONERROR( mywritef(f, "numsurf %d\n", 1) )
    for (int i = 0; i < vertexCount()->getSize(); i++) {
        RET_ONERROR( mywritef(f, "SURF 0x%x\n", lineFlags) )
        int index = sumVertices; 
        bool materialToWrite = true;
        int materialIndex = 0;
        if ((index >= 0) && color)
            materialIndex = color->getAc3dMaterialIndex(index);
        if ((index >= 0) && colors && (index < colors->getSize()) &&
            (materialIndex > -1)) { 
            RET_ONERROR( mywritef(f, "mat %d\n", materialIndex) )
            materialToWrite = false;
        }
        if ((materialToWrite) && (nmaterial)) {
            RET_ONERROR( mywritef(f, "mat %d\n",
                                  nmaterial->getAc3dMaterialIndex()) )
        } else
            RET_ONERROR( mywritef(f, "mat %d\n", 
                                  m_scene->getAc3dEmptyMaterial()) )
        RET_ONERROR( mywritef(f, "refs %d\n", vertexCount()->getValue(i)) )
        for (int j = 0; j < vertexCount()->getValue(i); j ++) {
            RET_ONERROR( mywritef(f, "%d 0 0\n", j) )
        }
        sumVertices += vertexCount()->getValue(i);
    } 
/*
    for (int i = 0; i < stringsToWrite.size(); i++) {
        RET_ONERROR( mywritestr(f, stringsToWrite[i]) )
        free(stringsToWrite[i]);
    }         
*/
    RET_ONERROR( mywritestr(f, "kids 0\n") )
    return 0;
}


int
NodeLineSet::writeLdrawDat(int f, int indent)
{
    MFVec3f *vertices = NULL;
    NodeCoordinate *ncoord = (NodeCoordinate *)(coord()->getValue());
    if (ncoord == NULL)
        return 0;
    else
        vertices = ncoord->point();
    if ((vertices == NULL))
        return 0;
    Matrix matrix;
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) matrix);
    RET_ONERROR( mywritestr(f, "0 //") )
    if (hasName())
        RET_ONERROR( mywritef(f, " %s", (const char *)getName()))
    RET_ONERROR( mywritef(f, " %s\n", (const char *)getProto()->getName(false)))

    if (hasName())
        RET_ONERROR( mywritef(f, "name \"%s\"\n", (const char *)getName()) )
        
    int sumVertices = 0;
    for (int i = 0; i < vertexCount()->getSize(); i++) {
        for (int j = 0; j < vertexCount()->getValue(i) - 1; j++)  {
            RET_ONERROR( mywritef(f, "2 24 ") )
            int index = sumVertices + j; 
            for (int k = 0; k < 2; k++) {
                Vec3f v(vertices->getValue(index + k)[0],
                        vertices->getValue(index + k)[1],
                        vertices->getValue(index + k)[2]);
                v = matrix * v;
                RET_ONERROR( mywritef(f, " %f %f %f",  v.z * LDRAW_SCALE, 
                                                      -v.y * LDRAW_SCALE, 
                                                       v.x * LDRAW_SCALE) )
            }
            RET_ONERROR( mywritestr(f, "\n") )
        }
        sumVertices += vertexCount()->getValue(i);
    }
    return 0;
}

