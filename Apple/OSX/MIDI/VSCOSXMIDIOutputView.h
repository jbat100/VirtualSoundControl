//
//  VSCOSXMIDIOutputView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 12/3/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "VSCMIDIOutput.h"

@interface VSCOSXMIDIOutputView : NSView

@property (nonatomic, assign) VSC::MIDI::Output::WPtr midiOutput;

@property (retain) IBOutlet NSTextField* numberTextField;
@property (retain) IBOutlet NSTextField* nameTextField;
@property (retain) IBOutlet NSTextField* virtualTextField;
@property (retain) IBOutlet NSTextField* statusTextField;
@property (retain) IBOutlet NSButton* mainActionButton;

+(CGFloat) defaultViewHeight;

-(IBAction) mainAction:(id)sender;

-(void) refresh;

@end
