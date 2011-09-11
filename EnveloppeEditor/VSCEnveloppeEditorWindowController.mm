//
//  VCSEnveloppeEditorWindowController.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 11/09/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "VSCEnveloppeEditorWindowController.h"

@interface VSCEnveloppeEditorWindowController ()

-(void) updateEnveloppePopUpMenu;

@end


@implementation VSCEnveloppeEditorWindowController

@synthesize loadedTextField, enveloppePopUpButton;

#pragma mark - NSWindowController Window Callbacks

-(void) windowDidLoad {
	NSLog(@"%@ window did load", self);
}

-(void) windowWillLoad {
	NSLog(@"%@ window will load", self);
}

#pragma mark - C++ Setters / Getters

-(VSCEnveloppePtr) getCurrentEnveloppe {
	return currentEnveloppe;
}

-(void) setCurrentEnveloppe:(VSCEnveloppePtr)enveloppe {
	currentEnveloppe = enveloppe;
}

#pragma mark - Button Callbacks

-(IBAction) testButtonClicked:(id)sender {
	NSLog(@"%@ window test button clicked", self);
}

#pragma mark - Interface Update 

-(void) updateEnveloppePopUpMenu {
	
}

@end
