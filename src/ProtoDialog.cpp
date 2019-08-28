/*
 * ProtoDialog.cpp
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
#include "ProtoDialog.h"
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
    "Vec3f"
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
# define YMAX 290
#endif
#define SCROLL_SIZE 5

static int ymax;

static void
ProtoDialogRedraw(void* data)
{
    SDC dc = swCreateDC(canvas);
    swSetFGColor(dc, 0xffffff);
    ProtoDialog* dialog = (ProtoDialog*) data;
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
ProtoDialogExposeCallback(void *data, int x, int y, int width, int height)
{
    ProtoDialogRedraw(data);
}

static void
ProtoDialogEnterCallback(void *data, int command)
{
    ProtoDialogRedraw(data);
}

static int fontHeight;
static long selectedText;

static void
ProtoDialogKeyCallback(void *data, int key, int value, int x, int y, 
                        int modifiers)
{
    // Left mouse button down
    if (key == SW_MOUSE1)
       if (value) {
           selectedText = (y  - Y_SPACING) / (fontHeight + Y_SPACING);
           ProtoDialogRedraw(data);
       }
}

ProtoDialog::ProtoDialog(SWND parent, Proto* oldProto)
  : Dialog(parent, IDD_PROTO)
{
    parent_window = parent;
    m_proto = oldProto;
    m_okFlag = false;
    LoadData();
    m_canClick = true;
    fontHeight = swGetFontHeight(swGetDefaultFont());
    selectedText = -1;
    accountYmax();
}

ProtoDialog::~ProtoDialog()
{
    swDestroyWindow(canvas);
    swDestroyWindow(scroll); 
}

void
ProtoDialog::OnCommand(void *vid)
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
        } else if (id == IDOK) {
            addEvent(true);
            m_okFlag = true;
            if (Validate())
                swEndDialog(IDOK);
        }
    }
}

void
ProtoDialog::eraseWidgets(void)
{
    swComboBoxSetSelection(swGetDialogItem(m_dlg, IDC_EVENT_OR_FIELD),0);
    swComboBoxSetSelection(swGetDialogItem(m_dlg, IDC_SF_OR_MF),0);
    swComboBoxSetSelection(swGetDialogItem(m_dlg, IDC_DATATYPE),0);
    swSetText(swGetDialogItem(m_dlg, IDC_EVENT_NAME), "");
}

void
ProtoDialog::setWidgets(const char *eventString, int typeEnum, 
                         const char *eventName)
{
    char sfOrMf[3] = { '\0' };
    const char *type = typeEnumToString(typeEnum);
    mystrncpy_danger(sfOrMf, type, 2);
    const char *dataType = (strlen(type) > 2) ? &type[2] : "";

    for (unsigned int i=0;i<(sizeof(eventOrField)/sizeof(const char*));i++)
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
ProtoDialog::addEvent(bool onlyTry)
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

    Proto* proto=m_proto;
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

    if (alreadyUsed) {
        if (onlyTry) 
            return;
        TheApp->MessageBox(IDS_NAME_IN_USE, eventName);
        return;
    }

    bool invalidChars = false;
    if (isalpha(eventName[0])) {
       for (unsigned int i=1; i < strlen(eventName); i++)
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
        m_proto->addField(typeEnum, eventName);
    } else if (strcmp(eventOrField[event],"exposedField") == 0) {
        m_proto->addExposedField(typeEnum, eventName);
    } else if (strcmp(eventOrField[event],"eventIn") == 0) { 
        m_proto->addEventIn(typeEnum, eventName); 
    } else if (strcmp(eventOrField[event],"eventOut") == 0) {
        m_proto->addEventOut(typeEnum, eventName); 
    } else
        assert(0);

    selectedText = -1;
    ProtoUpdate hint(proto);
    proto->getScene()->UpdateViews(NULL, UPDATE_PROTO, &hint);
    proto->getScene()->updateNodePROTOs(proto);

    swInvalidateWindow(canvas);
    eraseWidgets();
}

bool
ProtoDialog::deleteEvent()
{
    Proto* proto = m_proto;
    InterfaceArray *interfaceData = m_proto->getInterfaceData(true);
    if (proto==NULL)
        return false;
    if (selectedText == -1)
        return false;
    for (long i = 0 ; i < (long)interfaceData->size() ; i++) 
        if (i == selectedText) {
            int ind = interfaceData->get(i)->m_elementIndex;
            switch (interfaceData->get(i)->m_elementEnum) {
              case EL_EXPOSED_FIELD:
                if (m_proto->getExposedField(ind)->getNumIs() > 0) {
                    TheApp->MessageBoxId(IDS_CUT_IS_FIRST);
                    return false;
                }                     
                proto->deleteExposedField(ind);
                break;
              case EL_FIELD:
                if (m_proto->getField(ind)->getNumIs() > 0) {
                    TheApp->MessageBoxId(IDS_CUT_IS_FIRST);
                    return false;
                }                     
                proto->deleteField(ind);
                break;
              case EL_EVENT_IN:
                if (m_proto->getEventIn(ind)->getNumIs() > 0) {
                    TheApp->MessageBoxId(IDS_CUT_IS_FIRST);
                    return false;
                }                     
                proto->deleteEventIn(ind);
                break;
              case EL_EVENT_OUT:
                if (m_proto->getEventOut(ind)->getNumIs() > 0) {
                    TheApp->MessageBoxId(IDS_CUT_IS_FIRST);
                    return false;
                }
                proto->deleteEventOut(ind);
                break;
            }
        }

    ProtoUpdate hint(proto);
    proto->getScene()->UpdateViews(NULL, UPDATE_PROTO, &hint);
    proto->getScene()->updateNodePROTOs(proto);
  
    swInvalidateWindow(canvas);
    return true;
}

void
ProtoDialog::copyEvent(bool andDelete)
{
    InterfaceArray *interfaceData = m_proto->getInterfaceData(true);
    Proto* proto=m_proto;
    if (proto==NULL)
        return;
    if (selectedText == -1)
        return;
    const char *eventOrField = "";
    int typeEnum = 0;
    const char *name = "";
    bool lineFound = false;
    for (long i = 0 ; i < (long)interfaceData->size() ; i++) 
        if (i == selectedText) {
            lineFound = true;
            int ind = interfaceData->get(i)->m_elementIndex;
            switch (interfaceData->get(i)->m_elementEnum) {
              case EL_FIELD:
                eventOrField = "field";
                typeEnum = proto->getField(ind)->getType();
                name = proto->getField(ind)->getName(false);
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
     if (andDelete && lineFound) {
         if (deleteEvent())
             setWidgets(eventOrField, typeEnum, name);
         else
             eraseWidgets();
     } else if (lineFound)
         setWidgets(eventOrField, typeEnum, name);
}

void
ProtoDialog::changeEvent()
{
     copyEvent(true);
}
bool
ProtoDialog::Validate()
{
    return true;
}

void
ProtoDialog::LoadData()
{
    SWND comboEventOrField = swGetDialogItem(m_dlg, IDC_EVENT_OR_FIELD);
    swComboBoxDeleteAll(comboEventOrField);
    for (unsigned int i=0;i<(sizeof(eventOrField)/sizeof(const char*));i++)
        swComboBoxAppendItem(comboEventOrField, eventOrField[i]);

    SWND comboSfOrMf = swGetDialogItem(m_dlg, IDC_SF_OR_MF);
    swComboBoxDeleteAll(comboSfOrMf);
    for (unsigned int i=0;i<(sizeof(SfOrMf)/sizeof(const char*));i++)
        swComboBoxAppendItem(comboSfOrMf, SfOrMf[i]);

    SWND comboDataType = swGetDialogItem(m_dlg, IDC_DATATYPE);
    swComboBoxDeleteAll(comboDataType);
    for (unsigned int i=0;i<(sizeof(DataType)/sizeof(const char*));i++)
        swComboBoxAppendItem(comboDataType, DataType[i]);

    scroll = swCreateScrolledWindow(XPOS, YPOS, XMAX-XPOS, YMAX-YPOS, m_dlg);
    canvas = swCreateCanvas("protoDialog", 0, 0, XMAX, YMAX, scroll);
    swScrolledWindowSetChild(scroll, canvas);
    swSetScrollSizes(scroll, XMAX-XPOS-SCROLL_SIZE, YMAX-YPOS-SCROLL_SIZE);

    swSetClientData(canvas, this);
    swSetEnterCallback(canvas, ProtoDialogEnterCallback);
    swSetKeyCallback(canvas, ProtoDialogKeyCallback);
    swSetExposeCallback(canvas, ProtoDialogExposeCallback);
    swInvalidateWindow(canvas);

    swShowWindow(canvas);
}

void 
ProtoDialog::accountYmax()
{
    InterfaceArray *interfaceData = m_proto->getInterfaceData(true);
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
ProtoDialog::drawInterface()
{
    InterfaceArray *interfaceData = m_proto->getInterfaceData(true);
    SDC dc = swCreateDC(canvas);
    if (!m_canClick) {
        swSetFGColor(dc, SW_RED(0xFFFFFF));
        swDrawText(dc, X_SPACING, Y_SPACING + fontHeight, "editor is running");
        return;
    }
    Proto* proto = m_proto;
    if (proto == NULL)
        return;
    int y = 0;
    for (long i = 0 ; i < (long)interfaceData->size() ; i++) {
        MyString text = "";
        int ind = interfaceData->get(i)->m_elementIndex;
        switch (interfaceData->get(i)->m_elementEnum) {
          case EL_FIELD:
            text += "field ";
            text += typeEnumToString(proto->getField(ind)->getType());
            text += " ";
            text += proto->getField(ind)->getName(false);
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


