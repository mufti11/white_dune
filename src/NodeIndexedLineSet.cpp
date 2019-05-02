/*
 * NodeIndexedLineSet.cpp
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

#include "NodeIndexedLineSet.h"
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
#include "NodePointSet.h"
#include "NodeFogCoordinate.h"
#include "Util.h"
#include "LdrawDefines.h"

ProtoIndexedLineSet::ProtoIndexedLineSet(Scene *scene)
  : GeometryProto(scene, "IndexedLineSet")
{
    attrib.set(
          addExposedField(MFNODE, "attrib", new MFNode(),
                          VERTEX_ATTRIBUTE_NODE));
    setFieldFlags(attrib, FF_X3D_ONLY); 
    color.set(
          addExposedField(SFNODE, "color", new SFNode(NULL), COLOR_NODE));
    coord.set(
          addExposedField(SFNODE, "coord", new SFNode(NULL), COORDINATE_NODE));
    colorIndex.set(
          addField(MFINT32, "colorIndex", new MFInt32(), new SFInt32(-1)));
    colorPerVertex.set(
          addField(SFBOOL, "colorPerVertex", new SFBool(true)));
    coordIndex.set(
          addField(MFINT32, "coordIndex", new MFInt32(), new SFInt32(-1)));
    fogCoord.set(
          addExposedField(SFNODE, "fogCoord", new SFNode(NULL), 
                          X3D_FOG_COORDINATE));

    x3domGeometryCommonFields()

    ccw.set(
          addExposedField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_X3DOM_ONLY);

    solid.set(
          addExposedField(SFBOOL, "solid", new SFBool(true)));
    setFieldFlags(solid, FF_X3DOM_ONLY);

    addEventIn(MFINT32, "set_colorIndex");
    addEventIn(MFINT32, "set_coordIndex");
}

Node *
ProtoIndexedLineSet::create(Scene *scene)
{ 
    return new NodeIndexedLineSet(scene, this); 
}

NodeIndexedLineSet::NodeIndexedLineSet(Scene *scene, Proto *def)
  : GeometryNode(scene, def)
{
}

void 
NodeIndexedLineSet::draw()
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
NodeIndexedLineSet::lineDraw()
{
    Node *coord = ((SFNode *) getField(coord_Field()))->getValue();
    MFInt32 *mfColorIndex = colorIndex();
    MFInt32 *mfCoordIndex = coordIndex();
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
    int colorInc = 3;
    if (color()->getValue()) {
        if (color()->getValue()->getType() == VRML_COLOR) {
            colors = ((NodeColor *)(color()->getValue()))->color();
            if (mfColorIndex->getSize() != mfCoordIndex->getSize())
                mfColorIndex = mfCoordIndex;
        } else if (color()->getValue()->getType() == X3D_COLOR_RGBA) {
            colors = ((NodeColorRGBA *)(color()->getValue()))->color();
            if (mfColorIndex->getSize() != mfCoordIndex->getSize())
                mfColorIndex = mfCoordIndex;
            colorInc = 4;
        }
    }
    if (colors == NULL) {
        float c[4];
        glGetMaterialfv(GL_FRONT, GL_EMISSION, c);
        Util::myGlColor4fv(c);
    }

    if (!coord)
        return;

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

    bool inSet = false;

    int numLine = 0;
    int size = mfCoordIndex->getSize();
    for (int i = 0; i < size; i++) {
        int index = mfCoordIndex->getValue(i);
        if (index < 0) {
            if (inSet) {
                glEnd();
                inSet = false;
                numLine++;
            }
        } else {
            if (!inSet) {
                glBegin(GL_LINE_STRIP);
                inSet = true;
            }
            if (colors != NULL) {
                int cindex = -1; 
                if (colorPerVertex()->getValue()) {
                    if (i < mfColorIndex->getSize())
                        cindex = mfColorIndex->getValue(i) * colorInc;
                } else {
                    if (mfColorIndex->getSize() > 0) {
                        if (numLine < mfColorIndex->getSize())
                            cindex = mfColorIndex->getValue(numLine) * colorInc;
                    } else
                        cindex = numLine * colorInc;
                }
                if ((cindex >= 0) && (cindex < colors->getSize())) 
                    Util::myGlColor3fv(colors->getValues() + cindex);
            }
            if (index < coordSize) {
                if (coords)
                    glVertex3fv(coords->getValue(index));
                else
                    glVertex3dv(coordsDouble->getValue(index));
#ifdef HAVE_GLFOGCOORDF
                if (fogDepth) {
                    int fogIndex = fogDepth->getSize() - 1;
                    if (index < fogDepth->getSize())
                        fogIndex = index;
                    if (fogIndex > -1)
                        glFogCoordf(fogDepth->getValue(fogIndex));
               }
#endif
            }
        }
    }
    if (inSet) glEnd();
    glEnable(GL_LIGHTING);
    glPopAttrib();
}

Node * 
NodeIndexedLineSet::toPointSet(void)
{
    NodePointSet *node = (NodePointSet *)m_scene->createNode("PointSet");
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
NodeIndexedLineSet::getMinBoundingBox(void)
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
NodeIndexedLineSet::getMaxBoundingBox(void)
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
NodeIndexedLineSet::flip(int index)
{
    NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
    if (ncoord)
        if (ncoord->getType() == VRML_COORDINATE)
            ncoord->flip(index);
}

void
NodeIndexedLineSet::swap(int fromTo)
{
    NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
    if (ncoord)
        if (ncoord->getType() == VRML_COORDINATE) 
             ncoord->swap(fromTo);
}

int NodeIndexedLineSet::getProfile(void) const
{ 
    if (hasInput("set_colorIndex"))
        return PROFILE_IMMERSIVE;
    if (hasInput("set_coordIndex"))
        return PROFILE_IMMERSIVE;
    return PROFILE_INTERCHANGE; 
}

int
NodeIndexedLineSet::writeAc3d(int f, int indent)
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
    MFInt32 *colorIndex = this->colorIndex();
    bool colorPerVertexProblem = colorPerVertex()->getValue();
    if (!colors || (colors && (colors->getSize() == 0)))
        if (colorPerVertexProblem)
            colorPerVertexProblem = false;
    if (colorPerVertexProblem) {
        const char *name = "";
        if (hasName())
            name = getName();
        else
            name = getProto()->getName(m_scene->isX3d());
        swDebugf("Warning: color node of %s node can not ", name);
        swDebugf("be exported to AC3D cause of colorPerVertex TRUE\n");
    }
    RET_ONERROR( mywritef(f, "numvert %d\n", vertices->getSFSize()) )
    for (int i = 0; i < vertices->getSFSize(); i++)
        RET_ONERROR( mywritef(f, "%f %f %f\n",
                              vertices->getValue(i)[0],
                              vertices->getValue(i)[1],
                              vertices->getValue(i)[2]) )
    int numberPolyLines = 0;
    MyArray<char *> stringsToWrite;
    int lineFlags = 0x2; // line
    int startPolyLine = 0;
    bool inPolyLine = false;
    for (int i = 0; i < coordIndex()->getSize(); i++)
        if (coordIndex()->getValue(i) < 0) {
            if (inPolyLine) {
                numberPolyLines++;
                char buf[1024];  
                mysnprintf(buf, 1023, "SURF 0x%x\n", lineFlags);
                stringsToWrite.append(strdup(buf));
                int index = -1; 
                bool materialToWrite = true;
                if (!colorPerVertexProblem) {
                    if (colorIndex && (colorIndex->getSize() > 0)) {
                        if (i < colorIndex->getSize()) {
                            index = colorIndex->getValue(i);
                        }
                    } else
                        index = i;
                    int materialIndex = 0;
                    if ((index >= 0) && color)
                        materialIndex = color->getAc3dMaterialIndex(index);
                    if ((index >= 0) && colors && (index < colors->getSize()) &&
                        (materialIndex > -1)) { 
                        mysnprintf(buf, 1023, "mat %d\n", materialIndex);
                        stringsToWrite.append(strdup(buf));
                        materialToWrite = false;
                    }
                }
                if ((materialToWrite) && (nmaterial)) {
                    mysnprintf(buf, 1023, "mat %d\n", 
                                          nmaterial->getAc3dMaterialIndex());
                } else
                    mysnprintf(buf, 1023, "mat %d\n",
                                          m_scene->getAc3dEmptyMaterial());
                stringsToWrite.append(strdup(buf));

                mysnprintf(buf, 1023, "refs %d\n", i - startPolyLine);
                stringsToWrite.append(strdup(buf));
                for (int j = startPolyLine; j < i; j++) {       
                    mysnprintf(buf, 1023, "%d 0 0\n", 
                               coordIndex()->getValue(j));
                    stringsToWrite.append(strdup(buf));
                }
            } 
            startPolyLine = i + 1;
            inPolyLine = false;
        } else {
            inPolyLine = true;
        }    
    RET_ONERROR( mywritef(f, "numsurf %d\n", numberPolyLines) )
    for (int i = 0; i < stringsToWrite.size(); i++) {
        RET_ONERROR( mywritestr(f, stringsToWrite[i]) )
        free(stringsToWrite[i]);
    }         
    RET_ONERROR( mywritestr(f, "kids 0\n") )
    return 0;
}


int
NodeIndexedLineSet::writeLdrawDat(int f, int indent)
{
    MFVec3f *vertices = NULL;
    NodeCoordinate *ncoord = (NodeCoordinate *)(coord()->getValue());
    if (ncoord == NULL)
        return 0;
    else
        vertices = ncoord->point();
    if ((vertices == NULL) || (coordIndex()->getValues() == NULL))
        return 0;
    Matrix matrix;
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) matrix);
    RET_ONERROR( mywritestr(f, "0 //") )
    if (hasName())
        RET_ONERROR( mywritef(f, " %s", (const char *)getName()))
    RET_ONERROR( mywritef(f, " %s\n", (const char *)getProto()->getName(false)))

    if (hasName())
        RET_ONERROR( mywritef(f, "name \"%s\"\n", (const char *)getName()) )
        
    for (int i = 0; i < coordIndex()->getSize(); i++) {
        if ((i > 0) &&  
            (coordIndex()->getValue(i) != -1) &&
            (coordIndex()->getValue(i - 1) != -1)) {
            RET_ONERROR( mywritef(f, "2 24 ") )
            for (int j = 0; j < 2; j++)  {
                int index = coordIndex()->getValue(i - j); 
                Vec3f v(vertices->getValue(index)[0],
                        vertices->getValue(index)[1],
                        vertices->getValue(index)[2]);
                v = matrix * v;
                RET_ONERROR( mywritef(f, " %f %f %f",  v.z * LDRAW_SCALE, 
                                                      -v.y * LDRAW_SCALE, 
                                                       v.x * LDRAW_SCALE) )
            }
            RET_ONERROR( mywritestr(f, "\n") )
        }
    }
    return 0;
}

