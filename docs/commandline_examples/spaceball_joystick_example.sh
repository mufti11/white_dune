#!/bin/sh

# This file is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.

cat /dev/null > /dev/ttyS1
dune -spaceball /dev/ttyS1 -all=2,5,,0.0000002 -joystick /dev/input/js0 $*


