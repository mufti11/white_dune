# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

JAVAC = echo

all: classes/helloWorld.class

X3dSceneGraph.java: helloWorld.x3dv helloWorld2.x3dv ../../../bin/dune
	../../../bin/dune -prefix Pre_ -java helloWorld.x3dv > X3dSceneGraph.java
	../../../bin/dune -prefix Pre2_ -java helloWorld2.x3dv >> X3dSceneGraph.java
	
helloWorld.java: mainHelloWorld.java X3dSceneGraph.java
	cat X3dSceneGraph.java mainHelloWorld.java > helloWorld.java

classes/helloWorld.class: helloWorld.java X3dSceneGraph.java
	$(JAVAC) -d classes -g helloWorld.java 

../../../bin/dune:
	(cd ../../.. && make)

clean:
	rm -f classes/*.class X3dSceneGraph.java helloWorld.java a.out

allNodesTest: allNodesTestX3d.x3dv ../../../bin/dune
	../../../bin/dune -java allNodesTestX3d.x3dv > X3dSceneGraph.java
	cat X3dSceneGraph.java mainHelloWorld.java > helloWorld.java
	$(JAVAC) -d classes -g helloWorld.java 

