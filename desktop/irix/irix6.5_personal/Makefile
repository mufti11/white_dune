# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

all:
	sh -c "echo INSTALLDIR=$$HOME/.desktop-`hostname` > makefile.host"
	sh -c "cat makefile.static >>  makefile.host"
	$(MAKE) -f  makefile.host
