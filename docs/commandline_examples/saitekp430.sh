#!/bin/sh

# This file is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.

dune -joystick /dev/input/js1 -z=-2 -yrot=-3 -xrot=5 -zrot=4
