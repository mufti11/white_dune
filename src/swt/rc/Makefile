# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

CC = gcc
AWK = gawk
LEX = flex --nounistd
YACC = bison -o y.tab.c

LIBS = 
LIBDIRS =
INCDIRS = -I../include
# WARNINGS = -Wall -Wwrite-strings
CFLAGS = -g $(WARNINGS) $(INCDIRS) -g -O0 -fopenmp
MAKEDEPEND=gcc -MM
YFLAGS = -d

TARGET = rc

OBJS = \
	rcparse.o \
	rclex.o \
	rc.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

SRCS = rc.c

DISTFILES = $(SRCS) rc.h rcparse.y rclex.l Makefile.in

clean:
		rm -f $(OBJS) rcparse.c rclex.c rc rc.exe

clobber:
		rm -f $(TARGET)

dname:
		mkdir ../../`cat ../../.dname`/swt/rc
		ln $(DISTFILES) ../../`cat ../../.dname`/swt/rc

depend:		$(SRCS)
		cp Makefile Makefile.bak
		$(AWK) "{ print; } /^# DO NOT DELETE/ { exit; }" < Makefile.bak > Makefile
		$(MAKEDEPEND) $(INCDIRS) $(SRCS) >> Makefile

y.tab.h:	rcparse.c

rclex.o:	y.tab.h rc.h

rcparse.o:	rc.h

# DO NOT DELETE this line -- it's make depend food
rc.o: rc.c rc.h
