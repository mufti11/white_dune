#!/bin/sh
# This program is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

if test $# != 1 ; then
   echo "Usage: $0 user" 1>&2
   echo "Give \"user\" access to the NXT USB device" 1>&2 
   exit 1
fi

NXTUSER="$1"

if test "X_$USER" != "X_root" ; then
   echo "warning: this script usually requires root access" 1>&2
fi

cat > /etc/udev/rules.d/40-nxt.rules << EOT
# USB udev rules for Mindstorms NXT

SYSFS{idVendor}=="0694", SYSFS{idProduct}=="0002", MODE="0660", OWNER="$NXTUSER", GROUP="users"
EOT
