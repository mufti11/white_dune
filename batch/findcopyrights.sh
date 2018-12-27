#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

echo for all .c/.h/.cpp/.m4 source files:

if test -x `dirname $0`/../bin/dune; then
   `dirname $0`/../bin/dune --copyrightdetails
else
   dune --copyrightdetails
   if test "$?" != "0"; then
      echo run \"dune --copyrightdetails\" for copyright details
   fi
fi

TMPFILE=/tmp/findcopyrights_$$

find `dirname $0`/.. -type f -print > $TMPFILE

awk -v dir=`dirname $0`/.. '
   function chompdir(str) {
      sub(dir "/", "", str)
      return str
   }
   { flag=0 }
   $1~/libC\+\+RWD\.h$/ { flag=1; print "file: " $1 "\nCopyright: (c) Stefan Wolf, 2010.\nLicense: Modified BSD style\nRedistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:\n1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.\n2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.\n3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.\n\nTHIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF\nMERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,\nINCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS\nOF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR\nTORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n\n" }
   $1~/libC\+\+RWD\.cpp$/ { flag=1; print "file: " $1 "\nCopyright: (c) Stefan Wolf, 2010.\nLicense: Modified BSD style\nRedistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:\n1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.\n2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.\n3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.\n\nTHIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF\nMERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,\nINCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS\nOF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR\nTORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n\n" }
   $1~/libC\+\+RWD\/main.cpp$/ { flag=1; print "file: " $1 "\nCopyright: (c) Mark J. Kilgard, 1997\nCopyright: (c) 1993, Silicon Graphics, Inc.\nLicense: Custom\n/* ALL RIGHTS RESERVED\n * Permission to use, copy, modify, and distribute this software for \n * any purpose and without fee is hereby granted, provided that the above\n * copyright notice appear in all copies and that both the copyright notice\n * and this permission notice appear in supporting documentation, and that \n * the name of Silicon Graphics, Inc. not be used in advertising\n * or publicity pertaining to distribution of the software without specific,\n * written prior permission. \n *\n * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"\n * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,\n * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR\n * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON\n * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,\n * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY\n * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,\n * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF\n * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN\n * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON\n * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE\n * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.\n * \n * US Government Users Restricted Rights \n * Use, duplication, or disclosure by the Government is subject to\n * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph\n * (c)(1)(ii) of the Rights in Technical Data and Computer Software\n * clause at DFARS 252.227-7013 and/or in similar or successor\n * clauses in the FAR or the DOD or NASA FAR Supplement.\n * Unpublished-- rights reserved under the copyright laws of the\n * United States.  Contractor/manufacturer is Silicon Graphics,\n * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.\n *\n * OpenGL(TM) is a trademark of Silicon Graphics, Inc.\n */\n\n" }
   $1~/libCRWD.h$/ { flag=1; print "file: " $1 "\nCopyright: (c) Stefan Wolf, 2010.\nCopyright: (c) J. "MUFTI" Scheurich, 2010.\nLicense: Modified BSD style\nRedistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:\n1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.\n2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.\n3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.\n\nTHIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF\nMERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,\nINCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS\nOF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR\nTORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n" }
   $1~/libCRWD\.c$/ { flag=1; print "file:" $1 "\nCopyright: (c) Stefan Wolf, 2010.\nCopyright: (c) J. "MUFTI" Scheurich, 2010.\nLicense: Modified BSD style\nRedistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:\n1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.\n2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.\n3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.\n\nTHIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF\nMERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,\nINCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS\nOF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR\nTORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n" }
   $1~/libCRWD\/main.c$/ { flag=1; print "file:" $1 "\nCopyright: (c) Mark J. Kilgard, 1997\nCopyright: (c) 1993, Silicon Graphics, Inc.\nLicense: Custom\n/* ALL RIGHTS RESERVED\n * Permission to use, copy, modify, and distribute this software for \n * any purpose and without fee is hereby granted, provided that the above\n * copyright notice appear in all copies and that both the copyright notice\n * and this permission notice appear in supporting documentation, and that \n * the name of Silicon Graphics, Inc. not be used in advertising\n * or publicity pertaining to distribution of the software without specific,\n * written prior permission. \n *\n * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"\n * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,\n * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR\n * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON\n * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,\n * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY\n * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,\n * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF\n * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN\n * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON\n * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE\n * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.\n * \n * US Government Users Restricted Rights \n * Use, duplication, or disclosure by the Government is subject to\n * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph\n * (c)(1)(ii) of the Rights in Technical Data and Computer Software\n * clause at DFARS 252.227-7013 and/or in similar or successor\n * clauses in the FAR or the DOD or NASA FAR Supplement.\n * Unpublished-- rights reserved under the copyright laws of the\n * United States.  Contractor/manufacturer is Silicon Graphics,\n * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.\n *\n * OpenGL(TM) is a trademark of Silicon Graphics, Inc.\n */\n\n" }
   $1~/Dampers\.wrl$/ { flag=1; print "file: " $1 "\nCopyright: Herbert Stocker\nLicense: Custom\nPlease use the code you find here in any content or application you like\nor modify it in any way\n" }
   $1~/10x20\.pcf\.gz$/ { flag=1; print "file: " $1 "\nLicence: Custom\n10x20.pcf.gz contains the string \"Public domain font.  Share and enjoy.\"\n" }
   $1~/test_texture\.png$/ { flag=1; print "file: " $1 "\nLicence: Custom\nfrom Copyright-Free Photo Archive: http://gimp-savvy.com/PHOTO-ARCHIVE/ Public domain.\n" }
   $1~/remove_containerField\.py/ { flag=1; print "\nLicence: Custom\nLicense of " $1 "\n\nremove_containerField.py declared public domain by author\n" } 
   $1~/xsltransformjs\.js/ { flag=1; print "file: " $1 "\nLicence: Custom\nLicense of " $1 "\n\nxsltransformjs.js original author declares xlstransformjs.js public domain\n" } 
   $1~/xsl_js\.bat/ { flag=1; print "file: " $1 "\nLicence: Custom\nLicense of " $1 "\n\noriginal author declares xsl_js.bat public domain\n" }
   # matched by --copyright
   $1~/\.h$/ { flag=1 }
   $1~/\.cpp$/ { flag=1 }
   $1~/\.o$/ { flag=1 }
   $1~/\.a$/ { flag=1 }
   $1~/BooleanSequencerExample\.wrl$/ { flag=1 }
   $1~/BooleanSequencerPROTO\.wrl$/ { flag=1 }
   $1~/IntegerSequencerPROTO\.wrl$/ { flag=1 }
   $1~/BooleanFilterPROTO\.wrl$/ { flag=1 }
   $1~/BooleanTogglePROTO\.wrl$/ { flag=1 }
   $1~/BooleanTriggerPROTO\.wrl$/ { flag=1 }
   $1~/IntegerTriggerPROTO\.wrl$/ { flag=1 }
   $1~/TimeTriggerPROTO\.wrl$/ { flag=1 }
   $1~/Arc2DPROTO\.wrl$/ { flag=1 }
   $1~/ArcClose2DPROTO\.wrl$/ { flag=1 }
   $1~/Circle2DPROTO\.wrl$/ { flag=1 }
   $1~/Disk2DPROTO\.wrl$/ { flag=1 }
   $1~/Polyline2DPROTO\.wrl$/ { flag=1 }
   $1~/Polypoint2DPROTO\.wrl$/ { flag=1 }
   $1~/Rectangle2DPROTO\.wrl$/ { flag=1 }
   $1~/TriangleSet2DPROTO\.wrl$/ { flag=1 }
   # copyright in /lib/textures/
   $1~/lib\/textures/ { flag=1; libtextures = libtextures "\n" $1 }
   $1~/\.jpg$/ || $1~/\.png$/ { flag=1;
                 if (system("identify -verbose " $1 " | grep \"the Free Software Foundation gives unlimited\" > /dev/null")==0)
                    simple[simpleLen++] = chompdir($1)
                 else if (system("identify -verbose " $1 " | grep \"under the terms of the GNU General Public\" > /dev/null")==0)
                    gnu[gnuLen++] = chompdir($1);
               }
   $1~/xj3dFmaxJoints/ { flag=1;lgnu[lgnuLen++] = chompdir($1);}
   $1~/\.tar$/ { flag=1;gnu[gnuLen++] = chompdir($1); }

   { 
      if (flag == 0) { 
         # simple permissive copyright
         if (system("grep \"unlimited permission to copy, distribute and modify it\" " $1 " > /dev/null")==0)
            simple[simpleLen++] = chompdir($1);
         else if (system("grep \"under the terms of the GNU General Public\" " $1 " > /dev/null")==0)
            gnu[gnuLen++] = chompdir($1);
         else if (system("grep \"under the terms of the GNU Library General Public\" " $1 " > /dev/null")==0)
            lgnu[lgnuLen++] = chompdir($1);
         else if (system("grep \"licensed under the GNU LGPL v2.1\" " $1 " > /dev/null")==0)
            lgnu[lgnuLen++] = chompdir($1);
         else
            rest[restLen++] = chompdir($1)
      }
   }

END { 
   print ""
   print "The following statement is about the files in /lib/textures/README"
   system(" cat " dir "/lib/textures/README")
   print ""
   print "The following files use the GNU General Public License:"
   if (ARGV[1] == "debian")
      print "See /usr/share/common-licenses/GPL for the full license"
   print ""
   for (i = 0; i < gnuLen; i++)
        print gnu[i]
   print ""
   print "The following files use the GNU Library General Public License:"
   if (ARGV[1] == "debian")
      print "See /usr/share/common-licenses/LGPL for the full license"
   print ""
   for (i = 0; i < lgnuLen; i++)
        print lgnu[i]
   print ""
   print "The following files use the copyright statement:"
   print ""
   print "The/This file/Makefile/script/program is free software; the Free Software" 
   print "Foundation gives unlimited permission to copy, distribute and modify it."
   print ""
   for (i = 0; i < simpleLen; i++)
        print simple[i]
   print ""
   for (i = 0; i < restLen; i++)
        print rest[i]
}' $TMPFILE

rm $TMPFILE
