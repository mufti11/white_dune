Often the default X11 font is "fixed", which ist far to small eg.
on a 1024x600 pixel monitor (netbook). Start dune in the commandline. 
on this netbook with the option -fn e.g. 

$ dune -fn lucidasanstypewriter-bold-12

This works with the Fedora 19 netbook.
to search for other sized fonts you can use something like 

$ xlsfonts | grep lucidasanstypewriter-bold-

Some systems (like debian wheezy) support something like

$ dune -fn 10x20

======================================
Why not gimp as default image editor ? 
======================================

gimp is a good image editor, but it is not a good tool for commandline.
When you use e.g. "gimp test.png" on the commandline, you get e.g. with 
the gimp of fedora 19 or debian wheezy the following errormessage:

GIMP Message

You can use this dialog to save to the GIMP XCF
format. Use File->Export to export to other file formats.

In fact you can use File->Export instead of File->Save, but this is 
confusiong 8-( so the default uses a other image editor like pinta.
If you need to use gimp, simply replace the image editor setting
