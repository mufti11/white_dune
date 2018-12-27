#!/bin/sh

# This file is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.

# green mode: hover mode with the thumb sticks
# red mode: rocket mode with the thumb sticks
dune -joystick /dev/input/js0 -x=0 -y=1 -z=-4 -xrot=6 -yrot=-3 -zrot=5 $*

