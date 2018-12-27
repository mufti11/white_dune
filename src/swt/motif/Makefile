# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

CC = gcc
AWK = gawk
RANLIB = /usr/bin/ranlib 

LIBS =   -L/usr/local/lib -L/usr/local/lib -g -lXpm -lXm -lXt -lX11
INCDIRS = -I../.. -I../include  -I/usr/local/include -I/usr/local/include/eigenlib -I.//src -I.//vcglib -I.//vcglib/eigenlib -I/home/mufti/vcglib -I.//../vcglib -I/home/mufti/vcglib/eigenlib -I.//../vcglib/eigenlib -I/usr/include/freetype2/ -I/opt/X11/include/freetype2/  -I/usr/X11R6/include/ -I/usr/X11R6/LessTif/Motif1.2/include -I/usr/local/include -I/usr/local/include
# WARNINGS = -Wall -Wwrite-strings
CFLAGS =  -g $(WARNINGS) $(INCDIRS) 
DLIB = ../lib/libswt.so.0.1
SLIB = ../lib/libswt.a

target: $(SLIB)
#target: $(DLIB) $(SLIB)

OBJS = \
	swt.o \
	browser.o \
	tree.o \
	header.o \
	groupbox.o \
	statusbar.o \
	toolbar.o \
 	util.o \
	resload.o

SRCS = $(OBJS:o=c)

DISTFILES = $(SRCS) copy.xbm copy_mask.xbm link.xbm link_mask.xbm \
	    error.xbm error_mask.xbm dune.xbm dune_mask.xbm Makefile.in

$(DLIB):	$(OBJS)
		$(CC) -shared -o $(DLIB) $(OBJS)

$(SLIB):	$(OBJS)
	        rm -f $(SLIB)
		ar -r $(SLIB) $(OBJS)
		$(RANLIB) $(SLIB)

clean:
		rm -f $(OBJS) $(LIB) *.bak core


realclean: clean
	cp Makefile Makefile.bak
	awk "{ print; } /^# DO NOT DELETE/ { exit; }" < Makefile.bak > Makefile


dname:
		mkdir ../../`cat ../../.dname`/swt/motif
		ln $(DISTFILES) ../../`cat ../../.dname`/swt/motif

depend:
	cp Makefile Makefile.bak
	$(AWK) "{ print; } /^# DO NOT DELETE/ { exit; }" < Makefile.bak > Makefile
	if test "$(CC)" = "gcc" ; then \
           mkdep="gcc -MM";\
        else \
           if test "`uname`" = "SunOS" ; then\
              mkdep="$(CC) -xM";\
           else\
              if test "`uname`" = "HP-UX" ; then\
                  mkdep="$(CXX) +make -E";\
               else\
                  mkdep="$(CC) -M";\
              fi\
           fi\
        fi;\
        $$mkdep $(INCDIRS) $(SRCS) >> Makefile

efence:
	$(CC) -c $(CFLAGS) -DDO_NOT_X_DESTROY_IMAGE=1 swt.c
	make

# DO NOT DELETE this line -- it's make depend food
