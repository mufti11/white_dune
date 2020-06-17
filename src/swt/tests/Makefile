# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

CPP = gcc -E
CC = gcc
AWK = gawk
CXXCPP = g++ -E
CXX = g++

LIBS =   -L/usr/local/lib -L/usr/local/lib -g ../../mysnprintf.o ../lib/libswt.a -lusb -lswresample -lswscale -lavformat -lavutil -lavcodec -ljpeg -lpng -lz -lexpat -lGLU -lGL -lcurl -lXi -lfreetype -lXm -lXext -lXt -lXmu -lX11 -lm -ldl -lgomp   -L/usr/local/lib  -L/usr/X11R6/LessTif/Motif1.2/lib -losdCPU  

INCDIRS = -I../include

# WARNINGS = -Wall -Wwrite-strings
CFLAGS = -g $(WARNINGS) $(INCDIRS)   -fopenmp 
CXXFLAGS = $(CFLAGS)

TARGETS = swTest text buttons doodle drawing tree restest draw1

OBJS=	\
	swTest.o \
	text.o \
	buttons.o \
	doodle.o \
	drawing.o \
	tree.o \
	restest.o 

OBJSCXX= \
        draw1.o

.SUFFIXES: .cpp

.cpp.o:
	$(CXX) -g $(INCDIRS) $(CXXFLAGS) -c $<

SRCS= $(OBJS:o=c) $(OBJSCXX:o=cpp)

DISTFILES=	$(SRCS) Makefile.in res.rc bitmap1.bmp resource.h

targets:	$(TARGETS)

swTest:		swTest.o ../lib/libswt.a
		$(CC) swTest.o $(LIBS) -o swTest

text:		text.o ../lib/libswt.a
		$(CC) text.o $(LIBS) -o text

buttons:	buttons.o ../lib/libswt.a
		$(CC) buttons.o $(LIBS) -o buttons

doodle:		doodle.o ../lib/libswt.a
		$(CC) doodle.o $(LIBS) -o doodle

drawing:	drawing.o ../lib/libswt.a
		$(CC) drawing.o $(LIBS) -o drawing

tree:		tree.o ../lib/libswt.a
		$(CC) tree.o $(LIBS) -o tree

draw1:		draw1.o ../lib/libswt.a
		$(CXX) draw1.o $(LIBS) -o draw1

resource.c:	res.rc bitmap1.bmp
		sed 's.\\\\./.' < res.rc > res.rc.c
		$(CXXCPP) res.rc.c | grep -v '//' | ../rc/rc > resource.c
		rm res.rc.c

restest:	restest.o resource.o ../lib/libswt.a
		$(CC) restest.o resource.o $(LIBS) -o restest

clean:
		rm -f $(OBJS) $(OBJSCXX) \
                      resource.c resource.o \
                      drawing swTest doodle restest \
                      drawing.exe swTest.exe doodle.exe restest.exe \
	              tree buttons text draw1 \
	              tree.exe buttons.exe text.exe draw1.exe

realclean: clean
	cp Makefile Makefile.bak
	awk '{ print } /^# DO NOT DELETE THIS LINE/ { print ""; exit }' < Makefile.bak > Makefile

clobber:
		rm -f $(TARGETS)

dname:		$(DISTFILES)
		mkdir ../../`cat ../../.dname`/swt/tests
		ln $(DISTFILES) ../../`cat ../../.dname`/swt/tests

depend:
		cp Makefile Makefile.bak
		$(AWK) "{ print; } /^# DO NOT DELETE/ { exit; }" < Makefile.bak > Makefile

		gcc -MM $(INCDIRS) $(SRCS) >> Makefile

# DO NOT DELETE this line -- it's make depend food
