# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

INSTALLDIR=/usr/lib/filetype/install

ICONCATDIR=/usr/lib/desktop/iconcatalog/pages

INSTDIRS=$(INSTALLDIR) $(ICONCATDIR) $(INSTALLDIR)/iconlib

FTRFILES=$(INSTALLDIR)/Dune.ftr 

ICONLIBFILES=$(INSTALLDIR)/iconlib/Dunebackground.fti \
             $(INSTALLDIR)/iconlib/Dune.fti \
             $(INSTALLDIR)/iconlib/Dune.open.fti \
             $(INSTALLDIR)/iconlib/Dune4kids.fti \
             $(INSTALLDIR)/iconlib/Dune4kids.open.fti \
             $(INSTALLDIR)/iconlib/nostereoDune.fti \
             $(INSTALLDIR)/iconlib/nostereoDune.open.fti 

ICONCATFILES=$(ICONCATDIR)/WebTools/dune \
             $(ICONCATDIR)/WebTools/nostereodune \
             $(ICONCATDIR)/WebTools/fm_cosmo2vrml97 

INSTFILES=$(FTRFILES) $(ICONLIBFILES) # $(ICONCATFILES)

DATA=../data

$(INSTALLDIR)/filetype/install.otr: $(INSTFILES)
	sh -c "(cd $(INSTALLDIR)/.. && make)"

Dune.ftr: Dune.ftr.m4 duneoptions.m4
	m4 duneoptions.m4 Dune.ftr.m4 > Dune.ftr

$(INSTALLDIR)/Dune.ftr: Dune.ftr $(INSTDIRS) 
	install -m 644 -f $(INSTALLDIR) Dune.ftr 

$(INSTALLDIR)/iconlib/Dunebackground.fti: $(DATA)/Dunebackground.fti $(INSTDIRS)
	(cd $(DATA) && install -m 644 -f $(INSTALLDIR)/iconlib Dunebackground.fti)

$(INSTALLDIR)/iconlib/Dune.fti: $(DATA)/Dune.fti $(INSTDIRS)
	(cd $(DATA) && install -m 644 -f $(INSTALLDIR)/iconlib Dune.fti)

$(INSTALLDIR)/iconlib/Dune.open.fti: $(DATA)/Dune.open.fti $(INSTDIRS)
	(cd $(DATA) && install -m 644 -f $(INSTALLDIR)/iconlib Dune.open.fti)
	
$(INSTALLDIR)/iconlib/Dune4kids.fti: $(DATA)/Dune4kids.fti $(INSTDIRS)
	(cd $(DATA) && install -m 644 -f $(INSTALLDIR)/iconlib Dune4kids.fti)

$(INSTALLDIR)/iconlib/Dune4kids.open.fti: $(DATA)/Dune4kids.open.fti $(INSTDIRS)
	(cd $(DATA) && install -m 644 -f $(INSTALLDIR)/iconlib Dune4kids.open.fti)
	
$(INSTALLDIR)/iconlib/nostereoDune.fti: $(DATA)/nostereoDune.fti $(INSTDIRS)
	(cd $(DATA) && install -m 644 -f $(INSTALLDIR)/iconlib nostereoDune.fti) 

$(INSTALLDIR)/iconlib/nostereoDune.open.fti: $(DATA)/nostereoDune.open.fti $(INSTDIRS)
	(cd $(DATA) && install -m 644 -f $(INSTALLDIR)/iconlib nostereoDune.open.fti)
	
$(INSTALLDIR)/install:
	mkdir -p $(INSTALLDIR)/install

$(INSTALLDIR)/iconlib:
	 mkdir -p $(INSTALLDIR)/iconlib

$(INSTALLDIR):
	mkdir -p $(INSTALLDIR)

#$(ICONCATDIR)/WebTools/dune:
#	mkdir -p $(ICONCATDIR)/WebTools
#	ln -sf /usr/freeware/bin/dune $@

#$(ICONCATDIR)/WebTools/nostereodune:
#	mkdir -p $(ICONCATDIR)/WebTools
#	ln -sf /usr/freeware/bin/nostereodune $@

#$(ICONCATDIR)/WebTools/fm_cosmo2vrml97:
#	mkdir -p $(ICONCATDIR)/WebTools
#	ln -sf /usr/freeware/bin/fm_cosmo2vrml97 $@



