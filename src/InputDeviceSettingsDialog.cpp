/*
 * InputDeviceSettingsDialog.cpp
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
// InputDeviceSettingsDialog.cpp: implementation of the 
// InputDeviceSettingsDialog class.
// This Programm collect the InputDevice informations and format them.
// Haining Zhi Mai.2004 ICA Uni-Stuttgart
// A part of project white_dune.
//////////////////////////////////////////////////////////////////////

#include "DuneApp.h"
#include "InputDeviceSettingsDialog.h"
#include "AflockSettingsDialog.h"
#include "InputDevice.h"
#include "stdafx.h"
#include "resource.h"
#include "swt.h"

InputDeviceSettingsDialog::InputDeviceSettingsDialog(SWND Parent, 
                                                     InputDevice *device) :
      Dialog(Parent, IDD_INPUT_DEVICE)
{
    parent = Parent;
    combo[0] =              swGetDialogItem(m_dlg, IDC_COMBO_X);
    combo[1] =              swGetDialogItem(m_dlg, IDC_COMBO_Y);
    combo[2] =              swGetDialogItem(m_dlg, IDC_COMBO_Z);
    combo[3] =              swGetDialogItem(m_dlg, IDC_COMBO_RX);
    combo[4] =              swGetDialogItem(m_dlg, IDC_COMBO_RY);
    combo[5] =              swGetDialogItem(m_dlg, IDC_COMBO_RZ);
    edit_max_axis =         swGetDialogItem(m_dlg, IDC_EDIT_MAX_AXIS);
    commandline =           swGetDialogItem(m_dlg, IDC_COMMANDLINE);
    check_sendalways =      swGetDialogItem(m_dlg, IDC_CHECK_SENDALWAYS);
    check_dcfocus =         swGetDialogItem(m_dlg, IDC_CHECK_DCFOCUS);
    edit_factor[0] =        swGetDialogItem(m_dlg, IDC_FACTORX);
    edit_factor[1] =        swGetDialogItem(m_dlg, IDC_FACTORY);
    edit_factor[2] =        swGetDialogItem(m_dlg, IDC_FACTORZ);
    edit_factor[3] =        swGetDialogItem(m_dlg, IDC_FACTORRX);
    edit_factor[4] =        swGetDialogItem(m_dlg, IDC_FACTORRY);
    edit_factor[5] =        swGetDialogItem(m_dlg, IDC_FACTORRZ);
    edit_accel[0] =         swGetDialogItem(m_dlg, IDC_ACCELX);
    edit_accel[1] =         swGetDialogItem(m_dlg, IDC_ACCELY);
    edit_accel[2] =         swGetDialogItem(m_dlg, IDC_ACCELZ);
    edit_accel[3] =         swGetDialogItem(m_dlg, IDC_ACCELRX);
    edit_accel[4] =         swGetDialogItem(m_dlg, IDC_ACCELRY);
    edit_accel[5] =         swGetDialogItem(m_dlg, IDC_ACCELRZ);
    check_wheel[0] =        swGetDialogItem(m_dlg, IDC_WHEELX);
    check_wheel[1] =        swGetDialogItem(m_dlg, IDC_WHEELY);
    check_wheel[2] =        swGetDialogItem(m_dlg, IDC_WHEELZ);
    check_wheel[3] =        swGetDialogItem(m_dlg, IDC_WHEELRX);
    check_wheel[4] =        swGetDialogItem(m_dlg, IDC_WHEELRY);
    check_wheel[5] =        swGetDialogItem(m_dlg, IDC_WHEELRZ);
    edit_ignore[0] =        swGetDialogItem(m_dlg, IDC_IGNOREX);
    edit_ignore[1] =        swGetDialogItem(m_dlg, IDC_IGNOREY);
    edit_ignore[2] =        swGetDialogItem(m_dlg, IDC_IGNOREZ);
    edit_ignore[3] =        swGetDialogItem(m_dlg, IDC_IGNORERX);
    edit_ignore[4] =        swGetDialogItem(m_dlg, IDC_IGNORERY);
    edit_ignore[5] =        swGetDialogItem(m_dlg, IDC_IGNORERZ);
    check_not[0] =          swGetDialogItem(m_dlg, IDC_NX);
    check_not[1] =          swGetDialogItem(m_dlg, IDC_NY);
    check_not[2] =          swGetDialogItem(m_dlg, IDC_NZ);
    check_not[3] =          swGetDialogItem(m_dlg, IDC_NRX);
    check_not[4] =          swGetDialogItem(m_dlg, IDC_NRY);
    check_not[5] =          swGetDialogItem(m_dlg, IDC_NRZ);

    inputDevice = device;

    for (i = 0; i < 6; i++)
        axes_name[i] = device->getAxesInfo(i);
    axes_name[6] = "all";
    axes_name[7] = "allxyz";
    axes_name[8] = "allrot";

    max_axes=6;

    LoadData();
}
 
void InputDeviceSettingsDialog::OnCommand(void *vid)
{     
    int id = swFromPtr(vid);
    switch(id) {
#ifdef HAVE_AFLOCK
      case IDC_OPTIONS_AFLOCK:
        {
        if (TheApp->getNumberAflockDevices() > 0) {
            OnCommand(swToPtr(IDOK));
        } else {
            swMessageBox(m_dlg, "no aflock device yet", "white_dune", SW_MB_OK, 
                         SW_MB_ERROR);
        }
        break;
        }
#endif
      case IDC_SAVE: 
        showNumbers();
        SaveData();  
        break;
      case IDC_OKSAVE:
        //save and close dialog
        SaveData();
        OnCommand(swToPtr(IDOK));
        break;
      case IDC_BUTTON_DEFAULT:
        //set default values
        SetDefault();
        break;
      default:
        Dialog::OnCommand(swToPtr(id));
        break;
    }
}

bool InputDeviceSettingsDialog::Validate()
{
   return true;
}    

void InputDeviceSettingsDialog::LoadData() 
{
    //clear axis combo_axes
    for (i=0;i<6;i++) {
        swComboBoxDeleteAll(combo[i]);
    }
    //get the max_number of axes.
    max_axis=inputDevice->get_number_max_axis();
    mysnprintf(buf,128,"%d", max_axis);
    swSetText(edit_max_axis,buf);
    max_axes=max_axis;

    //init the combo_axes informationen
    for (i=0;i<max_axis;i++) {
        mysnprintf(buf, 128, "%d", i);
        for (j=0;j<6;j++) {
            swComboBoxAppendItem(combo[j], buf);
        }
    }
    for (i=0;i<6;i++) {
        swComboBoxAppendItem(combo[i], "none");
    }

    //read the combo_axes Selection
    for (j=0;j<6;j++) {
        if (j <= max_axis)
            swComboBoxSetSelection(combo[j],inputDevice->
                                   getAxisFromInformation(axes_name[j]));
        else
            swComboBoxSetSelection(combo[j],max_axis);
        if (inputDevice->get_ignore(j))
            swComboBoxSetSelection(combo[j],max_axis);
    }

    //read sendalways
    swSetCheck(check_sendalways,inputDevice->sendalways());
    //read (don't care focus)
    swSetCheck(check_dcfocus,TheApp->dontCareFocus());

    for (i=max_axes;i<6;i++) { 
        swSetCheck(check_not[i],0);
        swSetText(edit_factor[i],"");
        swSetText(edit_accel[i],"");
        swSetCheck(check_wheel[i],false);
        swSetText(edit_ignore[i],"");
    }

    showNumbers();

    for (i=0; i< max_axes;i++) { 
        if (inputDevice->get_factor(swComboBoxGetSelection(combo[i])) == 1.0f)
            swSetText(edit_factor[i],"");
        if (inputDevice->get_acceleration(swComboBoxGetSelection(combo[i])) == 
            1.0f)
            swSetText(edit_accel[i],"");
        if (inputDevice->get_zero_size_fraction(swComboBoxGetSelection(combo[i])) == 0.0f)
            swSetText(edit_ignore[i],"");
    }

    enable_all(true);
}

void InputDeviceSettingsDialog::addAxesData(MyString string, 
                                            SWND edit_factor, SWND edit_accel,
                                            bool wheel, SWND edit_ignore)
{
    bool hasFactor = true;
    char factor[128];
    swGetText(edit_factor,factor,128);
    if (strcmp(factor,"0.0")==0)
        hasFactor = false;
    bool hasAccel = true;
    char accel[128];
    swGetText(edit_accel,accel,128);
    if (strcmp(accel,"1.0")==0)
        hasAccel = false;
    bool hasIgnore = true;
    char ignore[128];
    swGetText(edit_ignore,ignore,128);
    if (strcmp(ignore,"0.0")==0)
    if (hasIgnore == 0.0)
        hasIgnore = false;
    if (hasIgnore || wheel || hasAccel || hasFactor) {
        if (hasFactor) 
            string += factor;
        if (hasIgnore || wheel || hasAccel) {
            string += ",";
            if (hasAccel)
                string += accel;
            if (hasIgnore || wheel) {
                string += ",";
                if (wheel)
                    string += "wheel";
                if (hasIgnore) {
                    string += ",";
                    string += ignore;
                }
            }
        }   
    }
} 

void InputDeviceSettingsDialog::addAxisData(MyString string, bool minus,
                                            int axis, SWND factor, SWND accel,
                                            bool wheel, SWND ignore)
{
    char buf[128];
    string += "=";
    if (minus)
        string += "-";
    mysnprintf(buf,128,"%d",axis);
    string += buf;
    string += ",";
    addAxesData(string, factor, accel, wheel, ignore);
} 

void InputDeviceSettingsDialog::SaveData()
{
    //click button save_Settings 

    // write all paramater to command[]
    Array<MyString> command;
    command_sum=0;

    //check the max_number_axis

    command[command_sum] = "";

    command[command_sum] += inputDevice->getDeviceOption();
    command[command_sum] += " ";
    command[command_sum] += "\"";
    command[command_sum] += inputDevice->getDeviceName();
    command[command_sum] += "\"";

    command[command_sum] += "\n";
    command_sum++;

    command[command_sum] += "-axes=";
    swGetText(edit_max_axis,buf,128);
    command[command_sum] += buf;
    command_sum++;

    // set the axis information
    for (i=0;i<6;i++) {
        command[command_sum] = "";
        if (swComboBoxGetSelection(combo[i])<max_axis) {
            command[command_sum] += "-";
            command[command_sum] += axes_name[i];
            int axis = swComboBoxGetSelection(combo[i]);
            bool minus = false;
            if (swGetCheck(check_not[i]))
                minus = true;
            addAxisData(command[command_sum], minus, axis,
                        edit_factor[i], edit_accel[i], 
                        swGetCheck(check_wheel[i]), edit_ignore[i]);
        } else if (swComboBoxGetSelection(combo[i])>=max_axis) {
            command[command_sum] += "-none=";
            command[command_sum].catInt(i);
        }
        command_sum++;
    }

    //set sendalways
    if (swGetCheck(check_sendalways)) { 
        command[command_sum] = "";
        command[command_sum] += "-sendalways";
        command_sum++;
    }

    //set don't care focus
    if (swGetCheck(check_dcfocus)) { 
        command[command_sum] = "";
        command[command_sum] += "-dontcarefocus";
        command_sum++;
    }

    //print the commandline
    MyString line = "";
    for (i=0;i<command_sum;i++) {
        line += command[i];
        line += " ";
    }
    swSetText(commandline,line);

    //run the Commandline
    for (i=1;i<command_sum;i++)
        TheApp->setInputDeviceSetting(inputDevice,command[i]);

    TheApp->PrintMessageWindowsId(IDS_INPUT_DEVICE_PARAMETERS_SAVED);
}

void InputDeviceSettingsDialog::enable_all(bool flag)
{
    for (i=0;i<6;i++) {
        swEnableWindow(edit_factor[i],flag);
        swEnableWindow(edit_accel[i],flag);
        swEnableWindow(check_wheel[i],flag);
        swEnableWindow(edit_ignore[i],flag);
        swEnableWindow(check_not[i],flag);
    }                
    swEnableWindow(check_sendalways,flag);
    swEnableWindow(edit_max_axis,flag);
    swEnableWindow(commandline,flag);
    swEnableWindow(check_dcfocus,flag);
}

void InputDeviceSettingsDialog::SetDefault()
{
    char buff[128];
    Array<MyString> command;
    int command_sum=0;

    command[command_sum]="";
    mysnprintf(buff,128,"%s","-axes=6");
    command[command_sum++]+=buff;

    command[command_sum]="";
    mysnprintf(buff,128,"%s","-x=0");
    command[command_sum++]+=buff;

    command[command_sum]="";
    mysnprintf(buff,128,"%s","-y=1");
    command[command_sum++]+=buff;

    command[command_sum]="";
    mysnprintf(buff,128,"%s","-z=2");
    command[command_sum++]+=buff;

    command[command_sum]="";
    mysnprintf(buff,128,"%s","-xrot=3");
    command[command_sum++]+=buff;

    command[command_sum]="";
    mysnprintf(buff,128,"%s","-yrot=4");
    command[command_sum++]+=buff;

    command[command_sum]="";
    mysnprintf(buff,128,"%s","-zrot=5");
    command[command_sum++]+=buff;

    command[command_sum]="";
    mysnprintf(buff,128,"%s","-all=1,1,,0");
    command[command_sum++]+=buff;

    //print the commandline

    MyString line = "";
    for (i=0;i<command_sum;i++) {
        line += " ";
        line += command[i];
    }

    swSetText(commandline,line);

    //run the Commandline
    for (i=0; i < command_sum; i++)
        TheApp->setInputDeviceSetting(inputDevice,command[i]);

    //Update the dialog
    LoadData();
}

void InputDeviceSettingsDialog::showNumbers(void)
{

    //read nagation,factor,accel,wheel,ignore
    for (i=0;i<max_axes;i++) {
        if (inputDevice->get_sign(swComboBoxGetSelection(combo[i]))==-1)
        swSetCheck(check_not[i],1);
        if (inputDevice->get_sign(swComboBoxGetSelection(combo[i]))==1)
            swSetCheck(check_not[i],0);

        mysnprintf(buf, 128, "%g", 
                   inputDevice->get_factor(swComboBoxGetSelection(combo[i])));
        swSetText(edit_factor[i],buf);

        mysnprintf(buf, 128, "%g", 
              inputDevice->get_acceleration(swComboBoxGetSelection(combo[i])));
        swSetText(edit_accel[i],buf);

        swSetCheck(check_wheel[i],!inputDevice->get_zero_on_release(
                                         swComboBoxGetSelection(combo[i])));

        mysnprintf(buf, 128, "%g", inputDevice->get_zero_size_fraction(
              swComboBoxGetSelection(combo[i])));
        swSetText(edit_ignore[i],buf);
    }
}
