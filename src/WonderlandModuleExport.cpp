/*
 * WonderlandModuleExport.cpp
 *
 * Copyright (C) 2009 J. "MUFTI" Scheurich
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (see the file "COPYING" for details); if 
 * not, write to the Free Software Foundation, Inc., 675 Mass Ave, 
 * Cambridge, MA 02139, USA.
 */

/*
 * Copyright (c) 2004-2009, Sun Microsystems, Inc., All Rights Reserved
 *
 * Redistributions in source code form must reproduce the above
 * copyright and this condition.
 *
 * The contents of this file are subject to the GNU General Public
 * License, Version 2 (the "License"); you may not use this file
 * except in compliance with the License. A copy of the License is
 * available at http://www.opensource.org/licenses/gpl-license.php.
 *\n") )
 * Sun designates this particular file as subject to the "Classpath"
 * exception as provided by Sun in the License file that accompanied
 * this code.
 */

// The license file:

/*
The GNU General Public License (GPL) Version 2, June 1991
Copyright (C) 1989, 1991 Free Software Foundation, Inc.
59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
Everyone is permitted to copy and distribute verbatim copies
of this license document, but changing it is not allowed.

Preamble

The licenses for most software are designed to take away your freedom to share
and change it. By contrast, the GNU General Public License is intended to
guarantee your freedom to share and change free software--to make sure the
software is free for all its users. This General Public License applies to most
of the Free Software Foundation's software and to any other program whose author
commit to using it. (Some other Free Software Foundation software is covered by
the GNU Library General Public License instead.) You can apply it to your
programs, too.

When we speak of free software, we are referring to freedom, not price. Our
General Public Licenses are designed to make sure that you have the freedom to
distribute copies of free software (and charge for this service if you wish),
that you receive source code or can get it if you want it, that you can change
the software or use pieces of it in new free programs; and that you know you
can do these things.

To protect your rights, we need to make restrictions that forbid anyone to deny
you these rights or to ask you to surrender the rights. These restrictions
translate to certain responsibilities for you if you distribute copies of the
software, or if you modify it.

For example, if you distribute copies of such a program, whether gratis or for
a fee, you must give the recipients all the rights that you have. You must make
sure that they, too, receive or can get the source code. And you must show them
these terms so they know their rights.

We protect your rights with two steps: (1) copyright the software, and (2) offer
you this license which gives you legal permission to copy, distribute and/or
modify the software.

Also, for each author's protection and ours, we want to make certain that
everyone understands that there is no warranty for this free software. If the
software is modified by someone else and passed on, we want its recipients to
know that what they have is not the original, so that any problems introduced
by others will not reflect on the original authors' reputations.

Finally, any free program is threatened constantly by software patents. We wish 
to avoid the danger that redistributors of a free program will individually
obtain patent licenses, in effect making the program proprietary. To prevent
this, we have made it clear that any patent must be licensed for everyone's
free use or not licensed at all.

The precise terms and conditions for copying, distribution and modification
follow.

TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

0. This License applies to any program or other work which contains a notice
placed by the copyright holder saying it may be distributed under the terms of
this General Public License. The "Program", below, refers to any such program
or work, and a "work based on the Program" means either the Program or any
derivative work under copyright law: that is to say, a work containing the
Program or a portion of it, either verbatim or with modifications and/or
translated into another language. (Hereinafter, translation is included without
limitation in the term "modification".) Each licensee is addressed as "you".

Activities other than copying, distribution and modification are not covered by
this License; they are outside its scope. The act of running the Program is not
restricted, and the output from the Program is covered only if its contents
constitute a work based on the Program (independent of having been made by
running the Program). Whether that is true depends on what the Program does.

1. You may copy and distribute verbatim copies of the Program's source code as
you receive it, in any medium, provided that you conspicuously and appropriately
publish on each copy an appropriate copyright notice and disclaimer of warranty;
 keep intact all the notices that refer to this License and to the absence of
any warranty; and give any other recipients of the Program a copy of this
License along with the Program.

You may charge a fee for the physical act of transferring a copy, and you may
at your option offer warranty protection in exchange for a fee.

2. You may modify your copy or copies of the Program or any portion of it, thus
forming a work based on the Program, and copy and distribute such modifications
or work under the terms of Section 1 above, provided that you also meet all of
these conditions:

a) You must cause the modified files to carry prominent notices stating that
you changed the files and the date of any change.

b) You must cause any work that you distribute or publish, that in whole or in
part contains or is derived from the Program or any part thereof, to be licensed
as a whole at no charge to all third parties under the terms of this License.

c) If the modified program normally reads commands interactively when run, you
must cause it, when started running for such interactive use in the most
ordinary way, to print or display an announcement including an appropriate
copyright notice and a notice that there is no warranty (or else, saying that
you provide a warranty) and that users may redistribute the program under these
conditions, and telling the user how to view a copy of this License. (Exception:
if the Program itself is interactive but does not normally print such an
announcement, your work based on the Program is not required to print an
announcement.)

These requirements apply to the modified work as a whole. If identifiable
sections of that work are not derived from the Program, and can be reasonably
considered independent and separate works in themselves, then this License, and
its terms, do not apply to those sections when you distribute them as separate
works. But when you distribute the same sections as part of a whole which is a
work based on the Program, the distribution of the whole must be on the terms of
this License, whose permissions for other licensees extend to the entire whole,
and thus to each and every part regardless of who wrote it.

Thus, it is not the intent of this section to claim rights or contest your
rights to work written entirely by you; rather, the intent is to exercise the
right to control the distribution of derivative or collective works based on
the Program.

In addition, mere aggregation of another work not based on the Program with the
Program (or with a work based on the Program) on a volume of a storage or
distribution medium does not bring the other work under the scope of this
License.

3. You may copy and distribute the Program (or a work based on it, under
Section 2) in object code or executable form under the terms of Sections 1 and
2 above provided that you also do one of the following:

a) Accompany it with the complete corresponding machine-readable source code,
which must be distributed under the terms of Sections 1 and 2 above on a medium
customarily used for software interchange; or,

b) Accompany it with a written offer, valid for at least three years, to give
any third party, for a charge no more than your cost of physically performing
source distribution, a complete machine-readable copy of the corresponding
source code, to be distributed under the terms of Sections 1 and 2 above on a
medium customarily used for software interchange; or,

c) Accompany it with the information you received as to the offer to distribute
corresponding source code. (This alternative is allowed only for noncommercial
distribution and only if you received the program in object code or executable
form with such an offer, in accord with Subsection b above.)

The source code for a work means the preferred form of the work for making
modifications to it. For an executable work, complete source code means all
the source code for all modules it contains, plus any associated interface
definition files, plus the scripts used to control compilation and installation
of the executable. However, as a special exception, the source code distributed
need not include anything that is normally distributed (in either source or
binary form) with the major components (compiler, kernel, and so on) of the
operating system on which the executable runs, unless that component itself
accompanies the executable.

If distribution of executable or object code is made by offering access to copy
from a designated place, then offering equivalent access to copy the source
code from the same place counts as distribution of the source code, even though
third parties are not compelled to copy the source along with the object code.

4. You may not copy, modify, sublicense, or distribute the Program except as
expressly provided under this License. Any attempt otherwise to copy, modify,
sublicense or distribute the Program is void, and will automatically terminate
your rights under this License. However, parties who have received copies, or
rights, from you under this License will not have their licenses terminated so
long as such parties remain in full compliance.

5. You are not required to accept this License, since you have not signed it.
However, nothing else grants you permission to modify or distribute the Program
or its derivative works. These actions are prohibited by law if you do not
accept this License. Therefore, by modifying or distributing the Program (or
any work based on the Program), you indicate your acceptance of this License to
do so, and all its terms and conditions for copying, distributing or modifying
the Program or works based on it.

6. Each time you redistribute the Program (or any work based on the Program),
the recipient automatically receives a license from the original licensor to
copy, distribute or modify the Program subject to these terms and conditions.
You may not impose any further restrictions on the recipients' exercise of the
rights granted herein. You are not responsible for enforcing compliance by
third parties to this License.

7. If, as a consequence of a court judgment or allegation of patent infringement
or for any other reason (not limited to patent issues), conditions are imposed
on you (whether by court order, agreement or otherwise) that contradict the
conditions of this License, they do not excuse you from the conditions of this
License. If you cannot distribute so as to satisfy simultaneously your
obligations under this License and any other pertinent obligations, then as a
consequence you may not distribute the Program at all. For example, if a patent
license would not permit royalty-free redistribution of the Program by all
those who receive copies directly or indirectly through you, then the only way
you could satisfy both it and this License would be to refrain entirely from
distribution of the Program.

If any portion of this section is held invalid or unenforceable under any
particular circumstance, the balance of the section is intended to apply and
the section as a whole is intended to apply in other circumstances.

It is not the purpose of this section to induce you to infringe any patents or
other property right claims or to contest validity of any such claims; this
section has the sole purpose of protecting the integrity of the free software
distribution system, which is implemented by public license practices. Many
people have made generous contributions to the wide range of software
distributed through that system in reliance on consistent application of that
system; it is up to the author/donor to decide if he or she is willing to
distribute software through any other system and a licensee cannot impose that
choice.

This section is intended to make thoroughly clear what is believed to be a
consequence of the rest of this License.

8. If the distribution and/or use of the Program is restricted in certain
countries either by patents or by copyrighted interfaces, the original
copyright holder who places the Program under this License may add an explicit
geographical distribution limitation excluding those countries, so that
distribution is permitted only in or among countries not thus excluded. In such
case, this License incorporates the limitation as if written in the body of
this License.

9. The Free Software Foundation may publish revised and/or new versions of the
General Public License from time to time. Such new versions will be similar in
spirit to the present version, but may differ in detail to address new problems
or concerns.

Each version is given a distinguishing version number. If the Program specifies
a version number of this License which applies to it and "any later version",
you have the option of following the terms and conditions either of that
version or of any later version published by the Free Software Foundation. If 
the Program does not specify a version number of this License, you may choose
any version ever published by the Free Software Foundation.

10. If you wish to incorporate parts of the Program into other free programs
whose distribution conditions are different, write to the author to ask for
permission. For software which is copyrighted by the Free Software Foundation,
write to the Free Software Foundation; we sometimes make exceptions for this.
Our decision will be guided by the two goals of preserving the free status of
all derivatives of our free software and of promoting the sharing and reuse of
software generally.

NO WARRANTY

11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY FOR
THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW. EXCEPT WHEN OTHERWISE
STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES PROVIDE THE
PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE. THE ENTIRE RISK AS TO THE QUALITY AND
PERFORMANCE OF THE PROGRAM IS WITH YOU. SHOULD THE PROGRAM PROVE DEFECTIVE,
YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.

12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING WILL
ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR REDISTRIBUTE THE 
PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY GENERAL,
SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OR
INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR DATA
BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD PARTIES OR A
FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS), EVEN IF SUCH HOLDER
OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

END OF TERMS AND CONDITIONS

How to Apply These Terms to Your New Programs

If you develop a new program, and you want it to be of the greatest possible
use to the public, the best way to achieve this is to make it free software 
which everyone can redistribute and change under these terms.

To do so, attach the following notices to the program. It is safest to attach i
them to the start of each source file to most effectively convey the exclusion 
of warranty; and each file should have at least the "copyright" line and a 
pointer to where the full notice is found.

One line to give the program's name and a brief idea of what it does.

Copyright (C) <year> <name of author>

This program is free software; you can redistribute it and/or modify it under 
the terms of the GNU General Public License as published by the Free Software 
Foundation; either version 2 of the License, or (at your option) any later 
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA

Also add information on how to contact you by electronic and paper mail.

If the program is interactive, make it output a short notice like this when it
starts in an interactive mode:

Gnomovision version 69, Copyright (C) year name of author Gnomovision comes
with ABSOLUTELY NO WARRANTY; for details type `show w'. This is free software,
and you are welcome to redistribute it under certain conditions; type `show c'
for details.

The hypothetical commands `show w' and `show c' should show the appropriate
parts of the General Public License. Of course, the commands you use may be
called something other than `show w' and `show c'; they could even be mouse-
clicks or menu items--whatever suits your program.

You should also get your employer (if you work as a programmer) or your school,
if any, to sign a "copyright disclaimer" for the program, if necessary. Here is
a sample; alter the names:

Yoyodyne, Inc., hereby disclaims all copyright interest in the program
`Gnomovision' (which makes passes at compilers) written by James Hacker.

signature of Ty Coon, 1 April 1989
Ty Coon, President of Vice

This General Public License does not permit incorporating your program into
proprietary programs. If your program is a subroutine library, you may consider
it more useful to permit linking proprietary applications with the library. If
this is what you want to do, use the GNU Library General Public License instead
of this License.

"CLASSPATH" EXCEPTION TO THE GPL VERSION 2

Certain source files distributed by Sun Microsystems, Inc.  are subject to the
following clarification and special exception to the GPL Version 2, but only
where Sun has expressly included in the particular source file's header the
words "Sun designates this particular file as subject to the "Classpath"
exception as provided by Sun in the License file that accompanied this code." 

Linking this library statically or dynamically with other modules is making a
combined work based on this library.  Thus, the terms and conditions of the GNU
General Public License Version 2 cover the whole combination. 

As a special exception, the copyright holders of this library give you 
permission to link this library with independent modules to produce an 
executable, regardless of the license terms of these independent modules, 
and to copy and distribute the resulting executable under terms of your 
choice, provided that you also meet, for each linked independent module, 
the terms and conditions of the license of that module.  An independent 
module is a module which is not derived from or based on this library.  If 
you modify this library, you may extend this exception to your version of 
the library, but you are not obligated to do so.  If you do not wish to do 
so, delete this exception statement from your version.
*/

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>

#include "config.h"
#include "stdafx.h"
#include "swt.h"
#include "MyString.h"
#include "WonderlandModuleExport.h"
#include "Scene.h"
#include "DuneApp.h"
#include "NodeWonderlandImportJava.h"

#define OLD_JME 1

WonderlandModuleExport::~WonderlandModuleExport()
{ 
    if (m_artPath != NULL) {
        free(m_artPath); 
        m_artPath = NULL;
    }    
}

static int 
writeCellChangeMessage(int f, const char *upperName, const char *lowerName) 
{
    RET_ONERROR( mywritestr(f, "/**\n") )
    RET_ONERROR( mywritestr(f, " * Project Wonderland\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * Copyright (c) 2004-2009, Sun Microsystems, Inc., All Rights Reserved\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * Redistributions in source code form must reproduce the above\n") )
    RET_ONERROR( mywritestr(f, " * copyright and this condition.\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * The contents of this file are subject to the GNU General Public\n") )
    RET_ONERROR( mywritestr(f, " * License, Version 2 (the \"License\"); you may not use this file\n") )
    RET_ONERROR( mywritestr(f, " * except in compliance with the License. A copy of the License is\n") )
    RET_ONERROR( mywritestr(f, " * available at http://www.opensource.org/licenses/gpl-license.php.\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * Sun designates this particular file as subject to the \"Classpath\"\n") )
    RET_ONERROR( mywritestr(f, " * exception as provided by Sun in the License file that accompanied\n") )
    RET_ONERROR( mywritestr(f, " * this code.\n") )
    RET_ONERROR( mywritestr(f, " */\n") )
    RET_ONERROR( mywritestr(f, "package org.jdesktop.wonderland.modules.") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, ".common;\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.common.cell.CellID;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.common.cell.messages.CellMessage;\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "public class ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellChangeMessage extends CellMessage {\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    private String message = null;\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    public String getMessage() {\n") )
    RET_ONERROR( mywritestr(f, "        return this.message;\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    public void setMessage(String message) {\n") )
    RET_ONERROR( mywritestr(f, "        this.message = message;\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    public ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellChangeMessage(CellID cellID, String message) {\n") )
    RET_ONERROR( mywritestr(f, "        super(cellID);\n") )
    RET_ONERROR( mywritestr(f, "        this.message = message;\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "}\n") )
    return 0;    
}

static int 
writeCellFactory(int f, const char *upperName, const char *lowerName)
{
    RET_ONERROR( mywritestr(f, "/**\n") )
    RET_ONERROR( mywritestr(f, " * Project Wonderland\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * Copyright (c) 2004-2009, Sun Microsystems, Inc., All Rights Reserved\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * Redistributions in source code form must reproduce the above\n") )
    RET_ONERROR( mywritestr(f, " * copyright and this condition.\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * The contents of this file are subject to the GNU General Public\n") )
    RET_ONERROR( mywritestr(f, " * License, Version 2 (the \"License\"); you may not use this file\n") )
    RET_ONERROR( mywritestr(f, " * except in compliance with the License. A copy of the License is\n") )
    RET_ONERROR( mywritestr(f, " * available at http://www.opensource.org/licenses/gpl-license.php.\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * Sun designates this particular file as subject to the \"Classpath\"\n") )
    RET_ONERROR( mywritestr(f, " * exception as provided by Sun in the License file that accompanied\n") )
    RET_ONERROR( mywritestr(f, " * this code.\n") )
    RET_ONERROR( mywritestr(f, " */\n") )
    RET_ONERROR( mywritestr(f, "package org.jdesktop.wonderland.modules.") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, ".client;\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "import java.net.URL;\n") )
    RET_ONERROR( mywritestr(f, "import java.io.InputStream;\n") )
    RET_ONERROR( mywritestr(f, "import java.io.IOException;\n") )
    RET_ONERROR( mywritestr(f, "import java.awt.Toolkit;\n") )
    RET_ONERROR( mywritestr(f, "import java.awt.Image;\n") )
    RET_ONERROR( mywritestr(f, "import java.util.Properties;\n") )
    RET_ONERROR( mywritestr(f, "import java.util.ResourceBundle;\n") )
    RET_ONERROR( mywritestr(f, "import java.util.logging.Logger;\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.cell.registry.annotation.CellFactory;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.cell.registry.spi.CellFactorySPI;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.common.cell.state.CellServerState;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.modules.") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, ".common.") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellServerState;\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "@CellFactory\n") )
    RET_ONERROR( mywritestr(f, "public class ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellFactory implements CellFactorySPI {\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    private final static ResourceBundle BUNDLE = ResourceBundle.getBundle(\n") )
    RET_ONERROR( mywritestr(f, "            \"org/jdesktop/wonderland/modules/") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, "/client/\" +\n") )
    RET_ONERROR( mywritestr(f, "            \"resources/Bundle\");\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    private final static Logger LOGGER =\n") )
    RET_ONERROR( mywritestr(f, "            Logger.getLogger(") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellFactory.class.getName());\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    public String[] getExtensions() {\n") )
    RET_ONERROR( mywritestr(f, "        return new String[] {};\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    public <T extends CellServerState> T getDefaultCellServerState(Properties props) {\n") )
    RET_ONERROR( mywritestr(f, "        ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellServerState state = new ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellServerState();\n") )
    RET_ONERROR( mywritestr(f, "        state.setName(\"") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "\");\n") )
    RET_ONERROR( mywritestr(f, "        return (T)state;\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    public String getDisplayName() {\n") )
    RET_ONERROR( mywritestr(f, "        return BUNDLE.getString(\"") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, "\");\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    public Image getPreviewImage() {\n") )
    RET_ONERROR( mywritestr(f, "        URL url = ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellFactory.class.getResource(") )
    RET_ONERROR( mywritestr(f, "\"resources/x3d.png\");\n") )
    RET_ONERROR( mywritestr(f, "        try {\n") )
    RET_ONERROR( mywritestr(f, "            InputStream stream = url.openStream();\n") )
    RET_ONERROR( mywritestr(f, "            return Toolkit.getDefaultToolkit().createImage(url);\n") )
    RET_ONERROR( mywritestr(f, "        } catch (IOException e) {\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "        return null;\n") )
    RET_ONERROR( mywritestr(f, "   }\n") )
    RET_ONERROR( mywritestr(f, "}\n") )
    return 0;
}    

static int
writeCellMONodeInit(int f, const char *nodeName, const char *callbackName)
{
    RET_ONERROR( mywritestr(f, "        My") )
    RET_ONERROR( mywritestr(f, nodeName) )
    RET_ONERROR( mywritestr(f, callbackName) )
    RET_ONERROR( mywritestr(f, "Callback" ) )
    RET_ONERROR( mywritestr(f, " my") )
    RET_ONERROR( mywritestr(f, nodeName) )
    RET_ONERROR( mywritestr(f, callbackName) )
    RET_ONERROR( mywritestr(f, "Callback" ) )
    RET_ONERROR( mywritestr(f, " = new My") )
    RET_ONERROR( mywritestr(f, nodeName) )
    RET_ONERROR( mywritestr(f, callbackName) )
    RET_ONERROR( mywritestr(f, "Callback" ) )
    RET_ONERROR( mywritestr(f, "();\n") )

    RET_ONERROR( mywritestr(f, "        ") )
    RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
    RET_ONERROR( mywritestr(f, nodeName) )
    RET_ONERROR( mywritestr(f, ".set") )
    RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
    RET_ONERROR( mywritestr(f, nodeName) )
    RET_ONERROR( mywritestr(f, callbackName) )
    RET_ONERROR( mywritestr(f, "Callback(my") )
    RET_ONERROR( mywritestr(f, nodeName) )
    RET_ONERROR( mywritestr(f, callbackName) )
    RET_ONERROR( mywritestr(f, "Callback" ) )
    RET_ONERROR( mywritestr(f, ");\n") )
    return 0;
}
    
static int 
writeCellMO(int f, const char *upperName, const char *lowerName) 
{
    RET_ONERROR( mywritestr(f, "/**\n") )
    RET_ONERROR( mywritestr(f, " * Project Wonderland\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * Copyright (c) 2004-2009, Sun Microsystems, Inc., All Rights Reserved\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * Redistributions in source code form must reproduce the above\n") )
    RET_ONERROR( mywritestr(f, " * copyright and this condition.\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * The contents of this file are subject to the GNU General Public\n") )
    RET_ONERROR( mywritestr(f, " * License, Version 2 (the \"License\"); you may not use this file\n") )
    RET_ONERROR( mywritestr(f, " * except in compliance with the License. A copy of the License is\n") )
    RET_ONERROR( mywritestr(f, " * available at http://www.opensource.org/licenses/gpl-license.php.\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * Sun designates this particular file as subject to the \"Classpath\"\n") )
    RET_ONERROR( mywritestr(f, " * exception as provided by Sun in the License file that accompanied\n") )
    RET_ONERROR( mywritestr(f, " * this code.\n") )
    RET_ONERROR( mywritestr(f, " */\n") )
    RET_ONERROR( mywritestr(f, "package org.jdesktop.wonderland.modules.") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, ".server;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.common.cell.ClientCapabilities;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.common.cell.messages.CellMessage;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.common.cell.state.CellClientState;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.common.cell.state.CellServerState;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.modules.") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, ".common.") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellChangeMessage;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.modules.") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, ".common.") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellClientState;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.modules.") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, ".common.") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellServerState;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.server.cell.AbstractComponentMessageReceiver;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.server.cell.CellMO;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.server.cell.ChannelComponentMO;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.server.comms.WonderlandClientID;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.server.comms.WonderlandClientSender;\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "public class ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellMO extends CellMO {\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    private String message = null;\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    public ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellMO() {\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    @Override\n") )
    RET_ONERROR( mywritestr(f, "    public String getClientCellClassName(WonderlandClientID clientID, ClientCapabilities capabilities) {\n") )
    RET_ONERROR( mywritestr(f, "         return \"org.jdesktop.wonderland.modules.") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, ".client.") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "Cell\";\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    @Override\n") )
    RET_ONERROR( mywritestr(f, "    public CellClientState getClientState(CellClientState state, WonderlandClientID clientID, ClientCapabilities capabilities) {\n") )
    RET_ONERROR( mywritestr(f, "        if (state == null) {\n") )
    RET_ONERROR( mywritestr(f, "            state = new ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellClientState();\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "        ((") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellClientState)state).setMessage(message);\n") )
    RET_ONERROR( mywritestr(f, "        return super.getClientState(state, clientID, capabilities);\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    @Override\n") )
    RET_ONERROR( mywritestr(f, "    public CellServerState getServerState(CellServerState state) {\n") )
    RET_ONERROR( mywritestr(f, "        if (state == null) {\n") )
    RET_ONERROR( mywritestr(f, "            state = new ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellServerState();\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "        ((") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellServerState)state).setMessage(message);\n") )
    RET_ONERROR( mywritestr(f, "        return super.getServerState(state);\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    @Override\n") )
    RET_ONERROR( mywritestr(f, "    public void setServerState(CellServerState state) {\n") )
    RET_ONERROR( mywritestr(f, "        super.setServerState(state);\n") )
    RET_ONERROR( mywritestr(f, "        this.message = ((") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellServerState)state).getMessage();\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    @Override\n") )
    RET_ONERROR( mywritestr(f, "    protected void setLive(boolean live) {\n") )
    RET_ONERROR( mywritestr(f, "        super.setLive(live);\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "        ChannelComponentMO channel = getComponent(ChannelComponentMO.class);\n") )
    RET_ONERROR( mywritestr(f, "        if (live == true) {\n") )
    RET_ONERROR( mywritestr(f, "            channel.addMessageReceiver(") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellChangeMessage.class,\n") )
    RET_ONERROR( mywritestr(f, "                (ChannelComponentMO.ComponentMessageReceiver)new ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellMessageReceiver(this));\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "        else {\n") )
    RET_ONERROR( mywritestr(f, "            channel.removeMessageReceiver(") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellChangeMessage.class);\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    private static class ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellMessageReceiver extends AbstractComponentMessageReceiver {\n") )
    RET_ONERROR( mywritestr(f, "        public ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellMessageReceiver(") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellMO cellMO) {\n") )
    RET_ONERROR( mywritestr(f, "            super(cellMO);\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "        public void messageReceived(WonderlandClientSender sender, WonderlandClientID clientID, CellMessage cellMessage) {\n") )
    RET_ONERROR( mywritestr(f, "            ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellChangeMessage sccm = (") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellChangeMessage) cellMessage;\n") )
    RET_ONERROR( mywritestr(f, "            ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellMO cellMO = (") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellMO)getCell();\n") )
    RET_ONERROR( mywritestr(f, "            cellMO.message = sccm.getMessage();\n") )
    RET_ONERROR( mywritestr(f, "            cellMO.sendCellMessage(clientID, cellMessage);\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "}\n") )
    return 0;
}

static int writeImportJavaError = 0;

static bool writeCImportJava(Node *node, void *data)
{
    int *f = (int *)data;
    if (node->getType() == DUNE_WONDERLAND_IMPORT_JAVA)
        if (((NodeWonderlandImportJava *)node)->writeCode(*f) != 0) {
            writeImportJavaError = -1;
            return false;
        }
    return true;
}

static int 
writeCellRendererX3dData(int f, const char *upperName, const char *lowerName, 
                         Scene *scene, bool manyClasses) 
{
    const char *prefix = TheApp->getCPrefix();

    if (strlen(prefix) == 0)
        swDebugf("internal error: prefix not set !\n");
 
    RET_ONERROR( mywritestr(f, "/**\n") )
    RET_ONERROR( mywritestr(f, " * Copyright (c) 2009, J. \"MUFTI\" Scheurich\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * Redistributions in source code form must reproduce the above\n") )
    RET_ONERROR( mywritestr(f, " * copyright and this condition.\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * The contents of this file are subject to the GNU General Public\n") )
    RET_ONERROR( mywritestr(f, " * License, Version 2 (the \"License\"); you may not use this file\n") )
    RET_ONERROR( mywritestr(f, " * except in compliance with the License. A copy of the License is\n") )
    RET_ONERROR( mywritestr(f, " * available at http://www.opensource.org/licenses/gpl-license.php.\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * Sun designates this particular file as subject to the \"Classpath\"\n") )
    RET_ONERROR( mywritestr(f, " * exception as provided by Sun in the License file that accompanied\n") )
    RET_ONERROR( mywritestr(f, " * this code.\n") )
    RET_ONERROR( mywritestr(f, " */\n") )
    RET_ONERROR( mywritestr(f, "package org.jdesktop.wonderland.modules.") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, ".client.jme.cellrenderer;\n") )
    RET_ONERROR( mywritestr(f, "\n") )

    writeImportJavaError = 0;
    scene->getRoot()->doWithBranch(writeCImportJava, &f);    
    if (writeImportJavaError != 0)
        return writeImportJavaError;

    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "class ") )
    RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
    RET_ONERROR( mywritestr(f, "ClassName {\n") )
    RET_ONERROR( mywritestr(f, "    public static String complete(String className) {\n") )
    RET_ONERROR( mywritestr(f, "        return \"org.jdesktop.wonderland.modules.") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, ".client.jme.cellrenderer.\" + className;\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "}\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( scene->write(f, "./export.x3dv", TEMP_SAVE | JAVA_SOURCE | 
                                                 WONDERLAND |
                              (manyClasses ? MANY_JAVA_CLASSES : 0)) )
    RET_ONERROR( mywritestr(f, "\n") )
    return 0;
}

static int
writeCellRendererX3dProtos(int f, const char *upperName, const char *lowerName, 
                           Scene *scene)
{
    const char *prefix = TheApp->getCPrefix();

    if (strlen(prefix) == 0) {
        swDebugf("internal error: prefix not set !\n");
        return -1;
    }

    for (int i = 0; i < scene->getNumProtoNames(); i++) {
        const char *protoName = scene->getProtoName(i);
        Proto *proto = scene->getProto(protoName);
        if (proto->getType() < scene->getNumberBuildinProtos())
            continue;
        RET_ONERROR( mywritestr(f, "class My") )
        RET_ONERROR( mywritestr(f, protoName) )
        RET_ONERROR( mywritestr(f, "Callback extends ") )
        RET_ONERROR( mywritestr(f, prefix) )
        RET_ONERROR( mywritestr(f, protoName) )
        RET_ONERROR( mywritestr(f, "DoWithDataCallback {\n") )
        RET_ONERROR( mywritestr(f, "\n") )
        RET_ONERROR( mywritestr(f, "   public void doWithData(") )
        RET_ONERROR( mywritestr(f, prefix) )
        RET_ONERROR( mywritestr(f, "Node node) {\n") )
        RET_ONERROR( mywritestr(f, "      ") )
        RET_ONERROR( mywritestr(f, prefix) )
        RET_ONERROR( mywritestr(f, protoName) )
        RET_ONERROR( mywritestr(f, " ") )
        RET_ONERROR( mywritestr(f, protoName) )
        RET_ONERROR( mywritestr(f, " = (") )
        RET_ONERROR( mywritestr(f, prefix) )
        RET_ONERROR( mywritestr(f, protoName) )
        RET_ONERROR( mywritestr(f, ") node;\n") )
        RET_ONERROR( mywritestr(f, "      Node jme") )
        RET_ONERROR( mywritestr(f, protoName) )
        RET_ONERROR( mywritestr(f, " = new Node(\"") )
        RET_ONERROR( mywritestr(f, protoName) )
        RET_ONERROR( mywritestr(f, "\");\n") )
        RET_ONERROR( mywritestr(f, "      if (") )
        RET_ONERROR( mywritestr(f, protoName) )
        RET_ONERROR( mywritestr(f, ".m_protoRoot != null) {\n") )
        RET_ONERROR( mywritestr(f, "         if (") )
        RET_ONERROR( mywritestr(f, protoName) )
        RET_ONERROR( mywritestr(f, ".m_protoRoot.m_data != null)\n") )
        RET_ONERROR( mywritestr(f, "             jme") )
        RET_ONERROR( mywritestr(f, protoName) )
        RET_ONERROR( mywritestr(f, ".attachChild((Spatial)") )
        RET_ONERROR( mywritestr(f, protoName) )
        RET_ONERROR( mywritestr(f, ".m_protoRoot.m_data);\n") )
        RET_ONERROR( mywritestr(f, "      }\n") )
        RET_ONERROR( mywritestr(f, "      node.m_data = jme") )
        RET_ONERROR( mywritestr(f, protoName) )
        RET_ONERROR( mywritestr(f, ";\n") )
        RET_ONERROR( mywritestr(f, "   }\n") )
        RET_ONERROR( mywritestr(f, "}\n") )
        RET_ONERROR( mywritestr(f, "\n") )
    }
    RET_ONERROR( mywritestr(f, "\n") )
    return 0;
}

int 
writeCellRenderer(int f, const char *upperName, const char *lowerName,
                  Scene *scene) 
{
    const char *prefix = TheApp->getCPrefix();

    if (strlen(prefix) == 0)
        swDebugf("internal error: prefix not set !\n");

    RET_ONERROR( writeCellRendererCode(f, upperName, lowerName, scene) )

    RET_ONERROR( writeCellRendererX3dProtos(f, upperName, lowerName, scene) )

    RET_ONERROR( mywritestr(f, "public class ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellRenderer extends BasicRenderer {\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    ") )
    RET_ONERROR( mywritestr(f, TheApp->getCSceneGraphName()) )
    RET_ONERROR( mywritestr(f, " sceneGraph;\n") )
    RET_ONERROR( mywritestr(f, "    private Node node = null;\n") )
    RET_ONERROR( mywritestr(f, "    public static ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellRenderer m_renderer = null;\n") )
    RET_ONERROR( mywritestr(f, "    private UpdateProcessor processor;\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    class UpdateProcessor extends ProcessorComponent {\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "        NewFrameCondition condition;\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "        public void arm() {\n") )
    RET_ONERROR( mywritestr(f, "            setArmingCondition(condition);\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "        public void disarm() {\n") )
    RET_ONERROR( mywritestr(f, "            setArmingCondition(null);\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "        public void compute(ProcessorArmingCollection arg0) {\n") )
    RET_ONERROR( mywritestr(f, "            sceneGraph.") )
    RET_ONERROR( mywritestr(f, TheApp->getCProcessEventsName()) )
    RET_ONERROR( mywritestr(f, "();\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "        public void commit(ProcessorArmingCollection arg0) {\n") )
    RET_ONERROR( mywritestr(f, "            ClientContextJME.getWorldManager().addToUpdateList(node);\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "        public void initialize() {\n") )
    RET_ONERROR( mywritestr(f, "           condition = new NewFrameCondition(this);\n") )
    RET_ONERROR( mywritestr(f, "           setArmingCondition(condition);\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    
    RET_ONERROR( mywritestr(f, "    void initRootNode(Node rootNode) {\n") )
    RET_ONERROR( mywritestr(f, "        ") )
    RET_ONERROR( mywritestr(f, "sceneGraph = new ") )
    RET_ONERROR( mywritestr(f, TheApp->getCSceneGraphName()) )
    RET_ONERROR( mywritestr(f, "();\n") )
    RET_ONERROR( mywritestr(f, "        m_renderer = this;\n") )

    RET_ONERROR( writeCellMONodeInit(f, "TriangleSet", "DoWithData") )
    RET_ONERROR( writeCellMONodeInit(f, "IndexedTriangleSet", "DoWithData") )
    RET_ONERROR( writeCellMONodeInit(f, "IndexedFaceSet", "DoWithData") )
    RET_ONERROR( writeCellMONodeInit(f, "IndexedLineSet", "DoWithData") )
    RET_ONERROR( writeCellMONodeInit(f, "PointSet", "DoWithData") )

    RET_ONERROR( writeCellMONodeInit(f, "Group", "DoWithData") )
    RET_ONERROR( writeCellMONodeInit(f, "StaticGroup", "DoWithData") )
    RET_ONERROR( writeCellMONodeInit(f, "Transform", "DoWithData") )
    RET_ONERROR( writeCellMONodeInit(f, "Shape", "DoWithData") )

    RET_ONERROR( writeCellMONodeInit(f, "Text", "DoWithData") )
    RET_ONERROR( writeCellMONodeInit(f, "Anchor", "DoWithData") )
    RET_ONERROR( writeCellMONodeInit(f, "Switch", "DoWithData") )
    RET_ONERROR( writeCellMONodeInit(f, "Collision", "DoWithData") )
    RET_ONERROR( writeCellMONodeInit(f, "Billboard", "DoWithData") )
    RET_ONERROR( writeCellMONodeInit(f, "LOD", "DoWithData") )

    RET_ONERROR( writeCellMONodeInit(f, "CADPart", "DoWithData") )
    RET_ONERROR( writeCellMONodeInit(f, "CADAssembly", "DoWithData") )
    RET_ONERROR( writeCellMONodeInit(f, "CADLayer", "DoWithData") )
    RET_ONERROR( writeCellMONodeInit(f, "CADFace", "DoWithData") )

    RET_ONERROR( writeCellMONodeInit(f, "TimeSensor", "DoWithData") )
    RET_ONERROR( writeCellMONodeInit(f, "TouchSensor", "DoWithData") )
    RET_ONERROR( writeCellMONodeInit(f, "PlaneSensor", "DoWithData") )
    RET_ONERROR( writeCellMONodeInit(f, "CylinderSensor", "DoWithData") )
    RET_ONERROR( writeCellMONodeInit(f, "SphereSensor", "DoWithData") )
    RET_ONERROR( writeCellMONodeInit(f, "ProximitySensor", "DoWithData") )
    RET_ONERROR( writeCellMONodeInit(f, "KeySensor", "DoWithData") )
    RET_ONERROR( writeCellMONodeInit(f, "StringSensor", "DoWithData") )

    RET_ONERROR( mywritestr(f, "\n") )

    RET_ONERROR( writeCellMONodeInit(f, "PositionInterpolator", "ProcessEvent") )
    RET_ONERROR( writeCellMONodeInit(f, "PositionInterpolator2D", "ProcessEvent") )
    RET_ONERROR( writeCellMONodeInit(f, "OrientationInterpolator", "ProcessEvent") )
    RET_ONERROR( writeCellMONodeInit(f, "ColorInterpolator", "ProcessEvent") )
    RET_ONERROR( writeCellMONodeInit(f, "ScalarInterpolator", "ProcessEvent") )
    RET_ONERROR( writeCellMONodeInit(f, "CoordinateInterpolator", "ProcessEvent") )
    RET_ONERROR( writeCellMONodeInit(f, "CoordinateInterpolator2D", "ProcessEvent") )
    RET_ONERROR( writeCellMONodeInit(f, "NormalInterpolator", "ProcessEvent") )

    RET_ONERROR( writeCellMONodeInit(f, "Transform", "ProcessEvent") )

    RET_ONERROR( writeCellMONodeInit(f, "Material", "ProcessEvent") )
    RET_ONERROR( writeCellMONodeInit(f, "TextureTransform", "ProcessEvent") )

    RET_ONERROR( writeCellMONodeInit(f, "Coordinate", "ProcessEvent") )
    RET_ONERROR( writeCellMONodeInit(f, "Normal", "ProcessEvent") )
    RET_ONERROR( writeCellMONodeInit(f, "Color", "ProcessEvent") )
    RET_ONERROR( writeCellMONodeInit(f, "TextureCoordinate", "ProcessEvent") )

    RET_ONERROR( writeCellMONodeInit(f, "Text", "ProcessEvent") )
    RET_ONERROR( writeCellMONodeInit(f, "Switch", "ProcessEvent") )

    RET_ONERROR( writeCellMONodeInit(f, "TimeSensor", "ProcessEvent") )
    RET_ONERROR( writeCellMONodeInit(f, "TouchSensor", "ProcessEvent") )
    RET_ONERROR( writeCellMONodeInit(f, "PlaneSensor", "ProcessEvent") )
    RET_ONERROR( writeCellMONodeInit(f, "CylinderSensor", "ProcessEvent") )
    RET_ONERROR( writeCellMONodeInit(f, "SphereSensor", "ProcessEvent") )
    RET_ONERROR( writeCellMONodeInit(f, "ProximitySensor", "ProcessEvent") )
    RET_ONERROR( writeCellMONodeInit(f, "KeySensor", "ProcessEvent") )
    RET_ONERROR( writeCellMONodeInit(f, "StringSensor", "ProcessEvent") )

    for (int i = 0; i < scene->getNumProtoNames(); i++) {
        const char *protoName = scene->getProtoName(i);
        Proto *proto = scene->getProto(protoName);
        if (proto->getType() < scene->getNumberBuildinProtos())
            continue;
        RET_ONERROR( writeCellMONodeInit(f, protoName, "DoWithData") )
    }

    RET_ONERROR( mywritestr(f, "        sceneGraph.root.m_data = new ") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Data();\n") )
    RET_ONERROR( mywritestr(f, "        ((") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Data)sceneGraph.root.m_data).node = rootNode;\n") )
    RET_ONERROR( mywritestr(f, "        sceneGraph.doWithData();\n") )
    RET_ONERROR( mywritestr(f, "        ") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.buildTouchableNodesList();\n") )
    RET_ONERROR( mywritestr(f, "        ") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.buildPlaneSensorNodesList();\n") )
    RET_ONERROR( mywritestr(f, "        ") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.buildCylinderSensorNodesList();\n") )
    RET_ONERROR( mywritestr(f, "        ") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.buildSphereSensorNodesList();\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )

    RET_ONERROR( mywritestr(f, "    public ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellRenderer(Cell cell) {\n") )
    RET_ONERROR( mywritestr(f, "        super(cell);\n") )
    RET_ONERROR( mywritestr(f, "        ") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.logger = logger;\n") )
    RET_ONERROR( mywritestr(f, "        ") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.cell = cell;\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )

    RET_ONERROR( mywritestr(f, "    public void update") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "() {\n") )
    RET_ONERROR( mywritestr(f, "        String message = ((") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "Cell) cell).getMessage();\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "        String moduleName = \"") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, "\";\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "        String cellName = cell.getCellID().toString();\n") )
    RET_ONERROR( mywritestr(f, "        String pressed = \"pressed_\";\n") )
    RET_ONERROR( mywritestr(f, "        boolean isPressed = false;\n") )
    RET_ONERROR( mywritestr(f, "        String released = \"released_\";\n") )
    RET_ONERROR( mywritestr(f, "        boolean isReleased = false;\n") )
    RET_ONERROR( mywritestr(f, "        String key = \"key_\";\n") )
    RET_ONERROR( mywritestr(f, "        boolean isKey = false;\n") )
    RET_ONERROR( mywritestr(f, "        String dragged = \"dragged_\";\n") )
    RET_ONERROR( mywritestr(f, "        boolean isDragged = false;\n") )
    RET_ONERROR( mywritestr(f, "        ") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.hitPlaneSensor = null;\n") )
    RET_ONERROR( mywritestr(f, "        ") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.hitCylinderSensor = null;\n") )
    RET_ONERROR( mywritestr(f, "        ") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.hitSphereSensor = null;\n") )
    RET_ONERROR( mywritestr(f, "        String error = \"error_\";\n") )
    RET_ONERROR( mywritestr(f, "        if (message != null) {\n") )
    RET_ONERROR( mywritestr(f, "            if (message.startsWith(key)){\n") )
    RET_ONERROR( mywritestr(f, "                message = message.substring(key.length());\n") )
    RET_ONERROR( mywritestr(f, "                isKey = true;\n") )
    RET_ONERROR( mywritestr(f, "            }\n") )
    RET_ONERROR( mywritestr(f, "            if (message.startsWith(pressed)) {\n") )
    RET_ONERROR( mywritestr(f, "                isPressed = true;\n") )
    RET_ONERROR( mywritestr(f, "                message = message.substring(pressed.length());\n") )
    RET_ONERROR( mywritestr(f, "            } else if (message.startsWith(released)) {\n") )
    RET_ONERROR( mywritestr(f, "                isReleased = true;\n") )
    RET_ONERROR( mywritestr(f, "                message = message.substring(released.length());\n") )
    RET_ONERROR( mywritestr(f, "            } else if (message.startsWith(dragged)) {\n") )
    RET_ONERROR( mywritestr(f, "                isDragged = true;\n") )
    RET_ONERROR( mywritestr(f, "                message = message.substring(dragged.length());\n") )
    RET_ONERROR( mywritestr(f, "            } else if (message.startsWith(error)) {\n") )
    RET_ONERROR( mywritestr(f, "                if (message.equals(\"error_getHitIntersectionPointLocal\")) {\n") )
    RET_ONERROR( mywritestr(f, "                    String errorMessage = \"\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"\\nPatch needed: getHitIntersectionPointLocal() delivers null\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"please change wonderland/core/src/classes/org/jdesktop/wonderland/client/jme/input/MouseDraggedEvent3D.java\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"around line 135 from\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"      public Vector3f getHitIntersectionPointLocal () {\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"          if (hitPickDetails == null) {\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"              return null;\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"          } else {\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"              Vector3f posWorld = hitPickDetails.getPosition();\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"              if (posWorld == null) return null;\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"              CollisionComponent cc = hitPickDetails.getCollisionComponent();\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"              Node node = cc.getNode();\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"              node.getLocalToWorldMatrix(world2Local);\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"              world2Local.invert();\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"to \\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"      public Vector3f getHitIntersectionPointLocal () {\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"          if (hitPickDetails == null) {\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"              return null;\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"          } else {\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"              Vector3f posWorld = hitPickDetails.getPosition();\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"              if (posWorld == null) return null;\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"              CollisionComponent cc = hitPickDetails.getCollisionComponent();\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"              Node node = cc.getNode();\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"              world2Local = new Matrix4f();    // insert this new line\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"              node.getLocalToWorldMatrix(world2Local);\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    errorMessage += \"              world2Local.invert();\\n\";\n") )
    RET_ONERROR( mywritestr(f, "                    ") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.logger.warning(errorMessage);\n") )
    RET_ONERROR( mywritestr(f, "                }\n") )
    RET_ONERROR( mywritestr(f, "                return;\n") )
    RET_ONERROR( mywritestr(f, "            }\n") )
    RET_ONERROR( mywritestr(f, "            if (!message.startsWith(moduleName))\n") )
    RET_ONERROR( mywritestr(f, "                return;\n") )
    RET_ONERROR( mywritestr(f, "            message = message.substring(moduleName.length() + 1);\n") )
    RET_ONERROR( mywritestr(f, "            if (isKey) {\n") )
    RET_ONERROR( mywritestr(f, "                String charPrefix = \"char_\";\n") )
    RET_ONERROR( mywritestr(f, "                if (message.startsWith(charPrefix)) {\n") )
    RET_ONERROR( mywritestr(f, "                    message = message.substring(charPrefix.length());\n") )
    RET_ONERROR( mywritestr(f, "                    ") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.handleKeySensors(isPressed, isReleased, -1, message);\n") )
    RET_ONERROR( mywritestr(f, "                    ") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.handleStringSensors(isPressed, isReleased, -1, message);\n") )
    RET_ONERROR( mywritestr(f, "                } else {\n") )
    RET_ONERROR( mywritestr(f, "                    int keycode = Integer.parseInt(message);\n") )
    RET_ONERROR( mywritestr(f, "                    ") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.handleKeySensors(isPressed, isReleased, keycode, message);\n") )
    RET_ONERROR( mywritestr(f, "                    ") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.handleStringSensors(isPressed, isReleased, keycode, message);\n") )
    RET_ONERROR( mywritestr(f, "                }\n") )
    RET_ONERROR( mywritestr(f, "            } else if (isDragged) {\n") )
    RET_ONERROR( mywritestr(f, "                float trackpoint[] = { 0, 0, 0 };\n") )
    RET_ONERROR( mywritestr(f, "                int index = message.indexOf(\"_\");\n") )
    RET_ONERROR( mywritestr(f, "                String nodeString = message.substring(0, index);\n") )
    RET_ONERROR( mywritestr(f, "                int nodeNumber = Integer.parseInt(nodeString);\n") )
    RET_ONERROR( mywritestr(f, "                message = message.substring(nodeString.length() + 1);\n") )
    RET_ONERROR( mywritestr(f, "                index = message.indexOf(\"_\");\n") )
    RET_ONERROR( mywritestr(f, "                trackpoint[0] = Float.parseFloat(message.substring(0, index));\n") )
    RET_ONERROR( mywritestr(f, "                message = message.substring(index + 1);\n") )
    RET_ONERROR( mywritestr(f, "                index = message.indexOf(\"_\");\n") )
    RET_ONERROR( mywritestr(f, "                trackpoint[1] = Float.parseFloat(message.substring(0, index));\n") )
    RET_ONERROR( mywritestr(f, "                message = message.substring(index + 1);\n") )
    RET_ONERROR( mywritestr(f, "                trackpoint[2] = Float.parseFloat(message);\n") )
    RET_ONERROR( mywritestr(f, "                ") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.handlePlaneSensors(") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.getNode(nodeNumber), trackpoint);\n") )
    RET_ONERROR( mywritestr(f, "                ") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.handleCylinderSensors(") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.getNode(nodeNumber), trackpoint);\n") )
    RET_ONERROR( mywritestr(f, "                ") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.handleSphereSensors(") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.getNode(nodeNumber), trackpoint);\n") )
    RET_ONERROR( mywritestr(f, "            } else {\n") )
    RET_ONERROR( mywritestr(f, "                int nodeNumber = Integer.parseInt(message);\n") )
    RET_ONERROR( mywritestr(f, "                ") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.handleTouchSensors(") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.getNode(nodeNumber), isPressed, isReleased);\n") )
    RET_ONERROR( mywritestr(f, "            }\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "        ClientContextJME.getWorldManager().addToUpdateList(node);\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    private Node get") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "Node(String name, String message) {\n") )
    RET_ONERROR( mywritestr(f, "        Node node = null;\n") )
    RET_ONERROR( mywritestr(f, "        ") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.reset();\n") )
    RET_ONERROR( mywritestr(f, "        node = new Node();\n") )
    RET_ONERROR( mywritestr(f, "        initRootNode(node);\n") )
    RET_ONERROR( mywritestr(f, "        return node;\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    protected Node createSceneGraph(Entity entity) {\n") )
    RET_ONERROR( mywritestr(f, "        String name = cell.getCellID().toString();\n") )
    RET_ONERROR( mywritestr(f, "        String message = ((") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "Cell)cell).getMessage();\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "        Node ") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, "Node = this.get") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "Node(name, message);\n") )
    RET_ONERROR( mywritestr(f, "        if (") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, "Node == null) {\n") )
    RET_ONERROR( mywritestr(f, "            node = new Node();\n") )
    RET_ONERROR( mywritestr(f, "            return node;\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "        node = new Node();\n") )
    RET_ONERROR( mywritestr(f, "        node.attachChild(") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, "Node);\n") )
    RET_ONERROR( mywritestr(f, "        node.setModelBound(new BoundingBox());\n") )
    RET_ONERROR( mywritestr(f, "        node.updateModelBound();\n") )
    RET_ONERROR( mywritestr(f, "        node.setName(\"Cell_\"+cell.getCellID()+\":\"+cell.getName());\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "        ") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.viewCell = ((") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "Cell) cell).getAvatar().getViewCell();\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "        CellServerState cellServerState = ((") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "Cell) cell).getCellServerState(cell);\n") )
    RET_ONERROR( mywritestr(f, "        ") )
    RET_ONERROR( mywritestr(f, prefix) )
    RET_ONERROR( mywritestr(f, "_Util.positionComponentServerState = (PositionComponentServerState) cellServerState.getComponentServerState(PositionComponentServerState.class);") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "        processor = new UpdateProcessor();\n") )
    RET_ONERROR( mywritestr(f, "        entity.addComponent(ProcessorComponent.class, processor);\n") )
    RET_ONERROR( mywritestr(f, "        if (processor != null)\n") )
    RET_ONERROR( mywritestr(f, "            processor.arm();\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "        return node;\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "}\n") )
    return 0;
}

static int 
writeCellServerState(int f, const char *upperName, const char *lowerName) 
{
    RET_ONERROR( mywritestr(f, "/**\n") )
    RET_ONERROR( mywritestr(f, " * Project Wonderland\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * Copyright (c) 2004-2009, Sun Microsystems, Inc., All Rights Reserved\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * Redistributions in source code form must reproduce the above\n") )
    RET_ONERROR( mywritestr(f, " * copyright and this condition.\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * The contents of this file are subject to the GNU General Public\n") )
    RET_ONERROR( mywritestr(f, " * License, Version 2 (the \"License\"); you may not use this file\n") )
    RET_ONERROR( mywritestr(f, " * except in compliance with the License. A copy of the License is\n") )
    RET_ONERROR( mywritestr(f, " * available at http://www.opensource.org/licenses/gpl-license.php.\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * Sun designates this particular file as subject to the \"Classpath\"\n") )
    RET_ONERROR( mywritestr(f, " * exception as provided by Sun in the License file that accompanied\n") )
    RET_ONERROR( mywritestr(f, " * this code.\n") )
    RET_ONERROR( mywritestr(f, " */\n") )
    RET_ONERROR( mywritestr(f, "package org.jdesktop.wonderland.modules.") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, ".common;\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "import javax.xml.bind.annotation.XmlElement;\n") )
    RET_ONERROR( mywritestr(f, "import javax.xml.bind.annotation.XmlRootElement;\n") )
    RET_ONERROR( mywritestr(f, "import javax.xml.bind.annotation.XmlTransient;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.common.cell.state.CellServerState;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.common.cell.state.annotation.ServerState;\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "@XmlRootElement(name=\"") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, "-cell\")\n") )
    RET_ONERROR( mywritestr(f, "@ServerState\n") )
    RET_ONERROR( mywritestr(f, "public class ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellServerState extends CellServerState {\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    @XmlElement(name=\"message\")\n") )
    RET_ONERROR( mywritestr(f, "    private String message = \"\";\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    public ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellServerState() {\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    @XmlTransient public String getMessage() { return this.message; }\n") )
    RET_ONERROR( mywritestr(f, "    public void setMessage(String message) { this.message = message; }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    @Override\n") )
    RET_ONERROR( mywritestr(f, "    public String getServerClassName() {\n") )
    RET_ONERROR( mywritestr(f, "        return \"org.jdesktop.wonderland.modules.") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, ".server.") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellMO\";\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "}\n") )
    return 0;
}

static int 
writeCell(int f, const char *upperName, const char *lowerName)
{
    RET_ONERROR( mywritestr(f, "/**\n") )
    RET_ONERROR( mywritestr(f, " * Project Wonderland\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * Copyright (c) 2004-2009, Sun Microsystems, Inc., All Rights Reserved\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * Redistributions in source code form must reproduce the above\n") )
    RET_ONERROR( mywritestr(f, " * copyright and this condition.\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * The contents of this file are subject to the GNU General Public\n") )
    RET_ONERROR( mywritestr(f, " * License, Version 2 (the \"License\"); you may not use this file\n") )
    RET_ONERROR( mywritestr(f, " * except in compliance with the License. A copy of the License is\n") )
    RET_ONERROR( mywritestr(f, " * available at http://www.opensource.org/licenses/gpl-license.php.\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * Sun designates this particular file as subject to the \"Classpath\"\n") )
    RET_ONERROR( mywritestr(f, " * exception as provided by Sun in the License file that accompanied\n") )
    RET_ONERROR( mywritestr(f, " * this code.\n") )
    RET_ONERROR( mywritestr(f, " */\n") )
    RET_ONERROR( mywritestr(f, "package org.jdesktop.wonderland.modules.") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, ".client;\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "import com.jme.math.Vector3f;;\n") )
    RET_ONERROR( mywritestr(f, "import com.jme.scene.TriMesh;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.modules.") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, ".client.jme.cellrenderer.") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellRenderer;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.common.cell.state.CellServerState;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.common.cell.messages.CellServerStateResponseMessage;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.common.cell.messages.CellServerStateRequestMessage;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.common.messages.ResponseMessage;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.common.cell.CellStatus;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.common.cell.state.CellClientState;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.cell.Cell;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.cell.Cell.RendererType;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.cell.CellCache;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.cell.CellRenderer;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.cell.ChannelComponent;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.cell.ChannelComponent.ComponentMessageReceiver;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.cell.annotation.UsesCellComponent;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.cell.view.LocalAvatar;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.contextmenu.ContextMenuActionListener;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.contextmenu.ContextMenuItem;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.contextmenu.ContextMenuItemEvent;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.contextmenu.SimpleContextMenuItem;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.contextmenu.cell.ContextMenuComponent;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.contextmenu.spi.ContextMenuFactorySPI;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.input.Event;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.input.EventClassListener;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.jme.input.MouseButtonEvent3D;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.jme.input.MouseDraggedEvent3D;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.jme.input.KeyEvent3D;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.jme.input.MouseEvent3D.ButtonId;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.scenemanager.event.ContextEvent;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.comms.CellClientSession;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.common.cell.CellID;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.common.cell.messages.CellMessage;\n") )
    RET_ONERROR( mywritestr(f, "import java.awt.event.KeyEvent;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.modules.") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, ".common.") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellChangeMessage;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.modules.") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, ".common.") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellClientState;\n") )
    RET_ONERROR( mywritestr(f, "\n") )

    RET_ONERROR( mywritestr(f, "public class ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "Cell extends Cell {\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    private String message = null;\n") )
    RET_ONERROR( mywritestr(f, "    private ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellRenderer renderer = null;\n") )
    RET_ONERROR( mywritestr(f, "    private MouseEventListener listener = null;\n") )
    RET_ONERROR( mywritestr(f, "\n") )

    RET_ONERROR( mywritestr(f, "    public CellServerState getCellServerState(Cell cell) {\n") )
    RET_ONERROR( mywritestr(f, "        ResponseMessage responseMessage = cell.sendCellMessageAndWait(new CellServerStateRequestMessage(cell.getCellID()));\n") )
    RET_ONERROR( mywritestr(f, "        if (responseMessage == null) {\n") )
    RET_ONERROR( mywritestr(f, "            return null;\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "        CellServerStateResponseMessage cellServerStateResponseMessage = (CellServerStateResponseMessage) responseMessage;\n") )
    RET_ONERROR( mywritestr(f, "        CellServerState state = cellServerStateResponseMessage.getCellServerState();\n") )
    RET_ONERROR( mywritestr(f, "        return state;\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )

    RET_ONERROR( mywritestr(f, "    public CellClientSession getSession() {\n") )
    RET_ONERROR( mywritestr(f, "        return (CellClientSession)getCellCache().getSession();\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )

    RET_ONERROR( mywritestr(f, "    public LocalAvatar getAvatar() {\n") )
    RET_ONERROR( mywritestr(f, "        return ((CellClientSession) getSession()).getLocalAvatar();\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )

    RET_ONERROR( mywritestr(f, "    @UsesCellComponent private ContextMenuComponent contextComp = null;\n") )
    RET_ONERROR( mywritestr(f, "    private ContextMenuFactorySPI menuFactory = null;\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    public ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "Cell(CellID cellID, CellCache cellCache) {\n") )
    RET_ONERROR( mywritestr(f, "        super(cellID, cellCache);\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )

    RET_ONERROR( mywritestr(f, "    @Override\n") )
    RET_ONERROR( mywritestr(f, "    public void setClientState(CellClientState state) {\n") )
    RET_ONERROR( mywritestr(f, "        super.setClientState(state);\n") )
    RET_ONERROR( mywritestr(f, "        this.message = ((") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellClientState)state).getMessage();\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    public String getMessage() {\n") )
    RET_ONERROR( mywritestr(f, "        return this.message;\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    @Override\n") )
    RET_ONERROR( mywritestr(f, "    public void setStatus(CellStatus status, boolean increasing) {\n") )
    RET_ONERROR( mywritestr(f, "        super.setStatus(status, increasing);\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "        if (status == CellStatus.DISK && increasing == false) {\n") )
    RET_ONERROR( mywritestr(f, "            listener.removeFromEntity(renderer.getEntity());\n") )
    RET_ONERROR( mywritestr(f, "            listener = null;\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "            ChannelComponent channel = getComponent(ChannelComponent.class);\n") )
    RET_ONERROR( mywritestr(f, "            if ((") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellChangeMessage.class != null) && (channel != null))\n") )
    RET_ONERROR( mywritestr(f, "                channel.removeMessageReceiver(") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellChangeMessage.class);\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "            if (menuFactory != null) {\n") )
    RET_ONERROR( mywritestr(f, "                contextComp.removeContextMenuFactory(menuFactory);\n") )
    RET_ONERROR( mywritestr(f, "                menuFactory = null;\n") )
    RET_ONERROR( mywritestr(f, "            }\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "        else if (status == CellStatus.RENDERING && increasing == true) {\n") )
    RET_ONERROR( mywritestr(f, "            listener = new MouseEventListener();\n") )
    RET_ONERROR( mywritestr(f, "            listener.addToEntity(renderer.getEntity());\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "            ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellMessageReceiver recv = new ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellMessageReceiver();\n") )
    RET_ONERROR( mywritestr(f, "            ChannelComponent channel = getComponent(ChannelComponent.class);\n") )
    RET_ONERROR( mywritestr(f, "            channel.addMessageReceiver(") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellChangeMessage.class, recv);\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "            if (menuFactory == null) {\n") )
    RET_ONERROR( mywritestr(f, "                final MenuItemListener l = new MenuItemListener();\n") )
    RET_ONERROR( mywritestr(f, "                menuFactory = new ContextMenuFactorySPI() {\n") )
    RET_ONERROR( mywritestr(f, "                    public ContextMenuItem[] getContextMenuItems(ContextEvent event) {\n") )
    RET_ONERROR( mywritestr(f, "                        return new ContextMenuItem[]{\n") )
    RET_ONERROR( mywritestr(f, "                                    new SimpleContextMenuItem(\"Send mouseclick\", l)\n") )
    RET_ONERROR( mywritestr(f, "                                };\n") )
    RET_ONERROR( mywritestr(f, "                    }\n") )
    RET_ONERROR( mywritestr(f, "                };\n") )
    RET_ONERROR( mywritestr(f, "                contextComp.addContextMenuFactory(menuFactory);\n") )
    RET_ONERROR( mywritestr(f, "            }\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )

    RET_ONERROR( mywritestr(f, "    @Override\n") )
    RET_ONERROR( mywritestr(f, "    protected CellRenderer createCellRenderer(RendererType rendererType) {\n") )
    RET_ONERROR( mywritestr(f, "        if (rendererType == RendererType.RENDERER_JME) {\n") )
    RET_ONERROR( mywritestr(f, "            this.renderer = new ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellRenderer(this);\n") )
    RET_ONERROR( mywritestr(f, "            return this.renderer;\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "        else {\n") )
    RET_ONERROR( mywritestr(f, "            return super.createCellRenderer(rendererType);\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )

    RET_ONERROR( mywritestr(f, "    class MenuItemListener implements ContextMenuActionListener {\n") )
    RET_ONERROR( mywritestr(f, "        public void actionPerformed(ContextMenuItemEvent event) {\n") )
    RET_ONERROR( mywritestr(f, "            renderer.update") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "();\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "            ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellChangeMessage msg = new ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellChangeMessage(getCellID(), message);\n") )
    RET_ONERROR( mywritestr(f, "            sendCellMessage(msg);\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )

    RET_ONERROR( mywritestr(f, "    // internal keyCodes must be > 20 cause of X3D keycodes;\n") )
    RET_ONERROR( mywritestr(f, "    public static final int keyCodeEnter     = 21;\n") )
    RET_ONERROR( mywritestr(f, "    public static final int keyCodeDelete    = 22;\n") )
    RET_ONERROR( mywritestr(f, "    public static final int keyCodeBackspace = 23;\n") )
    RET_ONERROR( mywritestr(f, "    public static final int keyCodeAlt       = 24;\n") )
    RET_ONERROR( mywritestr(f, "    public static final int keyCodeControl   = 25;\n") )
    RET_ONERROR( mywritestr(f, "    public static final int keyCodeShift     = 26;\n") )
    RET_ONERROR( mywritestr(f, "\n") )

    RET_ONERROR( mywritestr(f, "    public String handleKeyInput(KeyEvent3D ke) {\n") )
    RET_ONERROR( mywritestr(f, "        /* Translation table for keyboard input\n") )    
    RET_ONERROR( mywritestr(f, "        // see Chapter 21.4.1 KeySensor of ISO/IEC 19775-1:2008\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "        key       |  wonderland     |  x3d keyPress/Release |  x3d actionKeyPress/Release  |  internal used keycode\n") )
    RET_ONERROR( mywritestr(f, "        ----------+-----------------+-----------------------+------------------------------+-----------------------\n") )
    RET_ONERROR( mywritestr(f, "        HOME      |  VK_HOME        |                       |  13                          |\n") )
    RET_ONERROR( mywritestr(f, "        ----------+-----------------+-----------------------+------------------------------+----------------------\n") )
    RET_ONERROR( mywritestr(f, "        PGDN      |  VK_PAGE_DOWN   |                       |  16                          |\n") )
    RET_ONERROR( mywritestr(f, "        ----------+-----------------+-----------------------+------------------------------+----------------------\n") )
    RET_ONERROR( mywritestr(f, "        LEFT      |  VK_LEFT        |                       |  19                          |\n") )
    RET_ONERROR( mywritestr(f, "        ----------+-----------------+-----------------------+------------------------------+----------------------\n") )
    RET_ONERROR( mywritestr(f, "        END       |  VK_END         |                       |  14                          |\n") )
    RET_ONERROR( mywritestr(f, "        ----------+-----------------+-----------------------+------------------------------+----------------------\n") )
    RET_ONERROR( mywritestr(f, "        UP        |  VK_UP          |                       |  17                          |\n") )
    RET_ONERROR( mywritestr(f, "        ----------+-----------------+-----------------------+------------------------------+----------------------\n") )
    RET_ONERROR( mywritestr(f, "        RIGHT     |  VK_RIGHT       |                       |  20                          |\n") )
    RET_ONERROR( mywritestr(f, "        ----------+-----------------+-----------------------+------------------------------+----------------------\n") )
    RET_ONERROR( mywritestr(f, "        PGUP      |  VK_PAGE_UP     |                       |  15                          |\n") )
    RET_ONERROR( mywritestr(f, "        ----------+-----------------+-----------------------+------------------------------+----------------------\n") )
    RET_ONERROR( mywritestr(f, "        DOWN      |  VK_DOWN        |                       |  18                          |\n") )
    RET_ONERROR( mywritestr(f, "        ----------+-----------------+-----------------------+------------------------------+----------------------\n") )
    RET_ONERROR( mywritestr(f, "        F1-F12    |  VK_F1-VK_F12   |                       |  1-12                        |\n") )
    RET_ONERROR( mywritestr(f, "        ----------+-----------------+-----------------------+------------------------------+----------------------\n") )
    RET_ONERROR( mywritestr(f, "        ALT       |  VK_ALT         |                       |  KeySensor.altKey TRUE       | keyCodeAlt (24)\n") )
    RET_ONERROR( mywritestr(f, "        ----------+-----------------+-----------------------+------------------------------+----------------------\n") )
    RET_ONERROR( mywritestr(f, "        CTRL      |  VK_CONTROL     |                       |  KeySensor.controlKey TRUE   | keyCodeControl (25)\n") )
    RET_ONERROR( mywritestr(f, "        ----------+-----------------+-----------------------+------------------------------+----------------------\n") )
    RET_ONERROR( mywritestr(f, "        SHIFT     |  VK_SHIFT       |                       |  KeySensor.shiftKey TRUE     | keyCodeShift (26)\n") )
    RET_ONERROR( mywritestr(f, "        ----------+-----------------+-----------------------+------------------------------+----------------------\n") )
    RET_ONERROR( mywritestr(f, "        a-z       |  65-90          | \"a\"-\"z\"           |                              |\n") )
    RET_ONERROR( mywritestr(f, "        ----------+-----------------+-----------------------+------------------------------+----------------------\n") )
    RET_ONERROR( mywritestr(f, "        A-Z       |  65-90          | \"A\"-\"Z:            |                              |\n") )
    RET_ONERROR( mywritestr(f, "        ----------+-----------------+-----------------------+------------------------------+----------------------\n") )
    RET_ONERROR( mywritestr(f, "        Backspace |  VK_BACK_SPACE  |                       |                              | keyCodeBackspace (23)\n") )
    RET_ONERROR( mywritestr(f, "        ----------+-----------------+-----------------------+------------------------------+----------------------\n") )
    RET_ONERROR( mywritestr(f, "        Delete    |  VK_DELETE      |                       |                              | keyCodeDelete (22)\n") )
    RET_ONERROR( mywritestr(f, "        ----------+-----------------+-----------------------+------------------------------+----------------------\n") )
    RET_ONERROR( mywritestr(f, "        Enter     |  VK_ENTER       |                       |                              | keyCodeEnter (21)\n") )
    RET_ONERROR( mywritestr(f, "        ----------+-----------------+-----------------------+------------------------------+----------------------\n") )
    RET_ONERROR( mywritestr(f, "        ascii     |  ?              | ascii character       |                              |\n") )
    RET_ONERROR( mywritestr(f, "        */\n") )    
    RET_ONERROR( mywritestr(f, "        String ret = \"\";\n") )
    RET_ONERROR( mywritestr(f, "        int key = ke.getKeyCode();\n") )
    RET_ONERROR( mywritestr(f, "        switch (key) {\n") )
    RET_ONERROR( mywritestr(f, "          case KeyEvent.VK_HOME:\n") )
    RET_ONERROR( mywritestr(f, "            ret += \"13\";\n") )
    RET_ONERROR( mywritestr(f, "            break;\n") )
    RET_ONERROR( mywritestr(f, "          case KeyEvent.VK_PAGE_DOWN:\n") )
    RET_ONERROR( mywritestr(f, "            ret += \"16\";\n") )
    RET_ONERROR( mywritestr(f, "            break;\n") )
    RET_ONERROR( mywritestr(f, "          case KeyEvent.VK_LEFT:\n") )
    RET_ONERROR( mywritestr(f, "            ret += \"19\";\n") )
    RET_ONERROR( mywritestr(f, "            break;\n") )
    RET_ONERROR( mywritestr(f, "          case KeyEvent.VK_END:\n") )
    RET_ONERROR( mywritestr(f, "            ret += \"14\";\n") )
    RET_ONERROR( mywritestr(f, "            break;\n") )
    RET_ONERROR( mywritestr(f, "          case KeyEvent.VK_UP:\n") )
    RET_ONERROR( mywritestr(f, "            ret += \"17\";\n") )
    RET_ONERROR( mywritestr(f, "            break;\n") )
    RET_ONERROR( mywritestr(f, "          case KeyEvent.VK_RIGHT:\n") )
    RET_ONERROR( mywritestr(f, "            ret += \"20\";\n") )
    RET_ONERROR( mywritestr(f, "            break;\n") )
    RET_ONERROR( mywritestr(f, "          case KeyEvent.VK_PAGE_UP:\n") )
    RET_ONERROR( mywritestr(f, "            ret += \"15\";\n") )
    RET_ONERROR( mywritestr(f, "            break;\n") )
    RET_ONERROR( mywritestr(f, "          case KeyEvent.VK_DOWN:\n") )
    RET_ONERROR( mywritestr(f, "            ret += \"18\";\n") )
    RET_ONERROR( mywritestr(f, "            break;\n") )
    RET_ONERROR( mywritestr(f, "          case KeyEvent.VK_ALT:\n") )
    RET_ONERROR( mywritestr(f, "            ret += keyCodeAlt;\n") )
    RET_ONERROR( mywritestr(f, "            break;\n") )
    RET_ONERROR( mywritestr(f, "          case KeyEvent.VK_CONTROL:\n") )
    RET_ONERROR( mywritestr(f, "            ret += keyCodeControl;\n") )
    RET_ONERROR( mywritestr(f, "            break;\n") )
    RET_ONERROR( mywritestr(f, "          case KeyEvent.VK_SHIFT:\n") )
    RET_ONERROR( mywritestr(f, "            ret += keyCodeShift;\n") )
    RET_ONERROR( mywritestr(f, "            break;\n") )
    RET_ONERROR( mywritestr(f, "          case KeyEvent.VK_DELETE:\n") )
    RET_ONERROR( mywritestr(f, "            ret += keyCodeDelete;\n") )
    RET_ONERROR( mywritestr(f, "            break;\n") )
    RET_ONERROR( mywritestr(f, "          case KeyEvent.VK_BACK_SPACE:\n") )
    RET_ONERROR( mywritestr(f, "            ret += keyCodeBackspace;\n") )
    RET_ONERROR( mywritestr(f, "            break;\n") )
    RET_ONERROR( mywritestr(f, "          case KeyEvent.VK_ENTER:\n") )
    RET_ONERROR( mywritestr(f, "            ret += keyCodeEnter;\n") )
    RET_ONERROR( mywritestr(f, "            break;\n") )
    RET_ONERROR( mywritestr(f, "          default:\n") )
    RET_ONERROR( mywritestr(f, "            if ((key >= KeyEvent.VK_F1) && (key <= KeyEvent.VK_F12))\n") )
    RET_ONERROR( mywritestr(f, "                ret += key - KeyEvent.VK_F1 + 1;\n") )
    RET_ONERROR( mywritestr(f, "            else\n") )
    RET_ONERROR( mywritestr(f, "                ret += \"char_\" + ke.getKeyChar();\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "        return ret;\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )

    RET_ONERROR( mywritestr(f, "    class MouseEventListener extends EventClassListener {\n") )
    RET_ONERROR( mywritestr(f, "        @Override\n") )
    RET_ONERROR( mywritestr(f, "        public Class[] eventClassesToConsume() {\n") )
    RET_ONERROR( mywritestr(f, "            return new Class[]{\n") )
    RET_ONERROR( mywritestr(f, "                MouseButtonEvent3D.class,\n") )
    RET_ONERROR( mywritestr(f, "                MouseDraggedEvent3D.class,\n") )
    RET_ONERROR( mywritestr(f, "                KeyEvent3D.class\n") )
    RET_ONERROR( mywritestr(f, "            };\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "\n") )

    RET_ONERROR( mywritestr(f, "        @Override\n") )
    RET_ONERROR( mywritestr(f, "        public void commitEvent(Event event) {\n") )
    RET_ONERROR( mywritestr(f, "            boolean sendMsg = false;\n") )
    RET_ONERROR( mywritestr(f, "            message = \"no_message\";\n") )
    RET_ONERROR( mywritestr(f, "            String moduleName = \"") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, "\";\n") )
    RET_ONERROR( mywritestr(f, "            if (event.getClass() == MouseButtonEvent3D.class) {\n") )
    RET_ONERROR( mywritestr(f, "                MouseButtonEvent3D mbe = (MouseButtonEvent3D) event;\n") )
    RET_ONERROR( mywritestr(f, "                if (mbe.getButton() != ButtonId.BUTTON1) {\n") )
    RET_ONERROR( mywritestr(f, "                    return;\n") )
    RET_ONERROR( mywritestr(f, "                }\n") )
    RET_ONERROR( mywritestr(f, "                sendMsg = true;\n") )
    RET_ONERROR( mywritestr(f, "                message = \"\";\n") )
    RET_ONERROR( mywritestr(f, "                if (mbe.isPressed())\n") )
    RET_ONERROR( mywritestr(f, "                    message += \"pressed_\";\n") )
    RET_ONERROR( mywritestr(f, "                else if (mbe.isReleased())\n") )
    RET_ONERROR( mywritestr(f, "                    message += \"released_\";\n") )
    RET_ONERROR( mywritestr(f, "                else\n") )
    RET_ONERROR( mywritestr(f, "                    return;\n") )
    RET_ONERROR( mywritestr(f, "                TriMesh tri = mbe.getPickDetails().getTriMesh();\n") )
    RET_ONERROR( mywritestr(f, "                if (tri != null)\n") )
    RET_ONERROR( mywritestr(f, "                    message += tri.getName();\n") )
    RET_ONERROR( mywritestr(f, "            } else if (event.getClass() == MouseDraggedEvent3D.class) {\n") )
    RET_ONERROR( mywritestr(f, "                MouseDraggedEvent3D mde = (MouseDraggedEvent3D) event;\n") )
    RET_ONERROR( mywritestr(f, "                sendMsg = true;\n") )
    RET_ONERROR( mywritestr(f, "                Vector3f point = null;\n") )
    RET_ONERROR( mywritestr(f, "                try {\n") )
    RET_ONERROR( mywritestr(f, "                    point = mde.getHitIntersectionPointLocal();\n") )
    RET_ONERROR( mywritestr(f, "                } catch (java.lang.NullPointerException e) {\n") )
    RET_ONERROR( mywritestr(f, "                    message = \"error_getHitIntersectionPointLocal\";\n") )
    RET_ONERROR( mywritestr(f, "                }\n") )
    RET_ONERROR( mywritestr(f, "                if (point != null) {\n") )
    RET_ONERROR( mywritestr(f, "                    message = \"dragged_\";\n") )
    RET_ONERROR( mywritestr(f, "                    Vector3f local = getLocalTransform().getTranslation(null);\n") )
    RET_ONERROR( mywritestr(f, "                    TriMesh tri = mde.getPickDetails().getTriMesh();\n") )
    RET_ONERROR( mywritestr(f, "                    if (tri != null)\n") )
    RET_ONERROR( mywritestr(f, "                        message += tri.getName();\n") )
    RET_ONERROR( mywritestr(f, "                    message += \"_\";\n") )
    RET_ONERROR( mywritestr(f, "                    message += point.getX() - 2 * local.getX();\n") )
    RET_ONERROR( mywritestr(f, "                    message += \"_\";\n") )
    RET_ONERROR( mywritestr(f, "                    message += point.getY() - 2 * local.getY();\n") )
    RET_ONERROR( mywritestr(f, "                    message += \"_\";\n") )
    RET_ONERROR( mywritestr(f, "                    message += point.getZ() - 2 * local.getZ();\n") )
    RET_ONERROR( mywritestr(f, "                }\n") )
    RET_ONERROR( mywritestr(f, "            } else if (event.getClass() == KeyEvent3D.class) {\n") )
    RET_ONERROR( mywritestr(f, "                KeyEvent3D ke = (KeyEvent3D) event;\n") )
    RET_ONERROR( mywritestr(f, "                message = \"key\";\n") )
    RET_ONERROR( mywritestr(f, "                message += \"_\";\n") )
    RET_ONERROR( mywritestr(f, "                if (ke.isPressed()) {\n") )
    RET_ONERROR( mywritestr(f, "                    sendMsg = true;\n") )
    RET_ONERROR( mywritestr(f, "                    message += \"pressed_\";\n") )
    RET_ONERROR( mywritestr(f, "                } else if (ke.isReleased()) {\n") )
    RET_ONERROR( mywritestr(f, "                    sendMsg = true;\n") )
    RET_ONERROR( mywritestr(f, "                    message += \"released_\";\n") )
    RET_ONERROR( mywritestr(f, "                }\n") )
    RET_ONERROR( mywritestr(f, "                message += moduleName;\n") )
    RET_ONERROR( mywritestr(f, "                message += \"_\";\n") )
    RET_ONERROR( mywritestr(f, "                message += handleKeyInput(ke);\n") )
    RET_ONERROR( mywritestr(f, "            }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "            if (sendMsg) {\n") )
    RET_ONERROR( mywritestr(f, "                renderer.update") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "();\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "                ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellChangeMessage msg = new ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellChangeMessage(getCellID(), message);\n") )
    RET_ONERROR( mywritestr(f, "                sendCellMessage(msg);\n") )
    RET_ONERROR( mywritestr(f, "            }\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )

    RET_ONERROR( mywritestr(f, "    class ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellMessageReceiver implements ComponentMessageReceiver {\n") )
    RET_ONERROR( mywritestr(f, "        public void messageReceived(CellMessage cellMessage) {\n") )
    RET_ONERROR( mywritestr(f, "            ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellChangeMessage sccm = (") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellChangeMessage)cellMessage;\n") )
    RET_ONERROR( mywritestr(f, "            if (sccm.getSenderID().equals(getCellCache().getSession().getID()) == false) {\n") )
    RET_ONERROR( mywritestr(f, "                message = sccm.getMessage();\n") )
    RET_ONERROR( mywritestr(f, "                renderer.update") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "();\n") )
    RET_ONERROR( mywritestr(f, "            }\n") )
    RET_ONERROR( mywritestr(f, "        }\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "}\n") )
    return 0;
}

static int 
writeInfoContextMenuFactory(int f, const char *upperName, const char *lowerName)
{
    RET_ONERROR( mywritestr(f, "/**\n") )
    RET_ONERROR( mywritestr(f, " * Project Wonderland\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * Copyright (c) 2004-2009, Sun Microsystems, Inc., All Rights Reserved\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * Redistributions in source code form must reproduce the above\n") )
    RET_ONERROR( mywritestr(f, " * copyright and this condition.\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * The contents of this file are subject to the GNU General Public\n") )
    RET_ONERROR( mywritestr(f, " * License, Version 2 (the \"License\"); you may not use this file\n") )
    RET_ONERROR( mywritestr(f, " * except in compliance with the License. A copy of the License is\n") )
    RET_ONERROR( mywritestr(f, " * available at http://www.opensource.org/licenses/gpl-license.php.\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * Sun designates this particular file as subject to the \"Classpath\"\n") )
    RET_ONERROR( mywritestr(f, " * exception as provided by Sun in the License file that accompanied\n") )
    RET_ONERROR( mywritestr(f, " * this code.\n") )
    RET_ONERROR( mywritestr(f, " */\n") )
    RET_ONERROR( mywritestr(f, "package org.jdesktop.wonderland.modules.") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, ".client;\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "import javax.swing.JOptionPane;\n") )
    RET_ONERROR( mywritestr(f, "import javax.swing.SwingUtilities;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.cell.Cell;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.contextmenu.ContextMenuActionListener;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.contextmenu.ContextMenuItem;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.contextmenu.ContextMenuItemEvent;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.contextmenu.SimpleContextMenuItem;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.contextmenu.annotation.ContextMenuFactory;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.contextmenu.spi.ContextMenuFactorySPI;\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.client.scenemanager.event.ContextEvent;\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "@ContextMenuFactory\n") )
    RET_ONERROR( mywritestr(f, "public class InfoContextMenuFactory implements ContextMenuFactorySPI {\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    public ContextMenuItem[] getContextMenuItems(ContextEvent event) {\n") )
    RET_ONERROR( mywritestr(f, "        return new ContextMenuItem[] {\n") )
    RET_ONERROR( mywritestr(f, "            new SimpleContextMenuItem(\"Info\", new ContextMenuActionListener() {\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "                public void actionPerformed(ContextMenuItemEvent event) {\n") )
    RET_ONERROR( mywritestr(f, "                    Cell cell = event.getCell();\n") )
    RET_ONERROR( mywritestr(f, "                    final String msg = \"Cell selected has ID \" + cell.getCellID();\n") )
    RET_ONERROR( mywritestr(f, "                    SwingUtilities.invokeLater(new Runnable() {\n") )
    RET_ONERROR( mywritestr(f, "                        public void run() {\n") )
    RET_ONERROR( mywritestr(f, "                            JOptionPane.showMessageDialog(null, msg);\n") )
    RET_ONERROR( mywritestr(f, "                        }\n") )
    RET_ONERROR( mywritestr(f, "                    });\n") )
    RET_ONERROR( mywritestr(f, "                }\n") )
    RET_ONERROR( mywritestr(f, "            })\n") )
    RET_ONERROR( mywritestr(f, "        };\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "}\n") )
    return 0;
}

static int 
writeBuild_xml(int f, const char *upperName, const char *lowerName)
{
    RET_ONERROR( mywritestr(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n") )
    RET_ONERROR( mywritestr(f, "<!-- \n") )
    RET_ONERROR( mywritestr(f, "   according to mail from Nicole Yankelovich and Wilfried Stuettgen from SUN:\n") )
    RET_ONERROR( mywritestr(f, "   https://wonderland-modules.dev.java.net/source/browse/wonderland-modules/trunk/0.5/stable/shape-module-tutorial/\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "   The included java-codes have a licence statement (GPL with \"Classpath\n") )
    RET_ONERROR( mywritestr(f, "   exception\") whereas the xml files doesn't. The files under\n") )
    RET_ONERROR( mywritestr(f, "   consideration are:\n") )
    RET_ONERROR( mywritestr(f, "   shape-tutorial-module/build.xml\n") )
    RET_ONERROR( mywritestr(f, "   shape-tutorial-module/nbproject/project.xml\n") )
    RET_ONERROR( mywritestr(f, "   shape-tutorial-module/my.module.properties\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "   Can you confirm that it is permitted to use this files under eg. GPL\n") )
    RET_ONERROR( mywritestr(f, "   license with Classpath exception ?\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "   Yes, that's correct. Those are build files that are considered part of\n") )
    RET_ONERROR( mywritestr(f, "   the code and are therefore covered under the same license.\n") )
    RET_ONERROR( mywritestr(f, "-->\n") )
    RET_ONERROR( mywritestr(f, " \n") )
    RET_ONERROR( mywritestr(f, "<project name=\"wonderland-${module.name}-module\" default=\"dist\" basedir=\".\">\n") )
    RET_ONERROR( mywritestr(f, "    <property file=\"my.module.properties\"/>\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    <!-- current directory needed in all scripts -->\n") )
    RET_ONERROR( mywritestr(f, "    <dirname property=\"current.dir\" file=\"${ant.file.wonderland-${module.name}-module}\"/>\n") )
    RET_ONERROR( mywritestr(f, "    <property name=\"modules.dir\" location=\"${wonderland.dir}/modules\"/>\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    <property name=\"module.src\" value=\"${module.plugin.src}\"/>\n") )
    RET_ONERROR( mywritestr(f, "    <property name=\"module.jarname\" value=\"${module.name}\"/>\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    <!-- import common build file -->\n") )
    RET_ONERROR( mywritestr(f, "    <import file=\"${modules.dir}/build-tools/build-scripts/module-build.xml\"/>\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    <target name=\"build\" depends=\"-module-init, -module-compile-common,\n") )
    RET_ONERROR( mywritestr(f, "                                                -module-compile-server,\n") )
    RET_ONERROR( mywritestr(f, "                                                -module-compile-client\"/>\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    <target name=\"dist\" depends=\"build\">\n") )
    RET_ONERROR( mywritestr(f, "        <mkdir dir=\"${module.dist.dir}\"/>\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "        <module name=\"${module.name}\" majorVersion=\"${module.version.major}\" minorVersion=\"${module.version.minor}\" jarfile=\"${module.dist.dir}/${module.jarname}.jar\"\n") )
    RET_ONERROR( mywritestr(f, "                description=\"${module.description}\" builddir=\"${build.dir}\">\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "            <client>\n") )
    RET_ONERROR( mywritestr(f, "                <clientjar name=\"${module.name}-client\" basedir=\"${build.classes.dir}\">\n") )
    RET_ONERROR( mywritestr(f, "                    <include name=\"${module.src}/client/**\"/>\n") )
    RET_ONERROR( mywritestr(f, "                    <include name=\"${module.src}/common/**\"/>\n") )
    RET_ONERROR( mywritestr(f, "                    <fileset dir=\"${current.dir}/src/classes\">\n") )
    RET_ONERROR( mywritestr(f, "                        <include name=\"${module.src}/client/resources/**\"/>\n") )
    RET_ONERROR( mywritestr(f, "                    </fileset>\n") )
    RET_ONERROR( mywritestr(f, "                </clientjar>\n") )
    RET_ONERROR( mywritestr(f, "            </client>\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "            <server>\n") )
    RET_ONERROR( mywritestr(f, "                <serverjar name=\"${module.name}-server\" basedir=\"${build.classes.dir}\">\n") )
    RET_ONERROR( mywritestr(f, "                    <include name=\"${module.src}/server/**\"/>\n") )
    RET_ONERROR( mywritestr(f, "                    <include name=\"${module.src}/common/**\"/>\n") )
    RET_ONERROR( mywritestr(f, "                </serverjar>\n") )
    RET_ONERROR( mywritestr(f, "            </server>\n") )
    RET_ONERROR( mywritestr(f, "            <art dir=\"${current.dir}/art\"/>\n") )
    RET_ONERROR( mywritestr(f, "        </module>\n") )
    RET_ONERROR( mywritestr(f, "    </target>\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    <target name=\"deploy\" depends=\"dist, -module-deploy\"/>\n") )
    RET_ONERROR( mywritestr(f, "    <target name=\"clean\" depends=\"-module-clean\"/>\n") )
    RET_ONERROR( mywritestr(f, "</project>\n") )
    return 0;
}

static int 
writeReadme_txt(int f, const char *upperName, const char *lowerName)
{
    RET_ONERROR( mywritestr(f, "This directory contains the wonderland (java source) export from\n") )
    RET_ONERROR( mywritestr(f, "the ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, " X3DV/VRML file written by the white_dune application\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "To compile to a dist/") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, ".jar wonderland module file, just type \"ant\"\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "For some wonderland versions a wonderland module file dist/") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, ".jar is rather useless, in this case type \"ant deploy\" to compile and load the module\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "It is also possible to use the \"nbproject/project.xml\" file with the\n") )
    RET_ONERROR( mywritestr(f, "netbeans application\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    return 0;
}

static int 
writeProject_xml(int f, const char *upperName, const char *lowerName)
{
    RET_ONERROR( mywritestr(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n") )
    RET_ONERROR( mywritestr(f, "<!--\n") )
    RET_ONERROR( mywritestr(f, "   according to mail from Nicole Yankelovich and Wilfried Stuettgen from SUN:\n") )
    RET_ONERROR( mywritestr(f, "   https://wonderland-modules.dev.java.net/source/browse/wonderland-modules/trunk/0.5/stable/shape-module-tutorial/\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "   The included java-codes have a licence statement (GPL with \"Classpath\n") )
    RET_ONERROR( mywritestr(f, "   exception\") whereas the xml files doesn't. The files under\n") )
    RET_ONERROR( mywritestr(f, "   consideration are:\n") )
    RET_ONERROR( mywritestr(f, "   shape-tutorial-module/build.xml\n") )
    RET_ONERROR( mywritestr(f, "   shape-tutorial-module/nbproject/project.xml\n") )
    RET_ONERROR( mywritestr(f, "   shape-tutorial-module/my.module.properties\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "   Can you confirm that it is permitted to use this files under eg. GPL\n") )
    RET_ONERROR( mywritestr(f, "   license with Classpath exception ?\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "   Yes, that's correct. Those are build files that are considered part of\n") )
    RET_ONERROR( mywritestr(f, "   the code and are therefore covered under the same license.\n") )
    RET_ONERROR( mywritestr(f, "-->\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "<project xmlns=\"http://www.netbeans.org/ns/project/1\">\n") )
    RET_ONERROR( mywritestr(f, "    <type>org.netbeans.modules.ant.freeform</type>\n") )
    RET_ONERROR( mywritestr(f, "    <configuration>\n") )
    RET_ONERROR( mywritestr(f, "        <general-data xmlns=\"http://www.netbeans.org/ns/freeform-project/1\">\n") )
    RET_ONERROR( mywritestr(f, "            <!-- Do not use Project Properties customizer when editing this file manually. -->\n") )
    RET_ONERROR( mywritestr(f, "            <name>") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, "</name>\n") )
    RET_ONERROR( mywritestr(f, "            <properties>\n") )
    RET_ONERROR( mywritestr(f, "                <property name=\"project.license\">gplv2</property>\n") )
    RET_ONERROR( mywritestr(f, "            </properties>\n") )
    RET_ONERROR( mywritestr(f, "            <folders>\n") )
    RET_ONERROR( mywritestr(f, "                <source-folder>\n") )
    RET_ONERROR( mywritestr(f, "                    <label>core/src/classes</label>\n") )
    RET_ONERROR( mywritestr(f, "                    <type>java</type>\n") )
    RET_ONERROR( mywritestr(f, "                    <location>core/src/classes</location>\n") )
    RET_ONERROR( mywritestr(f, "                </source-folder>\n") )
    RET_ONERROR( mywritestr(f, "            </folders>\n") )
    RET_ONERROR( mywritestr(f, "            <ide-actions>\n") )
    RET_ONERROR( mywritestr(f, "                <action name=\"build\">\n") )
    RET_ONERROR( mywritestr(f, "                    <target>dist</target>\n") )
    RET_ONERROR( mywritestr(f, "                </action>\n") )
    RET_ONERROR( mywritestr(f, "                <action name=\"clean\">\n") )
    RET_ONERROR( mywritestr(f, "                    <target>clean</target>\n") )
    RET_ONERROR( mywritestr(f, "                </action>\n") )
    RET_ONERROR( mywritestr(f, "                <action name=\"rebuild\">\n") )
    RET_ONERROR( mywritestr(f, "                    <target>clean</target>\n") )
    RET_ONERROR( mywritestr(f, "                    <target>dist</target>\n") )
    RET_ONERROR( mywritestr(f, "                </action>\n") )
    RET_ONERROR( mywritestr(f, "                <action name=\"javadoc\">\n") )
    RET_ONERROR( mywritestr(f, "                    <target>javadoc</target>\n") )
    RET_ONERROR( mywritestr(f, "                </action>\n") )
    RET_ONERROR( mywritestr(f, "            </ide-actions>\n") )
    RET_ONERROR( mywritestr(f, "            <export>\n") )
    RET_ONERROR( mywritestr(f, "                <type>folder</type>\n") )
    RET_ONERROR( mywritestr(f, "                <location>dist</location>\n") )
    RET_ONERROR( mywritestr(f, "                <build-target>dist</build-target>\n") )
    RET_ONERROR( mywritestr(f, "            </export>\n") )
    RET_ONERROR( mywritestr(f, "            <view>\n") )
    RET_ONERROR( mywritestr(f, "                <items>\n") )
    RET_ONERROR( mywritestr(f, "                    <source-folder style=\"packages\">\n") )
    RET_ONERROR( mywritestr(f, "                        <label>core/src/classes</label>\n") )
    RET_ONERROR( mywritestr(f, "                        <location>core/src/classes</location>\n") )
    RET_ONERROR( mywritestr(f, "                    </source-folder>\n") )
    RET_ONERROR( mywritestr(f, "                    <source-file>\n") )
    RET_ONERROR( mywritestr(f, "                        <location>build.xml</location>\n") )
    RET_ONERROR( mywritestr(f, "                    </source-file>\n") )
    RET_ONERROR( mywritestr(f, "                </items>\n") )
    RET_ONERROR( mywritestr(f, "                <context-menu>\n") )
    RET_ONERROR( mywritestr(f, "                    <ide-action name=\"build\"/>\n") )
    RET_ONERROR( mywritestr(f, "                    <ide-action name=\"rebuild\"/>\n") )
    RET_ONERROR( mywritestr(f, "                    <ide-action name=\"clean\"/>\n") )
    RET_ONERROR( mywritestr(f, "                    <ide-action name=\"javadoc\"/>\n") )
    RET_ONERROR( mywritestr(f, "                    <action>\n") )
    RET_ONERROR( mywritestr(f, "                        <label>Run Server</label>\n") )
    RET_ONERROR( mywritestr(f, "                        <target>run-server</target>\n") )
    RET_ONERROR( mywritestr(f, "                    </action>\n") )
    RET_ONERROR( mywritestr(f, "                    <action>\n") )
    RET_ONERROR( mywritestr(f, "                        <label>Debug Server</label>\n") )
    RET_ONERROR( mywritestr(f, "                        <target>debug-server</target>\n") )
    RET_ONERROR( mywritestr(f, "                    </action>\n") )
    RET_ONERROR( mywritestr(f, "                </context-menu>\n") )
    RET_ONERROR( mywritestr(f, "            </view>\n") )
    RET_ONERROR( mywritestr(f, "            <subprojects/>\n") )
    RET_ONERROR( mywritestr(f, "        </general-data>\n") )
    RET_ONERROR( mywritestr(f, "        <java-data xmlns=\"http://www.netbeans.org/ns/freeform-project-java/1\">\n") )
    RET_ONERROR( mywritestr(f, "            <compilation-unit>\n") )
    RET_ONERROR( mywritestr(f, "                <package-root>core/src/classes</package-root>\n") )
    RET_ONERROR( mywritestr(f, "                <built-to>dist</built-to>\n") )
    RET_ONERROR( mywritestr(f, "                <source-level>1.5</source-level>\n") )
    RET_ONERROR( mywritestr(f, "            </compilation-unit>\n") )
    RET_ONERROR( mywritestr(f, "        </java-data>\n") )
    RET_ONERROR( mywritestr(f, "    </configuration>\n") )
    RET_ONERROR( mywritestr(f, "</project>\n") )
    return 0;
}

static int 
writeProjectReadmeTxt(int f)
{
    RET_ONERROR( mywritestr(f, "When compiling a wonderland module, the java compiler may get out of memory\n") )
    RET_ONERROR( mywritestr(f, "resources.\n") )
    RET_ONERROR( mywritestr(f, "To fix the problem, you need to set the memory limits by extending the javac\n") )
    RET_ONERROR( mywritestr(f, "tag in the  file wonderland/build-tools/build-scripts/build-setup.xml e.g.\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    <javac ...\n") )
    RET_ONERROR( mywritestr(f, "        fork=\"true\"\n") )
    RET_ONERROR( mywritestr(f, "        memoryinitialsize=\"256m\"\n") )
    RET_ONERROR( mywritestr(f, "        memorymaximumsize=\"1024m\"\n") )
    RET_ONERROR( mywritestr(f, "    >\n") )
    return 0;
}
 
static int 
writeMyModuleProperties(int f, const char *upperName, const char *lowerName,
                        const char *exportDirectory)
{
    RET_ONERROR( mywritestr(f, "#################################################################################################\n") )
    RET_ONERROR( mywritestr(f, "# my.module.properties - Property files for Wonderland module projects                          #\n") )
    RET_ONERROR( mywritestr(f, "#                                                                                               #\n") )
    RET_ONERROR( mywritestr(f, "# Customize the properties in this file for your module project. A Wonderland module contains   #\n") )
    RET_ONERROR( mywritestr(f, "# a collection art, code, WFSs, etc..                                                           #\n") )
    RET_ONERROR( mywritestr(f, "#################################################################################################\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "#   according to mail from Nicole Yankelovich and Wilfried Stuettgen from SUN:\n") )
    RET_ONERROR( mywritestr(f, "#   https://wonderland-modules.dev.java.net/source/browse/wonderland-modules/trunk/0.5/stable/shape-module-tutorial/\n") )
    RET_ONERROR( mywritestr(f, "#\n") )
    RET_ONERROR( mywritestr(f, "#   The included java-codes have a licence statement (GPL with \"Classpath\n") )
    RET_ONERROR( mywritestr(f, "#   exception\") whereas the xml files doesn't. The files under\n") )
    RET_ONERROR( mywritestr(f, "#   consideration are:\n") )
    RET_ONERROR( mywritestr(f, "#   shape-tutorial-module/build.xml\n") )
    RET_ONERROR( mywritestr(f, "#   shape-tutorial-module/nbproject/project.xml\n") )
    RET_ONERROR( mywritestr(f, "#   shape-tutorial-module/my.module.properties\n") )
    RET_ONERROR( mywritestr(f, "#\n") )
    RET_ONERROR( mywritestr(f, "#   Can you confirm that it is permitted to use this files under eg. GPL\n") )
    RET_ONERROR( mywritestr(f, "#   license with Classpath exception ?\n") )
    RET_ONERROR( mywritestr(f, "#\n") )
    RET_ONERROR( mywritestr(f, "#   Yes, that's correct. Those are build files that are considered part of\n") )
    RET_ONERROR( mywritestr(f, "#   the code and are therefore covered under the same license.\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "#\n") )
    RET_ONERROR( mywritestr(f, "# Property: module.name (required)\n") )
    RET_ONERROR( mywritestr(f, "# The unique name of the module\n") )
    RET_ONERROR( mywritestr(f, "#\n") )
    RET_ONERROR( mywritestr(f, "module.name=") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "#\n") )
    RET_ONERROR( mywritestr(f, "# Property: module.version.major (required)\n") )
    RET_ONERROR( mywritestr(f, "# The major version number of the module\n") )
    RET_ONERROR( mywritestr(f, "#\n") )
    RET_ONERROR( mywritestr(f, "module.version.major=1\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "#\n") )
    RET_ONERROR( mywritestr(f, "# Property: module.version.minor (required)\n") )
    RET_ONERROR( mywritestr(f, "# The minor version number of the module, defaults to 0\n") )
    RET_ONERROR( mywritestr(f, "#\n") )
    RET_ONERROR( mywritestr(f, "module.version.minor=0\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "#\n") )
    RET_ONERROR( mywritestr(f, "# Property: module.description (optional)\n") )
    RET_ONERROR( mywritestr(f, "# A textual description of the module\n") )
    RET_ONERROR( mywritestr(f, "#\n") )
    RET_ONERROR( mywritestr(f, "module.description=A export of a VRML/X3DV file to a") )
    RET_ONERROR( mywritestr(f, " cell type module for Wonderland\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "#\n") )
    RET_ONERROR( mywritestr(f, "# Property: wonderland.dir (required)\n") )
    RET_ONERROR( mywritestr(f, "# The location of the Wonderland source\n") )
    RET_ONERROR( mywritestr(f, "#\n") )
    RET_ONERROR( mywritestr(f, "wonderland.dir=../../../\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "#\n") )
    RET_ONERROR( mywritestr(f, "# Property: module.plugin.src (optional)\n") )
    RET_ONERROR( mywritestr(f, "# Beneath src/classes/, where is the module code located (common/, client/, server/)\n") )
    RET_ONERROR( mywritestr(f, "#\n") )
    RET_ONERROR( mywritestr(f, "module.plugin.src=org/jdesktop/wonderland/modules/") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, "\n") )
    return 0;
}

static int 
writeCellClientState(int f, const char *upperName, const char *lowerName)
{
    RET_ONERROR( mywritestr(f, "/**\n") )
    RET_ONERROR( mywritestr(f, " * Project Wonderland\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * Copyright (c) 2004-2009, Sun Microsystems, Inc., All Rights Reserved\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * Redistributions in source code form must reproduce the above\n") )
    RET_ONERROR( mywritestr(f, " * copyright and this condition.\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * The contents of this file are subject to the GNU General Public\n") )
    RET_ONERROR( mywritestr(f, " * License, Version 2 (the \"License\"); you may not use this file\n") )
    RET_ONERROR( mywritestr(f, " * except in compliance with the License. A copy of the License is\n") )
    RET_ONERROR( mywritestr(f, " * available at http://www.opensource.org/licenses/gpl-license.php.\n") )
    RET_ONERROR( mywritestr(f, " *\n") )
    RET_ONERROR( mywritestr(f, " * Sun designates this particular file as subject to the \"Classpath\"\n") )
    RET_ONERROR( mywritestr(f, " * exception as provided by Sun in the License file that accompanied\n") )
    RET_ONERROR( mywritestr(f, " * this code.\n") )
    RET_ONERROR( mywritestr(f, " */\n") )
    RET_ONERROR( mywritestr(f, "package org.jdesktop.wonderland.modules.") )
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, ".common;\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "import org.jdesktop.wonderland.common.cell.state.CellClientState;\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "public class ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellClientState extends CellClientState {\n") )
    RET_ONERROR( mywritestr(f, "    private String message = null;\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    public ") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, "CellClientState() {\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    public String getMessage() {\n") )
    RET_ONERROR( mywritestr(f, "        return message;\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "    public void setMessage(String message) {\n") )
    RET_ONERROR( mywritestr(f, "        this.message = message;\n") )
    RET_ONERROR( mywritestr(f, "    }\n") )
    RET_ONERROR( mywritestr(f, "\n") )
    RET_ONERROR( mywritestr(f, "}\n") )
    return 0;
}

static int writeBundleProperties(int f, const char *upperName, 
                                        const char *lowerName)
{
    RET_ONERROR( mywritestr(f, lowerName) )
    RET_ONERROR( mywritestr(f, "=") )
    RET_ONERROR( mywritestr(f, upperName) )
    RET_ONERROR( mywritestr(f, " (from X3D file)") )
    RET_ONERROR( mywritestr(f, "\n") )
    return 0;
}

/* almost a XPM file */
/*
"128 128 3 1",
".      c #FFFFFF",
"+      c #000000",
*/
static const char *from_x3d_file_almostxpm[] = {
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
".......................++++++++.................................................................................................",
".....................++++++++++.................................................................................................",
"....................+++++++++++.................................................................................................",
"...................++++++++++++.................................................................................................",
"...................+++++++......................................................................................................",
"...................++++++.......................................................................................................",
"...................++++++.......................................................................................................",
"...................++++++.......................................................................................................",
"................+++++++++++++++...++++++....+++++.........++++++++...........++++++...++++++.......++++++.......................",
"................+++++++++++++++...++++++..+++++++.......++++++++++++.........++++++..+++++++++...++++++++++.....................",
"................+++++++++++++++...++++++.++++++++.....++++++++++++++++.......++++++++++++++++++.+++++++++++.....................",
"................+++++++++++++++...+++++++++++++++....++++++++++++++++++......+++++++++++++++++++++++++++++++....................",
"................+++++++++++++++...+++++++++++++++...++++++++++++++++++++.....+++++++++++++++++++++++++++++++....................",
"...................++++++.........+++++++++.....+...++++++++....++++++++.....++++++++....+++++++++....+++++++...................",
"...................++++++.........++++++++.........+++++++........++++++.....+++++++......+++++++......++++++...................",
"...................++++++.........+++++++..........++++++..........++++++....+++++++......+++++++......++++++...................",
"...................++++++.........++++++...........++++++..........++++++....++++++.......++++++.......++++++...................",
"...................++++++.........++++++...........++++++..........++++++....++++++.......++++++.......++++++...................",
"...................++++++.........++++++...........++++++..........++++++....++++++.......++++++.......++++++...................",
"...................++++++.........++++++...........++++++..........++++++....++++++.......++++++.......++++++...................",
"...................++++++.........++++++...........++++++..........++++++....++++++.......++++++.......++++++...................",
"...................++++++.........++++++...........+++++++........+++++++....++++++.......++++++.......++++++...................",
"...................++++++.........++++++............+++++++......+++++++.....++++++.......++++++.......++++++...................",
"...................++++++.........++++++............++++++++++++++++++++.....++++++.......++++++.......++++++...................",
"...................++++++.........++++++.............++++++++++++++++++......++++++.......++++++.......++++++...................",
"...................++++++.........++++++..............++++++++++++++++.......++++++.......++++++.......++++++...................",
"...................++++++.........++++++...............++++++++++++++........++++++.......++++++.......++++++...................",
"...................++++++.........++++++..................++++++++...........++++++.......++++++.......++++++...................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"......................++++++++...........++++++++........+++++++++.............++++++++++++++...................................",
".......................+++++++...........+++++++......++++++++++++++...........++++++++++++++++++...............................",
"........................+++++++.........+++++++......++++++++++++++++..........+++++++++++++++++++..............................",
"........................++++++++.......++++++++......+++++++++++++++++.........+++++++++++++++++++++............................",
".........................++++++++......+++++++.......++++++++++++++++++........++++++++++++++++++++++...........................",
"..........................+++++++.....+++++++........++++......++++++++........++++++.......++++++++++..........................",
"..........................++++++++...+++++++.........+..........+++++++........++++++.........++++++++..........................",
"...........................++++++++.++++++++.....................++++++........++++++...........+++++++.........................",
"............................+++++++.+++++++......................++++++........++++++............++++++.........................",
".............................+++++++++++++.......................++++++........++++++............++++++.........................",
".............................+++++++++++++.....................+++++++.........++++++.............++++++........................",
"..............................+++++++++++................++++++++++++..........++++++.............++++++........................",
"...............................+++++++++.................+++++++++++...........++++++.............++++++........................",
"...............................+++++++++.................++++++++++++..........++++++.............++++++........................",
"...............................++++++++++................+++++++++++++.........++++++.............++++++........................",
"..............................+++++++++++................++++++++++++++........++++++.............++++++........................",
".............................+++++++++++++......................++++++++.......++++++.............++++++........................",
"............................+++++++++++++++......................+++++++.......++++++............+++++++........................",
"............................+++++++.+++++++.......................++++++.......++++++............++++++.........................",
"...........................+++++++...+++++++......................++++++.......++++++...........+++++++.........................",
"..........................++++++++...++++++++.......+............+++++++.......++++++.........++++++++..........................",
"..........................+++++++.....+++++++.......++++........++++++++.......++++++.......++++++++++..........................",
".........................+++++++.......+++++++......+++++++++++++++++++........++++++++++++++++++++++...........................",
"........................++++++++.......++++++++.....+++++++++++++++++++........+++++++++++++++++++++............................",
"........................+++++++.........+++++++.....++++++++++++++++++.........+++++++++++++++++++..............................",
".......................+++++++...........+++++++.....+++++++++++++++...........++++++++++++++++++...............................",
"......................+++++++............++++++++.......+++++++++..............++++++++++++++...................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"..........................................++++++++...++++++.......++++++........................................................",
"........................................++++++++++...++++++.......++++++........................................................",
".......................................+++++++++++...++++++.......++++++........................................................",
"......................................++++++++++++...++++++.......++++++........................................................",
"......................................+++++++........++++++.......++++++........................................................",
"......................................++++++.........++++++.......++++++........................................................",
"......................................++++++......................++++++........................................................",
"......................................++++++......................++++++........................................................",
"...................................+++++++++++++++...++++++.......++++++.............++++++++...................................",
"...................................+++++++++++++++...++++++.......++++++...........++++++++++++.................................",
"...................................+++++++++++++++...++++++.......++++++.........++++++++++++++++...............................",
"...................................+++++++++++++++...++++++.......++++++........++++++++++++++++++..............................",
"...................................+++++++++++++++...++++++.......++++++.......++++++++.....++++++..............................",
"......................................++++++.........++++++.......++++++.......++++++........++++++.............................",
"......................................++++++.........++++++.......++++++.......++++++.........+++++.............................",
"......................................++++++.........++++++.......++++++......++++++..........++++++............................",
"......................................++++++.........++++++.......++++++......++++++++++++++++++++++............................",
"......................................++++++.........++++++.......++++++......++++++++++++++++++++++............................",
"......................................++++++.........++++++.......++++++......++++++++++++++++++++++............................",
"......................................++++++.........++++++.......++++++......++++++++++++++++++++++............................",
"......................................++++++.........++++++.......++++++......++++++............................................",
"......................................++++++.........++++++.......++++++.......+++++............................................",
"......................................++++++.........++++++.......++++++.......++++++.............+.............................",
"......................................++++++.........++++++.......++++++.......++++++++.......+++++.............................",
"......................................++++++.........++++++.......++++++........+++++++++++++++++++.............................",
"......................................++++++.........++++++.......++++++.........++++++++++++++++++.............................",
"......................................++++++.........++++++.......++++++...........++++++++++++++++.............................",
"......................................++++++.........++++++.......++++++..............++++++++++................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................",
"................................................................................................................................"};

#ifdef HAVE_LIBPNG
#include <png.h>
/*
 * write_png_file() based on example application 
 *
 * Copyright 2002-2010 Guillaume Cottenceau.
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 *
 */
static int write_png_file(const char *file_name, const char *strings[], 
                          int width, int height, 
                          png_byte color_type, png_byte bit_depth) 
{
    png_structp png_ptr;
    png_infop info_ptr;

    png_bytep *row_pointers = (png_bytep *)malloc(height * sizeof(png_bytep));
    for (int y = 0; y < height; y++) {
        png_byte* row = (png_byte *)malloc(width * 3 * sizeof(png_byte));
        for (int x = 0; x < width; x++) {
            png_byte* ptr = &(row[x * 3]);
            if (strings[y][x] == '+') {
                ptr[0] = 0;
                ptr[1] = 0;
                ptr[2] = 0;
            } else {
                ptr[0] = 255;
                ptr[1] = 255;
                ptr[2] = 255;
            }
        }
        row_pointers[y] = row;
     }

     /* create file */
     FILE *fp = fopen(file_name, "wb");
     if (!fp) {
         swDebugf("[write_png_file] File %s could not be opened for writing", file_name);
         return 1;
     }

     /* initialize stuff */
     png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

     if (!png_ptr) {
         swDebugf("[write_png_file] png_create_write_struct failed");
         return 1;
     }

     info_ptr = png_create_info_struct(png_ptr);
     if (!info_ptr) {
         swDebugf("[write_png_file] png_create_info_struct failed");
         return 1;
     }

     if (setjmp(png_jmpbuf(png_ptr))) {
         swDebugf("[write_png_file] Error during init_io");
         return 1;
     }

     png_init_io(png_ptr, fp);

     /* write header */
     if (setjmp(png_jmpbuf(png_ptr))) {
         swDebugf("[write_png_file] Error during writing header");
         return 1;
     }
  
     png_set_IHDR(png_ptr, info_ptr, width, height,
                  bit_depth, color_type, PNG_INTERLACE_NONE,
                  PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

     png_write_info(png_ptr, info_ptr);


     /* write bytes */
     if (setjmp(png_jmpbuf(png_ptr))) {
         swDebugf("[write_png_file] Error during writing bytes");
         return 1;
     }

     png_write_image(png_ptr, row_pointers);


     /* end write */
     if (setjmp(png_jmpbuf(png_ptr))) {
         swDebugf("[write_png_file] Error during end of write");
         return 1;
     }

     png_write_end(png_ptr, NULL);

     fclose(fp);

     /* cleanup heap allocation */
     for (int i = 0; i < height; i++)
         free(row_pointers[i]);
     free(row_pointers);

     return 0;
}
#endif

static int openFile(const char* path)
{
    int f = open(path, O_WRONLY | O_CREAT
#ifdef _WIN32
                  ,_S_IREAD | _S_IWRITE);
#else
                  ,S_IRUSR | S_IWUSR);
#endif
    if (f == -1)
        TheApp->MessageBoxPerror(path);
    return f;
}

static int makeDirectory(const char *directory)
{
    struct stat inodeInfo;
    if (stat(directory, &inodeInfo) == 0) {
        if ((inodeInfo.st_mode & S_IFMT) == S_IFDIR)
            return 0;
        else {
            swDebugf("error: can not create %s as a directory:", directory);
            swDebugf("already exists\n"); 
            return -1;
        }
    }
#ifdef _WIN32
    if (mkdir(directory) == -1) {
#else
    if (mkdir(directory, 0755) == -1) {
#endif
        TheApp->MessageBoxPerror(directory);
        return -1;
    }
    return 0;
}

static int isFile(const char *filename)
{
    struct stat inodeInfo;
    if (stat(filename, &inodeInfo) == 0)
        if (S_ISREG(inodeInfo.st_mode))
            return -1;
    return 0;
}

int
WonderlandModuleExport::writeArtPath(int filedes, const char *fileName) const
{
    RET_ONERROR( mywritestr(filedes, "\"wla://") )
    RET_ONERROR( mywritestr(filedes, m_lowerName) )
    RET_ONERROR( mywritestr(filedes, "/") )
    URL url(fileName);
    RET_ONERROR( mywritestr(filedes, url.GetFileName()) )
    RET_ONERROR( mywritestr(filedes, "\"") )
    return swCopyFile(NULL, url.ToPath(), m_artPath);
}

void  
WonderlandModuleExport::setArtPath(const char *path)
{ 
    static const char *artPath = "art";
    int artPathLen = strlen(path) + strlen(artPath) + 1;
    if (m_artPath != NULL)
        free(m_artPath);
    m_artPath = (char *)malloc(artPathLen);
    strcpy(m_artPath, path);
    strcat(m_artPath, artPath);
}

int
WonderlandModuleExport::write(const char* directory, const char* name, 
                              Scene *scene, bool manyClasses)
{
    char *upperName = strdup(name);
    upperName[0] = toupper(upperName[0]);

    char *lowerName = strdup(upperName);
    lowerName[0] = tolower(upperName[0]);

    m_lowerName = lowerName;
    
    const char *exportDirectory = WONDERLAND_EXPORT_DIRECTORY;
    char slash = swGetPathSeperator();

    MyString path = "";
    path += directory;
    if (path[path.length() - 1] != slash)
        path += slash;
    path += exportDirectory;
    RET_ONERROR( makeDirectory(path) )
    path += slash;

    path += lowerName;
    RET_ONERROR( makeDirectory(path) )
    path += slash;
    setArtPath(path);

    MyString nextFile = "";
    nextFile += path;
    nextFile += "build.xml";
    if (!isFile(nextFile)) {
        int f = openFile(nextFile);
        if (f < 0)
            return f;
        RET_ONERROR( writeBuild_xml(f, upperName, lowerName) )
        RET_ONERROR( swTruncateClose(f) )
    }

    nextFile = "";
    nextFile += path;
    nextFile += "Readme.txt";
    if (!isFile(nextFile)) {
        int f = openFile(nextFile);
        if (f < 0)
            return f;
        RET_ONERROR( writeReadme_txt(f, upperName, lowerName) )
        RET_ONERROR( swTruncateClose(f) )
    }

    nextFile = "";
    nextFile += path;
    nextFile += "my.module.properties";
    if (!isFile(nextFile)) {
        int f = openFile(nextFile);
        if (f < 0)
            return f;
        RET_ONERROR( writeMyModuleProperties(f, upperName, lowerName, 
                                              exportDirectory) )
        RET_ONERROR( swTruncateClose(f) )
    }

    MyString nextDirectory = "";
    nextDirectory += path;
    nextDirectory += "art";
    RET_ONERROR( makeDirectory(nextDirectory) )

    nextDirectory = "";
    nextDirectory += path;
    nextDirectory += "nbproject";
    RET_ONERROR( makeDirectory(nextDirectory) )
    nextDirectory += slash;

    nextFile = "";
    nextFile += nextDirectory;
    if (!isFile(nextFile)) {
        nextFile += "project.xml";
        int f = openFile(nextFile);
        if (f < 0)
            return f;
        RET_ONERROR( writeProject_xml(f, upperName, lowerName) )
        RET_ONERROR( swTruncateClose(f) )
    }

    nextFile = "";
    nextFile += nextDirectory;
    if (!isFile(nextFile)) {
        nextFile += "Readme.txt";
        int f = openFile(nextFile);
        if (f < 0)
            return f;
        RET_ONERROR( writeProjectReadmeTxt(f) )
        RET_ONERROR( swTruncateClose(f) )
    }


    nextDirectory = "";
    nextDirectory += path;
    nextDirectory += "build";
    RET_ONERROR( makeDirectory(nextDirectory) )

    MyString classDir = "";
    classDir += path;
    classDir += "src";
    RET_ONERROR( makeDirectory(classDir) )
    classDir += slash;

    classDir += "classes";
    RET_ONERROR( makeDirectory(classDir) )
    classDir += slash;

    classDir += "org";
    RET_ONERROR( makeDirectory(classDir) )
    classDir += slash;

    classDir += "jdesktop";
    RET_ONERROR( makeDirectory(classDir) )
    classDir += slash;

    classDir += "wonderland";
    RET_ONERROR( makeDirectory(classDir) )
    classDir += slash;

    classDir += "modules";
    RET_ONERROR( makeDirectory(classDir) )
    classDir += slash;

    classDir += lowerName;
    RET_ONERROR( makeDirectory(classDir) )
    classDir += slash;

    nextDirectory = "";
    nextDirectory += classDir;
    nextDirectory += "client";
    RET_ONERROR( makeDirectory(nextDirectory) )
    nextDirectory += slash;

    MyString clientDir = "";
    clientDir += nextDirectory;

    nextFile = "";
    nextFile += nextDirectory;
    if (!isFile(nextFile)) {
        nextFile += "InfoContextMenuFactory.java";
        int f = openFile(nextFile);
        if (f < 0)
            return f;
        RET_ONERROR( writeInfoContextMenuFactory(f, upperName, lowerName) )
        RET_ONERROR( swTruncateClose(f) )
    }

    nextFile = "";
    nextFile += nextDirectory;
    nextFile += upperName;
    nextFile += "Cell.java";
    if (!isFile(nextFile)) {
        int f = openFile(nextFile);
        if (f < 0)
            return f;
        RET_ONERROR( writeCell(f, upperName, lowerName) )
        RET_ONERROR( swTruncateClose(f) )
    }

    nextFile = "";
    nextFile += nextDirectory;
    nextFile += upperName;
    nextFile += "CellFactory.java";
    if (!isFile(nextFile)) {
        int f = openFile(nextFile);
        if (f < 0)
            return f;
        RET_ONERROR( writeCellFactory(f, upperName, lowerName) )
        RET_ONERROR( swTruncateClose(f) )
    }

    nextDirectory += "resources";
    RET_ONERROR( makeDirectory(nextDirectory) )
    nextDirectory += slash;

    nextFile = "";
    nextFile += nextDirectory;
    nextFile += "Bundle.properties";
    if (!isFile(nextFile)) {
        int f = openFile(nextFile);
        if (f < 0)
            return f;
        RET_ONERROR( writeBundleProperties(f, upperName, lowerName) )
        RET_ONERROR( swTruncateClose(f) )
    }

#ifdef HAVE_LIBPNG
    nextFile = "";
    nextFile += nextDirectory;
    nextFile += "x3d.png";
    if (!isFile(nextFile))
        RET_ONERROR( write_png_file(nextFile, from_x3d_file_almostxpm, 
                                    128, 128, PNG_COLOR_TYPE_RGB, 8) )
#endif

    nextDirectory = "";
    nextDirectory += clientDir;
    nextDirectory += "cell";
    RET_ONERROR( makeDirectory(nextDirectory) )
    nextDirectory += slash;

    nextFile = "";
    nextFile += nextDirectory;
    nextFile += upperName;
    nextFile += "CellRenderer.java";
    if (!isFile(nextFile)) {
        int f = openFile(nextFile);
        if (f < 0)
            return f;
        RET_ONERROR( writeCellRenderer(f, upperName, lowerName, scene) )

        RET_ONERROR( swTruncateClose(f) )
    }

    nextFile = "";
    nextFile += nextDirectory;
    nextFile += upperName;
    nextFile += ".java";
    int f = openFile(nextFile);
    if (f < 0)
        return f;
    RET_ONERROR( writeCellRendererX3dData(f, upperName, lowerName, scene, 
                                          manyClasses) )
    RET_ONERROR( swTruncateClose(f) )

    nextDirectory = "";
    nextDirectory += classDir;
    nextDirectory += "server";
    RET_ONERROR( makeDirectory(nextDirectory) )
    nextDirectory += slash;


    nextFile = "";
    nextFile += nextDirectory;
    nextFile += upperName;
    nextFile += "CellMO.java";
    if (!isFile(nextFile)) {
        int f = openFile(nextFile);
        if (f < 0)
            return f;
        RET_ONERROR( writeCellMO(f, upperName, lowerName) )
        RET_ONERROR( swTruncateClose(f) )
    }
    
    nextDirectory = "";
    nextDirectory += classDir;
    nextDirectory += "common";
    RET_ONERROR( makeDirectory(nextDirectory) )
    nextDirectory += slash;


    nextFile = "";
    nextFile += nextDirectory;
    nextFile += upperName;
    nextFile += "CellChangeMessage.java";
    if (!isFile(nextFile)) {
        int f = openFile(nextFile);
        if (f < 0)
            return f;
        RET_ONERROR( writeCellChangeMessage(f, upperName, lowerName) )
        RET_ONERROR( swTruncateClose(f) )
    }

    nextFile = "";
    nextFile += nextDirectory;
    nextFile += upperName;
    nextFile += "CellServerState.java";
    if (!isFile(nextFile)) {
        int f = openFile(nextFile);
        if (f < 0)
            return f;
        RET_ONERROR( writeCellServerState(f, upperName, lowerName) )
        RET_ONERROR( swTruncateClose(f) )
    }

    
    nextFile = "";
    nextFile += nextDirectory;
    nextFile += upperName;
    nextFile += "CellClientState.java";
    if (!isFile(nextFile)) {
        int f = openFile(nextFile);
        if (f < 0)
            return f;
        RET_ONERROR( writeCellClientState(f, upperName, lowerName) )
        RET_ONERROR( swTruncateClose(f) )
    }

    free(upperName);
    free(lowerName);
    
    return 0;    
}

#ifdef TEST
int main(int argc, char** argv)
{
    if (argc != 2) {
        swDebugf("Usage:\n");
        swDebugf("%s directory\n", argv[0]);
        exit(1);
    }
    if (writeModule(argv[1], "Example", NULL) != 0)
        myperror("writeModule ");    
}
#endif

