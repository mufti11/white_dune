#!/bin/sh

# This file is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.

# please fit the install path of the white_dune.app application 

$HOME/Desktop/white_dune.app/Contents/MacOS/dune -display :0 -nostereo -SDLjoystick 0 -all=,,,0.000004 -y=-1 -xrot=2 -z=-3 -none=2 -allrot=0.2 $*

# you can add this options to the script white_dune.app/Contents/MacOS/dunestarter
