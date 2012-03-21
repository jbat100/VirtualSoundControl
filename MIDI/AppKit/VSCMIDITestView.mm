//
//  VSCMIDITestView.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 3/9/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCMIDITestView.h"
#import "NSString+VSCAdditions.h"
#import "VSCMIDITest.h"

#include <list>
#include <iterator>
#include "VSCMIDI.h"


@implementation VSCMIDITestView

@synthesize midiTest = _midiTest;

@synthesize midiInputsTable = _midiInputsTable;
@synthesize midiOutputsTable = _midiOutputsTable;
@synthesize refreshInputsButton = _refreshInputsButton;
@synthesize refreshOutputsButton = _refreshOutputsButton;
@synthesize createMidiOutputButton = _createMidiOutputButton;

@synthesize midiOutputTextField = _midiOutputTextField;

@synthesize midiChannelTextField = _midiChannelTextField;
@synthesize controlChannelTextField = _controlChannelTextField;
@synthesize controlValueTextField = _controlValueTextField;
@synthesize notePitchTextField = _notePitchTextField;
@synthesize noteVelocityTextField = _noteVelocityTextField;

@synthesize sendMidiControlButton = _sendMidiControlButton;
@synthesize sendMidiNoteOnButton = _sendMidiNoteOnButton;
@synthesize sendMidiNoteOffButton = _sendMidiNoteOffButton;



- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.

    }
    
    return self;
}

-(id)initWithCoder:(NSCoder *)aDecoder {
    
    self = [super initWithCoder:aDecoder];
    if (self) {

    }
    
    return self;
    
}


- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
}



@end
