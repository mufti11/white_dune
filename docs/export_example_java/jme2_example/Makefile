# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

# tested version of jME is jME2_0_1-Stable

JME_JAR=lib/jme.jar
JME_FONT_JAR=lib/jme-font.jar
JOGL=lib/jogl
LWJGL=lib/lwjgl
NATIVE=native/linux
MANY_CLASSES=-manyclasses

JARS=$$JME/lib/$(LWJGL)/lwjgl.jar:$$JME/lib/$(LWJGL)/lwjgl_util.jar:$$JME/lib/$(LWJGL)/lwjgl_util_applet.jar:$$JME/lib/$(JOGL)/gluegen-rt.jar:$$JME/lib/$(JOGL)/jogl.jar

JFLAGS=-Xlint:unchecked -g

run: classes/X3dSceneGraph.class classes/main.class 
	@if test "X_$$JME" = "X_" ; then echo environment variable '$$JME' must be set to the directory of $(JME_JAR) '(at least version jME2_0_1-Stable)' 1>&2 ; exit 1; fi
	java -Xms256m -Xmx1024m -XX:PermSize=64m -XX:MaxPermSize=128m -Djava.library.path="$$JME/lib/$(LWJGL)/$(NATIVE)/:" -cp classes:$$JME/$(JME_JAR):$$JME/$(JME_FONT_JAR):$(JARS) main

robot.java: robot.x3dv
	if test -x ../../../bin/dune ; then ../../../bin/dune -prefix X3d -3java $(MANY_CLASSES) robot.x3dv > robot.java ; else dune -prefix X3d -3java $(MANY_CLASSES) robot.x3dv > robot.java ; fi

test: robot.x3dv
	if test -x ../../../bin/dune ; then ../../../bin/dune -prefix X3d -3java robot.x3dv > robot.java ; else dune -prefix X3d -3java robot.x3dv > robot.java ; fi
	make

classes/X3dSceneGraph.class: robot.java
	@if test "X_$$JME" = "X_" ; then echo environment variable '$$JME' must be set to the directory of $(JME_JAR) '(at least version jME2_0_1-Stable)' 1>&2 ; exit 1; fi
	javac $(JFLAGS) -J-Xms256m -J-Xmx1024m -d classes -cp $$JME/$(JME_JAR) robot.java

classes/main.class: main.java
	javac $(JFLAGS) -J-Xms256m -J-Xmx1024m -d classes -cp $$JME/$(JME_JAR):$$JME/$(JME_FONT_JAR):classes main.java

debug: classes/X3dSceneGraph.class classes/main.class 
	@if test "X_$$JME" = "X_" ; then echo environment variable '$$JME' must be set to the directory of $(JME_JAR) '(at least version jME2_0_1-Stable)' 1>&2 ; exit 1; fi
	jdb -Xmx812m -Djava.library.path="$$JME/lib/$(LWJGL)/$(NATIVE)/:" -classpath .:classes:$(JARS):$$JME/$(JME_JAR):$$JME/$(JME_FONT_JAR) -Denv.class.path=.:classes:$(JARS):$$JME/$(JME_JAR):$$JME/$(JME_FONT_JAR) main

clean:
	rm -rf classes/* robot.java core.* hs_err_pid*.log
