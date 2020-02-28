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
SDL_JOYSTICK_SUBDIR = darwin
LIBS =  -L/Users/j.scheurich/lib/ -L/usr/local/lib -L/usr/local/lib -g -ljpeg -lpng -lz -lexpat -lGLU -lGL -lcurl -lXi -lmpfr -lgmp -lfreetype -lXm -lXp -lXext -lXt -lXmu -lX11 -lm -ldl -L/usr/X11R6/lib     -L/usr/X11/lib -multiply_defined warning -lXft -lXrender -lfontconfig   -framework IOKit -lCGAL -lboost_thread -lboost_system -losdCPU /usr/lib/libcrypto.dylib /usr/lib/libssl.dylib 
AWK = awk

CFLAGS = -I/Users/j.scheurich/include -I/usr/local/include -I/usr/local/include/eigenlib -I/usr/include/vcglib -I.//src -I.//vcglib -I.//vcglib/eigenlib -I/Users/j.scheurich/vcglib -I.//../vcglib -I/Users/j.scheurich/vcglib/eigenlib -I.//../vcglib/eigenlib -I/usr/include/freetype2/ -I/opt/X11/include/freetype2/ -I/usr/include/ffmpeg/   -I/usr/X11/include -I/usr/X11R6/include -DMACOSX=1 -I/usr/local/include -I/usr/local/include   -I$$PWD -I$$PWD/..  
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

