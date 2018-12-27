#!/bin/sh

# This file is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.

# usage of a Micro$oft Sidewinder 3D Pro under Linux
dune -joystick /dev/input/js0 -z=,3 -axes=3 $*
