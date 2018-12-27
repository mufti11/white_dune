/*
 * InputDeviceNameSelectionDialog.cpp
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

//////////////////////////////////////////////////////////////////////
// InputDeviceNameSelectionDialog.cpp: implementation of the 
// InputDeviceNameSelectionDialog class.
// This Programm collect the InputDevice name information
// Haining Zhi Mai.2004 ICA Uni-Stuttgart
// A part of project white_dune.
//////////////////////////////////////////////////////////////////////

#include "DuneApp.h"
#include "InputDeviceNameSelectionDialog.h"
#include "InputDevice.h"
#include "stdafx.h"
#include "resource.h"
#include "swt.h"

InputDeviceNameSelectionDialog::InputDeviceNameSelectionDialog(SWND Parent,
                                                               const char *opt):
      Dialog(Parent, IDD_INPUT_DEVICE_NAME_SELECTION)
{
    parent = Parent;
    device_option = opt;
    combo_device_names = swGetDialogItem(m_dlg, 
                                         IDC_COMBO_INPUTDEVICE_KNOWN_NAMES);
    edit_device_name = swGetDialogItem(m_dlg, IDC_EDIT_DEVICE_NAME);
    device_names.resize(0);

    aflockFlag = false;
    inputDevice = NULL;

#ifdef HAVE_AFLOCK
    aflockDevice = NULL;
#endif

    swComboBoxDeleteAll(combo_device_names);
    static StringArray device_info;

#ifdef HAVE_AFLOCK
    if (strcmp(device_option, "-aflock") == 0) {
        aflockFlag = true;
        aflock *device = new aflock(NULL);
        device->guessInputDeviceNames(&device_names);
    } else
#endif
    {
        InputDevice *device = InputDevice::createInputDevice(device_option, 
                                                             NULL);
        device->guessInputDeviceNames(&device_names);
    }
    swComboBoxAppendItem(combo_device_names, "(none)");
    for (int i = 0;i < device_names.size(); i++) {
        device_info[i] = "";
        device_info[i] += device_names[i];
        if (/* rate known devices usually useless as 3D input devices */
            /* mouse */
            (strcasecmp(device_names[i], "mouse") == 0) ||
            (strcasecmp(device_names[i], "generic mouse") == 0) ||
            (strcasecmp(device_names[i], "virtual_pointer") == 0) || 
            (strcasecmp(device_names[i], "Virtual core pointer") == 0) ||
            (strcasecmp(device_names[i], "configured mouse") == 0) ||
            (strcasecmp(device_names[i], "SynPS/2 Synaptics TouchPad") == 0) ||
            /* keyboard */
            (strcasecmp(device_names[i], "keyboard") == 0) ||
            (strcasecmp(device_names[i], "generic keyboard") == 0) ||
            (strcasecmp(device_names[i], "Virtual core keyboard") == 0) ||
            (strcasecmp(device_names[i], "configured keyboard") == 0) ||
            /* graphics tablett */
            (strcasecmp(device_names[i], "Cursor") == 0) ||
            (strcasecmp(device_names[i], "Stylus") == 0) ||
            (strcasecmp(device_names[i], "Eraser") == 0) ||
            (strcasecmp(device_names[i], "Pad") == 0))
            device_info[i] +=" (not recommended)";
        swComboBoxAppendItem(combo_device_names, device_info[i]);
    }
    swComboBoxSetSelection(combo_device_names, -1);
}
 
void InputDeviceNameSelectionDialog::OnCommand(void *vid)
{     
    int id = swFromPtr(vid);
    switch(id) {
      case IDC_COMBO_INPUTDEVICE_KNOWN_NAMES:
        LoadComboBoxDataNames();
        break;
      case IDOK: 
        //save and close dialog
        SaveData();
        if (Validate()) {
            swEndDialog(IDOK);
        }
        break;
      case IDC_EDIT_DEVICE_NAME:
        break;
      default:
        Dialog::OnCommand(swToPtr(id));
        break;
    }
}

void InputDeviceNameSelectionDialog::LoadComboBoxDataNames(void)
{
    int selection = swComboBoxGetSelection(combo_device_names) - 1;
    if (selection >= 0) {
        swSetText(edit_device_name, device_names[selection]); 
        aflockFlag = (strcmp(device_names[selection], "-aflock") == 0);
    }
}

bool InputDeviceNameSelectionDialog::Validate()
{
    if (inputDevice!=NULL)
        return inputDevice->isValid();
#ifdef HAVE_AFLOCK
    if (aflockDevice!=NULL)
        return aflockDevice->isValid();
#endif
    return false;
}    

void InputDeviceNameSelectionDialog::SaveData()
{
    int device_nr = -1;
    char deviceName[128];
    deviceName[0] = 0;
    swGetText(edit_device_name, deviceName, 128); 
    if (strlen(deviceName) == 0) {
        int selection = swComboBoxGetSelection(combo_device_names) - 1;
        if (selection > 0)
            mysnprintf(deviceName, 127, "%s", (const char *)
                                              device_names[selection]); 
    }    
#ifdef HAVE_AFLOCK
    if (aflockFlag) {
        aflockDevice = NULL;
        device_nr = TheApp->searchAflockDevice(deviceName);
        if (device_nr > -1) 
            aflockDevice = TheApp->getAflockDevice(device_nr);
        else
            aflockDevice = new AflockDevice(deviceName);
        if (aflockDevice != NULL) {
            if (aflockDevice->isValid())
                TheApp->setAflockDevice(aflockDevice);
            else
                TheApp->MessageBox(aflockDevice->getErrorMessage());
        }
        return;
    }
#endif
    inputDevice = NULL;
    device_nr = TheApp->searchInputDevice(device_option, deviceName);
    if (device_nr > -1) 
        inputDevice = TheApp->getInputDevice(device_nr);
    else
        inputDevice = InputDevice::createInputDevice(device_option, deviceName);
    if (inputDevice != NULL) {
        if (inputDevice->isValid())
            TheApp->setInputDevice(inputDevice);
        else
            TheApp->MessageBox(inputDevice->getErrorMessage());
    }
}
