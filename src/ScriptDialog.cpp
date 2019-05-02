/*
 * ScriptDialog.cpp
 *
 * Copyright (C) 2003 J. "MUFTI" Scheurich
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

#include "stdafx.h"
#include <ctype.h>
#include "ScriptDialog.h"
#include "ScriptEdit.h"
#include "resource.h"
#include "MyString.h"
#include "FieldValue.h"
#include "NodeScript.h"
#include "SFMFTypes.h"
#include "Field.h"
#include "EventIn.h"
#include "EventOut.h"
#include "ExposedField.h"
#include "Element.h"
#include "DuneApp.h"

#define X_SPACING 2
#define Y_SPACING 2

static const char *eventOrField[] = { 
    " ",
    "eventIn",
    "eventOut",
    "field",
    "exposedField"
};

static const char *SfOrMf[] = { 
    " ",
    "SF",
    "MF"
};

static const char *DataType[] = { 
    " ",
    "Bool",
    "Color",
    "Float",
    "Image",
    "Int32",
    "Node",
    "Rotation",
    "String",
    "Time",
    "Vec2f",
    "Vec3f",
    "Double",
    "Vec2d",
    "Vec3d"
};


static SWND canvas;
static SWND scroll;
static SWND parent_window;
 
#define XPOS 56
#define YPOS 55
#ifdef _WIN32
# define XMAX 388
# define YMAX 160
#else
# define XMAX 463
# define YMAX 255
#endif
#define SCROLL_SIZE 5

static int ymax;

static void
ScriptDialogRedraw(void* data)
{
    SDC dc = swCreateDC(canvas);
    swSetFGColor(dc, 0xffffff);
    ScriptDialog* dialog = (ScriptDialog*) data;
    dialog->accountYmax();
    swFillRect(dc, 1, 1, XMAX-2, ymax-2);
    swSetFGColor(dc, 0);
    swDrawLine(dc,      0,      0, XMAX-1,   0);
    swDrawLine(dc,      0, ymax-1, XMAX-1, ymax-1);
    swDrawLine(dc,      0,      0,      0, ymax-1);
    swDrawLine(dc, XMAX-1,      0, XMAX-1, ymax-1);
    dialog->drawInterface();
    swDestroyDC(dc);
}

static void
ScriptDialogExposeCallback(void *data, int x, int y, int width, int height)
{
    ScriptDialogRedraw(data);
}

static void
ScriptDialogEnterCallback(void *data, int command)
{
    ScriptDialogRedraw(data);
}

static int fontHeight;
static int selectedText;

static void
ScriptDialogKeyCallback(void *data, int key, int value, int x, int y, 
                        int modifiers)
{
    // Left mouse button down
    if (key == SW_MOUSE1)
       if (value) {
           selectedText = (y  - Y_SPACING) / (fontHeight + Y_SPACING);
           ScriptDialogRedraw(data);
       }
}

static void
ScriptDialogEditorReadyCallback(void *data)
{
    ScriptDialog *dialog = (ScriptDialog *) data;
    dialog->OnEditorReadyCallback();
}

ScriptDialog::ScriptDialog(SWND parent, Node* oldNode)
  : Dialog(parent, IDD_SCRIPT)
{
    parent_window = parent;
    m_node = (DynamicFieldsNode *)oldNode;
    m_okFlag = false;
    LoadData();
    m_canClick = true;
    m_objectEdit = NULL;
    m_initialize = false;
    m_eventsProcessed = false;
    m_shutdown = false;
    if (m_node->getType() == VRML_SCRIPT) {
        NodeScript *scriptNode = (NodeScript *) oldNode;
        m_initialize = scriptNode->hasInitialize();
        m_eventsProcessed = scriptNode->hasEventsProcessed();
        m_shutdown = scriptNode->hasShutdown();
    }
    fontHeight = swGetFontHeight(swGetDefaultFont());
    selectedText = -1;
    accountYmax();
}

ScriptDialog::~ScriptDialog()
{
    swDestroyWindow(canvas);
    swDestroyWindow(scroll); 
}

void
ScriptDialog::OnCommand(void *vid)
{
    int id = swFromPtr(vid);
    if (m_canClick) {
        if (id == IDC_ADD_EVENT) {
            addEvent();
#ifdef _WIN32
            swSetFocus( swGetDialogItem(m_dlg, IDC_EVENT_OR_FIELD));
#endif
        } else if (id == IDC_DELETE_EVENT)
            deleteEvent();
        else if (id == IDC_COPY_EVENT) {
            addEvent(true);
            copyEvent();
        } else if (id == IDC_CHANGE_EVENT) {
            addEvent(true);
            changeEvent();
        } else if (id == IDC_ADD_INITIALIZE) {
            addInitialize();
        } else if (id == IDC_ADD_SHUTDOWN) {
            addShutdown();
        } else if (id == IDC_ADD_EVENTS_PROCESSED) {
            addEventsProcessed();
        } else if (id == ID_DUNE_FILE_PREVIEW) {
            addEvent(true);
            TheApp->OnFilePreview(m_node->getScene());
        } else if (id == IDC_SCRIPT_EDIT) {
            addEvent(true);
            m_objectEdit = NULL;
            switch (m_node->getType()) {
              case VRML_SCRIPT:
                m_objectEdit = new ScriptEdit((NodeScript *)m_node, m_dlg, 
                                         ScriptDialogEditorReadyCallback, this);
                break;
              case X3D_SHADER_PROGRAM:
              case X3D_PACKAGED_SHADER:
                m_objectEdit = new ShaderEdit(m_node, m_dlg, 
                                         ScriptDialogEditorReadyCallback, this);
                break;
            }
            if (m_objectEdit != NULL) {
                 stopClick();
                m_objectEdit->edit();
            }
        } else if (id == IDOK) {
            addEvent(true);
            m_okFlag = true;
            if (Validate())
                swEndDialog(IDOK);
        }
    }
}

void
ScriptDialog::OnEditorReadyCallback(void)
{
    if (m_objectEdit != NULL)
        delete  m_objectEdit;
    m_objectEdit = NULL;
    m_canClick = true;
    swInvalidateWindow(canvas);
}

void
ScriptDialog::eraseWidgets(void)
{
    swComboBoxSetSelection(swGetDialogItem(m_dlg, IDC_EVENT_OR_FIELD),0);
    swComboBoxSetSelection(swGetDialogItem(m_dlg, IDC_SF_OR_MF),0);
    swComboBoxSetSelection(swGetDialogItem(m_dlg, IDC_DATATYPE),0);
    swSetText(swGetDialogItem(m_dlg, IDC_EVENT_NAME), "");
}

void
ScriptDialog::setWidgets(const char *eventString, int typeEnum, 
                         const char *eventName, bool x3d)
{
    char sfOrMf[3] = { '\0' };
    const char *type = typeEnumToString(typeEnum);
    mystrncpy_danger(sfOrMf, type, 2);
    const char *dataType = (strlen(type) > 2) ? &type[2] : "";

    int sizeEventOrField = sizeof(eventOrField)/sizeof(const char*);
    if (!x3d)
        sizeEventOrField--;

    for (int i=0;i<sizeEventOrField;i++)
        if (strcmp(eventString, eventOrField[i]) == 0)
           swComboBoxSetSelection(swGetDialogItem(m_dlg, IDC_EVENT_OR_FIELD), 
                                                  i);
    for (unsigned int i=0;i<(sizeof(SfOrMf)/sizeof(const char*));i++)
        if (strcmp(sfOrMf, SfOrMf[i]) == 0)
           swComboBoxSetSelection(swGetDialogItem(m_dlg, IDC_SF_OR_MF), i);
    for (unsigned int i=0;i<(sizeof(DataType)/sizeof(const char*));i++)
        if (strcmp(dataType, DataType[i]) == 0)
           swComboBoxSetSelection(swGetDialogItem(m_dlg, IDC_DATATYPE), i);
    swSetText(swGetDialogItem(m_dlg, IDC_EVENT_NAME), eventName);
}


void
ScriptDialog::addEvent(bool onlyTry)
{
    int event = swComboBoxGetSelection(swGetDialogItem(m_dlg,
                                                       IDC_EVENT_OR_FIELD));

    int sfmf = swComboBoxGetSelection(swGetDialogItem(m_dlg, IDC_SF_OR_MF));
    if (sfmf == 0) return;

    if (event == 0) return;
    int type = swComboBoxGetSelection(swGetDialogItem(m_dlg, IDC_DATATYPE));
    if (type == 0) return;
 
    char eventName[1025];
    swGetText(swGetDialogItem(m_dlg, IDC_EVENT_NAME), eventName, 1024);
    if (eventName[0] == (char)0) return;

    Proto* proto=m_node->getProto();
    if (proto==NULL)
        return;

    bool alreadyUsed = false;
    for (int i=0; i < proto->getNumFields(); i++) 
        if (strcmp(eventName, proto->getField(i)->getName(false)) == 0)
            alreadyUsed = true;
    for (int i=0; i < proto->getNumEventIns(); i++) 
        if (strcmp(eventName, proto->getEventIn(i)->getName(false)) == 0)
            alreadyUsed = true;
    for (int i=0; i < proto->getNumEventOuts(); i++)
        if (strcmp(eventName, proto->getEventOut(i)->getName(false)) == 0)
            alreadyUsed = true;
    for (int i=0; i < proto->getNumExposedFields(); i++)
        if (strcmp(eventName, proto->getExposedField(i)->getName(false)) == 0)
            alreadyUsed = true;

    if (alreadyUsed) {
        if (onlyTry) 
            return;
        TheApp->MessageBox(IDS_NAME_IN_USE, eventName);
        return;
    }

    bool invalidChars = false;
    if (isalpha(eventName[0])) {
       for (unsigned int i = 1; i < strlen(eventName); i++)
          if (!isalpha(eventName[i]) && (!isdigit(eventName[i])) &&
               (eventName[i] != '_'))
              invalidChars = true;
    } else
        invalidChars = true;

    if (invalidChars) {
        TheApp->MessageBox(IDS_NAME_WITH_INVALID_CHARACTERS, eventName);
        return;
    }
    

    MyString typeString = SfOrMf[sfmf];
    typeString += DataType[type];

    int typeEnum = typeStringToEnum(typeString);
    if (typeEnum == -1) {
        TheApp->MessageBox(IDS_INVALID_DATATYPE, typeString);
        return;
    }

    if (strcmp(eventOrField[event],"field") == 0) {
        m_node->addField(typeEnum, eventName);
    } else if (strcmp(eventOrField[event],"eventIn") == 0) { 
        m_node->addEventIn(typeEnum, eventName); 
    } else if (strcmp(eventOrField[event],"eventOut") == 0) {
        m_node->addEventOut(typeEnum, eventName); 
    } else if (strcmp(eventOrField[event],"exposedField") == 0) {
        m_node->addExposedField(typeEnum, eventName);
    } else
        assert(0);

    selectedText = -1;
    m_node->update();

    swInvalidateWindow(canvas);
    eraseWidgets();
}

void 
ScriptDialog::addInitialize(void)
{
    if (m_node->getType() == VRML_SCRIPT) {
        ((NodeScript *)m_node)->wantInitialize(); 
        swInvalidateWindow(canvas);
    }
}

void                
ScriptDialog::addShutdown(void)
{
    if (m_node->getType() == VRML_SCRIPT) {
        ((NodeScript *)m_node)->wantShutdown();
        swInvalidateWindow(canvas);
    }
}

void                
ScriptDialog::addEventsProcessed(void)
{
    if (m_node->getType() == VRML_SCRIPT) {
        ((NodeScript *)m_node)->wantEventsProcessed();
        swInvalidateWindow(canvas);
    }
}

bool
ScriptDialog::deleteEvent()
{
    Proto* proto = m_node->getProto();
    InterfaceArray *interfaceData = m_node->getInterfaceData();
    if (proto==NULL)
        return false;
    if (selectedText == -1)
        return false;
    for (int i = 0 ; i < interfaceData->size() ; i++) 
        if (i == selectedText) {
            int ind = interfaceData->get(i)->m_elementIndex;
            switch (interfaceData->get(i)->m_elementEnum) {
              case EL_FIELD:
                proto->getField(ind)->setFlags(FF_DELETED);
                break;
              case EL_EVENT_IN:
                if (m_node->hasRoute(m_node->getInput(ind))) {
                    TheApp->MessageBoxId(IDS_CUT_ROUTES_FIRST);
                    return false;
                }                     
                proto->getEventIn(ind)->setFlags(FF_DELETED);
                break;
              case EL_EVENT_OUT:
                proto->getEventOut(ind)->setFlags(FF_DELETED);
                if (m_node->hasRoute(m_node->getOutput(ind))) {
                    TheApp->MessageBoxId(IDS_CUT_ROUTES_FIRST);
                    return false;
                }
                break;
              case EL_EXPOSED_FIELD:
                proto->getExposedField(ind)->setFlags(FF_DELETED);
                if (m_node->hasRoute(m_node->getOutput(ind))) {
                    TheApp->MessageBoxId(IDS_CUT_ROUTES_FIRST);
                    return false;
                }
                break;
              case EL_INITIALIZE:
                m_initialize = false;
                if (m_node->getType() == VRML_SCRIPT)
                    ((NodeScript *)m_node)->wantInitialize(false);
                break;
              case EL_EVENTS_PROCESSED:
                m_eventsProcessed = false;
                 if (m_node->getType() == VRML_SCRIPT)   
                    ((NodeScript *)m_node)->wantEventsProcessed(false);
                break;
              case EL_SHUTDOWN:
                m_shutdown = false;
                if (m_node->getType() == VRML_SCRIPT)   
                    ((NodeScript *)m_node)->wantShutdown(false);
                break;
            }
        }
  
    m_node->update();

    swInvalidateWindow(canvas);
    return true;
}

void
ScriptDialog::copyEvent(bool andDelete)
{
    InterfaceArray *interfaceData = m_node->getInterfaceData();
    Proto* proto=m_node->getProto();
    if (proto==NULL)
        return;
    if (selectedText == -1)
        return;
    const char *eventOrField = "";
    int typeEnum = 0;
    const char *name = "";
    bool lineFound = false;
    for (int i = 0 ; i < interfaceData->size() ; i++) 
        if (i == selectedText) {
            lineFound = true;
            int ind = interfaceData->get(i)->m_elementIndex;
            switch (interfaceData->get(i)->m_elementEnum) {
              case EL_FIELD:
                if (proto->getField(ind)->getExposedField() == NULL) {
                    eventOrField = "field";
                    typeEnum = proto->getField(ind)->getType();
                    name = proto->getField(ind)->getName(false);
                }
                break;
              case EL_EXPOSED_FIELD:
                eventOrField = "exposedField";
                typeEnum = proto->getExposedField(ind)->getType();
                name = proto->getExposedField(ind)->getName(false);
                break;
              case EL_EVENT_IN:
                eventOrField = "eventIn";
                typeEnum = proto->getEventIn(ind)->getType();
                name = proto->getEventIn(ind)->getName(false);
                break;
              case EL_EVENT_OUT:
                eventOrField = "eventOut";
                typeEnum = proto->getEventOut(ind)->getType();
                name = proto->getEventOut(ind)->getName(false);
                break;
              case EL_INITIALIZE:
                lineFound = false;
                break;
              case EL_EVENTS_PROCESSED:
                lineFound = false;
                break;
              case EL_SHUTDOWN:
                lineFound = false;
                break;
            }
        }
     bool x3d = m_node->getScene()->isX3d();
     if (andDelete && lineFound) {
         if (deleteEvent())
             setWidgets(eventOrField, typeEnum, name, x3d);
         else
             eraseWidgets();
     } else if (lineFound)
         setWidgets(eventOrField, typeEnum, name, x3d);
}

void
ScriptDialog::changeEvent()
{
     copyEvent(true);
}

bool
ScriptDialog::Validate()
{
    return true;
}

void
ScriptDialog::LoadData()
{
    bool x3d = m_node->getScene()->isX3d();
    SWND comboEventOrField = swGetDialogItem(m_dlg, IDC_EVENT_OR_FIELD);
    swComboBoxDeleteAll(comboEventOrField);
    int sizeEventOrField = sizeof(eventOrField)/sizeof(const char*);
    if (!x3d)
        sizeEventOrField--;
    for (int i=0;i<sizeEventOrField;i++)
        swComboBoxAppendItem(comboEventOrField, eventOrField[i]);

    SWND comboSfOrMf = swGetDialogItem(m_dlg, IDC_SF_OR_MF);
    swComboBoxDeleteAll(comboSfOrMf);
    for (unsigned int i = 0; i < (sizeof(SfOrMf)/sizeof(const char*)); i++)
        swComboBoxAppendItem(comboSfOrMf, SfOrMf[i]);

    SWND comboDataType = swGetDialogItem(m_dlg, IDC_DATATYPE);
    swComboBoxDeleteAll(comboDataType);
    for (unsigned int i = 0; i < (sizeof(DataType)/sizeof(const char*)); i++)
        swComboBoxAppendItem(comboDataType, DataType[i]);

    scroll = swCreateScrolledWindow(XPOS, YPOS, XMAX-XPOS, YMAX-YPOS, m_dlg);
    canvas = swCreateCanvas("scriptdialog", 0, 0, XMAX, YMAX, scroll);
    swScrolledWindowSetChild(scroll, canvas);
    swSetScrollSizes(scroll, XMAX + SCROLL_SIZE, YMAX + SCROLL_SIZE);

    swSetClientData(canvas, this);
    swSetEnterCallback(canvas, ScriptDialogEnterCallback);
    swSetKeyCallback(canvas, ScriptDialogKeyCallback);
    swSetExposeCallback(canvas, ScriptDialogExposeCallback);
    swInvalidateWindow(canvas);

    swShowWindow(canvas);
}

void 
ScriptDialog::accountYmax()
{
    InterfaceArray *interfaceData = m_node->getInterfaceData();
    ymax = (interfaceData->size() + 1) * (Y_SPACING + fontHeight) +
           2 * Y_SPACING; 
    if (ymax > YMAX-YPOS-SCROLL_SIZE) {
        swSetScrollSizes(scroll, XMAX-XPOS-SCROLL_SIZE, ymax);
        swSetSize(canvas, XMAX-XPOS-SCROLL_SIZE, ymax);
    } else {
        ymax = YMAX-YPOS-SCROLL_SIZE;
        swSetScrollSizes(scroll, XMAX-XPOS-SCROLL_SIZE, YMAX-YPOS-SCROLL_SIZE);
    }
}   


// display type and name of array of dynamic field's, eventIn's and eventOut's 
// of ScriptNode

void 
ScriptDialog::drawInterface()
{
    InterfaceArray *interfaceData = m_node->getInterfaceData();
    SDC dc = swCreateDC(canvas);
    if (!m_canClick) {
        swSetFGColor(dc, SW_RED(0xFFFFFF));
        swDrawText(dc, X_SPACING, Y_SPACING + fontHeight, "editor is running");
        return;
    }
    Proto* proto = m_node->getProto();
    if (proto == NULL)
        return;
    int y = 0;
    for (int i = 0 ; i < interfaceData->size() ; i++) {
        MyString text = "";
        int ind = interfaceData->get(i)->m_elementIndex;
        switch (interfaceData->get(i)->m_elementEnum) {
          case EL_FIELD:
            if (proto->getField(ind)->getExposedField() == NULL) {
                text += "field ";
                text += typeEnumToString(proto->getField(ind)->getType());
                text += " ";
                text += proto->getField(ind)->getName(false);
            }
            break;
          case EL_EXPOSED_FIELD:
            text += "exposedField ";
            text += typeEnumToString(proto->getExposedField(ind)->getType());
            text += " ";
            text += proto->getExposedField(ind)->getName(false);
            break;
          case EL_EVENT_OUT:
            text += "eventOut ";
            text += typeEnumToString(proto->getEventOut(ind)->getType());
            text += " ";
            text += proto->getEventOut(ind)->getName(false);
            break;
          case EL_EVENT_IN:
            text += "eventIn ";
            text += typeEnumToString(proto->getEventIn(ind)->getType());
            text += " ";
            text += proto->getEventIn(ind)->getName(false);
            break;
          case EL_INITIALIZE:
            text += "initalize";
            break;
          case EL_EVENTS_PROCESSED:
            text += "eventsProcessed";
            break;
          case EL_SHUTDOWN:
            text += "shutdown";
            break;
        }  
        y += Y_SPACING + fontHeight;
        if (selectedText == i)
            swSetFGColor(dc, swGetWindowColor(parent_window, 
                                             SW_COLOR_HIGHLIGHT));
        else
            swSetFGColor(dc, swGetWindowColor(parent_window, SW_COLOR_TEXT));
        swDrawText(dc, X_SPACING, y, (const char*) text);
    }
    swDestroyDC(dc);
}


