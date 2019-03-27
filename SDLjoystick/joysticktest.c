/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997, 1998, 1999, 2000, 2001, 2002  Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_joystick.c,v 1.4 2002/03/06 11:23:02 slouken Exp $";
#endif

/* This is the joystick API for Simple DirectMedia Layer */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL_joystick.h"

int main(int argc,char** argv)
{
     int i;
     SDL_Joystick *js;
     SDL_JoystickInit();

     if (SDL_NumJoysticks() == 0) {
         fprintf(stderr, "no joystick detected !\n");
         exit(1);
     }
     printf("number joysticks: %d\n", SDL_NumJoysticks());
     for (i=0;i<SDL_NumJoysticks();i++)
         printf("joystick name: %s\n", SDL_JoystickName(i));
     js = SDL_JoystickOpen(0);
     printf("num joystick axes: %d\n",SDL_JoystickNumAxes(js));
     while (1) {
        SDL_JoystickUpdate();
        for (i=0; i < SDL_JoystickNumAxes(js); i++)
            printf("%d: %d ", i, SDL_JoystickGetAxis(js, i));
        printf("\n");
     }
}
