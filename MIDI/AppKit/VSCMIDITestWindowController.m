//
//  VSCMIDITestWindowController.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 3/19/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCMIDITestWindowController.h"

#import "VSCMIDITestView.h"
#import "VSCMIDITest.h"

@interface VSCMIDITestWindowController ()

@end

@implementation VSCMIDITestWindowController

@synthesize midiTestView = _midiTestView;
@synthesize midiTest = _midiTest;

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
    
    
}

-(IBAction) sendMidiControlMessage:(id)sender {
    [self.midiTest sendMidiControlMessage];
}

-(IBAction) sendMidiNoteOnMessage:(id)sender {
    [self.midiTest sendMidiNoteOnMessage];
}

-(IBAction) sendMidiNoteOffMessage:(id)sender {
    [self.midiTest sendMidiNoteOffMessage];
}

@end
