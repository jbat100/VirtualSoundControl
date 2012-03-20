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


NSString* const VSCMIDIPortNameColumnIdentifier         = @"MIDIPortName";
NSString* const VSCMIDIPortNumberColumnIdentifier       = @"MIDIPortNumber";
NSString* const VSCMIDIPortVirtualColumnIdentifier      = @"MIDIPortVirtual";
NSString* const VSCMIDIPortSelectedColumnIdentifier     = @"MIDIPortSelected";



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



#pragma mark - NSButton Callbacks

-(IBAction) refreshInputs:(id)sender {
    //NSAssert(midiTest, @"_midi is NULL");
    if ([self.midiTest getMidi]) {
        [self.midiTest getMidi]->refreshInputPorts();
    }
}

-(IBAction) refreshOutputs:(id)sender {
    NSAssert([self.midiTest getMidi], @"_midi is NULL");
    if ([self.midiTest getMidi]) {
        [self.midiTest getMidi]->refreshOutputPorts();
    }
}

-(IBAction) setMidiOutputWithRowSelection:(id)sender {
    NSAssert([self.midiTest getMidi], @"_midi is NULL");
    
    if ([self.midiTest getMidi]) {
        NSInteger rowIndex = [self.midiOutputsTable selectedRow];
        const std::list<VSCMIDIOutputPort> portList = [self.midiTest getMidi]->getOutputPorts();
        std::list<VSCMIDIOutputPort>::const_iterator portIt = [self.midiTest getMidi]->getOutputPorts().begin();
        std::advance(portIt, rowIndex);
        VSCMIDIOutputPort outputPort = *portIt;
        [self.midiTest setMidiOutput:VSCMIDIOutputPtr(new VSCMIDIOutput(outputPort))];
    }
    
}


#pragma mark - Interface Helpers

-(void) updateMidiOutputTextField {
    if (_midiOutput) {
        VSCMIDIOutputPort p = _midiOutput->getOutputPort();
        if (p == VSCMIDIOutputPortVoid) {
            [[self midiOutputTextField] setStringValue:@"Midi output void"];
        }
        else {
            [[self midiOutputTextField] setStringValue:[NSString stringWithFormat:@"Midi output with name \"%@\", number %u (%@)", 
                                                        [NSString stringWithStdString:p.name], p.number, p.isVirtual ? @"virtual" : @"non-virtual"]];
        }
    }
    else {
        [[self midiOutputTextField] setStringValue:@"No midi output"];
    }
}


-(void) refreshInterface {
    
    [self.midiChannelTextField setStringValue:[NSString stringWithFormat:@"u", self.midiChannel]];
    [self.controlChannelTextField setStringValue:[NSString stringWithFormat:@"u", self.controlChannel]];
    [self.controlValueTextField setStringValue:[NSString stringWithFormat:@"u", self.controlValue]];
    [self.notePitchTextField setStringValue:[NSString stringWithFormat:@"u", self.pitchValue]];
    [self.noteVelocityTextField setStringValue:[NSString stringWithFormat:@"u", self.velocityValue]];
    
    [self.midiOutputsTable reloadData];
    [self.midiInputsTable reloadData];
    
}

- (BOOL)control:(NSControl *)control textShouldEndEditing:(NSText *)fieldEditor {
    
    
    
    return YES;
}


#pragma mark - NSTableView Delegate/Datasource

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView {
    
    if (aTableView == _midiInputsTable) {
        return _midi->getInputPorts().size();
    }
    
    else if (aTableView == _midiOutputsTable) {
        return _midi->getOutputPorts().size();
    }
    
    return 0;
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex {
    
    NSString* columnIdentifier = [aTableColumn identifier];
    
    if (aTableView == _midiInputsTable) {
        
        const std::list<VSCMIDIInputPort> portList = _midi->getInputPorts();
        std::list<VSCMIDIInputPort>::const_iterator portIt = _midi->getInputPorts().begin();
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
        
        const std::list<VSCMIDIOutputPort> portList = _midi->getOutputPorts();
        std::list<VSCMIDIOutputPort>::const_iterator portIt = _midi->getOutputPorts().begin();
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
