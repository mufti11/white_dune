#!/bin/sh

# This file is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.

dune -xinput spaceball -z=-2 -zrot=-5

# Under Linux in xorg.conf is something needed like
# 
# Section "ServerLayout"
...
#    InputDevice    "Magellan"
...
# EndSection
...
# Section "InputDevice"
#    Identifier     "Magellan"
#    Driver         "magellan"
#    Option         "SendCoreEvents" "false"
#    Option         "Device" "/dev/ttyS0"
## or for second serial interface
##   Option         "Device" "/dev/ttyS1"
## or for a USB-Serial port adapter
##   Option         "Device" "/dev/ttyUSB0"
# EndSection


