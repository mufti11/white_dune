//
//  MyDocument.m
//  white_dune
//
//  
 /*
 * MyDocument.m
 *
 * cocoa dune starter application
 *
 * Copyright (C) 2004 J. "MUFTI" Scheurich
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

//

#import "MyDocument.h"
@implementation MyDocument

NSMenu *dockMenu;
NSMenu *dockSubMenu;

-   (id) initWithNib:(NSString *)nibName dictionary:(NSDictionary *)dictionary controller:(id)controller
{
    self = [super init];
    if (self) {
        // Add your subclass-specific initialization here.
        // If an error occurs here, send a [self release] message and return nil.

	}
    return self;
}

- (NSString *)windowNibName
{
    // Override returning the nib file name of the document
    // If you need to use a subclass of NSWindowController or if your document supports multiple NSWindowControllers, you should remove this method and override -makeWindowControllers instead.
    return @"MyDocument";
}
- (void) dealloc
{
  [super dealloc];
}

- (void) awakeFromNib
{
  dockMenu = [[NSMenu alloc] init];
  [dockMenu setAutoenablesItems: NO];

}


- (void)windowControllerDidLoadNib:(NSWindowController *) aController
{
    [super windowControllerDidLoadNib:aController];

    // Add any code here that needs to be executed once the windowController has loaded the document's window.
    char* dunepath=getenv("DUNEPATH");
    if (dunepath==NULL) {
		printf("warning: $DUNEPATH not set, using .\n");
		dunepath=".";
	}
    char* dunestring="/dune -startX11aqua || ( RC=$? && osascript -l AppleScript -e 'tell Application \"Finder\" to display dialog \"white_dune error !\" ' ; if test $RC = 11 ; then osascript -l AppleScript -e 'tell Application \"Finder\" to display dialog \"white_dune error ! Is X11 for MacOSX available ? see http://www.apple.com/macosx/features/x11/\" ' ; else if test $RC != 1 ; then osascript -l AppleScript -e 'tell Application \"Finder\" to display dialog \"try to restore your file with menupoint File -> ???.dune_crash_???.wrl\" ' ; fi ; fi ) &";
	char* options=getenv("DUNEOPTIONS");
	int commandlen=strlen(dunepath)+strlen(dunestring)+2;
	if (options!=NULL)
	    commandlen+=strlen(options);
	char* command=(char *) malloc(commandlen);
    strcpy(command, dunepath);
	strcat(command, dunestring);
	strcat(command, " ");
	if (options!=NULL)
	    strcat(command, options);
	system(command);
}

- (NSData *)dataRepresentationOfType:(NSString *)aType
{
    // Insert code here to write your document from the given data.  You can also choose to override -fileWrapperRepresentationOfType: or -writeToFile:ofType: instead.
    return nil;
}

- (BOOL)readFromFile:(NSString *)fileName ofType:(NSString *)docType
{
    const char *file = [fileName lossyCString];
    char* dunepath=getenv("DUNEPATH");
    if (dunepath==NULL) {
		printf("warning: $DUNEPATH not set, using .\n");
		dunepath=".";
	}
    char* dunestring="/dune -startX11aqua";
	char* options=getenv("DUNEOPTIONS");
    char* errorstring=" || ( RC=$? && osascript -l AppleScript -e 'tell Application \"Finder\" to display dialog \"white_dune error !\" ' ; if test $RC = 11 ; then osascript -l AppleScript -e 'tell Application \"Finder\" to display dialog \"Is X11 for MacOSX available ? see http://www.apple.com/macosx/features/x11/\" ' ; else if test $RC != 1 ; then osascript -l AppleScript -e 'tell Application \"Finder\" to display dialog \"try to restore your file with menupoint File -> ???.dune_crash_???.wrl\" ' ; fi ; fi ) &";
	int commandlen=strlen(dunepath)+strlen(dunestring)+strlen(file)+6+strlen(errorstring);
	if (options!=NULL)
	    commandlen+=strlen(options);
    char* command=(char *) malloc(commandlen);
    strcpy(command, dunepath);
    strcat(command, dunestring);
    strcat(command, " ");
	if (options!=NULL)
	    strcat(command, options);
    strcat(command, " \"");
    strcat(command, file);
	strcat(command, "\"");
	strcat(command, errorstring);
    system(command);
    return false;
}

- (BOOL)loadDataRepresentation:(NSData *)data ofType:(NSString *)aType
{
    // Insert code here to read your document from the given data.  You can also choose to override -loadFileWrapperRepresentation:ofType: or -readFromFile:ofType: instead.
    return false;
}



- (NSMenu *)applicationDockMenu:(NSApplication *)sender
{
    if ([[menuRadio selectedCell] tag] == 0) {
        return nil;
    }
    return dockMenu;
}


- (IBAction)setRed:(id)sender
{
//    [colorWell setColor:[NSColor redColor]];
}

- (IBAction)setGreen:(id)sender
{
//    [colorWell setColor:[NSColor greenColor]];
}

- (IBAction)setBlue:(id)sender
{
//    [colorWell setColor:[NSColor blueColor]];
}


@end
