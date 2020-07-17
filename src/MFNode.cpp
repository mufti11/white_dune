/*
 * MFNode.cpp
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

#ifndef FLT_MIN
# include <float.h>
#endif

#include "MFNode.h"
#include "SFNode.h"
#include "Node.h"
#include "Proto.h"
#include "DuneApp.h"
#include "NodeCurveAnimation.h"

MFNode::MFNode()
{
    m_value = new NodeList();
    m_containerField = -1;
}

MFNode::MFNode(const MFNode &other)
{
    m_value = new NodeList();
    if (other.m_value)
        for (long i = 0; i < other.m_value->size(); i++) {
            m_value->set(i, other.m_value->get(i)->copy());
            m_value->get(i)->ref();
            if (!m_value->get(i)->isPROTO())
                m_value->get(i)->reInit();
        }
    m_containerField = ((MFNode *)&other)->getContainerField();
}

MFNode::MFNode(const MFNode *other)
{
    int size = other->m_value->size();

    m_value = new NodeList();
    for (int i = 0; i < size; i++) {
        m_value->set(i, other->m_value->get(i)->copy());
        m_value->get(i)->ref();
        if (!m_value->get(i)->isPROTO())
            m_value->get(i)->reInit();
    }
    m_containerField = ((MFNode *)other)->getContainerField();
}

MFNode::MFNode(const Node *other)
{
    m_value = new NodeList();
    m_value->set(0, other->copy());
    m_value->get(0)->ref();
     if (!m_value->get(0)->isPROTO())
         m_value->get(0)->reInit();
    m_containerField = -1;
}

MFNode::MFNode(NodeList *value)
{
    m_value = new NodeList();
    if (value == NULL)
        return;
    for (long i = 0; i < value->size(); i++) {
        Node *node = value->get(i);
        if (node != NULL) {
            node->ref();
            m_value->append(node);
        }
    }
    m_containerField = -1;
}

MFNode::~MFNode()
{
    for (long i = 0; i < m_value->size(); i++) {
        Node *node = m_value->get(i);
        if (node != NULL)
            node->unref();
    }
    delete m_value;
}

bool
MFNode::writeBrackets(void) const
{
    if (m_value->size() == 1) {
        if (m_value->get(0)->getType() == VRML_COMMENT)
            return true;
        return false;
    }
    return true;
}


int MFNode::writeData(int f, int i) const
{
    return m_value->get(i)->write(f, 0);
}

int MFNode::write(int f, int indent, bool writeBrackets) const
{ 
    if (writeBrackets) {
        if (!TheApp->GetkrFormating()) {
            RET_ONERROR( mywritestr(f, "\n") )
            TheApp->incSelectionLinenumber();
            RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
        }
        RET_ONERROR( mywritestr(f, "[\n") )
        TheApp->incSelectionLinenumber();
    } else if (isMFNode()) {
        RET_ONERROR( mywritestr(f, "\n") )
        TheApp->incSelectionLinenumber();
    }
    for (int i = 0; i < getSFSize(); i++) {
        Node *node = m_value->get(i);
        if (node != NULL) {
            RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
            RET_ONERROR( node->write(f, indent  + TheApp->GetIndent()) )
        }
    }
    if (writeBrackets) {
        if (!TheApp->GetkrFormating())
            RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
        else
            RET_ONERROR( indentf(f, indent) )
        RET_ONERROR( mywritestr(f, "]\n") )
        TheApp->incSelectionLinenumber();
    }
    return(0);
}

int 
MFNode::writeXml(int f, int indent, int containerField, bool avoidUse) 
const
{ 
    for (int i = 0; i < getSFSize(); i++) {
        Node *node = m_value->get(i);
        if (node != NULL) {
            RET_ONERROR( node->writeXml(f, indent  + TheApp->GetIndent(),
                                        m_containerField, avoidUse) )
        }
    }
    return(0);
}

const char *
MFNode::getTypeC(int languageFlag) const 
{ 
    if (languageFlag & JAVA_SOURCE)
        return TheApp->getCNodeNameArray(); 
    return TheApp->getCNodeNamePtr();
}

int         
MFNode::writeC(int filedes, const char* variableName, int languageFlag) const
{
    if (languageFlag & C_SOURCE) {
        RET_ONERROR( mywritestr(filedes, "m_") )
        RET_ONERROR( mywritestr(filedes, variableName) )
        if (getSFSize() == 0) {
            RET_ONERROR( mywritestr(filedes, " = ") )
            RET_ONERROR( mywritestr(filedes, "NULL;\n") )
            return 0;
        }
        RET_ONERROR( mywritef(filedes, "[%d];\n", getSFSize()) )
        for (int i = 0; i < getSFSize(); i++) {
            RET_ONERROR( mywritestr(filedes, "    ") )
            RET_ONERROR( mywritestr(filedes, "m_") )
            RET_ONERROR( mywritestr(filedes, variableName) )
            RET_ONERROR( mywritef(filedes, "[%d] =", i) )
            Node *node = m_value->get(i);
            bool writeNull = node == NULL;
            if (!writeNull) {
                if (node->getProto()->isMismatchingProto())
                    writeNull = true;
                if (node->getType() == VRML_COMMENT)
                    writeNull = true;
            }
            if (writeNull)
                RET_ONERROR( mywritestr(filedes, "NULL") )
            else {
                RET_ONERROR( mywritestr(filedes, "&") )
                if (node->getType() != DUNE_CURVE_ANIMATION)
                    RET_ONERROR( node->writeCVariable(filedes, languageFlag) )
                else {
                    NodeCurveAnimation *curve = (NodeCurveAnimation *) node;
                    Node *inter = curve->getPositionInterpolator();
                    RET_ONERROR( inter->writeCVariable(filedes, languageFlag) )
                    RET_ONERROR( mywritestr(filedes, ", &") )
                    inter = curve->getOrientationInterpolator();
                    RET_ONERROR( inter->writeCVariable(filedes, languageFlag) )
                }
            } 
            RET_ONERROR( mywritestr(filedes, ";\n") )
        }
    } else {
        RET_ONERROR( mywritestr(filedes, "m_") )
        RET_ONERROR( mywritestr(filedes, variableName) )
        if (languageFlag & CC_SOURCE)
            if (getSFSize() == 0) {
                RET_ONERROR( mywritestr(filedes, "[1];\n") );
                return 0;
            }
        if (languageFlag & CC_SOURCE)
            RET_ONERROR( mywritestr(filedes, "[]") )
        RET_ONERROR( mywritestr(filedes, " = { ") )
        for (int i = 0; i < getSFSize(); i++) {
            if (i != 0)
                RET_ONERROR( mywritestr(filedes, ", ") )
            Node *node = m_value->get(i);
            bool writeNull = node == NULL;
            if (!writeNull) {
                if (node->getProto()->isMismatchingProto())
                    writeNull = true;
                if (node->getType() == VRML_COMMENT)
                    writeNull = true;
            }
            if (writeNull) {
                if (languageFlag & JAVA_SOURCE)
                    RET_ONERROR( mywritestr(filedes, "null") )
                else
                    RET_ONERROR( mywritestr(filedes, "NULL") )
            } else {
                if (languageFlag & CC_SOURCE)
                    RET_ONERROR( mywritestr(filedes, "&") )
                if (node->getType() != DUNE_CURVE_ANIMATION)
                    RET_ONERROR( node->writeCVariable(filedes, languageFlag) )
                else {
                    NodeCurveAnimation *curve = (NodeCurveAnimation *) node;
                    Node *inter = curve->getPositionInterpolator();
                    RET_ONERROR( inter->writeCVariable(filedes, languageFlag) )
                    RET_ONERROR( mywritestr(filedes, ", ") )
                    if ((languageFlag & C_SOURCE) || (languageFlag & CC_SOURCE))
                        RET_ONERROR( mywritestr(filedes, "&") )
                    inter = curve->getOrientationInterpolator();
                    RET_ONERROR( inter->writeCVariable(filedes, languageFlag) )
                }
            } 
        }
        RET_ONERROR( mywritestr(filedes, " };\n") )
    }
    return 0;
}

int         
MFNode::writeCDeclaration(int filedes, int languageFlag) const
{
    for (int i = 0; i < getSFSize(); i++) {
        Node *node = m_value->get(i);
        if (node != NULL)
            RET_ONERROR( node->getProto()->writeCDeclaration(filedes,
                                                             languageFlag) )
    }
    return(0);
}

bool        
MFNode::readLine(int index, char *line)
{
    assert(0);
    return false;
}

bool
MFNode::equals(const FieldValue *value) const
{
    if (value->getType() == MFNODE) {
        NodeList *v = ((MFNode *) value)->getValues();
        if (v->size() != m_value->size()) return false;
        for (long i = 0; i < m_value->size(); i++)
            if (v->get(i) != m_value->get(i)) return false;
        return true;
    }
    return false;
}

FieldValue *
MFNode::addNode(Node *node, int index) const
{
    NodeList *list = new NodeList();
    for (long i = 0; i < m_value->size(); i++) {
        list->append(m_value->get(i));
    }
    if (index == -1)
        list->append(node);
    else
        list->insert(node, index);
    MFNode *ret = new MFNode(list);
    ret->setContainerField(m_containerField);
    return ret;
}

FieldValue *
MFNode::removeNode(Node *node, int index) const
{
    NodeList *list = new NodeList();
    for (long i = 0; i < m_value->size(); i++) {
        list->append(m_value->get(i));
    }
    for (long i = 0; i < list->size(); i++) {
        if (list->get(i) == node) {
            if (index == -1)
                list->remove(i);
            else 
                list->remove(index);
            break;
        }
    }
    MFNode *ret = new MFNode(list);
    return ret;
}

FieldValue *
MFNode::getSFValue(int index) const
{
    return new SFNode(m_value->get(index));
}

void
MFNode::setSFValue(int index, FieldValue *value)
{
    m_value[index] = ((SFNode *) value)->getValue();
}

void
MFNode::setSFValue(int index, const Node *value)
{
    m_value[index] = (Node *)value;
}

MyString
MFNode::getEcmaScriptComment(MyString name, int flags) const
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // array ([0] [1] [2] ...) of VRML node objects\n";
    }
    if (TheApp->GetEcmaScriptAddAllowedComponents()) {
        ret += indent;
        ret += "// allowed components:\n";

        ret += indent;
        ret += "   // depends from type of node: ";
        ret += name;
        ret += "[???].eventin_of_node or ";
        ret += name;
        ret += "[???].eventout_of_node\n";
    }
    if (TheApp->GetEcmaScriptAddAvailableFunctions()) {
        ret += indent;
        ret += "// available functions:\n";
        if (flags != EL_EVENT_IN) {
            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = new MFNode(sfnode_n1, sfnode_n2, ...);\n";
        }
        if (flags != EL_EVENT_OUT) {
            ret += indent;
            ret += "   // int_i = ";
            ret += name;
            ret += ".length();\n";

            ret += indent;
            ret += "   // string_str = ";
            ret += name;
            ret += ".toString();\n";
        }
    }
    if (TheApp->GetEcmaScriptAddBrowserObject()) {
        ret += indent;
        ret += "// related Browser Object functions:\n";
        if (flags != EL_EVENT_IN) {
            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = Browser.createVrmlFromString(string_vrmlsyntax);\n";
        }
        if (flags != EL_EVENT_OUT) {
            ret += indent;
            ret += "   // Browser.replaceWorld(";
            ret += name;
            ret += ");\n";

            ret += indent;
            ret += "   // Browser.loadURL(";
            ret += name;
            ret += ", mfstring_parameter);\n";
        }
    }
    if (TheApp->GetEcmaScriptAddExampleUsage()) {
        ret += indent;
        ret += "// example usage:\n";
        if (flags != EL_EVENT_IN) {
             ret += indent;
             ret += "   // ";
             ret += name;
             ret += " = Browser.createVrmlFromString('Shape {geometry Text {string \\\"A\\\"}} SpotLight {}');\n";

             ret += indent;
             ret += "   // ";
             ret += name;
             ret += "[0].geometry.string = 'hello';\n";
        } 
        if (flags != EL_EVENT_OUT) {
             ret += indent;
             ret += "   // string_str =";
             ret += name;
             ret += ".toString();\n";
        }
    }
    return ret;
}

void    
MFNode::preDraw()
{
    NodeList *childList = getValues();

    for (long i = 0; i < childList->size(); i++)
        childList->get(i)->preDraw();
}

void    
MFNode::draw(int pass, int mfNodeField)
{
    NodeList *childList = getValues();

    glPushName(mfNodeField);  // field offset

    for (long i = 0; i < childList->size(); i++)
        childList->get(i)->bind();

    glPushName(0);
    for (long i = 0; i < childList->size(); i++) {
        glLoadName(i);
        childList->get(i)->draw(pass);
    }
    glPopName();

    for (long i = 0; i < childList->size(); i++)
        childList->get(i)->unbind();

    glPopName();
}

#include "Proto.h"

void
MFNode::flip(int index)
{
    NodeList *childList = getValues();

    for (long i = 0; i < childList->size(); i++) {
        Node *node = childList->get(i);
        if (!node->getFlag(NODE_FLAG_FLIPPED)) {
            node->setFlag(NODE_FLAG_FLIPPED);
            node->flip(index);
        }
    }
}

void
MFNode::swap(int fromTo)
{
    NodeList *childList = getValues();

    for (long i = 0; i < childList->size(); i++) {
        Node *node = childList->get(i);
        if (!node->getFlag(NODE_FLAG_SWAPPED)) {
            node->setFlag(NODE_FLAG_SWAPPED);
            node->swap(fromTo);
        }
    }
}

int         
MFNode::countPolygons(void)
{
    int ret = 0;
    NodeList *childList = getValues();

    for (long i = 0; i < childList->size(); i++)
        ret += childList->get(i)->countPolygons();

    return ret;
}

int         
MFNode::countPrimitives(void)
{
    int ret = 0;
    NodeList *childList = getValues();

    for (long i = 0; i < childList->size(); i++)
        ret += childList->get(i)->countPrimitives();

    return ret;
}

int         
MFNode::countPolygons1Sided(void)
{
    int ret = 0;
    NodeList *childList = getValues();

    for (long i = 0; i < childList->size(); i++)
        ret += childList->get(i)->countPolygons1Sided();

    return ret;
}

int         
MFNode::countPolygons2Sided(void)
{
    int ret = 0;
    NodeList *childList = getValues();

    for (long i = 0; i < childList->size(); i++)
        ret += childList->get(i)->countPolygons2Sided();

    return ret;
}

void
MFNode::update()
{
    NodeList *childList = getValues();

    for (long i = 0; i < childList->size(); i++)
        childList->get(i)->update();
}

#include "Scene.h"

Node *
MFNode::convert2X3d(void)
{
    NodeList *childList = getValues();
    NodeList *newChilds = new NodeList();

    bool newNodes = false;
    for (long i = 0; i < childList->size(); i++) {
        if (childList->get(i) != NULL) {
            Node *newNode = childList->get(i)->convert2X3d();
            if (newNode != NULL) {
                newChilds->append(newNode);
                newNodes = true;
            } else
                newChilds->append(childList->get(i));
        }
    }
    if (newNodes) {            
        m_value = new NodeList();
        for (long i = 0; i < newChilds->size(); i++) {
            m_value->set(i, newChilds->get(i)->copy());
            m_value->get(i)->ref();
            if (!m_value->get(i)->isPROTO())
                m_value->get(i)->reInit();
        }
    }
    return NULL;
}

Node *
MFNode::convert2Vrml(void)
{
    NodeList *childList = getValues();
    NodeList *newChilds = new NodeList();

    bool newNodes = false;
    for (long i = 0; i < childList->size(); i++) {
        if (childList->get(i) != NULL) {
            Node *newNode = childList->get(i)->convert2Vrml();
            if (newNode != NULL) {
                newChilds->append(newNode);
                newNodes = true;
            } else
                newChilds->append(childList->get(i));
        }
    }
    if (newNodes) {            
        m_value = new NodeList();
        for (long i = 0; i < newChilds->size(); i++) {
            m_value->set(i, newChilds->get(i)->copy());
            m_value->get(i)->ref();
            if (!m_value->get(i)->isPROTO())
                m_value->get(i)->reInit();
        }
    }
    return NULL;
}

int 
MFNode::writeAc3d(int f, int indent) const
{
    RET_ONERROR( getValues()->writeAc3d(f, indent) )
    return 0;
}


int 
MFNode::writeRib(int f, int indent) const
{
    RET_ONERROR( getValues()->writeRib(f, indent) )
    return 0;
}

int 
MFNode::writePovray(int f, int indent) const
{
    RET_ONERROR( getValues()->writePovray(f, indent) )
    return 0;
}

void
MFNode::handleAc3dMaterial(ac3dMaterialCallback callback, Scene* scene)
{
    for (int i = 0; i < getSize(); i++)
        getValue(i)->handleAc3dMaterial(callback, scene);
}

int MFNode::writeCattGeo(Node *node, int f, int indent) const
{
    RET_ONERROR( getValues()->writeCattGeo(node, f, indent) )
    return 0;
}

int 
MFNode::writeLdrawDat(int f, int indent) const
{
    RET_ONERROR( getValues()->writeLdrawDat(f, indent) )
    return 0;
}

Vec3f               
MFNode::getBboxSize(void)
{
    Vec3f minResult(FLT_MAX, FLT_MAX, FLT_MAX);
    Vec3f maxResult(FLT_MIN, FLT_MIN, FLT_MIN);
    NodeList *childList = getValues();
    for (long i = 0; i < childList->size(); i++)
        if (childList->get(i) != NULL) {
            Vec3f min = childList->get(i)->getMinBoundingBox();
            Vec3f max = childList->get(i)->getMaxBoundingBox();
            if ((min.x == FLT_MAX) && (min.y == FLT_MAX) && (min.y == FLT_MAX) 
                && 
                (max.x == FLT_MIN) && (max.y == FLT_MIN) && (max.y == FLT_MIN))
                 continue;
        if (min.x < minResult.x)
            minResult.x = min.x;
        if (max.x > maxResult.x)
            maxResult.x = max.x;
        if (min.y < minResult.y)
            minResult.y = min.y;
        if (max.y > maxResult.y)
            maxResult.y = max.y;
        if (min.z < minResult.z)
            minResult.z = min.z;
        if (max.z > maxResult.z)
            maxResult.z = max.z;
        }
    if ((minResult.x == FLT_MAX) && 
        (minResult.y == FLT_MAX) && 
        (minResult.y == FLT_MAX) && 
        (maxResult.x == FLT_MIN) && 
        (maxResult.y == FLT_MIN) && 
        (maxResult.y == FLT_MIN))
        return Vec3f(-1, -1, -1);
    return Vec3f(maxResult.x - minResult.x, 
                 maxResult.y - minResult.y, 
                 maxResult.z - minResult.z);
}

Vec3f               
MFNode::getBboxCenter(void)
{
    Vec3f minResult(FLT_MAX, FLT_MAX, FLT_MAX);
    Vec3f maxResult(FLT_MIN, FLT_MIN, FLT_MIN);
    NodeList *childList = getValues();
    for (long i = 0; i < childList->size(); i++)
        if (childList->get(i) != NULL) {
            Vec3f min = childList->get(i)->getMinBoundingBox();
            Vec3f max = childList->get(i)->getMaxBoundingBox();
            if ((min.x == FLT_MAX) && (min.y == FLT_MAX) && (min.y == FLT_MAX) 
                && 
                (max.x == FLT_MIN) && (max.y == FLT_MIN) && (max.y == FLT_MIN))
                 continue;
        if (min.x < minResult.x)
            minResult.x = min.x;
        if (max.x > maxResult.x)
            maxResult.x = max.x;
        if (min.y < minResult.y)
            minResult.y = min.y;
        if (max.y > maxResult.y)
            maxResult.y = max.y;
        if (min.z < minResult.z)
            minResult.z = min.z;
        if (max.z > maxResult.z)
            maxResult.z = max.z;
        }
    if ((minResult.x == FLT_MAX) && 
        (minResult.y == FLT_MAX) && 
        (minResult.y == FLT_MAX) && 
        (maxResult.x == FLT_MIN) && 
        (maxResult.y == FLT_MIN) && 
        (maxResult.y == FLT_MIN))
        return Vec3f(-1, -1, -1);
    return Vec3f((maxResult.x + minResult.x) / 2.0f, 
                 (maxResult.y + minResult.y) / 2.0f, 
                 (maxResult.z + minResult.z) / 2.0f);
}


#include "Scene.h"

FieldValue *
MFNode::getRandom(Scene *scene, int nodeType)
{
    NodeList *values = new NodeList();
    int type = nodeType;
    for (int i = 0; i < INT_RAND(); i++) {
        if (nodeType == -1)
            type = (int)(RAND() * LAST_NODE);
        else if (type >= ANY_NODE)
            for (int i = 0; i < LAST_NODE; i++) {
                for (int j = LAST_NODE - i; j >= 0; j--) {
                   type = i + (int)(RAND() * j);
                   if (matchNodeClass(type, nodeType))
                       break;
                }
                if (matchNodeClass(type, nodeType))
                   break;
            }
        values->append(scene->createNode(type));
    }
    return new MFNode(values);
}

