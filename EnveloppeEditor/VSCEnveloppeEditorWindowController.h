//
//  VCSEnveloppeEditorWindowController.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 11/09/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSCEnveloppe.h"


@interface VSCEnveloppeEditorWindowController : NSWindowController {
	
	NSTextField* loadedTextField;
	NSButton* testButton;
	NSPopUpButton* enveloppePopUpButton;
	
	VSCEnveloppePtr currentEnveloppe;

}

@property (nonatomic, retain) IBOutlet NSTextField* loadedTextField;
@property (nonatomic, retain) IBOutlet NSButton* testButton;
@property (nonatomic, retain) IBOutlet NSPopUpButton* enveloppePopUpButton;

-(VSCEnveloppePtr) getCurrentEnveloppe;
-(void) setCurrentEnveloppe:(VSCEnveloppePtr)enveloppe;

-(IBAction) testButtonClicked:(id)sender;

@end
