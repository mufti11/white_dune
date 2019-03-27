# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

DISTFILES = swt.h keycodes.h res.h Makefile

swlanguages.h: swlanguages.h.m4 languages.m4 resource.m4 
	m4 languages.m4 swlanguages.h.m4 > swlanguages.h

dname:
	mkdir ../../`cat ../../.dname`/swt/include
	ln $(DISTFILES) ../../`cat ../../.dname`/swt/include
