# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

dune:
	cd src && $(MAKE) $$DUNEMAKEFLAGS

configure: configure.in
	autoconf

clean:
	rm -f config.cache config.log config.status configure.lineno confdefs.h 
	rm -rf autom4te.cache ii_files
	rm -f test/dune_selftest
	cd src && $(MAKE) clean
	cd packager && $(MAKE) clean
	cd docs/export_example_c && $(MAKE) clean
	cd docs/export_example_c++ && $(MAKE) clean
	cd docs/export_example_java && $(MAKE) clean
	rm -f desktop/macosx/Droplet-Applet/build/Droplet-Applet.build/Droplet-Applet.build/Objects-normal/ppc/main.o
	find . -name nohup.out -type f -print | xargs rm -f
	find . -name typescript -type f -print | xargs rm -f
	find . -name '.dune_preview_*.html' | xargs rm -f
	find . -name snap.rgb -type f -print | xargs rm -f
	find . -name core -type f -print | xargs rm -f
	find . -name 'core.[0-9]*' -type f -print | xargs rm -f
	find . -name dune.core -type f -print | xargs rm -f
	find . -name 'vgcore.[0-9]*' -type f -print | xargs rm -f
	find . -name '.afpDeleted*' -type f -print | xargs rm -f
	find . -name '._*' -print | xargs rm -f
	rm -rf conf[0-9]* NONE

realclean: clean
	rm -rf desktop/macosx/white_dune.app
	rm -f bin/dune bin/efencedune bin/dumadune bin/png2PixelTexture
	rm -f bin/dune.exe bin/efencedune.exe bin/dumadune.exe
	rm -f confdefs.h
	cd src && $(MAKE) realclean
	cd docs/developer_docs && make realclean 
	cd test && $(MAKE) realclean
	cd warbird && $(MAKE) clean
	find . -name '_xlk' -print | xargs rm -rf
	find . -name 'Makefile.bak' -print | xargs rm -f
	find . -name '.dune_preview_*' -print | xargs rm -f
	find . -name '.dune_textedit_*' -print | xargs rm -f
	find . -name '.dune_ecmascript_*' -print | xargs rm -f
	find . -name '.dune_shaderprogram_*' -print | xargs rm -f
	find . -name '.dune_upload_*' -print | xargs rm -f
	find . -name '.gdb_history' -print | xargs rm -f
	find . -name '.xvpics' -print | xargs rm -rf
	find . -name 'Untitled*.wrl' -print | grep -v "export" | xargs rm -f
	find . -name 'Untitled*.wrl,v' -print | xargs rm -f
	find . -name 'Untitled*.x3d' -print | xargs rm -f
	find . -name 'Untitled*.x3d,v' -print | xargs rm -f
	find . -name 'Untitled*.x3dv' -print | xargs rm -f
	find . -name 'Untitled*.x3dv,v' -print | xargs rm -f
	find . -name 'Untitled*.html' -print | xargs rm -f
	find . -name 'PureVrmlExport*.wrl' -print | xargs rm -f
	find . -name 'PureVrmlExport*.wrl,v' -print | xargs rm -f
	find . -name 'X3domExport*.html' -print | xargs rm -f
	find . -name 'X3domExport*.html,v' -print | xargs rm -f
	find . -name 'Ac3dExport*.ac' -print | xargs rm -f
	find . -name 'RibExport*.rib' -print | xargs rm -f
	find . -name 'Untitled*.rib' -print | xargs rm -f
	find . -name 'Untitled*.tif' -print | xargs rm -f
	find . -name 'Untitled*.mp4' -print | xargs rm -f
	find . -name '*.tx' -print | xargs rm -f
	find . -name 'ri.pic' -print | xargs rm -f
	find . -name 'divx2pass.log' -print | xargs rm -f
	find . -name 'divx2pass.log.mbtree' -print | xargs rm -f
	find . -name 'C++Export*.cc' -print | xargs rm -f
	find . -name 'LdrawExport*.dat' -print | xargs rm -f
	find . -name 'KanimExport*.wrl' -print | xargs rm -f
	find . -name 'KanimExport*.wrl,v' -print | xargs rm -f
	find . -name 'KanimExport*.kanim' -print | xargs rm -f
	find . -name 'Untitled*.x3dv' -print | xargs rm -f
	find . -name 'Untitled*.x3dv,v' -print | xargs rm -f
	find . -name 'Untitled*.x3d' -print | xargs rm -f
	find . -name 'Untitled*.x3d,v' -print | xargs rm -f
	find . -name 'Untitled*.html' -print | xargs rm -f
	find . -name 'gmon.out' -print | xargs rm -rf
	find . -name '*.gcno' -print | xargs rm -f
	find . -name '*.gcda' -print | xargs rm -f

tar.bz2:
	sh batch/mktarbz2.sh

tar.gz:
	sh batch/mktargz.sh

documentation:
	cd docs/developer_docs && $(MAKE)

selftest:
	cd test && $(MAKE) $$DUNEMAKEFLAGS

fastselftest:
	cd test && $(MAKE) $$DUNEMAKEFLAGS fasttests

debian:
	sh batch/makedebian.sh

debianclean:
	rm -rf build
	rm -rf debian

install:
	(cd src && make install)

# DO NOT DELETE
