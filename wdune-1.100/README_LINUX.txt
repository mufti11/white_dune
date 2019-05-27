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
When you use e.g. "gimp test.png" on the commandline, modify the image
and use File->Save, it doesn't write back the file.
In fact you can use File->Export instead of File->Save, but this is 
confusiong 8-( so the default uses a other image editor like pinta.
If you need to use gimp, simply replace the image editor setting
