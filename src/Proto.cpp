/*
 * Proto.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2005 J. "MUFTI" Scheurich
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
#include <ctype.h>

#ifndef _WIN32
# include <unistd.h>
# include <fcntl.h>
#endif

#include "resource.h"
#include "Proto.h"
#include "Scene.h"
#include "EventIn.h"
#include "EventOut.h"
#include "Field.h"
#include "FieldValue.h"
#include "ExposedField.h"
#include "DynamicFieldsNode.h"
#include "NodeTransform.h"

// code for a resuable constructor
void Proto::protoInitializer(Scene *scene, const MyString &name)
{
    metadata.set(
          addExposedField(SFNODE, "metadata", new SFNode(NULL), METADATA_NODE));
    setFieldFlags(metadata, FF_X3D_ONLY | FF_HIDDEN);

    m_scene = scene;
    m_name = name;
    m_protoNodes.resize(0);
    m_eventIns.resize(0);
    m_eventOuts.resize(0);
    m_fields.resize(0);
    m_exposedFields.resize(0);
    m_urls = NULL;
    m_isInternUrl = false;
    m_loaded = false;
    m_loading = false;
    m_defined = false;
    m_dynamicProto = false;
    m_fromProtoLibrary = false;
    m_inUse = false;
    m_writeCDeclarationWritten = false;   
    m_isInScene = false;
    m_showFieldsInit = false;
    m_numbers4KidsInit = false;
    m_unitLength = 1;
    m_unitAngle = 1;
    m_hasTimeSensor = false;
    buildExportNames();
}

Proto::Proto(Scene *scene, const MyString &name)
{
    protoInitializer(scene, name);
}   


static bool searchTimeSensor(Node *node, void *data)
{
    bool *found = (bool *)data;
    if (node->getType() == VRML_TIME_SENSOR)
        *found = true;
    return true;
}

Proto::Proto(Scene *scene, Proto *proto, int extensionFlag)
{
    bool x3d = false;
    int flag = 0;
    const char *extensionName = "";
    if (extensionFlag & FF_X3D_ONLY) { 
        extensionName = "X3d";
        flag = FF_X3D_ONLY;
        x3d = true;
    } else if (extensionFlag & FF_COVER_ONLY) {
        extensionName = "Cover";
        flag = FF_COVER_ONLY;
    } else if (extensionFlag & FF_KAMBI_ONLY) {
        extensionName = "Kambi";
        flag = FF_KAMBI_ONLY;
    }
    int len = 128;
    len += strlen(extensionName) + strlen(proto->getName(x3d));
    char *protoName = new char[len + 1]; 
    int count = 1;
    // search new PROTO name
    do {
        mysnprintf(protoName, len, "%s%s%d", extensionName, 
                   (const char *)proto->getName(x3d), count++);
    } while (scene->getProto(protoName) != NULL);
    protoInitializer(scene, protoName);
    m_loaded = proto->isLoaded();
    m_protoNodes[0] = proto->create(scene);
    m_protoNodes[0]->setOutsideProto(this);
    for (int i = 0; i < m_protoNodes[0]->getProto()->getNumEventIns(); i++) {
        EventIn *eventIn = m_protoNodes[0]->getProto()->getEventIn(i);
        if (eventIn->getExposedField() == NULL) {
            const MyString name = eventIn->getName(x3d);
            if (avoidElement(eventIn, flag))
                continue;
            addEventIn(eventIn->getType(), name);
            if (!(eventIn->getFlags() & flag)) {
                int dstEventIn = proto->lookupEventIn(name, x3d);
                EventIn *isEvent = m_protoNodes[0]->getProto()->getEventIn(i);
                isEvent->addIs(m_protoNodes[0], i, EL_EVENT_IN,
                               this, dstEventIn, EOF_IS);
            }
        } 
    }          
    for (int i = 0; i < m_protoNodes[0]->getProto()->getNumEventOuts(); i++) {
         EventOut *eventOut = m_protoNodes[0]->getProto()->getEventOut(i);
         if (eventOut->getExposedField() == NULL) {
             const MyString name = eventOut->getName(x3d);
             if (avoidElement(eventOut, flag))
                 continue;
             addEventOut(eventOut->getType(), name);
             if (!(eventOut->getFlags() & flag)) {
                 int dstEventOut = proto->lookupEventOut(name, x3d);
                 EventOut *isEvent = m_protoNodes[0]->getProto()->getEventOut(i);
                 isEvent->addIs(m_protoNodes[0], i, EL_EVENT_OUT,
                                this, dstEventOut, EIF_IS);
             }
         } 
    }          
    for (int i = 0; i < m_protoNodes[0]->getProto()->getNumFields(); i++) {
         Field *field = m_protoNodes[0]->getProto()->getField(i);
         const MyString name = field->getName(x3d);
         if (avoidElement(field, flag))
             continue;
         if (field->getExposedField())
             addExposedField(field->getType(), name, field->getDefault(x3d));
         else
             addField(field->getType(), name, field->getDefault(x3d));
         if (!(field->getFlags() & flag)) {
              int dstField = proto->lookupField(name, x3d);
              Field *isField = m_protoNodes[0]->getProto()->getField(i);
              isField->addIs(m_protoNodes[0], i, field->getType(),
                             this, dstField);
         } 
    }          
    for (int i = 0; i < m_protoNodes.size(); i++) {
        bool hasTimeSensor = false;
        m_protoNodes[i]->doWithBranch(searchTimeSensor, &hasTimeSensor, false);
        if (hasTimeSensor)
            m_hasTimeSensor;
    }
}

Proto::~Proto()
{
/*
    for (long i = 0; i < m_eventIns.size(); i++) 
        delete m_eventIns[i];
    for (long i = 0; i < m_eventOuts.size(); i++) 
        delete m_eventOuts[i];
    for (long i = 0; i < m_exposedFields.size(); i++) 
        delete m_exposedFields[i];
    for (long i = 0; i < m_fields.size(); i++) 
        delete m_fields[i];
*/
    for (long i = 0; i < m_protoNodes.size(); i++) 
        if (m_protoNodes[i])
            m_protoNodes[i]->unref();
}

bool Proto::avoidElement(Element *element, bool x3d)
{
    return avoidElement(element, x3d ? FF_VRML_ONLY : FF_X3D_ONLY);
}

bool Proto::avoidElement(Element *element, int flag)
{
    if (element->getFlags() & FF_ALWAYS)
        return true;
    if (element->getFlags() & FF_NEVER)
        return false;
    if (flag == FF_X3D_ONLY)
        if (element->getFlags() & FF_COVER_ONLY)
            return true;
    if (flag == FF_X3D_ONLY)
        if (element->getFlags() & FF_KAMBI_ONLY)
            return true;
    if (flag == FF_X3D_ONLY)
        if (element->getFlags() & FF_VRML_ONLY)
            return true;
    if (flag == FF_COVER_ONLY)
        if (element->getFlags() & FF_X3D_ONLY)
            return true;
    if (flag == FF_COVER_ONLY)
        if (element->getFlags() & FF_KAMBI_ONLY)
            return true;
    if (flag == FF_KAMBI_ONLY)
        if (element->getFlags() & FF_X3D_ONLY)
            return true;
    if (flag == FF_X3DOM_ONLY)
        return true;
    return false;  
}

Node *
Proto::create(Scene *scene)
{
    return new NodePROTO(scene, this);
}

MyString
Proto::buildCallbackClass(const char *name)
{
    MyString callbackClass = "";
    callbackClass += TheApp->getCPrefix();
    callbackClass += getCName(true);
    callbackClass += name;
    callbackClass += "Callback";
    return callbackClass;
}

MyString
Proto::buildCallbackName(const char *name)
{
    MyString callbackName = "";
    callbackName += name;
    callbackName += "Callback";
    callbackName += getCName(true);
    return callbackName;
}

// a dynamic node needs a extra name (DEF name) otherwise it would be ambiguous
void
Proto::buildExportNames(const char *nodeName)
{
    m_cName = "";
    if (isalpha(m_name[0]))
        m_cName += m_name[0];
    else
        m_cName += "A";  
    for (int i = 1; i < m_name.length(); i++)
        if (isalnum(m_name[i]))
            m_cName += m_name[i];
        else
            m_cName += "_";
    if (nodeName != NULL) {
        m_cName += "_";
        m_cName += nodeName;
     }

    m_className = "";
    m_className += TheApp->getCPrefix();
    m_className += getCName(true);

    m_renderCallbackClass = buildCallbackClass("Render");
    m_renderCallbackName = buildCallbackName("render");

    m_treeRenderCallbackClass = buildCallbackClass("TreeRender");
    m_treeRenderCallbackName = buildCallbackName("treeRender");

    m_createNormalsCallbackClass = buildCallbackClass("CreateNormals");
    m_createNormalsCallbackName = buildCallbackName("createNormals");

    m_doWithDataCallbackClass = buildCallbackClass("DoWithData");
    m_doWithDataCallbackName = buildCallbackName("doWithData");

    m_treeDoWithDataCallbackClass = buildCallbackClass("TreeDoWithData");
    m_treeDoWithDataCallbackName = buildCallbackName("treeDoWithData");

    m_processEventCallbackClass = buildCallbackClass("ProcessEvent");
    m_processEventCallbackName = buildCallbackName("processEvent");
}

void
Proto::removeFromIs(Node *node)
{
    for (long i = 0; i < m_fields.size(); i++)
        for (int j = 0; j < m_fields[i]->getNumIs(); j++)
            if (m_fields[i]->getIsNode(j)->hasAncestor(node))
                m_fields[i]->removeIs(j);
    for (long i = 0; i < m_eventIns.size(); i++)
        for (int j = 0; j < m_eventIns[i]->getNumIs(); j++)
            if (m_eventIns[i]->getIsNode(j)->hasAncestor(node))
                m_eventIns[i]->removeIs(j);;
    for (long i = 0; i < m_eventOuts.size(); i++)
        for (int j = 0; j < m_eventOuts[i]->getNumIs(); j++)
            if (m_eventOuts[i]->getIsNode(j)->hasAncestor(node))
                m_eventOuts[i]->removeIs(j);
    for (long i = 0; i < m_exposedFields.size(); i++)
        for (int j = 0; j < m_exposedFields[i]->getNumIs(); j++)
            if (m_exposedFields[i]->getIsNode(j)->hasAncestor(node))
                m_exposedFields[i]->removeIs(j);
}

void
Proto::deleteElements(void) 
{
    for (long i = 0; i < m_fields.size(); i++)
        if (m_fields[i] && m_fields[i]->getDefault(false)->isDefaultValue())
            m_nameOfFieldsWithDefaultValue.append(m_fields[i]->getName(false));
    int restSize = 1;
    m_fields.resize(restSize);
    m_eventIns.resize(restSize);
    m_eventOuts.resize(restSize);
    m_exposedFields.resize(restSize);
}

int
Proto::addOrUpdateElement(Element *element)
{
    switch( element->getElementType() ) {
      case EL_FIELD:
        for (long i = 0; i < m_fields.size(); i++)
            if (m_fields[i]->getName(false) == element->getName(false)) {
                m_fields[i] = (Field *)element;
                return i;
            }
        break;
      case EL_EVENT_IN:
        for (long i = 0; i < m_eventIns.size(); i++)
            if (m_eventIns[i]->getName(false) == element->getName(false)) {
                m_eventIns[i] = (EventIn *)element;
                return i;
            }
        break;
      case EL_EVENT_OUT:
        for (long i = 0; i < m_eventOuts.size(); i++)
            if (m_eventOuts[i]->getName(false) == element->getName(false)) {
                m_eventOuts[i] = (EventOut *)element;
                return i;
            }
        break;
      case EL_EXPOSED_FIELD:
        for (long i = 0; i < m_exposedFields.size(); i++)
            if (m_exposedFields[i]->getName(false) == element->getName(false)) {
                m_exposedFields[i] = (ExposedField *)element;
                return i;
            }
        break;
      default:
        m_scene->errorf("internal error:  unexpected element in Proto");
        break;
    }
    return addElement(element);
}

int
Proto::addElement(Element *element)
{
    switch( element->getElementType() ) {
      case EL_FIELD:
        {
        Field *field = (Field *) element;
        if (field->getDefault(false) == NULL) {
            swDebugf("ignoring element %s\n", (const char*)field->getName(false));
            return -1; 
        }
        for (long i = 0; i < m_nameOfFieldsWithDefaultValue.size(); i++)
            if (field->getName(false) == m_nameOfFieldsWithDefaultValue[i])
                field->getDefault(false)->setIsDefaultValue();
        m_fields.append(field);
        }
        return m_fields.size() - 1;
      case EL_EVENT_IN:
        m_eventIns.append((EventIn *) element);
        break;
      case EL_EVENT_OUT:
        m_eventOuts.append((EventOut *) element);
        break;
      case EL_EXPOSED_FIELD:
        return addExposedField((ExposedField *) element);
      default:
        m_scene->errorf("internal error:  unexpected element in Proto");
        break;
    }
    return -1;
}

int
Proto::addField(int fieldType, const MyString &name, FieldValue *defaultValue,
                FieldValue *min, FieldValue *max)
{
    FieldValue *value = defaultValue;
    if (value == NULL)
        value = typeDefaultValue(fieldType);
    m_fields.append(new Field(fieldType, name, value, NULL, min, max, 
                              ANY_NODE));
    return m_fields.size() - 1;
}

int
Proto::addField(int fieldType, const MyString &name, FieldValue *defaultValue,
                int nodeType)
{
    m_fields.append(new Field(fieldType, name, defaultValue, NULL,
                             NULL, NULL, nodeType));
    return m_fields.size() - 1;
}

int
Proto::addField(int fieldType, const MyString &name, FieldValue *defaultValue,
                int flags, const char **strings)
{
    m_fields.append(new Field(fieldType, name, defaultValue, NULL,
                             NULL, NULL, ANY_NODE, flags, strings));
    return m_fields.size() - 1;
}


int
Proto::addField(Field *field)
{
    m_fields.append(field);
    return m_fields.size() - 1;
}

int
Proto::addEventIn(int fieldType, const MyString &name)
{
    m_eventIns.append(new EventIn(fieldType, name));
    return m_eventIns.size() - 1;
}

int
Proto::addEventIn(int fieldType, const MyString &name, int flags)
{
    m_eventIns.append(new EventIn(fieldType, name, flags));
    return m_eventIns.size() - 1;
}

int
Proto::addEventIn(int fieldType, const MyString &name, int flags, int field)
{
    m_eventIns.append(new EventIn(fieldType, name, flags));
    m_eventIns[m_eventIns.size() - 1]->setField(field); 
    m_fields[field]->setEventIn(m_eventIns.size() - 1);
    return m_eventIns.size() - 1;
}

int
Proto::addEventOut(int fieldType, const MyString &name)
{
    m_eventOuts.append(new EventOut(fieldType, name));
    return m_eventOuts.size() - 1;
}

int
Proto::addEventOut(int fieldType, const MyString &name, int flags)
{
    m_eventOuts.append(new EventOut(fieldType, name, flags));
    return m_eventOuts.size() - 1;
}

int
Proto::addExposedField(int fieldType, const MyString &name,
                       FieldValue *defaultValue, 
                       FieldValue *min, FieldValue *max, 
                       const MyString &x3dName)
{
    FieldValue *value = defaultValue;
    if (value == NULL)
        value = typeDefaultValue(fieldType);
    return addExposedField(new ExposedField(fieldType, name, value, 
                                            min, max, ANY_NODE, 0, 
                                            NULL, x3dName));
}

int
Proto::addExposedField(int fieldType, const MyString &name,
                       FieldValue *defaultValue, int nodeType, 
                       const MyString &x3dName)
{
    return addExposedField(new ExposedField(fieldType, name,
                                            defaultValue, NULL, NULL, 
                                            nodeType, 0, NULL, x3dName));
}

int
Proto::addExposedField(int fieldType, const MyString &name,
                       FieldValue *defaultValue, int flags, 
                       const char **strings)
{
    return addExposedField(new ExposedField(fieldType, name,
                                            defaultValue, NULL, NULL, 
                                            0, flags, strings));
}

int
Proto::addExposedField(int fieldType, const MyString &name, 
                           FieldValue *defaultValue, 
                           FieldValue *min, FieldValue *max,
                           int nodeType, int flags, const char **strings,
                           const MyString &x3dName)
{
    return addExposedField(new ExposedField(fieldType, name,
                                            defaultValue, min, max, 
                                            nodeType, flags, strings, x3dName));
}

int
Proto::addExposedField(ExposedField *exposedField)
{
    const MyString &name = exposedField->getName(false);
    const MyString &x3dName = exposedField->getName(true);
    int type = exposedField->getType();
    FieldValue *value = exposedField->getValue();
    FieldValue *min = exposedField->getMin();
    FieldValue *max = exposedField->getMax();
    int flags = exposedField->getFlags();
    int nodeType = exposedField->getNodeType();
    const char **strings = exposedField->getStrings();

    m_exposedFields.append(exposedField);

    // now add a hidden Field with the same name
    m_fields.append(new Field(type, name, value, exposedField, min, max, 
                              nodeType, FF_HIDDEN | flags, strings, x3dName));

    exposedField->setFieldIndex(m_fields.size() - 1);
    exposedField->setField(m_fields[m_fields.size() - 1]);

    // now add a hidden EventIn called set_<name>
    char buf[1024];
    mysnprintf(buf, 1024, "set_%s", (const char *) name);
    m_eventIns.append(new EventIn(type, buf, flags, exposedField));

    exposedField->setEventIn(m_eventIns.size() - 1);
    m_eventOuts.append(new EventOut(type, name, flags, exposedField));

    exposedField->setEventOut(m_eventOuts.size() - 1);

    return m_fields.size() - 1;
}

void
Proto::deleteField(int index)
{
    m_fields.remove(index);
}

void
Proto::deleteEventIn(int index)
{
    m_eventIns.remove(index);
}

void
Proto::deleteEventOut(int index)
{
    m_eventOuts.remove(index);
}

void
Proto::deleteExposedField(int index)
{
    ExposedField *exposedField = m_exposedFields[index];
    for (int i = 0; i < getNumFields(); i++)
        if (m_fields[i]->getExposedField() == exposedField)
            m_fields.remove(i);
    for (int i = 0; i < getNumEventIns(); i++)
        if (m_eventIns[i]->getExposedField() == exposedField)
            m_eventIns.remove(i);
    for (int i = 0; i < getNumEventOuts(); i++)
        if (m_eventOuts[i]->getExposedField() == exposedField)
            m_eventOuts.remove(i);
    m_exposedFields.remove(index);
}

int
Proto::getExposedOfField(Field *field)
{
    const char *name = field->getName(m_scene->isX3d());
    int sField = -1;
    for (int i = 0; i < getNumExposedFields(); i++) {
        sField++;
        if (strcmp(name, getExposedField(i)->getName(m_scene->isX3d())) == 0)
            return sField;
    }
    return -1;
}

int
Proto::getFieldOfExposed(ExposedField *field)
{
    const char *name = field->getName(m_scene->isX3d());
    int sField = -1;
    for (int i = 0; i < getNumFields(); i++) {
        sField++;
        if (strcmp(name, getField(i)->getName(m_scene->isX3d())) == 0)
            return sField;
    }
    return -1;
}

int
Proto::lookupSimpleEventIn(const MyString &name, bool x3d) const
{
    for (long i = 0; i < m_eventIns.size(); i++)
        if (m_eventIns[i]->getName(x3d) == name) return i;
    if (m_protoNodes.size() > 0)
        m_protoNodes[0]->getProto()->lookupSimpleEventIn(name, x3d);
    return INVALID_INDEX;
}

int
Proto::lookupEventIn(const MyString &name, bool x3d) const
{
    int index;

    if ((index = lookupSimpleEventIn(name, x3d)) == INVALID_INDEX)
    {
        // simple search failed; look for an exposedField
        if ((index = lookupExposedField(name, x3d)) != INVALID_INDEX) {

            // now look up the corresponding eventIn
            char buf[1024];
            mysnprintf(buf, 1024, "set_%s", (const char *) name);
            index = lookupSimpleEventIn(buf, x3d);
        }
    }
    return index;
}

int
Proto::lookupSimpleEventOut(const MyString &name, bool x3d) const
{
    for (long i = 0; i < m_eventOuts.size(); i++)
        if (m_eventOuts[i]->getName(x3d) == name)
            return i;
    if (m_protoNodes.size() > 0)
        m_protoNodes[0]->getProto()->lookupSimpleEventOut(name, x3d);
    return INVALID_INDEX;
}

int
Proto::lookupEventOut(const MyString &name, bool x3d) const
{
    int index;

    if ((index = lookupSimpleEventOut(name, x3d)) == INVALID_INDEX)
    {
        // simple search failed; look for an exposedField
        if ((index = lookupExposedField(name, x3d)) != INVALID_INDEX) {

            // now look up the corresponding eventOut
            char buf[1024];
            mysnprintf(buf, 1024, "%s_changed", (const char *) name);
            index = lookupSimpleEventOut(buf, x3d);
        }
    }
    return index;
}

int
Proto::lookupField(const MyString &name, bool x3d) const
{
    for (long i = 0; i < m_fields.size(); i++)
        if (m_fields[i]->getName(x3d) == name)
            return i;
    if (m_protoNodes.size() > 0)
        m_protoNodes[0]->getProto()->lookupField(name, x3d);
    return INVALID_INDEX;
}

int
Proto::lookupExposedField(const MyString &name, bool x3d) const
{
    for (long i = 0; i < m_exposedFields.size(); i++)
        if (m_exposedFields[i]->getName(x3d) == name)
            return i;

    return INVALID_INDEX;
}

bool 
Proto::canWriteElement(Element *element, bool x3d) const
{
    if (element && element->getFlags() & FF_ALWAYS)
        return true;
    if (element && element->getFlags() & FF_NEVER)
        return false;
    if (element && x3d) {
        if (element->getFlags() & FF_COVER_ONLY)
            return false;
        if (element->getFlags() & FF_KAMBI_ONLY)
            return false;
        if (element->getFlags() & FF_VRML_ONLY)
            return false;
    } else
        if (element && element->getFlags() & FF_X3D_ONLY)
            return false;
    return true;
}

int 
Proto::writeEvents(int f, int indent, int flags) const
{
    int indent2 = indent;
    if (isX3dXml(flags))
        indent2 = indent + TheApp->GetIndent();
    for (long i = 0; i < m_fields.size(); i++)
        if (m_fields[i] && canWriteElement(m_fields[i], isX3d(flags)))
            RET_ONERROR( m_fields[i]->write(f, indent2, flags) )
    for (long i = 0; i < m_eventIns.size(); i++)
        if (m_eventIns[i] && canWriteElement(m_eventIns[i], isX3d(flags)))
            RET_ONERROR(m_eventIns[i]->write(f, indent2, flags) )
    for (long i = 0; i < m_eventOuts.size(); i++)
        if (m_eventOuts[i] && canWriteElement(m_eventOuts[i], isX3d(flags)))
            RET_ONERROR(m_eventOuts[i]->write(f, indent2, flags) )
    for (long i = 0; i < m_exposedFields.size(); i++)
        if (m_exposedFields[i] &&
            canWriteElement(m_exposedFields[i], isX3d(flags)))
                RET_ONERROR(m_exposedFields[i]->write(f, indent2, flags) )
    return(0);
}

static bool setIsProtoToBranch(Node *node, void *data)
{
    Proto *proto = (Proto *)data;
    node->ref();
    node->setOutsideProto(proto);
    return true;
}


void
Proto::define(Node *primaryNode, NodeList *nodes)
{
    if (primaryNode == NULL)
        return; 
    m_protoNodes[0] = primaryNode;
    for (long i = 0; i < (nodes->size() + 1); i++) {
        if (i > 0)
            m_protoNodes[i] = nodes->get(i - 1);
        if (m_protoNodes[i] != NULL)
            m_protoNodes[i]->doWithBranch(setIsProtoToBranch, this);
    }
    setIsNodeIndex();
}

int Proto::write(int f, int indent, int flags) const
{
    int indent2 = indent + TheApp->GetIndent();
    int indent3 = indent2 + TheApp->GetIndent();
    int indent4 = indent3 + TheApp->GetIndent();

    bool x3d = isX3d(flags);
    bool externProto = (m_urls != NULL);
    const char *appinfo = getAppinfo();
    const char *documentation = getDocumentation();
    if (isX3dXml(flags)) {
        RET_ONERROR( indentf(f, indent2) )
        RET_ONERROR( mywritestr(f, "<") )
        if (externProto)
            RET_ONERROR( mywritestr(f, "Extern") )
        RET_ONERROR( mywritestr(f, "ProtoDeclare name='") )
        RET_ONERROR( mywritestr(f, getName(x3d)) )
        RET_ONERROR( mywritestr(f, "' ") )
        if (strlen(appinfo) > 0) {
            RET_ONERROR( mywritestr(f, " appinfo='") )
            RET_ONERROR( mywritestr(f, appinfo) )
            RET_ONERROR( mywritestr(f, "'") )
        }
        if (strlen(documentation) > 0) {
            RET_ONERROR( mywritestr(f, " documentation='") )
            RET_ONERROR( mywritestr(f, documentation) )
            RET_ONERROR( mywritestr(f, "'") )
        }
        if (externProto) {
            RET_ONERROR( mywritestr(f, " url=") )
            const char *oldBase = m_scene->getURL();
            const char *newBase = m_scene->getNewURL();
            FieldValue *urls = m_urls;
            if (!TheApp->GetKeepURLs())
                urls = rewriteField(m_urls, oldBase, newBase, flags);
            RET_ONERROR( urls->writeXml(f, indent3) )
        }
        RET_ONERROR( mywritestr(f, ">\n") )
        TheApp->incSelectionLinenumber();

        RET_ONERROR( indentf(f, indent3) )
        RET_ONERROR( mywritestr(f, "<ProtoInterface>\n") )
        TheApp->incSelectionLinenumber();

        RET_ONERROR( writeEvents(f, indent2, 
                                 flags | (externProto ? WITHOUT_VALUE: 0)) )

        RET_ONERROR( indentf(f, indent3) )
        RET_ONERROR( mywritestr(f, "</ProtoInterface>\n") )
        TheApp->incSelectionLinenumber();

        if (!externProto) {

            RET_ONERROR( indentf(f, indent3) )
            RET_ONERROR( mywritestr(f, "<ProtoBody>\n") )
            TheApp->incSelectionLinenumber();    

            for (long i = 0; i < m_protoNodes.size(); i++) {
                if (::isX3dXml(flags))
                    RET_ONERROR( m_protoNodes.get(i)->writeXml(f, indent4) )
                else
                    RET_ONERROR( m_protoNodes.get(i)->write(f, indent4) )
            }
            RET_ONERROR( mywritestr(f, "\n") )
            for (long i = 0; i < m_protoNodes.size(); i++) 
                RET_ONERROR( m_protoNodes.get(i)->writeRoutes(f, indent4) )
            m_scene->writeRouteStrings(f, indent4, true);

            RET_ONERROR( indentf(f, indent3) )
            RET_ONERROR( mywritestr(f, "</ProtoBody>\n") )
            TheApp->incSelectionLinenumber();    
        }
        RET_ONERROR( indentf(f, indent2) )
        RET_ONERROR( mywritestr(f, "</") )
        if (externProto)
            RET_ONERROR( mywritestr(f, "Extern") )
        RET_ONERROR( mywritestr(f, "ProtoDeclare>\n") )
        RET_ONERROR( mywritestr(f, "\n") )
        TheApp->incSelectionLinenumber();
    } else {
        if (strlen(appinfo) > 0) {
            RET_ONERROR( mywritestr(f, "# ") )
            RET_ONERROR( mywritestr(f, getName(x3d)) )
            RET_ONERROR( mywritestr(f, " appinfo='") )
            RET_ONERROR( mywritestr(f, appinfo) )
            RET_ONERROR( mywritestr(f, "'") )
            RET_ONERROR( mywritestr(f, "\n") )
            TheApp->incSelectionLinenumber();
        }
        if (strlen(documentation) > 0) {
            RET_ONERROR( mywritestr(f, "# ") )
            RET_ONERROR( mywritestr(f, getName(x3d)) )
            RET_ONERROR( mywritestr(f, " documentation='") )
            RET_ONERROR( mywritestr(f, documentation) )
            RET_ONERROR( mywritestr(f, "'") )
            RET_ONERROR( mywritestr(f, "\n") )
            TheApp->incSelectionLinenumber();
        }
        RET_ONERROR( indentf(f, indent) )
        if (m_urls != NULL)
            RET_ONERROR( mywritestr(f, "EXTERN") )
        RET_ONERROR( mywritestr(f, "PROTO ") )
        RET_ONERROR( mywritestr(f, getName(x3d)) )
        if (!TheApp->GetkrFormating()) {
            RET_ONERROR( mywritestr(f, "\n") )
            TheApp->incSelectionLinenumber();
            RET_ONERROR( indentf(f, indent2) )
        } else
            RET_ONERROR( mywritestr(f, " ") )
        RET_ONERROR( mywritestr(f, "[\n") )
        TheApp->incSelectionLinenumber();
    
        RET_ONERROR( writeEvents(f, indent2, 
                                 flags | (externProto ? WITHOUT_VALUE : 0)) )
    
        if (!TheApp->GetkrFormating()) {
            TheApp->incSelectionLinenumber();
            RET_ONERROR( indentf(f, indent2) )
        } else
            RET_ONERROR( indentf(f, indent) )
    
        RET_ONERROR( mywritestr(f, "]") )

        RET_ONERROR( indentf(f, indent) )
        if (m_urls != NULL) {
            const char *oldBase = m_scene->getURL();
            const char *newBase = m_scene->getNewURL();
            FieldValue *urls = m_urls;
            if (!TheApp->GetKeepURLs())
                urls = rewriteField(m_urls, oldBase, newBase, flags);
            if (((MFString *)urls)->getSize() == 1) {
                RET_ONERROR( mywritestr(f, "\n") )
                TheApp->incSelectionLinenumber();
            }
            RET_ONERROR( urls->write(f, 0) )
        } else {
            RET_ONERROR( mywritestr(f, "\n") )
            TheApp->incSelectionLinenumber();
            RET_ONERROR( mywritestr(f, "{\n") )
            TheApp->incSelectionLinenumber();
            if (TheApp->GetkrFormating())
                indent = indent + TheApp->GetIndent();

            for (long i = 0; i < m_protoNodes.size(); i++) {
                RET_ONERROR( indentf(f, indent) )
                RET_ONERROR( m_protoNodes.get(i)->write(f, indent) )
            }
            for (long i = 0; i < m_protoNodes.size(); i++) 
                RET_ONERROR( m_protoNodes.get(i)->writeRoutes(f, indent) )
            m_scene->writeRouteStrings(f, indent, true);

            if (TheApp->GetkrFormating())
                indent = indent - TheApp->GetIndent();
            RET_ONERROR( indentf(f, indent) )
            RET_ONERROR( mywritestr(f, "}\n") )
            TheApp->incSelectionLinenumber();
        }
        RET_ONERROR( mywritestr(f, "\n") )
        TheApp->incSelectionLinenumber();

    }
    return(0);
}

void 
Proto::compareToIsNodes(Node *node)
{
    for (long i = 0; i < m_fields.size(); i++)
        if (m_fields[i])
            for (int j = 0; j < m_fields[i]->getNumIs(); j++)
                if (m_fields[i]->getIsNode(j) == node)
                    m_fields[i]->setIsNodeIndex(j, m_nodeIndex);
    for (long i = 0; i < m_eventIns.size(); i++)
        if (m_eventIns[i])
            for (int j = 0; j < m_eventIns[i]->getNumIs(); j++)
                if (m_eventIns[i]->getIsNode(j) == node)
                    m_eventIns[i]->setIsNodeIndex(j,m_nodeIndex);
    for (long i = 0; i < m_eventOuts.size(); i++)
        if (m_eventOuts[i])
            for (int j = 0; j < m_eventOuts[i]->getNumIs(); j++)
                if (m_eventOuts[i]->getIsNode(j) == node)
                    m_eventOuts[i]->setIsNodeIndex(j, m_nodeIndex);
    for (long i = 0; i < m_exposedFields.size(); i++)
        if (m_exposedFields[i])
            for (int j = 0; j < m_exposedFields[i]->getNumIs(); j++)
                if (m_exposedFields[i]->getIsNode(j) == node)
                    m_exposedFields[i]->setIsNodeIndex(j, m_nodeIndex);
    m_nodeIndex++;
}

static bool setIsNodeIndexToBranch(Node *node, void *data)
{
    ((Proto *)data)->compareToIsNodes(node);
    return true;
}

void 
Proto::setIsNodeIndex(void)
{
    m_nodeIndex = 0;
    for (int i = 0; i < getNumNodes(); i++)
        if (m_protoNodes[i] != NULL)
            m_protoNodes[i]->doWithBranch(setIsNodeIndexToBranch, this);
}


int
Proto::lookupIsEventIn(const char *name, int elementType) const
{
    for (long i = 0; i < m_eventIns.size(); i++)
        for (int j = 0; j < m_eventIns[i]->getNumIs(); j++)
            if (strcmp(m_eventIns[i]->getName(true), name) == 0) {
                if ((elementType != -1) && 
                    (m_eventIns[i]->getIsElementType(j) != elementType))
                    continue;
                return i;
            }
    return -1;
}


int
Proto::lookupIsExposedField(const char *name, int elementType) const
{
    for (long i = 0; i < m_exposedFields.size(); i++)
        for (int j = 0; j < m_exposedFields[i]->getNumIs(); j++)
            if (strcmp(m_exposedFields[i]->getName(true), name) == 0) {
                if ((elementType != -1) && 
                    (m_exposedFields[i]->getIsElementType(j) != elementType))
                    continue;
                return i;
            }
    return -1;
}

int
Proto::lookupIsField(Node *node, int field) const
{
    for (long i = 0; i < m_fields.size(); i++) {
        if (m_fields[i] == NULL)
            continue;
        for (int j = 0; j < m_fields[i]->getNumIs(); j++) {
            Node *isNode = m_fields[i]->getIsNode(j);
            if (isNode && isNode->isEqual(node))
                if (isNode->translateField(m_fields[i]->getIsField(j)) == 
                    field)
                    return i;
        }
    }
    return -1;
}

int
Proto::lookupIsEventIn(Node *node, int eventIn, int elementType) const
{
    for (long i = 0; i < m_eventIns.size(); i++) {
        if (m_eventIns[i] == NULL)
            continue;
        for (int j = 0; j < m_eventIns[i]->getNumIs(); j++)
            if (m_eventIns[i]->getIsNode(j) &&
                m_eventIns[i]->getIsNode(j)->isEqual(node))
                if (m_eventIns[i]->getIsField(j) == eventIn) {
                    if ((elementType != -1) && 
                        (m_eventIns[i]->getIsElementType(j) != elementType))
                        continue;
                    return i;
                }
    }
    return -1;
}

int
Proto::lookupIsEventOut(Node *node, int eventOut, int elementType) const
{
    for (long i = 0; i < m_eventOuts.size(); i++) {
        if (m_eventOuts[i] == NULL)
            continue;
        for (int j = 0; j < m_eventOuts[i]->getNumIs(); j++)
            if (m_eventOuts[i]->getIsNode(j) && 
                m_eventOuts[i]->getIsNode(j)->isEqual(node)) {
                if (m_eventOuts[i]->getIsField(j) == eventOut) {
                    if ((elementType != -1) && 
                        (m_eventOuts[i]->getIsElementType(j) != elementType))
                        continue;
                    return i;
                }
            }
    }
    return -1;
}


int
Proto::lookupIsExposedField(Node *node, int exposedField) const
{
    for (long i = 0; i < m_exposedFields.size(); i++) {
        if (m_exposedFields[i] == NULL)
            continue;
        for (int j = 0; j < m_exposedFields[i]->getNumIs(); j++) {
            Node *isNode = m_exposedFields[i]->getIsNode(j);
            if (isNode->isEqual(node))
                if (isNode->translateField(m_exposedFields[i]->getIsField(j)) ==
                    exposedField)
                    return i;
        }
    }
    return -1;
}

int
Proto::getNumIsMSNodes(void) const
{
    int ret = 0;
    for (long i = 0; i < m_fields.size(); i++) {
        if (m_fields[i] == NULL)
            continue;
        for (int j = 0; j < m_fields[i]->getNumIs(); j++)
            if ((m_fields[i]->getType() == MFNODE) ||
                (m_fields[i]->getType() == SFNODE))
                ret++;
    }
    for (long i = 0; i < m_exposedFields.size(); i++) {
        if (m_exposedFields[i] == NULL)
            continue;
        for (int j = 0; j < m_exposedFields[i]->getNumIs(); j++)
            if ((m_exposedFields[i]->getType() == MFNODE) ||
                (m_exposedFields[i]->getType() == SFNODE)) 
                ret++;
    }
    return ret;
}

Node *
Proto::getIsMSNode(int numNode) const
{
    int count = 0;
    for (long i = 0; i < m_fields.size(); i++) {
        if (m_fields[i] == NULL)
            continue;
        for (int j = 0; j < m_fields[i]->getNumIs(); j++) {
            if ((m_fields[i]->getType() == MFNODE) ||
                (m_fields[i]->getType() == SFNODE)) {
                if (numNode == count)
                    return m_fields[i]->getIsNode(j);                    
                count++;
            }
        }
    }
    for (long i = 0; i < m_exposedFields.size(); i++) {
        if (m_exposedFields[i] == NULL)
            continue;
        for (int j = 0; j < m_exposedFields[i]->getNumIs(); j++) {
            if ((m_exposedFields[i]->getType() == MFNODE) ||
                (m_exposedFields[i]->getType() == SFNODE)) {
                if (numNode == count)
                    return m_exposedFields[i]->getIsNode(j);                    
                count++;
            }
        }
    }
    return NULL;
}

int
Proto::getIsMSNodeField(int numNode) const
{
    int count = 0;
    for (long i = 0; i < m_fields.size(); i++) {
        if (m_fields[i] == NULL)
            continue;
        for (int j = 0; j < m_fields[i]->getNumIs(); j++) {
            if ((m_fields[i]->getType() == MFNODE) ||
                (m_fields[i]->getType() == SFNODE)) {
                if (numNode == count)
                    return m_fields[i]->getIsField(j);                    
                count++;
            }
        }
    }
    for (long i = 0; i < m_exposedFields.size(); i++) {
        if (m_exposedFields[i] == NULL)
            continue;
        for (int j = 0; j < m_exposedFields[i]->getNumIs(); j++) {
            if ((m_exposedFields[i]->getType() == MFNODE) ||
                (m_exposedFields[i]->getType() == SFNODE)) {
                if (numNode == count)
                    return m_exposedFields[i]->getIsField(j);                    
                count++;
            }
        }
    }
    return -1;
}

void
Proto::addURLs(FieldValue *urls)
{
    m_isInternUrl = false;
    m_urls = urls;
    m_defined = true;
}

void
Proto::addURLs(int urlSchema)
{
    m_isInternUrl = true;
    if (urlSchema == URL_NULL) {
        m_urls = NULL;       
        return; 
    }
    StringArray *urls = new StringArray();
    MyString url1 = "";
    url1 += m_name;
    url1 += "PROTO.wrl";
    urls->append(url1);

    m_defined = true;

    MyString url2 = "";
    switch(urlSchema) {
      case URL_VRML97_AMENDMENT1:
         url2 += "urn:web3d:vrml97:node:";
         url2 += m_name;
         break;
      case URL_X3D:
         url2 += "urn:web3d:x3d:node:";
         url2 += m_name;
         break;
      case URL_KAMBI:
        url2 += "urn:castle-engine.sourceforge.net:node:";
        url2 += m_name;
    }
    if (strlen(url2) > 0)
        urls->append(url2);
    
    MyString url3 = "";
    switch(urlSchema) {
      case URL_VRML97_AMENDMENT1:
#ifdef HAVE_VRML97_AMENDMENT1_PROTO_URL
         url3 += HAVE_VRML97_AMENDMENT1_PROTO_URL;
         url3 += "/";
         url3 += m_name;
         url3 += "PROTO.wrl";
#endif
         break;
      case URL_X3D:
#ifdef HAVE_X3D_PROTO_URL 
         url3 += HAVE_X3D_PROTO_URL;
         url3 += "/";
         url3 += m_name;
         url3 += "PROTO.wrl";
#endif
         break;
      case URL_SCRIPTED_NODES:
#ifdef HAVE_SCRIPTED_NODES_PROTO_URL
         url3 += HAVE_SCRIPTED_NODES_PROTO_URL;
         url3 += "/";
         url3 += m_name;
         url3 += "PROTO.wrl";
#endif
         break;
      case URL_COVER:
#ifdef HAVE_COVER_NODES_PROTO_URL 
         url3 += HAVE_COVER_NODES_PROTO_URL;
         url3 += "/";
         url3 += m_name;
         url3 += "PROTO.wrl";
#endif
         break;
      case URL_EXPORT_CONTAINER:
#ifdef HAVE_EXPORT_CONTAINER_PROTO_URL
         url3 += HAVE_EXPORT_CONTAINER_PROTO_URL;
         url3 += "/";
         url3 += m_name;
         url3 += "PROTO.wrl";
#endif
         break;
    }
    if (urlSchema == URL_KAMBI)
        url3 += "http://vrmlengine.sourceforge.net/fallback_prototypes.wrl";
    if (strlen(url3) > 0)
        urls->append(url3);

    MyString url4 = "";
    url4 += "http://wdune.ourproject.org/docs/";
    switch(urlSchema) {
      case URL_VRML97_AMENDMENT1:
         url4 += "vrml97Amendment1";
         url4 += "/";
         url4 += m_name;
         url4 += "PROTO.wrl";
         break;
      case URL_X3D:
         url4 += "x3d";
         url4 += "/";
         url4 += m_name;
         url4 += "PROTO.wrl";
         break;
         break;
      case URL_SCRIPTED_NODES:
         url4 += "scripted_Nodes";
         url4 += "/";
         url4 += m_name;
         url4 += "PROTO.wrl";
         break;
      case URL_COVER:
         url4 += "coverNodes";
         url4 += "/";
         url4 += m_name;
         url4 += "PROTO.wrl";
         break;
      case URL_EXPORT_CONTAINER:
         url4 += "exportContainers";
         url4 += "/";
         url4 += m_name;
         url4 += "PROTO.wrl";
         break;
    }
    if (strlen(url4) > 0)
        urls->append(url4);
    
    m_urls = new MFString(urls);

}

bool
Proto::checkIsExtension(Element *element, int flag)
{
    bool ret = false;
    bool x3d = (flag == FF_X3D_ONLY) ? true : false;
    const char* elementName = element->getElementName(false);
    if (element->getNumIs() == 1) {
        Proto *proto = element->getIsNode(0)->getProto();
        const char *isElementName = NULL;
        switch (element->getType()) {
          case EL_FIELD:
            isElementName = proto->getField(element->getIsField(0))->
                            getElementName(x3d);
            break;
          case EL_EVENT_IN:
            isElementName = proto->getEventIn(element->getIsField(0))->
                            getElementName(x3d);
            break;
          case EL_EVENT_OUT:
            isElementName = proto->getEventOut(element->getIsField(0))->
                            getElementName(x3d);
            break;
          case EL_EXPOSED_FIELD:
            isElementName = proto->getExposedField(element->getIsField(0))->
                            getElementName(x3d);
            break;
          default:
            return false;
        }
        if (strcmp(elementName, isElementName) == 0)
            ret = true;
        else
            return false;
    } else if (element->getNumIs() == 0) {
        Proto *proto = m_protoNodes[0]->getProto();
        int field = -1;
        switch (element->getType()) {
          case EL_FIELD:
            field = proto->lookupField(elementName, x3d);
            break;
          case EL_EVENT_IN:
            field = proto->lookupEventIn(elementName, x3d);
            break;
          case EL_EVENT_OUT:
            field = proto->lookupEventOut(elementName, x3d);
            break;
          case EL_EXPOSED_FIELD:
            field = proto->lookupExposedField(elementName, x3d);
            break;
          default:
            return false;
        }
        if (field == -1)
            return false;
        if (proto->getField(field)->getFlags() & flag)
            ret = true;
        else
           return false;  
    } else
        return false;
    return ret;
}

bool
Proto::isExtensionProto(int flag)
{
    bool ret = false;
    bool x3d = (flag == FF_X3D_ONLY) ? true : false;
    for (long i = 0; i < m_fields.size(); i++) {
        const char* name = m_fields[i]->getName(x3d);
        Element *element = m_fields[i];
        ExposedField *exposedField = m_fields[i]->getExposedField();
        if (exposedField) 
            element = exposedField;
        if ((element->getNumIs() == 1) && (m_protoNodes.size() > 0)) {
            Proto *proto = element->getIsNode(0)->getProto();
            Field *field = proto->getField(element->getIsField(0));
            if (strcmp(name, field->getName(x3d)) == 0)
                ret = true;
            else
                return false;
        } else if (element->getNumIs() == 0) {
            if ((flag == FF_COVER_ONLY) && 
                (m_fields[i]->getFlags() & FF_X3D_ONLY))
                continue;
            if ((flag == FF_KAMBI_ONLY) && 
                (m_fields[i]->getFlags() & FF_X3D_ONLY))
                continue;
            if (m_protoNodes.size() < 1)
                continue;
            if (m_protoNodes[0] == NULL)
                continue;
            Proto *proto = m_protoNodes[0]->getProto();
            int field = proto->lookupField(name, x3d);
            if (field == -1)
                return false;
            if (proto->getField(field)->getFlags() & flag)
                ret = true;
            else
               return false;  
        } else
            return false;
    }
    for (long i = 0; i < m_eventIns.size(); i++) {
        const char* name = m_eventIns[i]->getName(x3d);
        if (m_eventIns[i]->getExposedField()) {
            // already handled 
            continue;
        }
        if (m_eventIns[i]->getNumIs() == 1) {
            if ((flag == FF_COVER_ONLY) && 
                (m_eventIns[i]->getFlags() & FF_X3D_ONLY))
                continue;
            if ((flag == FF_KAMBI_ONLY) && 
                (m_eventIns[i]->getFlags() & FF_X3D_ONLY))
                continue;
            if (m_protoNodes.size() < 1)
                continue;
            Proto *proto = m_eventIns[i]->getIsNode(0)->getProto();
            EventIn *field = proto->getEventIn(m_eventIns[i]->getIsField(0));
            if (strcmp(name, field->getName(x3d)) == 0)
                ret = true;
            else
                return false;
        } else if (m_eventIns[i]->getNumIs() == 0) {
            if (m_protoNodes.size() < 1)
                continue;
            Proto *proto = m_protoNodes[0]->getProto();
            int field = proto->lookupEventIn(name, x3d);
            if (field == -1)
                return false;
            if (proto->getEventIn(field)->getFlags() & flag)
                ret = true;
            else
               return false;  
        } else
            return false;
    }
    for (long i = 0; i < m_eventOuts.size(); i++) {
        const char* name = m_eventOuts[i]->getName(x3d);
        if (m_eventOuts[i]->getExposedField()) {
            // already handled 
            continue;
        }
        if (m_eventOuts[i]->getNumIs() == 1) {
            Proto *proto = m_eventOuts[i]->getIsNode(0)->getProto();
            EventOut *field = proto->getEventOut(m_eventOuts[i]->getIsField(0));
            if (strcmp(name, field->getName(x3d)) == 0)
                ret = true;
            else
                return false;
        } else if (m_eventOuts[i]->getNumIs() == 0) {
            if ((flag == FF_COVER_ONLY) && 
                (m_eventOuts[i]->getFlags() & FF_X3D_ONLY))
                continue;
            if ((flag == FF_KAMBI_ONLY) && 
                (m_eventOuts[i]->getFlags() & FF_X3D_ONLY))
                continue;
            if (m_protoNodes.size() < 1)
                continue;
            Proto *proto = m_protoNodes[0]->getProto();
            int field = proto->lookupEventOut(name, x3d);
            if (field == -1)
                return false;
            if (proto->getEventOut(field)->getFlags() & flag)
                ret = true;
            else
               return false;  
        } else
            return false;
    }
    return ret;
}

bool                  
Proto::isMismatchingProto(void) {
   if (isKambiProto() && !(TheApp->getKambiMode()))
       return true;
   if (isCoverProto() && !(TheApp->getCoverMode()))
       return true;   
   return false;
}


void
Proto::setFieldFlags(int index, int flags)
{ 
    Field *field = m_fields[index];
    ExposedField *exposedField = field->getExposedField();
    if (exposedField) {
        exposedField->addFlags(flags);
        m_eventIns[exposedField->getEventIn()]->addFlags(flags);
        m_eventOuts[exposedField->getEventOut()]->addFlags(flags);
    }
    field->addFlags(flags); 
}

int
Proto::writeCDeclaration(int f, int languageFlag)
{
    if (m_name[0] == '#')
        return 0;
    if (m_writeCDeclarationWritten)
        return 0;
    m_writeCDeclarationWritten = true;
    if (languageFlag & C_SOURCE)
        RET_ONERROR( mywritestr(f, "struct ") )    
    else 
        RET_ONERROR( mywritestr(f, "class ") )
    RET_ONERROR( mywritestr(f, getClassName()) )
    if (languageFlag & CC_SOURCE) {
        RET_ONERROR( mywritestr(f, " : public ") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
    }
    if (languageFlag & JAVA_SOURCE) {
        RET_ONERROR( mywritestr(f, " extends ") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
    }
    RET_ONERROR( mywritestr(f, " {\n") )
    if (languageFlag & CC_SOURCE)
         RET_ONERROR( mywritestr(f, "public:\n") )
    // data
    if (languageFlag & C_SOURCE) {
        SFNode node;
        RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, node.getTypeC(languageFlag)) )
        RET_ONERROR( mywritestr(f, " m_parent;\n") )
        RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, node.getTypeC(languageFlag)) )
        RET_ONERROR( mywritestr(f, " m_protoRoot;\n") )
        RET_ONERROR( mywritestr(f, "    int") )
        RET_ONERROR( mywritestr(f, " m_type;\n") )
        RET_ONERROR( mywritestr(f, "    void") )
        RET_ONERROR( mywritestr(f, " *m_data;\n") )
    }
    for (long i = 0; i < m_eventIns.size(); i++)
        RET_ONERROR( writeCDeclarationEventIn(f, i, languageFlag) )
    for (long i = 0; i < m_eventOuts.size(); i++)
        RET_ONERROR( writeCDeclarationEventOut(f, i, languageFlag) )
    for (long i = 0; i < m_fields.size(); i++)
        RET_ONERROR( writeCDeclarationField(f, i, languageFlag) )
    writeCExtraFields(f, languageFlag);
    if (languageFlag & CC_SOURCE) {
        // callbacks
        RET_ONERROR( mywritestr(f, "\n") )
        RET_ONERROR( mywritestr(f, "    int getType() const ") )
        RET_ONERROR( mywritef(f, "{ return %d; }\n", getType()) )
        RET_ONERROR( mywritestr(f, "\n") )

        if ((getType() == VRML_INDEXED_FACE_SET) || 
            (getType() == VRML_INDEXED_LINE_SET) || 
            (getType() == X3D_LINE_SET) || 
            (getType() == VRML_POINT_SET)) {
            RET_ONERROR( mywritestr(f, "    void setGlName(int number)") )
            RET_ONERROR( mywritestr(f, "{ glName_number = number; }\n") )
            RET_ONERROR( mywritestr(f, "\n") )
        }

        RET_ONERROR( mywritestr(f, "    static ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "Callback ") )
        RET_ONERROR( mywritestr(f, "renderCallback;\n") )

        RET_ONERROR( mywritestr(f, "    static ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "Callback ") )
        RET_ONERROR( mywritestr(f, "treeRenderCallback;\n") )

        if (getType() == VRML_INDEXED_FACE_SET) {
            RET_ONERROR( mywritestr(f, "    static ") )
            RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
            RET_ONERROR( mywritestr(f, "Callback ") )
            RET_ONERROR( mywritestr(f, "createNormalsCallback;\n") )
        }

        RET_ONERROR( mywritestr(f, "    static ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "Callback ") )
        RET_ONERROR( mywritestr(f, "doWithDataCallback;\n") )

        RET_ONERROR( mywritestr(f, "    static ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "Callback ") )
        RET_ONERROR( mywritestr(f, "treeDoWithDataCallback;\n") )

        RET_ONERROR( mywritestr(f, "    static ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "ProcessEventCallback ") )
        RET_ONERROR( mywritestr(f, "processEventCallback;\n") )

        RET_ONERROR( mywritestr(f, "\n") )
    } else if (languageFlag & JAVA_SOURCE) {        
        // callbacks
        if ((getType() != VRML_COMMENT) && !(isMismatchingProto())) {
            RET_ONERROR( mywritestr(f, "    int getType() ") )
            RET_ONERROR( mywritef(f, "{ return %d; }\n", getType()) )
            RET_ONERROR( mywritestr(f, "\n") )

            RET_ONERROR( mywritestr(f, "    static ") ) 
            RET_ONERROR( mywritestr(f, getRenderCallbackClass()) )
            RET_ONERROR( mywritestr(f, " ") ) 
            RET_ONERROR( mywritestr(f, getRenderCallbackName()) )
            RET_ONERROR( mywritestr(f, ";\n") ) 
    
            RET_ONERROR( mywritestr(f, "    static void set") ) 
            RET_ONERROR( mywritestr(f, getRenderCallbackClass()) )
            RET_ONERROR( mywritestr(f, "(") ) 
            RET_ONERROR( mywritestr(f, getRenderCallbackClass()) )
            RET_ONERROR( mywritestr(f, " node) {\n") ) 
            RET_ONERROR( mywritestr(f, "        ") ) 
            RET_ONERROR( mywritestr(f, getRenderCallbackName()) )
            RET_ONERROR( mywritestr(f, " = node;\n") ) 
            RET_ONERROR( mywritestr(f, "    }\n\n") ) 

            RET_ONERROR( mywritestr(f, "    static ") ) 
            RET_ONERROR( mywritestr(f, getTreeRenderCallbackClass()) )
            RET_ONERROR( mywritestr(f, " ") ) 
            RET_ONERROR( mywritestr(f, getTreeRenderCallbackName()) )
            RET_ONERROR( mywritestr(f, ";\n") ) 
     
            RET_ONERROR( mywritestr(f, "    static void set") ) 
            RET_ONERROR( mywritestr(f, getTreeRenderCallbackClass()) )
            RET_ONERROR( mywritestr(f, "(") ) 
            RET_ONERROR( mywritestr(f, getTreeRenderCallbackClass()) )
            RET_ONERROR( mywritestr(f, " node) {\n") ) 
            RET_ONERROR( mywritestr(f, "        ") ) 
            RET_ONERROR( mywritestr(f, getTreeRenderCallbackName()) )
            RET_ONERROR( mywritestr(f, " = node;\n") ) 
            RET_ONERROR( mywritestr(f, "    }\n\n") ) 
    
            if (getType() == VRML_INDEXED_FACE_SET) {
                RET_ONERROR( mywritestr(f, "    static ") ) 
                RET_ONERROR( mywritestr(f, getCreateNormalsCallbackClass()) )
                RET_ONERROR( mywritestr(f, " ") ) 
                RET_ONERROR( mywritestr(f, getCreateNormalsCallbackName()) )
                RET_ONERROR( mywritestr(f, ";\n") ) 
        
                RET_ONERROR( mywritestr(f, "    static void set") ) 
                RET_ONERROR( mywritestr(f, getCreateNormalsCallbackClass()) )
                RET_ONERROR( mywritestr(f, "(") ) 
                RET_ONERROR( mywritestr(f, getCreateNormalsCallbackClass()) )
                RET_ONERROR( mywritestr(f, " node) {\n") ) 
                RET_ONERROR( mywritestr(f, "        ") ) 
                RET_ONERROR( mywritestr(f, getCreateNormalsCallbackName()) )
                RET_ONERROR( mywritestr(f, " = node;\n") ) 
                RET_ONERROR( mywritestr(f, "    }\n\n") ) 
            }
            if ((getType() == VRML_INDEXED_FACE_SET) || 
                (getType() == VRML_INDEXED_LINE_SET) || 
                (getType() == X3D_LINE_SET) || 
                (getType() == VRML_POINT_SET)) {
                RET_ONERROR( mywritestr(f, "    void setGlName(int number)") )
                RET_ONERROR( mywritestr(f, "{ glName_number = number; }\n") )
                RET_ONERROR( mywritestr(f, "\n") )
            }

            RET_ONERROR( mywritestr(f, "    static ") ) 
            RET_ONERROR( mywritestr(f, getDoWithDataCallbackClass()) )
            RET_ONERROR( mywritestr(f, " ") ) 
            RET_ONERROR( mywritestr(f, getDoWithDataCallbackName()) )
            RET_ONERROR( mywritestr(f, ";\n") ) 
    
            RET_ONERROR( mywritestr(f, "    static void set") ) 
            RET_ONERROR( mywritestr(f, getDoWithDataCallbackClass()) )
            RET_ONERROR( mywritestr(f, "(") ) 
            RET_ONERROR( mywritestr(f, getDoWithDataCallbackClass()) )
            RET_ONERROR( mywritestr(f, " node) {\n") ) 
            RET_ONERROR( mywritestr(f, "        ") ) 
            RET_ONERROR( mywritestr(f, getDoWithDataCallbackName()) )
            RET_ONERROR( mywritestr(f, " = node;\n") ) 
            RET_ONERROR( mywritestr(f, "    }\n\n") ) 

            RET_ONERROR( mywritestr(f, "    static ") ) 
            RET_ONERROR( mywritestr(f, getTreeDoWithDataCallbackClass()) )
            RET_ONERROR( mywritestr(f, " ") ) 
            RET_ONERROR( mywritestr(f, getTreeDoWithDataCallbackName()) )
            RET_ONERROR( mywritestr(f, ";\n") ) 
    
            RET_ONERROR( mywritestr(f, "    static void set") ) 
            RET_ONERROR( mywritestr(f, getTreeDoWithDataCallbackClass()) )
            RET_ONERROR( mywritestr(f, "(") ) 
            RET_ONERROR( mywritestr(f, getTreeDoWithDataCallbackClass()) )
            RET_ONERROR( mywritestr(f, " node) {\n") ) 
            RET_ONERROR( mywritestr(f, "        ") ) 
            RET_ONERROR( mywritestr(f, getTreeDoWithDataCallbackName()) )
            RET_ONERROR( mywritestr(f, " = node;\n\n") ) 
            RET_ONERROR( mywritestr(f, "    }\n\n") ) 

            RET_ONERROR( mywritestr(f, "    static ") ) 
            RET_ONERROR( mywritestr(f, getProcessEventCallbackClass()) )
            RET_ONERROR( mywritestr(f, " ") ) 
            RET_ONERROR( mywritestr(f, getProcessEventCallbackName()) )
            RET_ONERROR( mywritestr(f, ";\n") ) 

            RET_ONERROR( mywritestr(f, "    static void set") ) 
            RET_ONERROR( mywritestr(f, getProcessEventCallbackClass()) )
            RET_ONERROR( mywritestr(f, "(") ) 
            RET_ONERROR( mywritestr(f, getProcessEventCallbackClass()) )
            RET_ONERROR( mywritestr(f, " node) {\n") ) 
            RET_ONERROR( mywritestr(f, "        ") ) 
            RET_ONERROR( mywritestr(f, getProcessEventCallbackName()) )
            RET_ONERROR( mywritestr(f, " = node;\n") ) 
            RET_ONERROR( mywritestr(f, "    }\n\n") ) 
        }
    }

    if (languageFlag & C_SOURCE) {
        RET_ONERROR( mywritestr(f, "}") )
        RET_ONERROR( mywritestr(f, ";") )
        RET_ONERROR( mywritestr(f, "\n\n") )
    }

    if (languageFlag & C_SOURCE) {
        if (getType() == VRML_INDEXED_FACE_SET) {
            RET_ONERROR( mywritef(f, "    void %sSetGlName(",
                                  TheApp->getCPrefix()) )
            RET_ONERROR( mywritef(f, "struct %sIndexedFaceSet* self, ",
                                  TheApp->getCPrefix()) ) 
            RET_ONERROR( mywritestr(f, "int number)\n") );
            RET_ONERROR( mywritestr(f, "{\n") ) 
            RET_ONERROR( mywritestr(f, "    self->glName_number = number;\n") )
            RET_ONERROR( mywritestr(f, "}\n") ) 
            RET_ONERROR( mywritestr(f, "\n") )
        }
        if (getType() == VRML_INDEXED_LINE_SET) {
            RET_ONERROR( mywritef(f, "    void %sSetGlNameIndexedLineSet(",
                                  TheApp->getCPrefix()) )
            RET_ONERROR( mywritef(f, "struct %sIndexedFaceSet* self, ",
                                  TheApp->getCPrefix()) ) 
            RET_ONERROR( mywritestr(f, "int number)\n") );
            RET_ONERROR( mywritestr(f, "{\n") ) 
            RET_ONERROR( mywritestr(f, "    self->glName_number = number;\n") )
            RET_ONERROR( mywritestr(f, "}\n") ) 
            RET_ONERROR( mywritestr(f, "\n") )
        }
        if (getType() == X3D_LINE_SET) {
            RET_ONERROR( mywritef(f, "    void %sSetGlNameLineSet(",
                                  TheApp->getCPrefix()) )
            RET_ONERROR( mywritef(f, "struct %sIndexedFaceSet* self, ",
                                  TheApp->getCPrefix()) ) 
            RET_ONERROR( mywritestr(f, "int number)\n") );
            RET_ONERROR( mywritestr(f, "{\n") ) 
            RET_ONERROR( mywritestr(f, "    self->glName_number = number;\n") )
            RET_ONERROR( mywritestr(f, "}\n") ) 
            RET_ONERROR( mywritestr(f, "\n") )
        }
        if (getType() == VRML_POINT_SET) {
            RET_ONERROR( mywritef(f, "    void %sSetGlNamePointSet(",
                                  TheApp->getCPrefix()) )
            RET_ONERROR( mywritef(f, "struct %sIndexedFaceSet* self, ",
                                  TheApp->getCPrefix()) ) 
            RET_ONERROR( mywritestr(f, "int number)\n") );
            RET_ONERROR( mywritestr(f, "{\n") ) 
            RET_ONERROR( mywritestr(f, "    self->glName_number = number;\n") )
            RET_ONERROR( mywritestr(f, "}\n") ) 
            RET_ONERROR( mywritestr(f, "\n") )
        }
    }

    if (languageFlag & (CC_SOURCE | JAVA_SOURCE))
        RET_ONERROR( mywritestr(f, "    ") )
    if (languageFlag & C_SOURCE)
        RET_ONERROR( mywritestr(f, "void ") )
    // constructor
    RET_ONERROR( mywritestr(f, getClassName()) )
    if (languageFlag & C_SOURCE)
        RET_ONERROR( mywritestr(f, "Init") )
    RET_ONERROR( mywritestr(f, "(") )
    if (languageFlag & C_SOURCE) {
        RET_ONERROR( mywritestr(f, "struct ") )
        RET_ONERROR( mywritestr(f, getClassName()) )
        RET_ONERROR( mywritestr(f, "* self") )
    }
    RET_ONERROR( mywritestr(f, ") {\n") )
    if (languageFlag & C_SOURCE) {
        RET_ONERROR( mywritestr(f, "    self->m_protoRoot = NULL;\n") )
    }
    if (languageFlag & (C_SOURCE | CC_SOURCE))
        for (long i = 0; i < m_fields.size(); i++)
            RET_ONERROR( writeCConstructorField(f, i, languageFlag) )
    if (languageFlag & (CC_SOURCE | JAVA_SOURCE))
        RET_ONERROR( mywritestr(f, "    ") )
    RET_ONERROR( mywritestr(f, "    ") )
    if (languageFlag & C_SOURCE)
        RET_ONERROR( mywritestr(f, "self->") )
    RET_ONERROR( mywritestr(f, "extra_data = ") )
    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritestr(f, "null;\n") )       
    else
        RET_ONERROR( mywritestr(f, "NULL;\n") )
    if (languageFlag & (CC_SOURCE | JAVA_SOURCE))
        RET_ONERROR( mywritestr(f, "    ") )
    RET_ONERROR( mywritestr(f, "}\n") )
    if (languageFlag & C_SOURCE)
        RET_ONERROR( mywritestr(f, "\n") )
 
    if (languageFlag & C_SOURCE) {
        // callback declarations
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "Callback ") )
        RET_ONERROR( mywritestr(f, getRenderCallbackClass()) )
        RET_ONERROR( mywritestr(f, " = NULL;\n") )

        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "Callback ") )
        RET_ONERROR( mywritestr(f, getTreeRenderCallbackClass()) )
        RET_ONERROR( mywritestr(f, " = NULL;\n") )

        if (getType() == VRML_INDEXED_FACE_SET) {
            RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
            RET_ONERROR( mywritestr(f, "Callback ") )
            RET_ONERROR( mywritestr(f, getCreateNormalsCallbackClass()) )
            RET_ONERROR( mywritestr(f, " = NULL;\n") )
        }

        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "Callback ") )
        RET_ONERROR( mywritestr(f, getDoWithDataCallbackClass()) )
        RET_ONERROR( mywritestr(f, " = NULL;\n") )

        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "Callback ") )
        RET_ONERROR( mywritestr(f, getTreeDoWithDataCallbackClass()) )
        RET_ONERROR( mywritestr(f, " = NULL;\n\n") )

        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "ProcessEventCallback ") )
        RET_ONERROR( mywritestr(f, getProcessEventCallbackClass()) )
        RET_ONERROR( mywritestr(f, " = NULL;\n\n") )

        bool writeOut = true;
        if (getType() == VRML_SCRIPT) {
            if (m_scene->getScriptTypeWritten())
                writeOut = false;
            m_scene->setScriptTypeWritten();
        }
        if (writeOut) {
            // type
            RET_ONERROR( mywritestr(f, "int ") )
            RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
            RET_ONERROR( mywritestr(f, getName(true)) )
            RET_ONERROR( mywritestr(f, "Type") )
            RET_ONERROR( mywritef(f, " = %d;\n\n", getType()) )
        }

        // functions
        RET_ONERROR( mywritestr(f, "void ") )
        RET_ONERROR( mywritestr(f, getClassName()) )
        RET_ONERROR( mywritestr(f, "Render(") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
        RET_ONERROR( mywritestr(f, "* self, void *dataptr) {") )
        RET_ONERROR( mywritestr(f, "\n") )
        RET_ONERROR( mywritestr(f, "    if (") )
        RET_ONERROR( mywritestr(f, getRenderCallbackClass()) )
        RET_ONERROR( mywritestr(f, ")\n") )
        RET_ONERROR( mywritestr(f, "        ") )
        RET_ONERROR( mywritestr(f, getRenderCallbackClass()) )
        RET_ONERROR( mywritestr(f, "(self, dataptr);\n") )
        RET_ONERROR( mywritestr(f, "}\n") )

        RET_ONERROR( mywritestr(f, "void ") )
        RET_ONERROR( mywritestr(f, getClassName()) )
        RET_ONERROR( mywritestr(f, "TreeRender(") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
        RET_ONERROR( mywritestr(f, "* self, void *dataptr)") )
        RET_ONERROR( mywritestr(f, " {\n") )
        RET_ONERROR( mywritestr(f, "    int i;\n") )
        RET_ONERROR( mywritestr(f, "    if (") )
        RET_ONERROR( mywritestr(f, "((struct ") )
        RET_ONERROR( mywritestr(f, getClassName()) )
        RET_ONERROR( mywritestr(f, "*)self)->m_protoRoot != NULL)\n") )
        RET_ONERROR( mywritestr(f, "        ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "TreeRenderCallback(((struct ") )
        RET_ONERROR( mywritestr(f, getClassName()) )
        RET_ONERROR( mywritestr(f, "*)self)->m_protoRoot, dataptr);\n") )
        RET_ONERROR( mywritestr(f, "    else {\n") )

        for (long i = 0; i < m_fields.size(); i++)
            RET_ONERROR( writeCCallTreeField(f, i, "TreeRenderCallback", 
                                             languageFlag) )

        RET_ONERROR( mywritestr(f, "        ") )
        RET_ONERROR( mywritestr(f, getClassName()) )
        RET_ONERROR( mywritestr(f, "Render(self, dataptr);\n") )

        RET_ONERROR( mywritestr(f, "    }\n") )
        RET_ONERROR( mywritestr(f, "}\n") )

        if (getType() == VRML_INDEXED_FACE_SET) {
            RET_ONERROR( mywritestr(f, "void ") )
            RET_ONERROR( mywritestr(f, getClassName()) )
            RET_ONERROR( mywritestr(f, "CreateNormals(") )
            RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
            RET_ONERROR( mywritestr(f, "* self, void *dataptr) {") )
            RET_ONERROR( mywritestr(f, "\n") )
            RET_ONERROR( mywritestr(f, "    if (") )
            RET_ONERROR( mywritestr(f, getCreateNormalsCallbackClass()) )
            RET_ONERROR( mywritestr(f, ")\n") )
            RET_ONERROR( mywritestr(f, "        ") )
            RET_ONERROR( mywritestr(f, getCreateNormalsCallbackClass()) )
            RET_ONERROR( mywritestr(f, "(self, dataptr);\n") )
            RET_ONERROR( mywritestr(f, "}\n") )
        }

        // functions
        RET_ONERROR( mywritestr(f, "void ") )
        RET_ONERROR( mywritestr(f, getClassName()) )
        RET_ONERROR( mywritestr(f, "DoWithData(") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
        RET_ONERROR( mywritestr(f, "* self, void *dataptr)") )
        RET_ONERROR( mywritestr(f, " {\n") )
        RET_ONERROR( mywritestr(f, "    if (") )
        RET_ONERROR( mywritestr(f, getDoWithDataCallbackClass()) )
        RET_ONERROR( mywritestr(f, ")\n") )
        RET_ONERROR( mywritestr(f, "        ") )
        RET_ONERROR( mywritestr(f, getDoWithDataCallbackClass()) )
        RET_ONERROR( mywritestr(f, "(self, dataptr);\n") )
        RET_ONERROR( mywritestr(f, "}\n") )

        RET_ONERROR( mywritestr(f, "void ") )
        RET_ONERROR( mywritestr(f, getClassName()) )
        RET_ONERROR( mywritestr(f, "TreeDoWithData(") )
        RET_ONERROR( mywritestr(f, TheApp->getCNodeName()) ) 
        RET_ONERROR( mywritestr(f, "* self, ") )
        RET_ONERROR( mywritestr(f, "void *dataptr)") )
        RET_ONERROR( mywritestr(f, " {\n") )
        RET_ONERROR( mywritestr(f, "    int i;\n") )

        RET_ONERROR( mywritestr(f, "    if (") )
        RET_ONERROR( mywritestr(f, "((struct ") )
        RET_ONERROR( mywritestr(f, getClassName()) )
        RET_ONERROR( mywritestr(f, "*)self)->m_protoRoot != NULL)\n") )
        RET_ONERROR( mywritestr(f, "        ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "TreeDoWithDataCallback(((struct ") )
        RET_ONERROR( mywritestr(f, getClassName()) )
        RET_ONERROR( mywritestr(f, "*)self)->m_protoRoot, dataptr);\n") )
        RET_ONERROR( mywritestr(f, "    else {\n") )

        for (long i = 0; i < m_fields.size(); i++)
            RET_ONERROR( writeCCallTreeField(f, i, "TreeDoWithDataCallback", languageFlag) )

        RET_ONERROR( mywritestr(f, "        ") )
        RET_ONERROR( mywritestr(f, getClassName()) )
        RET_ONERROR( mywritestr(f, "DoWithData(self, dataptr);\n") )

        RET_ONERROR( mywritestr(f, "    }\n") )
        RET_ONERROR( mywritestr(f, "}\n") )
    }
    if (languageFlag & CC_SOURCE) {
        // memberfunctions
        RET_ONERROR( mywritestr(f, "    virtual void ") )
        RET_ONERROR( mywritestr(f, "treeRender(void *dataptr) {\n") )
        RET_ONERROR( mywritestr(f, "        int i;\n") )

        RET_ONERROR( mywritestr(f, "        if (") )
        RET_ONERROR( mywritestr(f, "treeRenderCallback) {\n") )
        RET_ONERROR( mywritestr(f, "            ") )
        RET_ONERROR( mywritestr(f, "treeRenderCallback(this, ") )
        RET_ONERROR( mywritestr(f, "dataptr);\n") )
        RET_ONERROR( mywritestr(f, "            return;\n") )
        RET_ONERROR( mywritestr(f, "        }\n") )

        RET_ONERROR( mywritestr(f, "        if (m_protoRoot != NULL)\n") )
        RET_ONERROR( mywritestr(f, "            m_protoRoot->") )
        RET_ONERROR( mywritestr(f, "treeRender(dataptr);\n") )
        RET_ONERROR( mywritestr(f, "        else {\n") )

        for (long i = 0; i < m_fields.size(); i++)
            RET_ONERROR( writeCCallTreeField(f, i, "TreeRender", languageFlag,
                                             "renderCallback") )

        RET_ONERROR( mywritestr(f, "            render(dataptr);\n") )
        RET_ONERROR( mywritestr(f, "        }\n") )

        RET_ONERROR( mywritestr(f, "    }\n") )

        RET_ONERROR( mywritestr(f, "    virtual void render(void *dataptr) {\n") )
        RET_ONERROR( mywritestr(f, "        if (") )
        RET_ONERROR( mywritestr(f, "renderCallback)\n") )
        RET_ONERROR( mywritestr(f, "            ") )
        RET_ONERROR( mywritestr(f, "renderCallback(this, dataptr);") )
        RET_ONERROR( mywritestr(f, "\n    }\n") )

        if (getType() == VRML_INDEXED_FACE_SET) {
            RET_ONERROR( mywritestr(f, "    virtual void createNormals(void *dataptr) {\n") )
            RET_ONERROR( mywritestr(f, "        if (") )
            RET_ONERROR( mywritestr(f, "createNormalsCallback)\n") )
            RET_ONERROR( mywritestr(f, "            ") )
            RET_ONERROR( mywritestr(f, "createNormalsCallback(this, dataptr);") )
            RET_ONERROR( mywritestr(f, "\n    }\n") )
        }

        RET_ONERROR( mywritestr(f, "    virtual void treeDoWithData(void *dataptr)") )
        RET_ONERROR( mywritestr(f, " {\n") )
        RET_ONERROR( mywritestr(f, "        int i;\n") )

        RET_ONERROR( mywritestr(f, "        if (") )
        RET_ONERROR( mywritestr(f, "treeDoWithDataCallback) {\n") )
        RET_ONERROR( mywritestr(f, "            ") )
        RET_ONERROR( mywritestr(f, "treeDoWithDataCallback(this, ") )
        RET_ONERROR( mywritestr(f, "dataptr);\n") )
        RET_ONERROR( mywritestr(f, "            return;\n") )
        RET_ONERROR( mywritestr(f, "        }\n") )


        RET_ONERROR( mywritestr(f, "        if (m_protoRoot != NULL)\n") )
        RET_ONERROR( mywritestr(f, "            m_protoRoot->") )
        RET_ONERROR( mywritestr(f, "treeDoWithData(dataptr);\n") )
        RET_ONERROR( mywritestr(f, "        else {\n") )

        for (long i = 0; i < m_fields.size(); i++)
            RET_ONERROR( writeCCallTreeField(f, i, "TreeDoWithData", 
                                             languageFlag, 
                                             "doWithDataCallback") )

        RET_ONERROR( mywritestr(f, "            doWithData(dataptr);\n") )

        RET_ONERROR( mywritestr(f, "        }\n") )
        RET_ONERROR( mywritestr(f, "    }\n") )

        RET_ONERROR( mywritestr(f, "    virtual void doWithData(void *dataptr)") )
        RET_ONERROR( mywritestr(f, " {\n") )
        RET_ONERROR( mywritestr(f, "        if (") )
        RET_ONERROR( mywritestr(f, "doWithDataCallback)\n") )
        RET_ONERROR( mywritestr(f, "            ") )
        RET_ONERROR( mywritestr(f, "doWithDataCallback(this, dataptr);" ) )
        RET_ONERROR( mywritestr(f, "\n") )
        RET_ONERROR( mywritestr(f, "    }\n") )
    
        RET_ONERROR( mywritestr(f, "}") )
        RET_ONERROR( mywritestr(f, ";") )
        RET_ONERROR( mywritestr(f, "\n\n") )

        // callbacks
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "Callback ") )
        RET_ONERROR( mywritestr(f, getClassName()) )
        RET_ONERROR( mywritestr(f, "::") )
        RET_ONERROR( mywritestr(f, "renderCallback = NULL;\n") )

        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "Callback ") )
        RET_ONERROR( mywritestr(f, getClassName()) )
        RET_ONERROR( mywritestr(f, "::") )
        RET_ONERROR( mywritestr(f, "treeRenderCallback = NULL;\n") )

        if (getType() == VRML_INDEXED_FACE_SET) {
            RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
            RET_ONERROR( mywritestr(f, "Callback ") )
            RET_ONERROR( mywritestr(f, getClassName()) )
            RET_ONERROR( mywritestr(f, "::") )
            RET_ONERROR( mywritestr(f, "createNormalsCallback = NULL;\n") )
        }

        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "Callback ") )
        RET_ONERROR( mywritestr(f, getClassName()) )
        RET_ONERROR( mywritestr(f, "::") )
        RET_ONERROR( mywritestr(f, "doWithDataCallback = NULL;\n") )

        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "Callback ") )
        RET_ONERROR( mywritestr(f, getClassName()) )
        RET_ONERROR( mywritestr(f, "::") )
        RET_ONERROR( mywritestr(f, "treeDoWithDataCallback = NULL;\n") )

        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "ProcessEventCallback ") )
        RET_ONERROR( mywritestr(f, getClassName()) )
        RET_ONERROR( mywritestr(f, "::") )
        RET_ONERROR( mywritestr(f, "processEventCallback") )
        RET_ONERROR( mywritestr(f, " = NULL;\n\n") )

        bool writeOut = true;
        if (getType() == VRML_SCRIPT) {
            if (m_scene->getScriptTypeWritten())
                writeOut = false;
            m_scene->setScriptTypeWritten();
        }
        if (writeOut) {
            // type
            RET_ONERROR( mywritestr(f, "int ") )
            RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
            RET_ONERROR( mywritestr(f, getName(true)) )
            RET_ONERROR( mywritestr(f, "Type") )
            RET_ONERROR( mywritef(f, " = %d;\n\n", getType()) )
        }
    }
    if (languageFlag & JAVA_SOURCE) {
        // memberfunctions
        RET_ONERROR( mywritestr(f, "    void treeRender(") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "Node dataptr, Object object) {\n") )
        RET_ONERROR( mywritestr(f, "        if (") )
        RET_ONERROR( mywritestr(f, getClassName()) ) 
        RET_ONERROR( mywritestr(f, ".") )
        RET_ONERROR( mywritestr(f, getTreeRenderCallbackName()) )
        RET_ONERROR( mywritestr(f, " != null) {\n") )
        RET_ONERROR( mywritestr(f, "            ") )
        RET_ONERROR( mywritestr(f, getClassName()) ) 
        RET_ONERROR( mywritestr(f, ".") )
        RET_ONERROR( mywritestr(f, getTreeRenderCallbackName()) )
        RET_ONERROR( mywritestr(f, ".treeRender(this, object);\n") )
        RET_ONERROR( mywritestr(f, "            return;\n") )
        RET_ONERROR( mywritestr(f, "        }\n") )
        for (long i = 0; i < m_fields.size(); i++)
            RET_ONERROR( writeCCallTreeField(f, i, "TreeRender", languageFlag,
                                             "renderCallback", true) )
        RET_ONERROR( mywritestr(f, "        if (m_protoRoot != null)\n") )
        RET_ONERROR( mywritestr(f, "            m_protoRoot.") )
        RET_ONERROR( mywritestr(f, "treeRender(dataptr, object);\n") )
        RET_ONERROR( mywritestr(f, "        if (") )
        RET_ONERROR( mywritestr(f, getClassName()) ) 
        RET_ONERROR( mywritestr(f, ".") )
        RET_ONERROR( mywritestr(f, getRenderCallbackName()) )
        RET_ONERROR( mywritestr(f, " != null)\n") )
        RET_ONERROR( mywritestr(f, "            ") )
        RET_ONERROR( mywritestr(f, getClassName()) ) 
        RET_ONERROR( mywritestr(f, ".") )
        RET_ONERROR( mywritestr(f, getRenderCallbackName()) )
        RET_ONERROR( mywritestr(f, ".render(this, object);\n") )
        RET_ONERROR( mywritestr(f, "    }\n") )

        RET_ONERROR( mywritestr(f, "    void treeDoWithData(") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "Node dataptr) {\n") )
        RET_ONERROR( mywritestr(f, "        if (") )
        RET_ONERROR( mywritestr(f, getClassName()) ) 
        RET_ONERROR( mywritestr(f, ".") )
        RET_ONERROR( mywritestr(f, getTreeDoWithDataCallbackName()) )
        RET_ONERROR( mywritestr(f, " != null) {\n") )
        RET_ONERROR( mywritestr(f, "            ") )
        RET_ONERROR( mywritestr(f, getClassName()) ) 
        RET_ONERROR( mywritestr(f, ".") )
        RET_ONERROR( mywritestr(f, getTreeDoWithDataCallbackName()) )
        RET_ONERROR( mywritestr(f, ".treeDoWithData(this);\n") )
        RET_ONERROR( mywritestr(f, "            return;\n") )
        RET_ONERROR( mywritestr(f, "        }\n") )
        for (long i = 0; i < m_fields.size(); i++)
            RET_ONERROR( writeCCallTreeField(f, i, "TreeDoWithData", 
                                             languageFlag, 
                                             "treeDoWithDataCallback") )
        RET_ONERROR( mywritestr(f, "        if (m_protoRoot != null)\n") )
        RET_ONERROR( mywritestr(f, "            m_protoRoot.") )
        RET_ONERROR( mywritestr(f, "treeDoWithData(dataptr);\n") )
        RET_ONERROR( mywritestr(f, "        if (") )
        RET_ONERROR( mywritestr(f, getClassName()) ) 
        RET_ONERROR( mywritestr(f, ".") )
        RET_ONERROR( mywritestr(f, getDoWithDataCallbackName()) )
        RET_ONERROR( mywritestr(f, " != null)\n") )
        RET_ONERROR( mywritestr(f, "            ") )
        RET_ONERROR( mywritestr(f, getClassName()) ) 
        RET_ONERROR( mywritestr(f, ".") )
        RET_ONERROR( mywritestr(f, getDoWithDataCallbackName()) )
        RET_ONERROR( mywritestr(f, ".doWithData(this);\n") )
        RET_ONERROR( mywritestr(f, "    }\n") )

        if (getType() == VRML_INDEXED_FACE_SET) {
            RET_ONERROR( mywritestr(f, "    void createNormals(Object dataptr) {\n") )
            RET_ONERROR( mywritestr(f, "        if (") )
            RET_ONERROR( mywritestr(f, getClassName()) ) 
            RET_ONERROR( mywritestr(f, ".") )
            RET_ONERROR( mywritestr(f, getCreateNormalsCallbackName()) )
            RET_ONERROR( mywritestr(f, " != null) {\n") )
            RET_ONERROR( mywritestr(f, "            ") )
            RET_ONERROR( mywritestr(f, getClassName()) ) 
            RET_ONERROR( mywritestr(f, ".") )
            RET_ONERROR( mywritestr(f, getCreateNormalsCallbackName()) )
            RET_ONERROR( mywritestr(f, ".createNormals(this, dataptr);\n") )
            RET_ONERROR( mywritestr(f, "            return;\n") )
            RET_ONERROR( mywritestr(f, "        }\n") )
            RET_ONERROR( mywritestr(f, "        if (") )
            RET_ONERROR( mywritestr(f, getClassName()) ) 
            RET_ONERROR( mywritestr(f, ".") )
            RET_ONERROR( mywritestr(f, getCreateNormalsCallbackName()) )
            RET_ONERROR( mywritestr(f, " != null)\n") )
            RET_ONERROR( mywritestr(f, "            ") )
            RET_ONERROR( mywritestr(f, getClassName()) ) 
            RET_ONERROR( mywritestr(f, ".") )
            RET_ONERROR( mywritestr(f, getCreateNormalsCallbackName()) )
            RET_ONERROR( mywritestr(f, ".createNormals(this, dataptr);\n") )
            RET_ONERROR( mywritestr(f, "    }\n") )
        }    
        RET_ONERROR( mywritestr(f, "}\n") )
    }

    RET_ONERROR( mywritestr(f, "\n\n") )

    if (languageFlag & JAVA_SOURCE) {
        // "callbacks"
        RET_ONERROR( mywritestr(f, "class ") )
        RET_ONERROR( mywritestr(f, getRenderCallbackClass()) )
        RET_ONERROR( mywritestr(f, " extends ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "RenderCallback {}\n") )

        RET_ONERROR( mywritestr(f, "class ") )
        RET_ONERROR( mywritestr(f, getTreeRenderCallbackClass()) )
        RET_ONERROR( mywritestr(f, " extends ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "TreeRenderCallback {}\n") )

        RET_ONERROR( mywritestr(f, "class ") )
        RET_ONERROR( mywritestr(f, getDoWithDataCallbackClass()) )
        RET_ONERROR( mywritestr(f, " extends ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "DoWithDataCallback {};\n") )

        RET_ONERROR( mywritestr(f, "class ") )
        RET_ONERROR( mywritestr(f, getTreeDoWithDataCallbackClass()) )
        RET_ONERROR( mywritestr(f, " extends ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "TreeDoWithDataCallback {};\n") )

        if (getType() == VRML_INDEXED_FACE_SET) {
            RET_ONERROR( mywritestr(f, "class ") )
            RET_ONERROR( mywritestr(f, getCreateNormalsCallbackClass()) )
            RET_ONERROR( mywritestr(f, " extends ") )
            RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
            RET_ONERROR( mywritestr(f, "CreateNormalsCallback {}\n") )
        }

         RET_ONERROR( mywritestr(f, "class ") )
         RET_ONERROR( mywritestr(f, getProcessEventCallbackClass()) )
         RET_ONERROR( mywritestr(f, " extends ") )
         RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
         RET_ONERROR( mywritestr(f, "ProcessEventCallback {};\n") )

         RET_ONERROR( mywritestr(f, "\n") )
 
        bool writeOut = true;
        if (getType() == VRML_SCRIPT) {
            if (m_scene->getScriptTypeWritten())
                writeOut = false;
            m_scene->setScriptTypeWritten();
        }
        if (writeOut) {
             // type 
             RET_ONERROR( mywritestr(f, "class ") )
             RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
             RET_ONERROR( mywritestr(f, getName(true)) )
             RET_ONERROR( mywritestr(f, "Type") )
             RET_ONERROR( mywritef(f, " {\n    static final int type = %d;\n}\n\n",
                                   getType()) )
        }
    }
    return 0;
}

int 
Proto::writeCDeclarationEventIn(int f, int i, int languageFlag)
{
    bool x3d = true;

    EventIn *eventIn = getEventIn(i);
    if (eventIn->getFlags() & (FF_VRML_ONLY | FF_NEVER | FF_X3DOM_ONLY))
        return 0;
    if (eventIn->getExposedField() != NULL)
        return 0;
    const char *name = eventIn->getName(x3d);
    RET_ONERROR( mywritestr(f, "    ") )
    RET_ONERROR( mywritestr(f, getTypeC(eventIn->getType(), languageFlag)) )
    if (isArrayInC(eventIn->getType())) {
        if (languageFlag & JAVA_SOURCE) {
            if ((eventIn->getType() != SFNODE) && 
                (eventIn->getType() != MFNODE))
                RET_ONERROR( mywritestr(f, "[]") )
        } else 
            RET_ONERROR( mywritestr(f, "*") )
    }
    RET_ONERROR( mywritestr(f, " ") )
    RET_ONERROR( mywritestr(f, name) )
    RET_ONERROR( mywritestr(f, ";\n") )
    if (isArrayInC(eventIn->getType()) && (languageFlag & 
                                           (C_SOURCE | CC_SOURCE))) {
        RET_ONERROR( mywritestr(f, "    int ") )
        RET_ONERROR( mywritestr(f, name) )
        RET_ONERROR( mywritestr(f, "_length") )
        RET_ONERROR( mywritestr(f, ";\n") )
    }
    return(0);
}

int 
Proto::writeCDeclarationEventOut(int f, int i, int languageFlag)
{
    bool x3d = true;

    EventOut *eventOut = getEventOut(i);
    if (eventOut->getFlags() & (FF_VRML_ONLY | FF_NEVER | FF_X3DOM_ONLY))
        return 0;
    if (eventOut->getExposedField() != NULL)
        return 0;
    const char *name = eventOut->getName(x3d);
    RET_ONERROR( mywritestr(f, "    ") )
    RET_ONERROR( mywritestr(f, getTypeC(eventOut->getType(), languageFlag)) )
    if (isArrayInC(eventOut->getType())) {
        if (languageFlag & JAVA_SOURCE) {
            if ((eventOut->getType() != SFNODE) && 
                (eventOut->getType() != MFNODE))
                RET_ONERROR( mywritestr(f, "[]") )
        } else 
            RET_ONERROR( mywritestr(f, "*") )
    }
    RET_ONERROR( mywritestr(f, " ") )
    RET_ONERROR( mywritestr(f, name) )
    RET_ONERROR( mywritestr(f, ";\n") )
    if (isArrayInC(eventOut->getType()) && (languageFlag & 
                                            (C_SOURCE | CC_SOURCE))) {
        RET_ONERROR( mywritestr(f, "    int ") )
        RET_ONERROR( mywritestr(f, name) )
        RET_ONERROR( mywritestr(f, "_length") )
        RET_ONERROR( mywritestr(f, ";\n") )
    }
    return(0);
}

int 
Proto::writeCDeclarationField(int f, int i, int languageFlag)
{
    bool x3d = true;

    Field *field = getField(i);
    if (field->getFlags() & (FF_VRML_ONLY | FF_NEVER | FF_X3DOM_ONLY))
        return 0;
    const char *name = field->getName(x3d);
    RET_ONERROR( mywritestr(f, "    ") )
    RET_ONERROR( mywritestr(f, getTypeC(field->getType(), languageFlag)) )
    if (isArrayInC(field->getType())) {
        if (languageFlag & JAVA_SOURCE) {
            if ((field->getType() != SFNODE) && (field->getType() != MFNODE))
                RET_ONERROR( mywritestr(f, "[]") )
        } else 
            RET_ONERROR( mywritestr(f, "*") )
    }
    RET_ONERROR( mywritestr(f, " ") )
    RET_ONERROR( mywritestr(f, name) )
    RET_ONERROR( mywritestr(f, ";\n") )
    if (isArrayInC(field->getType()) && (languageFlag & 
                                         (C_SOURCE | CC_SOURCE))) {
        RET_ONERROR( mywritestr(f, "    int ") )
        RET_ONERROR( mywritestr(f, name) )
        RET_ONERROR( mywritestr(f, "_length") )
        RET_ONERROR( mywritestr(f, ";\n") )
    }
    return(0);
}

int 
Proto::writeCConstructorField(int f, int i, int languageFlag)
{
    bool x3d = true;

    Field *field = getField(i);
    if (field->getFlags() & (FF_VRML_ONLY | FF_NEVER | FF_X3DOM_ONLY))
        return 0;
    FieldValue *value = field->getDefault(x3d);
    const char *name = field->getName(x3d);
    if (languageFlag & (CC_SOURCE | JAVA_SOURCE))
        RET_ONERROR( mywritestr(f, "    ") )
    RET_ONERROR( mywritestr(f, "    ") )
    if (languageFlag & C_SOURCE)
        RET_ONERROR( mywritestr(f, "self->") )
    RET_ONERROR( mywritestr(f, name) )
    RET_ONERROR( mywritestr(f, " = ") )
    RET_ONERROR( mywritestr(f, value->getDefaultC(languageFlag)) )
    RET_ONERROR( mywritestr(f, ";\n") )
    if (value->isArrayInC() && (languageFlag & (C_SOURCE | CC_SOURCE))) {
        if (languageFlag & (CC_SOURCE | JAVA_SOURCE))
            RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, "    ") )
        if (languageFlag & C_SOURCE)
            RET_ONERROR( mywritestr(f, "self->") )
        RET_ONERROR( mywritestr(f, name) )
        RET_ONERROR( mywritestr(f, "_length") )
        RET_ONERROR( mywritestr(f, " = 0;\n") )
    }
    return(0);
}

int 
Proto::writeCCallTreeField(int f, int i, const char *treeFunctionName, 
                           int languageFlag, const char *functionName,
                           bool useObject)
{
    bool x3d = true;

    Field *field = getField(i);
    if ((field->getType() != SFNODE) && (field->getType() != MFNODE))
        return 0; 
    if (field->getFlags() & (FF_VRML_ONLY | FF_NEVER | FF_X3DOM_ONLY))
        return 0;
    FieldValue *value = field->getDefault(x3d);
    const char *name = field->getName(x3d);
    MyString fieldName = "";
    fieldName += name;
    if (value->isArrayInC()) {
        if (languageFlag & CC_SOURCE)
            RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, "if (") )
        if (languageFlag & JAVA_SOURCE)
            RET_ONERROR( mywritestr(f, "(") )
        if (languageFlag & C_SOURCE) {
            RET_ONERROR( mywritestr(f, "((struct ") )
            RET_ONERROR( mywritestr(f, getClassName()) )
            RET_ONERROR( mywritestr(f, "*)self)->") )
        }
        RET_ONERROR( mywritestr(f, fieldName) )
        if (languageFlag & JAVA_SOURCE)
            RET_ONERROR( mywritestr(f, " != null") )
        if (languageFlag & JAVA_SOURCE) {
            if (functionName != NULL) {
                RET_ONERROR( mywritestr(f, ") && (") )
                RET_ONERROR( mywritestr(f, getClassName()) )
                RET_ONERROR( mywritestr(f, ".") )
                RET_ONERROR( mywritestr(f, functionName) )
                RET_ONERROR( mywritestr(f, getName(true)) )
                RET_ONERROR( mywritestr(f, " == null)") )
           }
        }  
        RET_ONERROR( mywritestr(f, ")\n") )
        if (languageFlag & CC_SOURCE)
            RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, "        ") )
        RET_ONERROR( mywritestr(f, "for (") )
        if (languageFlag & JAVA_SOURCE)
            RET_ONERROR( mywritestr(f, "int ") )
        RET_ONERROR( mywritestr(f, "i = 0; i < ") )
        if (languageFlag & C_SOURCE) {
            RET_ONERROR( mywritestr(f, "((struct ") )
            RET_ONERROR( mywritestr(f, getClassName()) )
            RET_ONERROR( mywritestr(f, "*)self)->") )
        }
        RET_ONERROR( mywritestr(f, name) )
        if (languageFlag & JAVA_SOURCE)
            RET_ONERROR( mywritestr(f, ".") )
        else
            RET_ONERROR( mywritestr(f, "_") )
        RET_ONERROR( mywritestr(f, "length; i++)\n") )
    }
    if (value->isArrayInC())
        fieldName += "[i]";
    if (value->isArrayInC())
        RET_ONERROR( mywritestr(f, "        ") )
    if (languageFlag & CC_SOURCE)
        RET_ONERROR( mywritestr(f, "    ") )
    RET_ONERROR( mywritestr(f, "    ") )
    RET_ONERROR( mywritestr(f, "    ") )
    RET_ONERROR( mywritestr(f, "if (") )
    if (languageFlag & C_SOURCE) {
        RET_ONERROR( mywritestr(f, "((struct ") )
        RET_ONERROR( mywritestr(f, getClassName()) )
        RET_ONERROR( mywritestr(f, "*)self)->") )
    }
    RET_ONERROR( mywritestr(f, fieldName) )
    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritestr(f, " != null") )
    RET_ONERROR( mywritestr(f, ")\n") )
    if (value->isArrayInC())
        RET_ONERROR( mywritestr(f, "        ") )
    RET_ONERROR( mywritestr(f, "    ") )
    if (languageFlag & CC_SOURCE)
        RET_ONERROR( mywritestr(f, "    ") )
    RET_ONERROR( mywritestr(f, "    ") )
    RET_ONERROR( mywritestr(f, "    ") )
    if (languageFlag & C_SOURCE) {
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, treeFunctionName) )
        RET_ONERROR( mywritestr(f, "(((struct ") )
        RET_ONERROR( mywritestr(f, getClassName()) )
        RET_ONERROR( mywritestr(f, "*)self)->") )
        RET_ONERROR( mywritestr(f, fieldName) )
        RET_ONERROR( mywritestr(f, ", dataptr);\n") )
    } else if (languageFlag & CC_SOURCE) {
        RET_ONERROR( mywritestr(f, fieldName) )
        RET_ONERROR( mywritestr(f, "->") )
        MyString lowFirst = "";
        lowFirst += tolower(treeFunctionName[0]);
        RET_ONERROR( mywritestr(f, lowFirst) )
        RET_ONERROR( mywritestr(f, treeFunctionName + 1) )
        RET_ONERROR( mywritestr(f, "(dataptr);\n") )
    } else if (languageFlag & JAVA_SOURCE) {
        RET_ONERROR( mywritestr(f, fieldName) )
        RET_ONERROR( mywritestr(f, ".") )
        MyString lowFirst = "";
        lowFirst += tolower(treeFunctionName[0]);
        RET_ONERROR( mywritestr(f, lowFirst) )
        RET_ONERROR( mywritestr(f, treeFunctionName + 1) )
        RET_ONERROR( mywritestr(f, "(dataptr") )
        if (useObject)
            RET_ONERROR( mywritestr(f, ", object") )
        RET_ONERROR( mywritestr(f, ");\n") )
    }
    return(0);
}

Element *
Proto::getElement(int elementType, int index) const
{
     if (elementType == EL_EVENT_IN)
         return getEventIn(index);
     if (elementType == EL_EVENT_OUT)
         return getEventOut(index);    
     if (elementType == EL_EXPOSED_FIELD)
         return getExposedField(index);    
     return getField(index);
}

bool        
Proto::showFields(bool x3d) 
{ 
    if (m_showFieldsInit == false) {
        m_showFieldsInit = true;
        int xfNodes = 0;
        for (int i = 0; i < getNumFields(); i++)
            if ((getField(i)->getType() == SFNODE) ||
                (getField(i)->getType() == MFNODE))
                if (i != metadata_Field())
                    if (m_scene->isValidElement(getField(i), x3d))
                        xfNodes++;
        m_showFields = false;
        if (xfNodes > 1)   
            m_showFields = true;
    }
    return m_showFields;    
}


int
Proto::writeCExtraFields(int f, int languageFlag)
{
    if ((languageFlag & C_SOURCE) | (languageFlag & CC_SOURCE)) {
            RET_ONERROR( mywritestr(f, "    void* extra_data;\n") )
    } else if ((languageFlag & JAVA_SOURCE)) {
            RET_ONERROR( mywritestr(f, "    Object extra_data;\n") )
    }
    if ((getType() == VRML_INDEXED_FACE_SET) || 
        (getType() == VRML_INDEXED_LINE_SET) || 
        (getType() == X3D_LINE_SET) || 
        (getType() == VRML_POINT_SET)) {
        RET_ONERROR( mywritestr(f, "    int glName_number;\n") )
    }
    if (getType() == VRML_TRANSFORM) {
        RET_ONERROR( mywritestr(f, "    int num_route_source;\n") )
        if ((languageFlag & C_SOURCE) | (languageFlag & CC_SOURCE)) {
            RET_ONERROR( mywritef(f, "    %sNode **route_sources;\n", 
                                     TheApp->getCPrefix()) )
        } else if ((languageFlag & JAVA_SOURCE)) {
            RET_ONERROR( mywritef(f, "    %sNode route_sources[];\n", 
                                     TheApp->getCPrefix()) )
        }
    }
    return 0;
}

// collect array of dynamic field's, eventIn's and eventOut's
void
Proto::buildInterfaceData(bool protoFlag) 
{
    bool x3d = m_scene->isX3d();
    m_interface.resize(0);
    Proto* proto = this;
    if (proto == NULL)
        return;
    int fieldFlags = FF_STATIC;
    for (int i = 0; i < proto->getNumFields(); i++)
        if (!(proto->getField(i)->getFlags() & fieldFlags)) 
            if (proto->getField(i)->getExposedField() == NULL)
                m_interface.append(new InterfaceData(EL_FIELD, i));
    // exposedFields are always FF_HIDDEN
    if (protoFlag)
        for (int i = 1; i < proto->getNumExposedFields(); i++)
            if (!(proto->getExposedField(i)->getFlags() & fieldFlags)) 
                m_interface.append(new InterfaceData(EL_EXPOSED_FIELD, i));
    fieldFlags = 0;
    if (!protoFlag)
        fieldFlags = FF_HIDDEN;
//        fieldFlags = FF_HIDDEN | FF_STATIC;
    for (int i = 0; i < proto->getNumEventOuts(); i++) {
        // ignore Exposedfields (eg. "url")
        bool isExposedField = false;
        for (long j = 0; j < proto->m_exposedFields.size(); j++) {
            MyString exposedName = "";
            exposedName += mystrdup(proto->getExposedField(j)->getName(x3d));
            exposedName += "_changed";
            if (strcmp(proto->getEventOut(i)->getName(x3d), exposedName) == 0)
               isExposedField = true;
        }
        if (isExposedField) 
            continue;
        if (!(proto->getEventOut(i)->getFlags() & fieldFlags))
            m_interface.append(new InterfaceData(EL_EVENT_OUT, i));
    }
    for (int i = 0; i < proto->getNumEventIns(); i++) {
        // ignore Exposedfields (eg. "url")
        bool isExposedField = false;
        for (long j = 0; j < proto->m_exposedFields.size(); j++) {
            MyString exposedName = "";
            exposedName += "set_";
            exposedName += proto->getExposedField(j)->getName(x3d);
            if (strcmp(proto->getEventIn(i)->getName(x3d), exposedName) == 0)
               isExposedField = true;
        }
        if (isExposedField) 
            continue;
        if (!(proto->getEventIn(i)->getFlags() & fieldFlags))
            m_interface.append(new InterfaceData(EL_EVENT_IN, i));
    }
}

bool 
Proto::matchNodeClass(int childType) const
{
    return ::matchNodeClass(getNodeClass(), childType);
}

class FiledesAndIndent {
public:
    int filedes;
    int indent;
};

static bool writeRoutesPerNode(Node *node, void *data)
{
    FiledesAndIndent *fid = (FiledesAndIndent *)data;
    node->setFlag(NODE_FLAG_TOUCHED);
    indentf(fid->filedes, fid->indent);
    node->writeRoutes(fid->filedes, 0);
    return true;
}

int         
NodePROTO::write(int filedes, int indent, bool avoidUse)
{ 
#if HAVE_NO_PROTOS_X3DOM
    if ((m_scene->getWriteFlags() & X3DOM) && (getProto()->getNumNodes() > 0)) {
        for (int k = 0; k < getProto()->getNumNodes(); k++)
            RET_ONERROR( getProto()->getNode(k)->write(filedes, indent,  true) )
        FiledesAndIndent fid;
        fid.filedes = filedes;
        fid.indent = indent;
        for (int i = 0; i < getProto()->getNumNodes(); i++) 
            getProto()->getNode(i)->doWithBranch(writeRoutesPerNode, &fid);
        return 0;
    }    
#endif
    return Node::write(filedes, indent); 
}

int         
NodePROTO::writeXml(int filedes, int indent, int containerField, bool avoidUse)
{ 
#if HAVE_NO_PROTOS_X3DOM
    if ((m_scene->getWriteFlags() & X3DOM) && (getProto()->getNumNodes() > 0)) {
        for (int k = 0; k < getProto()->getNumNodes(); k++)
            RET_ONERROR( getProto()->getNode(k)->writeXml(filedes, indent, 
                                                          containerField, true)
                       )

        Node *node = this;
        Proto *fromProto = node->getProto();

        for (int j = 0; j < fromProto->getNumEventOuts(); j++)
            for (int n = 0; n < fromProto->getEventOut(j)->getNumIs(); n++)
                if (fromProto->getEventOut(j)->getIsElementType(n) == 
                    EL_EVENT_OUT) {
                    const char *srcName =
                        fromProto->getEventOut(j)->getIsNode(n)->getName();
                    int field = fromProto->getEventOut(j)->getIsField(n);
                    EventOut *srcEvent = fromProto->getEventOut(j)->
                                             getIsNode(n)->
                                             getProto()->getEventOut(field);

                    SocketList::Iterator *i = NULL;

                    for (i = node->getOutput(j).first(); i != NULL; 
                         i = i->next()) {
                         Node *dstNode = i->item().getNode();
                         int dstEventOut = i->item().getField();
                         EventOut *dstEvent = 
                             dstNode->getProto()->getEventOut(dstEventOut);
                        indentf(filedes, 3 * TheApp->GetIndent());
                        RET_ONERROR( mywritef(filedes, 
                            "<ROUTE fromNode='%s' fromField='%s' ",
                            srcName, (const char *)srcEvent->getName(true)) )
                        RET_ONERROR( mywritef(filedes, 
                            "toNode='%s' toField='%s'></ROUTE>\n",
                            (const char *)dstNode->getName(), 
                            (const char *)dstEvent->getName(true)) )
                    }
                }

        FiledesAndIndent fid;
        fid.filedes = filedes;
        fid.indent = indent;
        for (int i = 0; i < getProto()->getNumNodes(); i++) 
            getProto()->getNode(i)->doWithBranch(writeRoutesPerNode, &fid);
        return 0;
    }    
#endif
    return Node::writeXml(filedes, indent); 
}

void NodePROTO::appendToIndexedNodes(Node *node)
{
    if (node != NULL)
        m_indexedNodes.append(node);
}

static MyArray<RouteInfo> routeInfo;

static bool fixEventOuts(Node *node, void *data)
{
    Node *compareNode = (Node *)data;
    if (compareNode == NULL)
        return true;
    if (node == NULL)
        return true;
    if ((node != compareNode) &&
        compareNode->getId() == node->getId()) {
        Proto *proto = node->getProto();
        for (int j = 0; j < proto->getNumEventIns(); j++) {
            EventIn *evIn = proto->getEventIn(j);
            int eventIn = j;
            SocketList::Iterator *i;
            int counter = 0;
            int listLength = node->getInput(j).size();
            static Node *sNode = NULL;
            static int sField = -1;
            for (i = node->getInput(j).first(); i != NULL;i = i->next()) {
                // list increases in loop
                if (counter++ > listLength)
                    break;
                RouteSocket s = i->item();                
                if ((s.getNode() != sNode) || (sField != s.getField())) {
                    node->getScene()->addRoute(s.getNode(), s.getField(), 
                                               compareNode, j);
                    routeInfo.append(RouteInfo(s.getNode(), s.getField(), 
                                               compareNode, j));
                }
            }
        }
    }
    return true;
}

static bool buildSetNodePROTO(Node *node, void *data)
{
    NodePROTO *self = (NodePROTO *)data;
    if (self == NULL)
        return true;
    node->setNodePROTO(self);
    return true;
}

static bool buildNodeIndexInBranch(Node *node, void *data)
{
    NodePROTO *self = (NodePROTO *)data;
    if (self == NULL)
        return true;

    for (int i = 0; i < self->getProto()->getNumNodes(); i++)
        self->getProto()->getNode(i)->doWithBranch(fixEventOuts, node, false);

//    node->setVariableName(node->getScene()->generateVariableName(node));
    self->appendToIndexedNodes(node);
    node->ref();
    node->setNodePROTO(self);
    return true;
}

static bool getIds(Node *node, void *data)
{
    MyArray<long> *ids = (MyArray<long> *)data;
    if (node) 
        (*ids).append(node->getId());
    return true;
}

static int idCounter = 0;

static bool setIds(Node *node, void *data)
{
    MyArray<long> *ids = (MyArray<long> *)data;
    if (node) {
        node->setId((*ids)[idCounter++]);
    }
    return true;
}

static bool getNames(Node *node, void *data)
{
    MyArray<const char *> *names = (MyArray<const char *> *)data;
    if (node) 
        (*names).append(node->getVariableName());
    return true;
}

static int nameCounter = 0;

static bool setNames(Node *node, void *data)
{
    MyArray<const char *> *names = (MyArray<const char *> *)data;
    if (node) {
        node->setVariableName((*names)[nameCounter++]);
    }
    return true;
}

void
NodePROTO::createPROTO(bool bcopy)
{
    bool x3d = m_scene->isX3d();
 
    m_isHumanoid = false;
    if (bcopy) {
        m_nodes.resize(0);
        for (int i = 0; i < m_proto->getNumNodes(); i++) {
            if (m_proto->getNode(i)) {
                long id = m_proto->getNode(i)->getId();
                idCounter = 0;
                MyArray<long> ids;
                m_proto->getNode(i)->doWithBranch(getIds, &ids, false);

                const char *name = m_proto->getNode(i)->getVariableName();
                nameCounter = 0;
                MyArray<const char *> names;
                m_proto->getNode(i)->doWithBranch(getNames, &names, false);

                m_proto->getNode(i)->doWithBranch(buildSetNodePROTO, this, 
                                                  false);
                m_nodes[i] = m_proto->getNode(i)->copy();   
                m_nodes[i]->setId(id);                
                m_nodes[i]->doWithBranch(setIds, &ids, false);
                m_nodes[i]->setVariableName(name);
                m_nodes[i]->doWithBranch(setNames, &names, false);
                m_proto->getNode(i)->copyOutputsTo(m_nodes[i]);
                m_proto->getNode(i)->copyInputsTo(m_nodes[i]);
                m_proto->getNode(i)->copyChildrenTo(m_nodes[i], true);
            }
        }
        for (int i = 0; i < routeInfo.size(); i++)
            m_scene->deleteRoute(routeInfo[i].src, routeInfo[i].eventOut,
                                 routeInfo[i].dest, routeInfo[i].eventIn);
        routeInfo.resize(0);
        m_indexedNodes.resize(0);
        for (int i = 0; i < m_proto->getNumNodes(); i++)
            m_nodes[i]->doWithBranch(buildNodeIndexInBranch, this, false);
        for (long i = 0; i < m_indexedNodes.size(); i++)
            if (m_indexedNodes[i])
               m_indexedNodes[i]->setScene(m_scene);
    }
    for (int i = 0; i < m_proto->getNumExposedFields(); i++) {
        ExposedField *field = m_proto->getExposedField(i);
        if (field && field->getFlags() & FF_IS)
            for (int j = 0; j < field->getNumIs(); j++) {
                Node *isNode = field->getIsNode(j);
                FieldValue *value = field->getValue()->copy();
                if (value && isNode) {
                    int isField = field->getIsField(j);
                    isNode->setField(isField, value);
                }
            }
    }
    for (int i = 0; i < m_proto->getNumFields(); i++) {
        Field *field = m_proto->getField(i);
        if (field && field->getFlags() & FF_IS)
            for (int j = 0; j < field->getNumIs(); j++) {
                Node *isNode = field->getIsNode(j);
                FieldValue *value = field->getDefault(x3d)->copy();
                if (value && isNode) {
                    int isField = field->getIsField(j);
                    isNode->setField(isField, value);
                }
            }
    }
    m_jointRotationField = -1;
    bool maybeJoint = false;
    if (strcmp(m_proto->getName(x3d), "Joint") == 0)
        if (m_nodes.size() > 0)
            if (m_nodes[0] != NULL)
                if (m_nodes[0]->getType() == VRML_TRANSFORM)
                    maybeJoint = true;
    if (strcmp(m_proto->getName(x3d), "Humanoid") == 0)
        if (m_nodes.size() > 0)
            if (m_nodes[0] != NULL)
                if (m_nodes[0]->getType() == VRML_TRANSFORM)
                    m_isHumanoid = true;
    for (int i = 0; i < m_proto->getNumFields(); i++) {
        Field *field = m_proto->getField(i);
        if (field && field->getFlags() & FF_IS)
            for (int j = 0; j < field->getNumIs(); j++) {
                Node *isNode = field->getIsNode(j);
                FieldValue *value = field->getDefault(x3d);
                if (value && isNode) {
                    value = value->copy();
                    int isField = field->getIsField(j);
                    isNode->setField(isField, value);
                }
            }
        // search for Joint.rotation field for H-Anim handle
        if (maybeJoint)
            if (strcmp(field->getName(false), "rotation") == 0)
                if (field && field->getType() == SFROTATION) {
                    m_jointRotationField = i;         
                    m_scene->setHasJoints();
                }                      
    }    
}

NodePROTO::NodePROTO(Scene *scene, Proto *proto)
  : DynamicFieldsNode(scene, proto)
{
    createPROTO();
    handleIs();
}

bool
NodePROTO::isEXTERNPROTO(void)
{
    if (isPROTO())
        if (m_proto->isExternProto())
            return true;
    return false;
}

static bool updateNode(Node *node, void *data)
{
    node->update();
    return true;
}


void
NodePROTO::update()
{
    for (int i = 0; i < m_proto->getNumNodes(); i++) {
        if (m_proto->getNode(i) != NULL)
            m_proto->getNode(i)->doWithBranch(updateNode, NULL, true, false, 
                                              false, false);
    }
    Node::update();
}

static bool reInitNode(Node *node, void *data)
{
    node->reInit();
    return true;
}


void
NodePROTO::reInit(void)
{
    bool x3d = m_scene->isX3d();
    for (int i = 0; i < m_proto->getNumNodes(); i++) {
        if (m_proto->getNode(i) != NULL)
            m_proto->getNode(i)->doWithBranch(reInitNode, NULL);
    }
    this->updateDynamicFields();
    for (int i = 0; i < m_numFields; i++) { 
         FieldValue *fieldValue = getField(i);
         if (fieldValue == NULL)
             continue;
         if (fieldValue->isDefaultValue())
             fieldValue = m_proto->getField(i)->getDefault(x3d);
         fieldValue = fieldValue->copy();
         setField(i, fieldValue);
    }
    Node::reInit();
}

Node *
NodePROTO::getIsNode(int nodeIndex)
{
    if (nodeIndex >=(int)m_indexedNodes.size()) {
        if (isLoaded() && !getProto()->isLoading())
            swDebugf("Bug: Internal parse error: nodeIndex >= %s",
                     "m_indexedNodes.size()\n");
        return NULL;
    }
    return m_indexedNodes[nodeIndex];
}


void
NodePROTO::preDraw()
{
    m_scene->pushUnitAngle();
    m_scene->setUnitAngle(getProto()->getUnitAngle());
    double unitLength = getProto()->getUnitLength();
    if (unitLength > 0)
        glScaled(1 / unitLength, 1 / unitLength, 1 / unitLength);

    if (m_indexedNodes.size() > 0)
        if (m_indexedNodes[0] != NULL)
            m_indexedNodes[0]->preDraw();
    for (long i = 1; i < m_indexedNodes.size(); i++) {
        if (m_indexedNodes[i]->getType() == VRML_TIME_SENSOR)
            m_indexedNodes[i]->preDraw();
    }

    if (unitLength > 0)
        glScaled(unitLength, unitLength, unitLength);
    m_scene->popUnitAngle();
}

void
NodePROTO::draw()
{
    m_scene->pushUnitAngle();
    m_scene->setUnitAngle(getProto()->getUnitAngle());
    double unitLength = getProto()->getUnitLength();
    if (unitLength > 0)
        glScaled(1 / unitLength, 1 / unitLength, 1 / unitLength);

    if (m_indexedNodes.size() > 0)
        if (m_indexedNodes[0] != NULL)
            m_indexedNodes[0]->draw();

    if (unitLength > 0)
        glScaled(unitLength, unitLength, unitLength);
    m_scene->popUnitAngle();
}
  
void
NodePROTO::draw(int pass)
{
    m_scene->pushUnitAngle();
    m_scene->setUnitAngle(getProto()->getUnitAngle());
    double unitLength = getProto()->getUnitLength();
    if (unitLength > 0)
        glScaled(1 / unitLength, 1 / unitLength, 1 / unitLength);

    if (m_indexedNodes.size() > 0)
        if (m_indexedNodes[0] != NULL)
            m_indexedNodes[0]->draw(pass);

    if (unitLength > 0)
        glScaled(unitLength, unitLength, unitLength);
    m_scene->popUnitAngle();
}   

int
NodePROTO::getType() const 
{ 
    if (m_scene)
        return m_scene->getProtoType(getProto());
    else if (m_indexedNodes.size() > 0)
        if (m_indexedNodes[0] != NULL)
            return m_indexedNodes[0]->getType();
    return -1; 
}

int 
NodePROTO::getNodeClass() const 
{ 
    if (m_indexedNodes.size() > 0)
        if (m_indexedNodes[0] != NULL)
            return m_indexedNodes[0]->getNodeClass() | PROTO_NODE;
    return Node::getNodeClass() | PROTO_NODE;
}

FieldValue *
NodePROTO::getField(int index) const
{
    return Node::getField(index);
}

void
NodePROTO::setField(int index, FieldValue *value, int cf)
{
    value->removeIsDefaultValue();
    Node::setField(index, value, cf);
}

void
NodePROTO::receiveProtoEvent(int eventOut, double timestamp, FieldValue *value)
{
    for (int j = 0; j < m_proto->getNumEventOuts(); j++) {
        SocketList::Iterator *i;
        for (i = m_outputs[j].first(); 
             i != NULL; i = i->next()) {
            RouteSocket s = i->item();
            s.getNode()->receiveEvent(s.getField(), swGetCurrentTime(),value);
        }
    }
}

void
NodePROTO::sendEvent(int eventOut, double timestamp, FieldValue *value)
{
    Node::sendEvent(eventOut, timestamp, value);
}

void
NodePROTO::receiveEvent(int eventIn, double timestamp, FieldValue *value)
{
    // handle IS
    EventIn *evIn = m_proto->getEventIn(eventIn);
    if ((m_isEventIns.size() > 0) && (m_isEventIns[eventIn] != NULL))
        evIn = m_isEventIns[eventIn];
    if (evIn && (evIn->getFlags() & FF_IS)) {
        if (getType() != VRML_SCRIPT) {
            for (int i = 0; i < evIn->getNumIs(); i++) {
                Node *isNode = evIn->getIsNode(i);
                isNode->receiveEvent(evIn->getIsField(i), timestamp, value);
            }
        }
    }
    NodeData::receiveEvent(eventIn, timestamp, value);
}

void        
NodePROTO::drawHandles(void)
{
    if (m_nodes.size() > 0)
        if (m_nodes[0] != NULL) {
            if (isJoint())
                ((NodeTransform *)m_nodes[0])->drawRotationHandles(0.1f);
            else
                m_nodes[0]->drawHandles();
        }
}

void        
NodePROTO::transform()
{
    if (m_nodes.size() > 0)
        if (m_nodes[0] != NULL)
            m_nodes[0]->transform();
}

int         
NodePROTO::countPolygons(void)
{
    if (m_nodes.size() > 0)
        if (m_nodes[0] != NULL)
            return m_nodes[0]->countPolygons();
    return 0;
}

int         
NodePROTO::countPrimitives(void)
{
    if (m_nodes.size() > 0)
        if (m_nodes[0] != NULL)
            return m_nodes[0]->countPrimitives();
    return 0;
}

int         
NodePROTO::countPolygons1Sided(void)
{
    if (m_nodes.size() > 0)
        if (m_nodes[0] != NULL)
            return m_nodes[0]->countPolygons1Sided();
    return 0;
}

int         
NodePROTO::countPolygons2Sided(void)
{
    if (m_nodes.size() > 0)
        if (m_nodes[0] != NULL)
            return m_nodes[0]->countPolygons2Sided();
    return 0;
}


Vec3f       
NodePROTO::getHandle(int handle, int *constraint, int *field)
{
    if (isJoint())
        if (m_nodes.size() > 0)
            if (m_nodes[0] != NULL)
                return m_nodes[0]->getHandle(handle, constraint, field);
    *field = getProto()->metadata_Field();
    return Vec3f(0.0f, 0.0f, 0.0f);
}

void
NodePROTO::setHandle(int handle, const Vec3f &v)
{
    if (isJoint())
        if (m_nodes.size() > 0)
            if (m_nodes[0] != NULL) {
                NodeTransform *transform = (NodeTransform *)m_nodes[0];
                transform->setHandle(handle, v);
                m_scene->setField(this, m_jointRotationField, 
                                new SFRotation(transform->rotation()->getQuat()));
            }
}

static bool getProfileInBranch(Node *node, void *data)
{
    int *profile = (int *)data;
    
    if (node != NULL)
        if (node->getProfile() > *profile)
            *profile = node->getProfile();
    if (*profile == PROFILE_FULL)
        return false;
    return true;
}

int
NodePROTO::getProfile(void) const
{
    int profile = PROFILE_IMMERSIVE;
    for (long i = 0; i < m_nodes.size(); i++)
         if (profile != PROFILE_FULL)
             m_nodes[i]->doWithBranch(getProfileInBranch, &profile);
    return profile;
}

void                
NodePROTO::getComponentsInBranch(DoWithNodeCallback callback, void *data) 
{
    for (long i = 0; i < m_nodes.size(); i++)
        m_nodes[i]->doWithBranch(callback, data);
}

bool
NodePROTO::canWriteAc3d()
{
   if (m_nodes.size() > 0)
       return m_nodes.canWriteAc3d();   
   return false;
}

int
NodePROTO::writeAc3d(int filedes, int indent)
{
   if (m_nodes.size() > 0)
       return m_nodes.writeAc3d(filedes, indent);   
   return 0;
}

void
NodePROTO::handleAc3dMaterial(ac3dMaterialCallback callback, Scene* scene)
{
   for (long i = 0; i < m_nodes.size(); i++)
       m_nodes.get(i)->handleAc3dMaterial(callback, scene);
}

int
NodePROTO::writeRib(int filedes, int indent)
{
   if (m_nodes.size() > 0)
       return m_nodes.writeRib(filedes, indent);   
   return 0;
}

bool 
NodePROTO::canWriteCattGeo()
{
   if (m_nodes.size() > 0)
       return m_nodes.canWriteCattGeo();   
   return false;
}

int         
NodePROTO::writeCattGeo(int filedes, int indent) 
{
   if (m_nodes.size() > 0)
       return m_nodes.writeCattGeo(this, filedes, indent);   
   return 0;
}

Node *
NodePROTO::getProtoRoot(void)
{ 
    if (m_indexedNodes.size() <= 0)
        return NULL;
    return m_indexedNodes[0];
}

int getMaskedNodeClass(int nodeClass)
{
    const unsigned int mask = ~(
                                NOT_SELF_NODE |
                                TEXTURE_NODE |
                                TEXTURE_COORDINATE_NODE |
                                TEXTURE_TRANSFORM_NODE |
                                GEOMETRY_NODE |
                                COLOR_NODE |
                                CHILD_NODE |
                                GROUPING_NODE |
                                URL_NODE |
                                PROTO_NODE
                               );

    return nodeClass & mask;
}

bool matchNodeClass(int nodeType, int childType, bool repeat)
{
    int typelist[] = {
        NOT_SELF_NODE,
        TEXTURE_NODE,
        TEXTURE_COORDINATE_NODE,
        TEXTURE_TRANSFORM_NODE,
        GEOMETRY_NODE,
        COLOR_NODE,
        CHILD_NODE,
        GROUPING_NODE,
        URL_NODE,
        PROTO_NODE
    };
    for (unsigned int i = 0; i < (sizeof(typelist) / sizeof(int)); i++)
        if ((nodeType & typelist[i]) && 
            (childType & typelist[i]))
            return true;

    if (getMaskedNodeClass(nodeType) == childType)
        return true;

    switch (getMaskedNodeClass(nodeType)) {
      // combined types
      case AUDIO_CLIP_OR_MOVIE_TEXTURE_NODE:
        switch (getMaskedNodeClass(childType)) { 
          case AUDIO_CLIP_NODE:
            return true;
          case MOVIE_TEXTURE_NODE:
            return true;
        }
        break;
      case BODY_COLLIDABLE_OR_BODY_COLLISION_SPACE_NODE:
        switch (getMaskedNodeClass(childType)) { 
          case BODY_COLLIDABLE_NODE:
            return true;
          case BODY_COLLISION_SPACE_NODE:
            return true;
        }
        break;
      case TEXTURE_OR_TEXTURE_3D_NODE:
        switch (getMaskedNodeClass(childType)) { 
          case TEXTURE_NODE:
            return true;
          case TEXTURE_3D_NODE:
            return true;
        }
        break;
      case NURBS_TEXTURE_COORDINATE_OR_TEXTURE_COORDINATE_NODE:
        switch (getMaskedNodeClass(childType)) { 
          case NURBS_TEXTURE_COORDINATE_NODE:
            return true;
          case TEXTURE_COORDINATE_NODE:
            return true;
        }
        break;
      case SHAPE_OR_LOD_NODE:
        switch (getMaskedNodeClass(childType)) { 
          case SHAPE_NODE:
            return true;
          case LOD_NODE:
            return true;
        }
        break;
      case SHAPE_OR_INLINE_NODE:
        switch (getMaskedNodeClass(childType)) { 
          case SHAPE_NODE:
            return true;
          case INLINE_NODE:
            return true;
        }
        break;
      case SPOTLIGHT_OR_DIRECTIONALLIGHT_OR_VIEWPOINT_NODE:
        switch (getMaskedNodeClass(childType)) {
          case VRML_SPOT_LIGHT:
            return true;
          case VRML_DIRECTIONAL_LIGHT:
            return true;
          case VIEWPOINT_NODE:
            return true;
          case VIEWPOINT_GROUP_NODE:
            return true;
        }
        break;
      case VIEWPOINT_OR_VIEWPOINT_GROUP_NODE:
        switch (getMaskedNodeClass(childType)) {
          case VIEWPOINT_NODE:
            return true;
          case VIEWPOINT_GROUP_NODE:
            return true;
        }
        break;
      case NURBS_CURVE_2D_OR_CONTOUR_POLYLINE_2D_NODE:
        switch (getMaskedNodeClass(childType)) {
          case VRML_NURBS_CURVE_2D:
            return true;
          case X3D_CONTOUR_POLYLINE_2D:
            return true;
        }
        break;
      case VOLUME_RENDER_STYLE_NODE:
        switch (getMaskedNodeClass(childType)) {
          case COMPOSABLE_VOLUME_RENDER_STYLE_NODE:
            return true;
          case VOLUME_RENDER_STYLE_NODE:
            return true;
        }
        break;
      case GENERATED_TEXTURE_COORDINATE_NODE:
        switch (getMaskedNodeClass(childType)) {
          case X3D_TEXTURE_COORDINATE_GENERATOR:
            return true;
          case KAMBI_MULTI_GENERATED_TEXTURE_COORDINATE:
            return true;
          case KAMBI_PROJECTED_TEXTURE_COORDINATE:
            return true;
        }
        break;
    }
    if (repeat)
        return matchNodeClass(childType, nodeType, false);
    return false;
}

bool
Proto::hasNumbers4kids(void)
{
    if (m_numbers4KidsInit == false) {
        m_numbers4KidsInit = true;
        m_numbers4Kids = false;
        for (long i = 0; i < m_fields.size(); i++)
           if (m_fields[i]->getFlags() & FF_4KIDS) {
               m_numbers4Kids = true;
               break;
           }
    }
    return m_numbers4Kids;
}

void
Proto::removeNode(int i)
{
    m_protoNodes[i]->unref();
    removeFromIs(m_protoNodes[i]);
    m_protoNodes.remove(i);
}

void                  
Proto::convert2X3d(void) 
{
    for (long i = 0; i < m_protoNodes.size(); i++)
        m_protoNodes[i]->convert2X3d();
}

void
Proto::convert2Vrml(void) 
{
    for (long i = 0; i < m_protoNodes.size(); i++)
        m_protoNodes[i]->convert2X3d();
}

