DISTFILES = \
	halftone.bmp \
	emptySocketSquare.bmp \
	fullSocketSquare.bmp \
	node_icons.bmp \
	vcr.bmp \
	standard_toolbar.bmp \
	dune.ico \
	Makefile

all:
	(cd .. && make)

dname:
	mkdir ../`cat ../.dname`/res
	ln $(DISTFILES) ../`cat ../.dname`/res
