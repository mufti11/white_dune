/*
 * InputDeviceSettingsDialog.h
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

#ifndef _INPUT_DEVICE_SETTINGS_DIALOG_H
#define _INPUT_DEVICE_SETTINGS_DIALOG_H

#ifndef _DIALOG_H
#include "Dialog.h"
#endif

class InputDevice;

// maximal number of possible arguments for devices (see parseCommandlineArgumentInputDevice)
#define MAX_LEN_COMMANDLINE 128

class InputDeviceSettingsDialog : public Dialog
{
public:
    InputDeviceSettingsDialog(SWND parent, InputDevice *inputDevice);
    void SetDefault(void);
    InputDevice *getInputDevice() { return inputDevice; }
protected:
    const char* axes_name[9];
    int device_index[256];
    char buf[128];
    int i,j,max_device,max_axes,max_axis,command_sum;
    bool aflock_present;

    void enable_all(bool flag);

    virtual void OnCommand(void *id);
    virtual void LoadData(void);
    virtual void SaveData(void);
    virtual bool Validate();
    void showNumbers();
    void LoadComboBoxData(void);
    void addAxesData(MyString string, 
                     SWND factor, SWND accel, bool wheel, SWND ignore);
    void addAxisData(MyString string, bool minus, int axis, 
                     SWND factor, SWND accel, bool wheel, SWND ignore);

    InputDevice *inputDevice;

    SWND parent;
    // define variables for the selectable data as SWND widget/control
    SWND combo[6];
    SWND edit_factor[6];
    SWND edit_accel[6];
    SWND check_wheel[6];
    SWND edit_ignore[6];
    SWND check_not[6];
    SWND check_headnav;
    SWND check_sendalways;
    SWND edit_max_axis;
    SWND commandline;
    SWND check_dcfocus;
};
#endif
