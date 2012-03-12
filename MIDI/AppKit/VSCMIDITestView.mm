//
//  VSCMIDITestView.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 3/9/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCMIDITestView.h"

#include "VSCMIDI.h"

@interface VSCMIDITestView () {
    
}

-(void) customInit;

@end

@implementation VSCMIDITestView

@synthesize midiInputsTable = _midiInputsTable;
@synthesize midiOutputsTable = _midiOutputsTable;
@synthesize refreshInputsButton = _refreshInputsButton;
@synthesize refreshOutputsButton = _refreshOutputsButton;
@synthesize createMidiOutputButton = _createMidiOutputButton;

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
        [self customInit];
    }
    
    return self;
}

-(id)initWithCoder:(NSCoder *)aDecoder {
    
    self = [super initWithCoder:aDecoder];
    if (self) {
        [self customInit];
    }
    
    return self;
    
}

-(void) customInit {
    
    midiChannel = 1;
    controlChannel = 1;
    controlValue = 80;
    pitchValue = 80;
    velocityValue = 80;
    
    
    [self setMidiControlCenter:VSCMIDIControlCenterPtr()];
}

- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
}

-(VSCMIDIControlCenterPtr) getMidiControlCenter {
    return _midiControlCenter;
}

-(void) setMidiControlCenter:(VSCMIDIControlCenterPtr)center {
    _midiControlCenter = center;
}

-(VSCMIDIControllerPtr) getCurrentMidiController {
    return _currentMidiController;
}

#pragma mark - MIDI Handling

-(BOOL) createMidiOutputWithPort:(VSCMIDIOutputPort)port {
    return NO;
}

#pragma mark - NSButton Callbacks

-(IBAction) refreshInputs:(id)sender {
    NSAssert(_midiControlCenter, @"_midiControlCenter is NULL");
    if (_midiControlCenter) {
        _midiControlCenter->refreshInputPorts();
    }
}

-(IBAction) refreshOutputs:(id)sender {
    NSAssert(_midiControlCenter, @"_midiControlCenter is NULL");
    if (_midiControlCenter) {
        _midiControlCenter->refreshOutputPorts();
    }
}

-(IBAction) createMidiOutput:(id)sender {
    NSAssert(_midiControlCenter, @"_midiControlCenter is NULL");
    if (_midiControlCenter) {
        
    }
}

-(IBAction) sendMidiControlMessage:(id)sender {
    
}

-(IBAction) sendMidiNoteOnMessage:(id)sender {
    
}

-(IBAction) sendMidiNoteOffMessage:(id)sender {
    
}

#pragma mark - NSTableView Delegate/Datasource

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView {
    return 0;
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex {
    return @"";
}

- (NSView *)tableView:(NSTableView *)tableView viewForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row {
    return nil;
}


@end
