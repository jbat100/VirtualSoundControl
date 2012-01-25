//
//  VCSEnveloppeEditorWindowController.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 11/09/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCEnveloppeView.h"
#import "VSCEnveloppe.h"


@interface VSCEnveloppeEditorWindowController : NSWindowController {
	
	VSCEnveloppeView* enveloppeView;
	
	NSTextField* loadedTextField;
	NSButton* testButton;
	NSPopUpButton* enveloppePopUpButton;
	
	VSCEnveloppePtr currentEnveloppe;
	
	NSMenu* fileMenu;
	NSMenuItem* fileTitleMenuItem;

}

@property (nonatomic, retain) IBOutlet VSCEnveloppeView* enveloppeView;
@property (nonatomic, retain) IBOutlet NSTextField* loadedTextField;
@property (nonatomic, retain) IBOutlet NSButton* testButton;
@property (nonatomic, retain) IBOutlet NSPopUpButton* enveloppePopUpButton;

/*
 *	C++ Setters/Getters
 */
-(VSCEnveloppePtr) getCurrentEnveloppe;
-(void) setCurrentEnveloppe:(VSCEnveloppePtr)enveloppe;

/*
 *	Interface callbacks
 */
-(IBAction) testButtonClicked:(id)sender;

@end
