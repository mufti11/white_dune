# Generated automatically from Makefile.in by configure.
# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

CPP = cc -E
CC = gcc
AWK = gawk
CXXCPP = g++ -E
CXX = g++

LIBS =  -L/usr/local/lib ../../mysnprintf.o ../lib/libswt.a -lpng -ljpeg -lz -lglut -lGLU -lGL -L/usr/X11R6/lib -lXi -lXm -lXext -lXt -lXmu -lX11 -lm -ldl  -L/usr/local/lib32 -L/usr/freeware/lib32 -L/usr/lib32 

INCDIRS = -I../include

# WARNINGS = -Wall -Wwrite-strings
CFLAGS = -g $(WARNINGS) $(INCDIRS) -I.
CXXFLAGS = $(CFLAGS)

TARGETS = restest

OBJSCXX= \
        Dialog2.o \
        Keyboard.o \
        ElevationGridDialog2.o \
	restest.o 

.SUFFIXES: .cpp

.cpp.o:
	$(CXX) -g $(INCDIRS) $(CXXFLAGS) -c $<

SRCS= $(OBJS:o=c) $(OBJSCXX:o=cpp)

DISTFILES=	$(SRCS) Makefile.in res.rc bitmap1.bmp resource.h

targets:	$(TARGETS)

resource.c:	res.rc bitmap1.bmp
		sed 's.\\\\./.' < res.rc > res.rc.c
		$(CXXCPP) res.rc.c | grep -v '//' | ../rc/rc > resource.c
		rm res.rc.c

restest:	restest.o resource.o Dialog2.o Keyboard.o ElevationGridDialog2.o ../lib/libswt.a
		$(CXX) restest.o Dialog2.o Keyboard.o ElevationGridDialog2.o resource.o $(LIBS) -o restest

clean:
		rm -f $(OBJS) $(OBJSCXX) \
                      resource.c resource.o restest

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
