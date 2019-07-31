# Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

CC = gcc
CXX = g++
SDL_JOYSTICK_SUBDIR = 
LIBS =   -L/usr/local/lib -L/usr/local/lib -g -lusb -lsball -lswresample -lswscale -lavformat -lavutil -lavcodec -lpng -ljpeg -lz -lexpat -lGLU -lGL -lcurl -lXi -lmpfr -lgmp -lfreetype -lXm -lXext -lXt -lXmu -lX11 -lm -ldl -lgomp   -L/usr/local/lib  -L/usr/X11R6/LessTif/Motif1.2/lib -lCGAL -lboost_thread -lboost_system -losdCPU 
AWK = gawk

CFLAGS =  -I/usr/local/include -I/usr/local/include/eigenlib -I/usr/include/vcglib -I.//src -I.//vcglib -I.//vcglib/eigenlib -I/home/mufti/vcglib -I.//../vcglib -I/home/mufti/vcglib/eigenlib -I.//../vcglib/eigenlib -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/ffmpeg/   -I/usr/X11R6/include/ -I/usr/X11R6/LessTif/Motif1.2/include -I/usr/local/include -I/usr/local/include   -I$$PWD -I$$PWD/..  -fopenmp
RANLIB = /usr/bin/ranlib

OBJS =  mini_SDL_joystick.o $(SDL_JOYSTICK_SUBDIR)/SDL_sysjoystick.o

libSDLjoystick.a: $(OBJS) 
	if test -f SDL_sysjoystick.o ; then mv SDL_sysjoystick.o $(SDL_JOYSTICK_SUBDIR)/SDL_sysjoystick.o ; fi 
	ar -r $@ $(OBJS)
	$(RANLIB) $@ 

joysticktest: joysticktest.c libSDLjoystick.a
	$(CC) -o $@ $(CFLAGS) joysticktest.c libSDLjoystick.a $(LIBS)

clean:
	rm -f joysticktest libSDLjoystick.a *.o */*.o

realclean: clean
	cp Makefile Makefile.bak
	awk '{ print } /^# DO NOT DELETE THIS LINE/ { print ""; exit }' < Makefile.bak > Makefile

depend: 
	$(CC) -M -I$$PWD $(SDL_JOYSTICK_SUBDIR)/SDL_sysjoystick.c >> Makefile

# DO NOT DELETE THIS LINE -- make depend depends on it.

