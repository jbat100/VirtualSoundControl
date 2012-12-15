//
//  VCSEnveloppeEditorWindowController.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 11/09/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSC::EnveloppeController.h"


@interface VSC::EnveloppeEditorWindowController : NSWindowController <NSWindowDelegate>  

@property (nonatomic, strong) IBOutlet VSC::EnveloppeController* enveloppeController;

/*
@property (nonatomic, strong) IBOutlet NSTextField* loadedTextField;
@property (nonatomic, strong) IBOutlet NSButton* testButton;
@property (nonatomic, strong) IBOutlet NSPopUpButton* enveloppePopUpButton;
*/

@end
