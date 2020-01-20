/*
 * ScriptDialog.h
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

#pragma once

#include "Dialog.h"
#include "NodeScript.h"
#include "Scene.h"
#include "Array.h"
#include "swt.h"

class ObjectEdit;

#include "swttypedef.h"

class ScriptDialog : public Dialog {
public:
                        ScriptDialog(SWND parent, Node* oldNode);
    virtual            ~ScriptDialog();

    void                OnEditorReadyCallback(void);

    void                drawInterface(void);
    void                accountYmax();

protected:
    void                LoadData();
    virtual void        addEvent(bool onlyTry = false);
    virtual void        copyEvent(bool andDelete = false);
    virtual void        changeEvent();
    virtual bool        deleteEvent();
    virtual bool        Validate();
    void                buildInterfaceData(void);
    void                setWidgets(const char *eventString, int typeEnum, 
                                   const char *eventName, bool x3d);
    void                eraseWidgets(void);
    void                OnCommand(void *id);

    void                startClick(void) { m_canClick = true; }
    void                stopClick(void) { m_canClick = false; }

    void                addInitialize(void);
    void                addShutdown(void);
    void                addEventsProcessed(void);

private:
    DynamicFieldsNode *m_node;
    bool               m_okFlag;
    bool               m_canClick;
    ObjectEdit        *m_objectEdit;                       
    bool               m_initialize;
    bool               m_eventsProcessed;
    bool               m_shutdown;
};

