//
//  VCSEnveloppeEditorWindowController.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 11/09/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCEnveloppeController.h"


@interface VSCEnveloppeEditorWindowController : NSWindowController <NSWindowDelegate>  

@property (nonatomic, strong) IBOutlet VSCEnveloppeController* enveloppeController;

/*
@property (nonatomic, strong) IBOutlet NSTextField* loadedTextField;
@property (nonatomic, strong) IBOutlet NSButton* testButton;
@property (nonatomic, strong) IBOutlet NSPopUpButton* enveloppePopUpButton;
*/

@end
