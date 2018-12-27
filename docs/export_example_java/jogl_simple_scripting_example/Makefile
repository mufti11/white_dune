# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

# tested version of jME is jME2_0_1-Stable

JOGL_HOME_JARS=$$JOGL_HOME/jar
MANY_CLASSES=-manyclasses

JARS=$(JOGL_HOME_JARS)/gluegen-rt.jar:$(JOGL_HOME_JARS)/jogl-all.jar

JFLAGS=-Xlint:unchecked -g

run: classes/X3dSceneGraph.class classes/x3d.class 
	java -cp .:classes:$(JARS) x3d

ScriptingExample.java: ScriptingExample.wrl
	if test -x ../../../bin/dune ; then ../../../bin/dune -prefix X3d -java $(MANY_CLASSES) ScriptingExample.wrl > ScriptingExample.java ; else dune -prefix X3d -java $(MANY_CLASSES) ScriptingExample.wrl > ScriptingExample.java ; fi

test: ScriptingExample.x3dv
	if test -x ../../../bin/dune ; then ../../../bin/dune -prefix X3d -java ScriptingExample.x3dv > ScriptingExample.java ; else dune -prefix X3d -java ScriptingExample.x3dv > ScriptingExample.java ; fi
	make

classes/X3dSceneGraph.class: ScriptingExample.java
	@if test "X_$$JOGL_HOME" = "X_" ; then echo environment variable '$$JOGL_HOME' must be set to the directory of jar/gluegen-rt.jar and jar/jogl-all.jar 1>&2 ; exit 1; fi
	javac $(JFLAGS) -d classes -cp $(JARS) ScriptingExample.java

classes/x3d.class: ../jogl_example/x3d.java
	@if test "X_$$JOGL_HOME" = "X_" ; then echo environment variable '$$JOGL_HOME' must be set to the directory of jar/gluegen-rt.jar and jar/jogl-all.jar 1>&2 ; exit 1; fi
	javac $(JFLAGS) -d classes -cp $(JARS):classes:. ../jogl_example/x3d.java

debug: classes/X3dSceneGraph.class classes/x3d.class 
	jdb -Xmx812m -classpath $(JARS):classes:. x3d

clean:
	rm -rf classes/* ScriptingExample.java core.* hs_err_pid*.log
