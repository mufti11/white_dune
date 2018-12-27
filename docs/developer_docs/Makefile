# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

all: man2html doxygen

doxygen: 
	sh mkdoxygen.sh

man2html:
	sh man2html.sh dune.1 > ../usage_docs/dune_1.html
	sh man2html.sh illegal2vrml.1 > ../usage_docs/illegal2vrml_1.html

realclean:
	rm -rf doxygen_out_html

jpgdoc:
	rm -rf /tmp/white_dune_docs
	mkdir /tmp/white_dune_docs
	cp -r ../../docs /tmp/white_dune_docs
	sh ./png2jpg.sh developer_docs dune_developer.html
	sh ./png2jpg.sh usage_docs dune_de.html
	sh ./png2jpg.sh usage_docs dune_en.html
	sh ./png2jpg.sh usage_docs dune_ru.html
	sh ./png2jpg.sh usage_docs dune_de_exercise.html
	sh ./png2jpg.sh usage_docs dune_en_exercise.html
	sh ./png2jpg.sh usage_docs dune_de_tutorial.html

documentation: all
