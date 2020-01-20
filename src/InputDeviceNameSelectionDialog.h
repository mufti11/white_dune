/*
 * InputDeviceNameSelectionDialog.h
 *
 * Copyright (C) 2000 Stephen F. White, 2004 Haining Zhi
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

class InputDevice;
class AflockDevice;

// maximal number of possible arguments for devices (see parseCommandlineArgumentInputDevice)
#define MAX_LEN_COMMANDLINE 128

class InputDeviceNameSelectionDialog : public Dialog
{
public:
    InputDeviceNameSelectionDialog(SWND parent, const char *deviceOption);
    InputDevice *getInputDevice(void) { return inputDevice; }
#ifdef HAVE_AFLOCK
    AflockDevice *getAflockDevice(void) { return aflockDevice; }
#endif
protected:
    virtual void OnCommand(void *id);
    virtual void SaveData(void);
    virtual bool Validate();
    void LoadComboBoxDataNames(void);

protected:
    const char *device_option;

    InputDevice *inputDevice;
    bool aflockFlag;
#ifdef HAVE_AFLOCK
    AflockDevice *aflockDevice;
#endif

    StringArray device_names;
public:
    SWND parent;
    // define variables for the selectable data as SWND widget/control
    SWND combo_device_names;
    SWND edit_device_name;
};

