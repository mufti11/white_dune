/*
 * InputDeviceSelectionDialog.cpp
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
// InputDeviceSelectionDialog.cpp: implementation of the 
// InputDeviceSelectionDialog class.
// This Programm collect the InputDevices or InputDevice options
// Haining Zhi Mai.2004 ICA Uni-Stuttgart
// A part of project white_dune.
//////////////////////////////////////////////////////////////////////

#include "DuneApp.h"
#include "InputDeviceSelectionDialog.h"
#include "InputDevice.h"
#include "AflockSettingsDialog.h"
#include "stdafx.h"
#include "resource.h"
#include "swt.h"

InputDeviceSelectionDialog::InputDeviceSelectionDialog(SWND Parent) :
      Dialog(Parent, IDD_INPUT_DEVICE_SELECTION)
{
    parent = Parent;
    button_delete_device = swGetDialogItem(m_dlg, IDC_DELETE_INPUTDEVICE);
    combo_existing_devices = swGetDialogItem(m_dlg, 
                                             IDC_COMBO_EXISTING_INPUTDEVICES);
    combo_device_options = swGetDialogItem(m_dlg, IDC_COMBO_INPUTDEVICE_OPTIONS);
    lastCommand = -1; 

    existing_devices.resize(0);

    inputDevice = NULL;
#ifdef HAVE_AFLOCK
    aflockDevice = NULL;
#endif
    deviceOption = "";

    swComboBoxDeleteAll(combo_existing_devices);
    swComboBoxAppendItem(combo_existing_devices,"(none)");
    for (int i = 0; i < TheApp->getNumberInputDevices(); i++) {
        char buf[256];
        InputDevice *inputDevice=TheApp->getInputDevice(i);
        existing_devices.append(inputDevice);
        mysnprintf(buf, 255, "%s %s", inputDevice->getDeviceOption(), 
                                      inputDevice->getDeviceName());
        swComboBoxAppendItem(combo_existing_devices, buf);
    }
#ifdef HAVE_AFLOCK
    for (int i = 0; i < TheApp->getNumberAflockDevices(); i++) {
        char buf[256];
        AflockDevice *aflockDevice=TheApp->getAflockDevice(i);
        mysnprintf(buf, 255, "-aflock %s", aflockDevice->getDeviceName());
        swComboBoxAppendItem(combo_existing_devices, buf);
        existing_devices.append(inputDevice);
    }
#endif
    swEnableWindow(combo_existing_devices, true);
    swComboBoxSetSelection(combo_existing_devices, 0);

    swComboBoxDeleteAll(combo_device_options);
    swComboBoxAppendItem(combo_device_options,"(none)");
#ifdef LINUX_JOYSTICK
    device_options.append("-joystick");
#else
# ifdef WINDOWS_JOYSTICK
    device_options.append("-joystick");
# endif
#endif

#ifdef HAVE_XINPUT
    device_options.append("-xinput");
#endif

#ifdef HAVE_LIBSBALL
    device_options.append("-spaceball");
#endif

#ifdef HAVE_NXT_DIALS
    device_options.append("-nxtdials");
#endif

#ifdef HAVE_WINDOWS_P5
    device_options.append("-p5");
#endif

#ifdef HAVE_SDL_JOYSTICK
    device_options.append("-SDLjoystick");
#endif

#ifdef HAVE_AFLOCK
    device_options.append("-aflock");
#endif
    for (int i = 0; i < device_options.size(); i++)
        swComboBoxAppendItem(combo_device_options, device_options[i]);

    swEnableWindow(combo_device_options, true);
    swComboBoxSetSelection(combo_device_options, 0);

    swEnableWindow(button_delete_device, false);
}
 
void InputDeviceSelectionDialog::OnCommand(void *vid)
{     
    int id = swFromPtr(vid);
    lastCommand = id;
    switch(id) {
#ifdef HAVE_AFLOCK
      case IDC_OPTIONS_AFLOCK:
        {
        SaveData();
        if (TheApp->getNumberAflockDevices() > 0) {
            if (Validate()) {
                swEndDialog(IDOK);
            }
        } else {
            swMessageBox(m_dlg, "no aflock device yet", "white_dune", SW_MB_OK, 
                         SW_MB_ERROR);
        }
        break;
        }
#endif
      case IDC_COMBO_INPUTDEVICE_OPTIONS:
        LoadComboBoxDataOptions();
        break;
      case IDC_DELETE_INPUTDEVICE:
        if (Validate()) {
            DeleteInputDevice();
            swEndDialog(IDOK);
        }
        break;
      case IDC_COMBO_EXISTING_INPUTDEVICES:
        LoadComboBoxExistingDevice();
        break;
      case IDOK: 
        if (Validate()) {
            swEndDialog(IDOK);
        }
        break;
      default:
        Dialog::OnCommand(swToPtr(id));
        break;
    }
}

void InputDeviceSelectionDialog::LoadComboBoxDataOptions(void)
{
    int selection = swComboBoxGetSelection(combo_device_options);
    if (selection <= 0)
        swEnableWindow(combo_existing_devices, true);
    else {
        swEnableWindow(button_delete_device, false);
        swComboBoxSetSelection(combo_existing_devices, 0);
        swEnableWindow(combo_existing_devices, false);
        deviceOption = "";
        deviceOption += device_options[selection - 1];
    }
}

void InputDeviceSelectionDialog::LoadComboBoxExistingDevice(void)
{
    deviceOption = "";
    int selection = swComboBoxGetSelection(combo_existing_devices) - 1;
    if (selection < 0) {
        swEnableWindow(button_delete_device, false);
        swEnableWindow(combo_device_options, true);
    } else if (selection < TheApp->getNumberInputDevices()) {
        swEnableWindow(button_delete_device, true);
        swEnableWindow(combo_device_options, false);
        swComboBoxSetSelection(combo_device_options, 0);
        inputDevice = TheApp->getInputDevice(selection);
    }
#ifdef HAVE_AFLOCK
    else  {
        selection -= TheApp->getNumberInputDevices();
        swEnableWindow(combo_device_options, false);
        aflockDevice = TheApp->getAflockDevice(selection - 
                                               TheApp->getNumberInputDevices());
    }
#endif
}

bool InputDeviceSelectionDialog::Validate()
{
    if (inputDevice != NULL)
        return inputDevice->isValid();
#ifdef HAVE_AFLOCK
    if (aflockDevice != NULL)
        return aflockDevice->isValid();
#endif
    if (deviceOption.length() > 0)
        return true;
    return false;
}    

void InputDeviceSelectionDialog::SaveData()
{
    LoadComboBoxDataOptions();
    LoadComboBoxExistingDevice();
}

void InputDeviceSelectionDialog::DeleteInputDevice(void)
{
    TheApp->deleteInputDevice(inputDevice);
    inputDevice = NULL;
}

