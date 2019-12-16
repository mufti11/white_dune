 /*
 * AflockSettingsDialog.cpp
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
// AflockSettingsDialog.cpp: implementation of the AflockSettingsDialog class.
// This Programm collect the InputDevice Informations and format them.
// Haining Zhi Mai.2004 ICA Uni-Stuttgart
// A part of Project white_dune.
//////////////////////////////////////////////////////////////////////

#include "DuneApp.h"
#include "swt.h"
#include "config.h"
#include "stdafx.h"
#include "resource.h"
#include "InputDevice.h"
#include "InputDeviceApp.h"

#ifdef HAVE_AFLOCK

#include "AflockSettingsDialog.h"

extern bool parseCommandlineArgumentInputDevice(int i,int argc, char** argv);


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AflockSettingsDialog::AflockSettingsDialog(SWND Parent, 
                                           AflockDevice *device) :
      Dialog(Parent, IDD_AFLOCK)
{
    aflockDevice = device;
    setDialog2();
    check_baud             = swGetDialogItem(m_dlg, IDC_CHECK_AFBAUD);
    check_block            = swGetDialogItem(m_dlg, IDC_CHECK_AFBLOCK);
    check_sync             = swGetDialogItem(m_dlg, IDC_CHECK_AFSYNC);
    check_numbirds         = swGetDialogItem(m_dlg, IDC_CHECK_AFNUMBIRDS);
    check_master           = swGetDialogItem(m_dlg, IDC_CHECK_AFMASTER);
    check_filter           = swGetDialogItem(m_dlg, IDC_CHECK_AFFILTER);
    check_report           = swGetDialogItem(m_dlg, IDC_CHECK_AFREPORT);
    check_calfile          = swGetDialogItem(m_dlg, IDC_CHECK_AFCALFILE);
    check_wand             = swGetDialogItem(m_dlg, IDC_CHECK_AFWAND);
    check_tracker          = swGetDialogItem(m_dlg, IDC_CHECK_AFTRACKER);
    check_headnavigation   = swGetDialogItem(m_dlg, IDC_CHECK_AFHEADNV);
    check_hemisphere       = swGetDialogItem(m_dlg, IDC_CHECK_AFHEMI);
    check_suddenchangelock = swGetDialogItem(m_dlg, IDC_CHECK_AFSUDDEN);
    edit_baudrate          = swGetDialogItem(m_dlg, IDC_EDIT_BAUDRATE);
    edit_calfile           = swGetDialogItem(m_dlg,IDC_EDIT_CALFILE);
    edit_numbirds          = swGetDialogItem(m_dlg,IDC_EDIT_NUMBIRDS);
    edit_wand              = swGetDialogItem(m_dlg,IDC_EDIT_WAND);
    edit_master            = swGetDialogItem(m_dlg,IDC_EDIT_MASTER);
    edit_tracker           = swGetDialogItem(m_dlg,IDC_EDIT_TRACKER);
    combo_filter           = swGetDialogItem(m_dlg,IDC_COMBO_FILTER);
    combo_hemisphere       = swGetDialogItem(m_dlg,IDC_COMBO_HEMISPHERE);
    commandline            = swGetDialogItem(m_dlg,IDC_TEXT_COMMANDLINE);
    edit_device_nr         = swGetDialogItem(m_dlg,IDC_EDIT_DEVICENR);

    hemisphere[0]="FRONT_HEM";
    hemisphere[1]="AFT_HEM";
    hemisphere[2]="UPPER_HEM";
    hemisphere[3]="LOWER_HEM";
    hemisphere[4]="LEFT_HEM";
    hemisphere[5]="RIGHT_HEM";

    filter[0]="AC_NARROW";
    filter[1]="AC_WIDE";
    filter[2]="AC_FILTER";

    //swSetCheck(check_sync,TheApp->getAflockDevice(TheApp->getNumberAflockDevices()-1)->sync);
    for (i=0;i<=2;i++) {
        swComboBoxAppendItem(combo_filter,filter[i]);
    }

    for (i=0;i<=5;i++) {
        swComboBoxAppendItem(combo_hemisphere,hemisphere[i]);
    }
    
    LoadData();
}


void AflockSettingsDialog::LoadData()
{
    //read inputdevice's Information
    swGetText(edit_device_nr,tmp,128);
    device_nr=atoi(tmp);
    if (TheApp->getAflockDevice(device_nr) == NULL) {
        swMessageBox(m_dlg,"no aflock device","White_dune",SW_MB_OK, 
                     SW_MB_WARNING);
        return;
    }
    mysnprintf(tmp,128,"%d",TheApp->getAflockDevice(device_nr)->getBaudrate());
    swSetText(edit_baudrate,tmp);
    mysnprintf(tmp,128,"%d",TheApp->getAflockDevice(device_nr)->getNumBrds());
    swSetText(edit_numbirds,tmp);
    mysnprintf(tmp,128,"%d",TheApp->getAflockDevice(device_nr)->getTransmit());
    swSetText(edit_transmit,tmp);

    // load hemisphere combobox
    mysnprintf(tmp,128,"%s",TheApp->getAflockDevice(device_nr)->getHemi());
    for (i=0;i<6;i++) {
        if (stringncmp(hemisphere[i],tmp)==0) {
            j=i;
            break;
        }
    }
    swComboBoxSetSelection(combo_hemisphere,j);

    // load filter combobox
    mysnprintf(tmp,128,"%s",TheApp->getAflockDevice(device_nr)->getFilt());
    for (i=0;i<3;i++) {
        if (stringncmp(filter[i],tmp)==0) {
            j=i;
            break;
        }
    }
    swComboBoxSetSelection(combo_filter,j);

    mysnprintf(tmp,128,"%s",TheApp->getAflockDevice(device_nr)->getCalfile());
    swSetText(edit_calfile,tmp);
    swSetCheck(check_suddenchangelock,
               TheApp->getAflockDevice(device_nr)->getSudden_change_lock());
    swSetCheck(check_sync,TheApp->getAflockDevice(device_nr)->getSync());
    swSetCheck(check_block,TheApp->getAflockDevice(device_nr)->getBlock());
    swSetCheck(check_report,TheApp->getAflockDevice(device_nr)->getReport());
    swSetCheck(check_headnavigation,
               TheApp->getInputDevice(device_nr)->getHeadNavigation());
    
    //set, if the value can be changed
    swEnableWindow(edit_transmit,swGetCheck(check_transmit));
    swEnableWindow(edit_baudrate,swGetCheck(check_baud));
    swEnableWindow(edit_calfile,swGetCheck(check_calfile));
    swEnableWindow(edit_numbirds,swGetCheck(check_numbirds));
    swEnableWindow(edit_wand,swGetCheck(check_wand));
    swEnableWindow(edit_master,swGetCheck(check_master));
    swEnableWindow(edit_tracker,swGetCheck(check_tracker));
    swEnableWindow(combo_filter,swGetCheck(check_filter));
    swEnableWindow(combo_hemisphere,swGetCheck(check_hemisphere));
    
}

void AflockSettingsDialog::OnCommand(void *vid)
{
    int id = swFromPtr(vid);
    switch(id) {
      //update AFLock Device, if it is present.
      case IDC_SAVE:
        command_sum=0;
        command_line = new char*[MAX_LEN_COMMANDLINE];
    
        if (swGetCheck(check_baud)) {
            command_line[command_sum] = new char[128];
            swGetText(edit_baudrate,tmp,128);
            mysnprintf(command_line[command_sum++],128,"%s %s"," -baud",tmp);
        }  
        if (swGetCheck(check_numbirds)) {
            command_line[command_sum] = new char[128];
            swGetText(edit_numbirds,tmp,128);
            mysnprintf(command_line[command_sum++],128,"%s %s"," -numbirds",tmp);
        }
        if (swGetCheck(check_master)) {
            command_line[command_sum] = new char[128];
            swGetText(edit_master,tmp,128);
            mysnprintf(command_line[command_sum++],128,"%s %s"," -master",tmp);
        }
        if (swGetCheck(check_wand)) {
            command_line[command_sum] = new char[128];
            swGetText(edit_wand,tmp,128);
            mysnprintf(command_line[command_sum++],128,"%s %s"," -wand",tmp);
        }
        if (swGetCheck(check_calfile)) {
            command_line[command_sum] = new char[128];
            swGetText(edit_calfile,tmp,128);
            mysnprintf(command_line[command_sum++],128,"%s %s"," -calfile",tmp);
        }
        if (swGetCheck(check_hemisphere)) {
            command_line[command_sum] = new char[128];
            mysnprintf(command_line[command_sum++],128,"%s %s"," -hemisphere",
                     hemisphere[swComboBoxGetSelection(combo_hemisphere)]);
        }
        if (swGetCheck(check_filter)) {
            command_line[command_sum] = new char[128];
            mysnprintf(command_line[command_sum++],128,"%s %s"," -filter",
            filter[swComboBoxGetSelection(combo_filter)]);
        }
        
        if (swGetCheck(check_sync)) {
            command_line[command_sum] = new char[128];
            mysnprintf(command_line[command_sum++],128," -sync 1");
        } else {
            command_line[command_sum] = new char[128];
            mysnprintf(command_line[command_sum++],128," -sync 0");
        }
        if (swGetCheck(check_report)) {
            command_line[command_sum] = new char[128];
            mysnprintf(command_line[command_sum++],128," -report 1");
        } else {
            command_line[command_sum] = new char[128];
            mysnprintf(command_line[command_sum++],128," -report 0");
        }
        if (swGetCheck(check_block)) {
            command_line[command_sum] = new char[128];
            mysnprintf(command_line[command_sum++],128," -block 1");
        } else {
            command_line[command_sum] = new char[128];
            mysnprintf(command_line[command_sum++],128," -block 0");
        }
        if (swGetCheck(check_suddenchangelock)) {
            command_line[command_sum] = new char[128];
            mysnprintf(command_line[command_sum++],128," -suddenchangelock 1");
        } else {
            command_line[command_sum] = new char[128];
            mysnprintf(command_line[command_sum++],128," -suddenchangelock 0");
        }
        if (swGetCheck(check_headnavigation)) {
            command_line[command_sum] = new char[128];
            mysnprintf(command_line[command_sum++],128," -headnavigation");
        }
        
        {
            //output the commandline
            MyString line = "";
            for (i=0;i<command_sum;i++) {
                line += " ";
                line += command_line[i];
            }

            swSetText(commandline,line);
        }

        //perform the commandline
        i=0;
        while (i<command_sum) {
            if (!parseCommandlineArgumentInputDevice(i,command_sum,
                                                     command_line))
                break;
            i++;
        }
      
        delete command_line;
      
        break;
    
    //set, if the value can be changed
    case IDC_CHECK_AFBAUD:
    case IDC_CHECK_AFNUMBIRDS:
    case IDC_CHECK_AFMASTER:
    case IDC_CHECK_AFFILTER:
    case IDC_CHECK_AFCALFILE:
    case IDC_CHECK_AFWAND:
    case IDC_CHECK_AFTRACKER:
    case IDC_CHECK_AFHEMI:
    
      swEnableWindow(edit_baudrate,swGetCheck(check_baud));
      swEnableWindow(edit_calfile,swGetCheck(check_calfile));
      swEnableWindow(edit_numbirds,swGetCheck(check_numbirds));
      swEnableWindow(edit_wand,swGetCheck(check_wand));
      swEnableWindow(edit_master,swGetCheck(check_master));
      swEnableWindow(edit_tracker,swGetCheck(check_tracker));
      swEnableWindow(combo_filter,swGetCheck(check_filter));
      swEnableWindow(combo_hemisphere,swGetCheck(check_hemisphere));
    
      break;
    case IDC_OKSAVE:
      OnCommand(swToPtr(IDC_SAVE));
      OnCommand(swToPtr(IDOK2));
      break;
    case IDC_BUTTON_DEFAULT:
      load_default();
      break;
    default:
      Dialog::OnCommand(swToPtr(id));
      break;
    }
}


AflockSettingsDialog::~AflockSettingsDialog()
{
}
    
void AflockSettingsDialog::load_default(void)
{
    command_sum=0;
    command_line = new char*[MAX_LEN_COMMANDLINE];
    
    command_line[command_sum] = new char[128];
    mysnprintf(command_line[command_sum++],128," -baud 38400");
    command_line[command_sum] = new char[128];
    mysnprintf(command_line[command_sum++],128," -numbirds 2");
    command_line[command_sum] = new char[128];
    mysnprintf(command_line[command_sum++],128," -master 1");
    //command_line[command_sum] = new char[128];
    //mysnprintf(command_line[command_sum++],128," -wand");
    command_line[command_sum] = new char[128];
    mysnprintf(command_line[command_sum++],128," -calfile");
    command_line[command_sum] = new char[128];
    mysnprintf(command_line[command_sum++],128," -hemisphere RIGHT_HEM");
    command_line[command_sum] = new char[128];
    mysnprintf(command_line[command_sum++],128," -filter FILTER_DEFAULT");
    command_line[command_sum] = new char[128];
    mysnprintf(command_line[command_sum++],128," -sync 0");
    command_line[command_sum] = new char[128];
    mysnprintf(command_line[command_sum++],128," -report Q");
    command_line[command_sum] = new char[128];
    mysnprintf(command_line[command_sum++],128," -block 0");
    command_line[command_sum] = new char[128];
    mysnprintf(command_line[command_sum++],128," -suddenchangelock 1");
    //command_line[command_sum] = new char[128];
    //mysnprintf(command_line[command_sum++],128," -headnavigation");
      
    //output the commandline
    MyString line = "";
    for (i=0;i<command_sum;i++) {
        line += " ";
        line += command_line[i];
    }

    swSetText(commandline,line);

    //perform the commandline
    i=0;
    while (i<command_sum) {
        if (!parseCommandlineArgumentInputDevice(i,command_sum,command_line))
            break;
        i++;
    }
    
    delete command_line;
    LoadData();
}


#else
// dummy function, cause some compilers do not like empty inputfiles....
static void AflockSettingsDialog_dummy(void) {}
#endif

