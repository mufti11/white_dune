#!/bin/sh
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

awk   -f `dirname $0`/collectViewpoints.awk $*

