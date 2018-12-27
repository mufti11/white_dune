# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

# tested version of jogl is jogl-2.1

MANY_CLASSES=-manyclasses

JARS=$$GLUEGEN_HOME/gluegen2-rt.jar:$$JOGL_HOME/jogl2.jar

JFLAGS=-Xlint:unchecked -g

run: classes/X3dSceneGraph.class classes/x3d.class 
	java -Djava.library.path=$$JOGL_HOME/ -cp .:classes:$(JARS) x3d

robot.java: robot.x3dv
	if test -x ../../../bin/dune ; then ../../../bin/dune -prefix X3d -java $(MANY_CLASSES) robot.x3dv > robot.java ; else dune -prefix X3d -java $(MANY_CLASSES) robot.x3dv > robot.java ; fi

classes/X3dSceneGraph.class: robot.java
	@if test "X_$$JOGL_HOME" = "X_" ; then echo environment variable '$$JOGL_HOME' must be set to the directory of jogl2.jar and native libraries 1>&2 ; exit 1; fi
	@if test "X_$$GLUEGEN_HOME" = "X_" ; then echo environment variable '$$GLUEGEN_HOME' must be set to the directory of gluegen2-rt.jar 1>&2 ; exit 1; fi
	javac $(JFLAGS) -d classes -cp $(JARS) robot.java

classes/x3d.class: x3d.java
	@if test "X_$$JOGL_HOME" = "X_" ; then echo environment variable '$$JOGL_HOME' must be set to the directory of jogl2.jar and native libraries 1>&2 ; exit 1; fi
	@if test "X_$$GLUEGEN_HOME" = "X_" ; then echo environment variable '$$GLUEGEN_HOME' must be set to the directory of gluegen2-rt.jar 1>&2 ; exit 1; fi
	javac $(JFLAGS) -d classes -cp $(JARS):classes:. x3d.java

debug: classes/X3dSceneGraph.class classes/x3d.class 
	jdb -Xmx812m -classpath $(JARS):classes:. x3d

clean:
	rm -rf classes/* robot.java core.* hs_err_pid*.log
