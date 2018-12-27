# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

all:
	mkdir -p $$HOME/.dt
	mkdir -p $$HOME/.dt/icons
	mkdir -p $$HOME/.dt/types
	cp icons/* $$HOME/.dt/icons
	cp types/white_dune.dt $$HOME/.dt/types/
	if test `uname` = SunOS ; then cp os/solaris/opt_White_Dune_bin_white_dune.dt $$HOME/.dt/types/white_dune.dt ; fi
	if test `uname` = AIX ; then cp os/aix/usr_local_bin_white_dune.dt $$HOME/.dt/types/white_dune.dt ; fi
	/usr/dt/bin/dtcreate $$HOME/.dt/types/white_dune.dt
