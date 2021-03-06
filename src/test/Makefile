# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

JAVA=echo

tests: dune slowtests testzeroclass fasttests

dune:
	cd ../src && $(MAKE) $$DUNEMAKEFLAGS

slowtests: dune
	cd ../src && $(MAKE) $$DUNEMAKEFLAGS FilesMissingInWindowsProjects.o

fasttests: dune
	rm -rf ../src/NONE
	sh testtabs.sh
	sh testresourcenumbers.sh
	sh testmenus.sh
	@echo no bmptopnm: can not compare iconsize of  node_icons.bmp node_icons2.bmp
	@echo no bmptopnm: can not compare iconsize of  standard_toolbar.bmp standard_toolbar2.bmp
	@echo no bmptopnm: can not compare iconsize of  vcr.bmp vcr2.bmp
	sh testprotos.sh > testprotos.cpp
	cd ../src && $(MAKE) $$DUNEMAKEFLAGS testprotos
	./dune_testprotos
	rm -f ./dune_testprotos ./dune_testprotos.exe testprotos.cpp
	cd ../src && $(MAKE) $$DUNEMAKEFLAGS selftest
	./dune_selftest > allNodesTest.cpp
	./dune_selftest -x3d > allNodesTestX3d.cpp
	rm -f ./dune_selftest dune_selftest.exe
	sh -c "cd ../src;ls Node*.h | \
             awk '{print \"#include \\\"\" \$$0 \"\\\"\"}' > ../test/AllNodes.h"
	cd ../src && $(MAKE) $$DUNEMAKEFLAGS allNodesTest
	./dune_allNodesTest > allNodesTest.wrl
	rm -f dune_allNodesTest dune_allNodesTest.exe
	cd ../src && $(MAKE) $$DUNEMAKEFLAGS allNodesTestX3d
	./dune_allNodesTestX3d > allNodesTestX3d.x3dv
	cp allNodesTestX3d.x3dv /tmp
	rm -f dune_allNodesTestX3d dune_allNodesTestX3d.exe
	cd ../src && $(MAKE) $$DUNEMAKEFLAGS selftest
	./dune_selftest allNodesTestX3d.x3dv 2>/dev/null >/dev/null
	$(MAKE) clean
	cd ../src/swt && $(MAKE) $$DUNEMAKEFLAGS test
	cd ../src/swt && $(MAKE) $$DUNEMAKEFLAGS testclean
	$(MAKE) export_source_test

export_source_test: dune
	cd .. && $(MAKE)
	cd ../docs/export_example_c/simple_example && $(MAKE)
	../docs/export_example_c/simple_example/helloWorld
	cd ../docs/export_example_c/simple_example && $(MAKE) clean
	cd ../docs/export_example_c++/simple_example && $(MAKE)
	../docs/export_example_c++/simple_example/helloWorld
	cd ../docs/export_example_c++/simple_example && $(MAKE) clean
	cd ../docs/export_example_java/simple_example && $(MAKE)
	cd ../docs/export_example_java/simple_example && $(JAVA) -cp classes helloWorld
	cd ../docs/export_example_java/simple_example && $(MAKE) clean

realclean: clean
	rm -f /tmp/allNodesTestX3d.x3dv

clean:
	rm -f dune_selftest dune_selftest.exe \
              printFieldType printFieldType.exe \
              printNodeType printNodeType.exe \
              printFieldNumber printFieldNumber.exe \
	      *allNodesTest* ../src/allNodesTest* AllNodes.h \
	      dune_testprotos dune_testprotos.exe testprotos.cpp \
	      testzeroclass testzeroclass.exe testzeroclass.cpp

testzeroclass: dune
	sh testzeroclass.sh > testzeroclass.cpp
	cd ../src && $(MAKE) $$DUNEMAKEFLAGS testzeroclass
	./testzeroclass

printFieldType: dune
	cd ../src && $(MAKE) $$DUNEMAKEFLAGS ../test/printFieldType

printNodeType: dune
	cd ../src && $(MAKE) $$DUNEMAKEFLAGS ../test/printNodeType

printFieldNumber: dune
	cd ../src && $(MAKE) $$DUNEMAKEFLAGS ../test/printFieldNumber
