# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

# tested version of jogl is jogl-2.1

MANY_CLASSES=-manyclasses

# guess jogl2 path, if it is not working use
# export GLUEGEN_HOME=/some/path/
# export JOGL_HOME=/some/path/
JARS=$$GLUEGEN_HOME/gluegen2-rt.jar:$$JOGL_HOME/jogl2.jar:/usr/lib/java/gluegen2-rt.jar:/usr/lib/java/jogl2.jar:/usr/share/java/gluegen2-rt.jar:/usr/share/java/jogl2.jar:/usr/share/java/jogl/gluegen2-rt.jar:/usr/share/java/jogl/jogl2.jar:/usr/lib64/java/gluegen2-rt.jar:/usr/lib64/java/jogl2.jar

JFLAGS=-Xlint:unchecked -g 

# guess library path...
LIBS=-Djava.library.path=/usr/lib/jogl/:/usr/lib64/jogl2/:/usr/lib64/

MEM="-J-Xmx2000m"

run: classes/X3dSceneGraph.class classes/x3d.class 
	java $(LIBS) -cp .:classes:$(JARS) x3d

robot.java: robot.x3dv
	(../../../bin/dune -prefix X3d -java $(MANY_CLASSES) robot.x3dv > robot.java) || \
	dune -prefix X3d -java $(MANY_CLASSES) robot.x3dv > robot.java ; fi

classes/X3dSceneGraph.class: robot.java
	@if test "X_$$JOGL_HOME" = "X_" ; then echo environment variable '$$JOGL_HOME' should be set to the directory of jogl2.jar and native libraries 1>&2 ; fi
	@if test "X_$$GLUEGEN_HOME" = "X_" ; then echo environment variable '$$GLUEGEN_HOME' should be set to the directory of gluegen2-rt.jar 1>&2 ; fi
	javac $(JFLAGS) -d classes -cp $(JARS) $(MEM) robot.java

classes/x3d.class: x3d.java
	@if test "X_$$JOGL_HOME" = "X_" ; then echo environment variable '$$JOGL_HOME' should be set to the directory of jogl2.jar and native libraries 1>&2 ; fi
	@if test "X_$$GLUEGEN_HOME" = "X_" ; then echo environment variable '$$GLUEGEN_HOME' should be set to the directory of gluegen2-rt.jar 1>&2 ; fi
	javac $(JFLAGS) -d classes -cp $(JARS):classes:. $(MEM) x3d.java

debug: classes/X3dSceneGraph.class classes/x3d.class 
	jdb -Xmx812m $(LIBS) -classpath $(JARS):classes:. x3d

clean:
	rm -rf classes/* robot.java core.* hs_err_pid*.log
