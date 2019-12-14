/*
 * InputDeviceSelectionDialog.h
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

class InputDeviceSelectionDialog : public Dialog
{
public:
    InputDeviceSelectionDialog(SWND parent);
    void SetDefault(void);
    InputDevice *getInputDevice(void) { return inputDevice; }
#ifdef HAVE_AFLOCK
    AflockDevice *getAflockDevice(void) { return aflockDevice; }
#endif 
    MyString getDeviceOption(void) { return deviceOption; }
    int getLastCommand() { return lastCommand; }
protected:
    virtual void OnCommand(void *id);
    virtual void SaveData(void);
    virtual bool Validate();

    void LoadComboBoxDataOptions(void);
    void LoadComboBoxExistingDevice(void);
    void DeleteInputDevice(void);
protected:
    InputDevice *inputDevice;
#ifdef HAVE_AFLOCK
    AflockDevice *aflockDevice;
#endif
    MyString deviceOption;
    int lastCommand;    

    StringArray device_options;
    MyArray<InputDevice *> existing_devices;
public:
    SWND parent;
    // define variables for the selectable data as SWND widget/control
    SWND combo_device_options;
    SWND combo_existing_devices;
    SWND button_delete_device;
};
