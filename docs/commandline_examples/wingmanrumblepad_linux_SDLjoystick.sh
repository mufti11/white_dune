#!/bin/sh

# This file is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.

dune -SDLjoystick 0 -all=,,,0.000005 -y=-1 -z=4 -xrot=6 -yrot=-3 -zrot=5 -none=2 -allrot=0.2 $*

