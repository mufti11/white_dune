/*
 * AflockSettingsDialog.h
 *
 * Copyright (C) 2000 Stephen F. White, Copyright (C) 2004 Haining Zhi
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

#ifdef HAVE_AFLOCK

#ifndef _AFLOCK_SETTINGS_DIALOG_H
#define _AFLOCK_SETTINGS_DIALOG_H

#ifndef _DIALOG_H
#include "Dialog.h"
#endif

#define MAX_LEN_COMMANDLINE 128
#define AFLOCK_DEVICE_NUMBER 4

class AflockSettingsDialog : public Dialog  
{
        AflockDevice *aflockDevice;
        char* device_name[16];
        int device_index[16];
        int device_nm[16];
        char tmp[128];
        char buf[1024];
        const char* hemisphere[6];
        const char* filter[3];
        char** command_line;
        int j,i,device_nr,max_device,command_sum;
        bool aflock_present;
        SWND check_baud,
             edit_baudrate,
             check_block,
             check_sync,
             check_transmit,
             edit_transmit,
             check_numbirds,
             edit_numbirds,
             check_master,
             edit_master,
             check_filter,
             combo_filter,
             check_report,
             check_calfile,
             edit_calfile,
             check_wand,
             edit_wand,
             check_tracker,
             edit_tracker,
             check_headnavigation,
             check_hemisphere,
             combo_hemisphere,
             check_suddenchangelock,
             edit_device_nr;

        SWND commandline;

        virtual void LoadData(void);
        virtual void OnCommand(void *id);
        void load_default(void);

        void SetDefault(void);
public:
        AflockSettingsDialog(SWND parent, AflockDevice *device);
        virtual ~AflockSettingsDialog();
};

#endif // !defined(AFLOCK_SETTINGS_DIALOG)

#endif
