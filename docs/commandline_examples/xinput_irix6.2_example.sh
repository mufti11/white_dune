#!/bin/sh

# This file is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.

dune -xinput spaceball -allxyz=20,200,,0.0000002 -allrot=40,400,,0.0000002 -xinput dial+buttons -x=0 -y=2 -z=4 -xrot=1 -yrot=3 -zrot=5 -all=1000,,wheel $*
