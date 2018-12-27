#!/bin/sh
#
# redhat desktop install script
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

sh build_redhat.sh

RPM_BUILD_ROOT=""
mkdir -p $RPM_BUILD_ROOT/usr/share/applications
mkdir -p $RPM_BUILD_ROOT/usr/share/icons/Bluecurve/48x48/apps

cp redhat/dune.desktop $RPM_BUILD_ROOT/usr/share/applications
cp dune.png $RPM_BUILD_ROOT/usr/share/icons/Bluecurve/48x48/apps/

cp redhat/dune4kids.desktop $RPM_BUILD_ROOT/usr/share/applications
cp dune4kids.png $RPM_BUILD_ROOT/usr/share/icons/Bluecurve/48x48/apps/



