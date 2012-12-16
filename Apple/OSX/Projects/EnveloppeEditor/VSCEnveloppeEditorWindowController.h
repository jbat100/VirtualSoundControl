//
//  VCSEnvelopeEditorWindowController.h
//  EnvelopeEditor
//
//  Created by Jonathan Thorpe on 11/09/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSC::EnvelopeController.h"


@interface VSC::EnvelopeEditorWindowController : NSWindowController <NSWindowDelegate>  

@property (nonatomic, strong) IBOutlet VSC::EnvelopeController* envelopeController;

/*
@property (nonatomic, strong) IBOutlet NSTextField* loadedTextField;
@property (nonatomic, strong) IBOutlet NSButton* testButton;
@property (nonatomic, strong) IBOutlet NSPopUpButton* envelopePopUpButton;
*/

@end
