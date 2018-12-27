#!/bin/sh
#
# redhat desktop file builder
#
# Copyright (C) 2004 Dries Verachtert
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program (see the file "COPYING" for details); if 
# not, write to the Free Software Foundation, Inc., 675 Mass Ave, 
# Cambridge, MA 02139, USA.

cp dune.desktop redhat
cat >> redhat/dune.desktop <<EOF
Categories=Application;Graphics;X-Red-Hat-Extra;
EOF

cp dune4kids.desktop redhat
cat >> redhat/dune4kids.desktop <<EOF
Categories=Application;Graphics;X-Red-Hat-Extra;
EOF
