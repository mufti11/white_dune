# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

DISTFILES= Makefile TODO

target:
	(cd include ; $(MAKE) $$DUNEMAKEFLAGS)
	(cd rc ; $(MAKE) $$DUNEMAKEFLAGS)
	(cd motif ; $(MAKE) $$DUNEMAKEFLAGS)

test:
	cd tests ; $(MAKE) $$DUNEMAKEFLAGS

testclean:
	cd tests ; $(MAKE) clean

clean:
	cd motif ; $(MAKE) clean
	cd rc ; $(MAKE) clean
	cd tests ; $(MAKE) clean
	cd win32 && rm -f swt.dep swt.plg swt.ncb swt.opt Debug/* Release/* \
	                  swt.sln swt.suo swt.vcproj.* dune.sln dune.suo
                          

realclean: clean
	cd motif ; $(MAKE) realclean                          
	cd tests ; $(MAKE) realclean                          

clobber:
	rm -f lib/libswt.so.* lib/libswt.a
	cd rc ; $(MAKE) clobber
	cd tests ; $(MAKE) clobber

dname:
	mkdir ../`cat ../.dname`/swt
	ln $(DISTFILES) ../`cat ../.dname`/swt
	cd include; $(MAKE) dist
	cd generic; $(MAKE) dist
	cd motif; $(MAKE) dist
	cd rc ; $(MAKE) dist
	cd win32; $(MAKE) dist
	cd tests; $(MAKE) dist
	mkdir ../`cat ../.dname`/swt/lib

efence:
	cd motif ; $(MAKE) efence
	cd rc ; $(MAKE)


