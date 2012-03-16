//
//  VSCMIDITestView.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 3/9/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCMIDITestView.h"
#import "NSString+VSCAdditions.h"

#include <list>
#include <iterator>
#include "VSCMIDI.h"


NSString* const VSCMIDIPortNameColumnIdentifier         = @"MIDIPortName";
NSString* const VSCMIDIPortNumberColumnIdentifier       = @"MIDIPortNumber";
NSString* const VSCMIDIPortVirtualColumnIdentifier      = @"MIDIPortVirtual";
NSString* const VSCMIDIPortSelectedColumnIdentifier     = @"MIDIPortSelected";

@interface VSCMIDITestView () {
    
}

-(void) customInit;
-(void) setMidiController:(VSCMIDIControllerPtr)controller;

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

-(void) setMidiController:(VSCMIDIControllerPtr)controller {
    _midiController = controller;
}

-(VSCMIDIControllerPtr) getMidiController {
    return _midiController;
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

-(IBAction) setMidiControllerWithCurrentMIDIOutputRowSelection:(id)sender {
    NSAssert(_midiControlCenter, @"_midiControlCenter is NULL");
    
    if (_midiControlCenter) {
        
        NSInteger rowIndex = [self.midiOutputsTable selectedRow];
        
        const std::list<VSCMIDIOutputPort> portList = _midiControlCenter->getOutputPorts();
        std::list<VSCMIDIOutputPort>::const_iterator portIt = _midiControlCenter->getOutputPorts().begin();
        std::advance(portIt, rowIndex);
        VSCMIDIOutputPort outputPort = *portIt;
        
        VSCMIDIControllerPtr controller = _midiControlCenter->controllerForOutputPort(outputPort);
        
        if (controller) {
            [self setMidiController:controller];
        }
        
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
    
    if (aTableView == _midiInputsTable) {
        return _midiControlCenter->getInputPorts().size();
    }
    
    else if (aTableView == _midiOutputsTable) {
        return _midiControlCenter->getOutputPorts().size();
    }
    
    return 0;
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex {
    
    NSString* columnIdentifier = [aTableColumn identifier];
    
    if (aTableView == _midiInputsTable) {
        
        const std::list<VSCMIDIInputPort> portList = _midiControlCenter->getInputPorts();
        std::list<VSCMIDIInputPort>::const_iterator portIt = _midiControlCenter->getInputPorts().begin();
        std::advance(portIt, rowIndex);
        
        if ([columnIdentifier isEqualToString:VSCMIDIPortNameColumnIdentifier]) {
            return [NSString stringWithStdString:portIt->name];
        }
        else if ([columnIdentifier isEqualToString:VSCMIDIPortNumberColumnIdentifier]) {
            return [NSNumber numberWithUnsignedInt:portIt->number];
        }
        else if ([columnIdentifier isEqualToString:VSCMIDIPortVirtualColumnIdentifier]) {
            return portIt->isVirtual ? @"Yes" : @"No";
        }
        else if ([columnIdentifier isEqualToString:VSCMIDIPortSelectedColumnIdentifier]) {
            return @"No";
        }
        
    }
    
    if (aTableView == _midiOutputsTable) {
        
        const std::list<VSCMIDIOutputPort> portList = _midiControlCenter->getOutputPorts();
        std::list<VSCMIDIOutputPort>::const_iterator portIt = _midiControlCenter->getOutputPorts().begin();
        std::advance(portIt, rowIndex);
        
        if ([columnIdentifier isEqualToString:VSCMIDIPortNameColumnIdentifier]) {
            return [NSString stringWithStdString:portIt->name];
        }
        else if ([columnIdentifier isEqualToString:VSCMIDIPortNumberColumnIdentifier]) {
            return [NSNumber numberWithUnsignedInt:portIt->number];
        }
        else if ([columnIdentifier isEqualToString:VSCMIDIPortVirtualColumnIdentifier]) {
            return portIt->isVirtual ? @"Yes" : @"No";
        }
        else if ([columnIdentifier isEqualToString:VSCMIDIPortSelectedColumnIdentifier]) {
            return @"No";
        }
        
    }
    
    
    return @"";
}

    


@end
