#!/bin/sh

# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

awk '/^INCLUDE/ { print $2 }' *.geo | xargs ls > /dev/null
