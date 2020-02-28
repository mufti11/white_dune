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
#define HELP_ANCHOR_KEY @"helpAnchor"
@implementation MyDocument

NSString *_helpAnchor;

- (id) initWithNib:(NSString *)nibName dictionary:(NSDictionary *)dictionary controller:(id)controller
{
    self = [super init];
    if (self) {
    
        // Add your subclass-specific initialization here.
        // If an error occurs here, send a [self release] message and return nil.
    
    }
    _helpAnchor = [[dictionary objectForKey:HELP_ANCHOR_KEY] retain];
    
    
    return self;
}

- (NSString *)windowNibName
{
    // Override returning the nib file name of the document
    // If you need to use a subclass of NSWindowController or if your document supports multiple NSWindowControllers, you should remove this method and override -makeWindowControllers instead.
    return @"MyDocument";
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
	unsigned long commandlen=strlen(dunepath)+strlen(dunestring)+2;
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

- (BOOL)loadDataRepresentation:(NSData *)data ofType:(NSString *)aType
{
    // Insert code here to read your document from the given data.  You can also choose to override -loadFileWrapperRepresentation:ofType: or -readFromFile:ofType: instead.
    return false;
}
- (IBAction) showHelp:(id) sender{
    NSAssert(_helpAnchor, @"Help anchor was not set");
    [[NSHelpManager sharedHelpManager] openHelpAnchor:_helpAnchor inBook:nil];
}


@end
