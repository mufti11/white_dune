//
//  MyDocument.h
//  white_dune
//
//  Created by MUFTI on Wed Mar 30 2005.
//  Copyright (c) 2005 __MyCompanyName__. All rights reserved.
//


#import <Cocoa/Cocoa.h>

@interface MyDocument : NSDocument
{
    IBOutlet NSMenu *dockMenu;
    IBOutlet id menuRadio;
    IBOutlet NSColorWell *colorWell;
}

- (IBAction)setRed:(id)sender;
- (IBAction)setGreen:(id)sender;
- (IBAction)setBlue:(id)sender;

@end
