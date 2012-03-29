//
//  VSCMIDITestController.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 3/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCMIDITestController.h"

#import "VSCMIDITest.h"
#import "VSCMIDITestView.h"
#import "NSString+VSCAdditions.h"

#include "VSCMIDI.h"
#include "VSCMIDIOutput.h"
#include "VSCException.h"

NSString* const VSCMIDIPortNameColumnIdentifier         = @"MIDIPortName";
NSString* const VSCMIDIPortNumberColumnIdentifier       = @"MIDIPortNumber";
NSString* const VSCMIDIPortVirtualColumnIdentifier      = @"MIDIPortVirtual";
NSString* const VSCMIDIPortSelectedColumnIdentifier     = @"MIDIPortSelected";

/*
 *  A detailed table of midi messages can be found here http://www.midi.org/techspecs/midimessages.php
 *  Based on table 3, the first general purpose controler chan is 16
 */

const unsigned int VSCDefaultGenericControlNumber = 16;
NSString* const VSCMIDIOtherControlChannelDescriptorString = @"Other";

@implementation VSCMIDITestController

@synthesize midiTestView = _midiTestView;
@synthesize midiTest = _midiTest;

@synthesize controlChannels = _controlChannels;

-(id) init {
    
    if ((self = [super init])) {
        
        self.controlChannels = [NSArray arrayWithObjects:
                                [NSNumber numberWithInt:(int)VSCMIDI::ControlBankSelect],
                                [NSNumber numberWithInt:(int)VSCMIDI::ControlModulationWheel],
                                [NSNumber numberWithInt:(int)VSCMIDI::ControlBreath],
                                [NSNumber numberWithInt:(int)VSCMIDI::ControlFoot],
                                [NSNumber numberWithInt:(int)VSCMIDI::ControlChannelVolume],
                                [NSNumber numberWithInt:(int)VSCMIDI::ControlBalance],
                                [NSNumber numberWithInt:(int)VSCMIDI::ControlPan], 
                                [NSNumber numberWithInt:(int)VSCMIDI::ControlExpression], 
                                [NSNumber numberWithInt:(int)-1], nil]; // -1 -> other
        
    }
    
    return self;
    
}

#pragma mark - KVO

-(void) setMidiTest:(VSCMIDITest *)midiTest {
    
    [_midiTest removeObserver:self forKeyPath:@"controlChannel"];
    _midiTest = midiTest;
    [_midiTest addObserver:self forKeyPath:@"controlChannel" options:NSKeyValueObservingOptionNew  context:NULL];
    
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
    if ([keyPath isEqual:@"controlChannel"] && object == self.midiTest) {
        
        BOOL isOtherControlChannel = YES;
        
        NSNumber* number = nil;
        
        for (number in self.controlChannels) {
            int controlChannel = [number intValue];
            if (self.midiTest.controlChannel == controlChannel) {
                isOtherControlChannel = NO;
                break;
            }
        }
        
        if (isOtherControlChannel) {
            //NSInteger index = [self.midiTestView.rtControlChannelComboBox indexOfSelectedItem];
            NSString* selectedItemString = (NSString*)[self.midiTestView.rtControlChannelComboBox objectValueOfSelectedItem];
            //NSString *selectedItemString = [self comboBox:self.midiTestView.rtControlChannelComboBox objectValueForItemAtIndex:index];
            if ([selectedItemString isEqualToString:VSCMIDIOtherControlChannelDescriptorString] == NO) {
                [self.midiTestView.rtControlChannelComboBox selectItemWithObjectValue:VSCMIDIOtherControlChannelDescriptorString];
            }
        }
        
        else {
            NSInteger index = [self.controlChannels indexOfObject:number];
            [self.midiTestView.rtControlChannelComboBox selectItemAtIndex:index];
        }
        
    }

}

#pragma mark - Interface Helpers

-(void) setupView {
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
    
    NSArray* formatters = [NSArray arrayWithObjects:
                           self.midiTestView.midiChannelTextField.formatter,
                           self.midiTestView.controlChannelTextField.formatter,
                           self.midiTestView.notePitchTextField.formatter,
                           self.midiTestView.noteVelocityTextField.formatter,
                           self.midiTestView.controlValueTextField.formatter, nil];
    
    for (NSNumberFormatter* formatter in formatters) {
        
        [formatter setAllowsFloats:NO];
        
        if (formatter == self.midiTestView.midiChannelTextField.formatter) [formatter setMinimum:[NSNumber numberWithUnsignedInt:1]];
        else [formatter setMinimum:[NSNumber numberWithUnsignedInt:0]];
        
        if (formatter == self.midiTestView.midiChannelTextField.formatter) [formatter setMaximum:[NSNumber numberWithUnsignedInt:16]];
        else [formatter setMaximum:[NSNumber numberWithUnsignedInt:127]];
        
    }
    
    [self updateMidiOutputTextField];
    
    [self.midiTest getMidi]->refreshInputPorts();
    [self.midiTest getMidi]->refreshOutputPorts();
    
    [self.midiTestView.midiInputsTable reloadData];
    [self.midiTestView.midiOutputsTable reloadData];
    
    
    for (NSNumber* number in self.controlChannels) {
        int controlNumber = [number intValue];
        try {
            std::string controlString = [self.midiTest getMidi]->controlNumberString((VSCMIDI::ControlNumber)controlNumber);
            [self.midiTestView.rtControlChannelComboBox addItemWithObjectValue:[NSString stringWithStdString:controlString]]; 
        } catch (VSCMIDIException& exception) {
            [self.midiTestView.rtControlChannelComboBox addItemWithObjectValue:VSCMIDIOtherControlChannelDescriptorString]; 
        }
    }
    
    
    //[self.midiTestView.rtControlChannelComboBox setUsesDataSource:YES];
    [self.midiTestView.rtControlChannelComboBox setHasVerticalScroller:YES];
    [self.midiTestView.rtControlChannelComboBox setNumberOfVisibleItems:10];
    //[self.midiTestView.rtControlChannelComboBox reloadData];
    
}

-(void) updateMidiOutputTextField {
    
    if ([self.midiTest getMidiOutput]) {
        
        const VSCMIDIOutputPort p = [self.midiTest getMidiOutput]->getOutputPort();
        
        if (p == VSCMIDIOutputPortVoid) {
            [[self.midiTestView midiOutputTextField] setStringValue:@"Midi output void"];
        }
        
        else {
            [[self.midiTestView midiOutputTextField] setStringValue:[NSString stringWithFormat:@"Midi output with name \"%@\", number %u (%@)", 
                                                        [NSString stringWithStdString:p.name], p.number, p.isVirtual ? @"virtual" : @"non-virtual"]];
        }
        
    }
   
    else {
        [[self.midiTestView midiOutputTextField] setStringValue:@"No midi output"];
    }
}

#pragma mark - NSButton Callbacks

-(IBAction) sendMidiControlMessage:(id)sender {
    [self.midiTest sendMidiControlMessage];
}

-(IBAction) sendMidiNoteOnMessage:(id)sender {
    [self.midiTest sendMidiNoteOnMessage];
}

-(IBAction) sendMidiNoteOffMessage:(id)sender {
    [self.midiTest sendMidiNoteOffMessage];
}

-(IBAction) refreshInputs:(id)sender {
    //NSAssert(midiTest, @"_midi is NULL");
    if ([self.midiTest getMidi]) {
        [self.midiTest getMidi]->refreshInputPorts();
        //const VSCMIDI::OutputPortList l = [self.midiTest getMidi]->getOutputPorts();
        std::cout << [self.midiTest getMidi]->outputPortDescription() << std::endl;
    }
    [self.midiTestView.midiInputsTable reloadData];
}

-(IBAction) refreshOutputs:(id)sender {
    NSAssert([self.midiTest getMidi], @"_midi is NULL");
    if ([self.midiTest getMidi]) {
        [self.midiTest getMidi]->refreshOutputPorts();
        std::cout << [self.midiTest getMidi]->outputPortDescription();
    }
    [self.midiTestView.midiOutputsTable reloadData];
}

-(IBAction) setMidiOutputWithRowSelection:(id)sender {
    NSAssert([self.midiTest getMidi], @"_midi is NULL");
    if ([self.midiTest getMidi]) {
        
        NSInteger rowIndex = [self.midiTestView.midiOutputsTable selectedRow];
        
        if (rowIndex < 0) {
            return;
        }
        
        const std::list<VSCMIDIOutputPort> portList = [self.midiTest getMidi]->getOutputPorts();
        std::list<VSCMIDIOutputPort>::const_iterator portIt = [self.midiTest getMidi]->getOutputPorts().begin();
        std::advance(portIt, rowIndex);
        VSCMIDIOutputPort outputPort = *portIt;
        [self.midiTest setMidiOutput:VSCMIDIOutputPtr(new VSCMIDIOutput(outputPort))];
    
        [self updateMidiOutputTextField];
        [self.midiTestView.midiOutputsTable reloadData];
        
    }
    
}

-(IBAction) controlSliderChangedValue:(id)sender {
    
    if (sender == self.midiTestView.rtControlSlider) {
        int val = (unsigned int)[(NSSlider*)self.midiTestView.rtControlSlider intValue];
        if (val >= 0 && val <= 127) {
            unsigned int uval = (unsigned int)val;
            if ([self.midiTest getMidiOutput]) {
                VSCMIDI::Message m = VSCMIDI::messageForControl(self.midiTest.midiChannel, self.midiTest.controlChannel, uval);
                [self.midiTest getMidiOutput]->sendMessage(m);
            }
        }
    }
    
}

#pragma mark - NSComboBox Delegate/DataSource

-(void)comboBoxSelectionDidChange:(NSNotification *)notification {
    
    if ([notification object] == self.midiTestView.rtControlChannelComboBox) {
        NSInteger index = [self.midiTestView.rtControlChannelComboBox indexOfSelectedItem];
        
        int controlChannel = [(NSNumber*)[self.controlChannels objectAtIndex:index] intValue];
        
        if (controlChannel < 0) {
            if (self.midiTest.controlChannel != VSCDefaultGenericControlNumber) {
                self.midiTest.controlChannel = VSCDefaultGenericControlNumber;
            }
        }
        
        else {
            VSCMIDI::ControlNumber num = (VSCMIDI::ControlNumber)[[self.controlChannels objectAtIndex:index] intValue];
            self.midiTest.controlChannel = (unsigned int)num;
        }
        
    }
    
}

/*
 *  We should not use dataSource for NSComboBox because it sucks. You can't do anything and I have proof:
 
 2012-03-28 17:38:47.911 EnveloppeEditor[6956:207] *** -[NSComboBoxCell selectItemWithObjectValue:] should not be called when usesDataSource is set to YES
 2012-03-28 17:38:47.911 EnveloppeEditor[6956:207] *** -[NSComboBoxCell indexOfItemWithObjectValue:] should not be called when usesDataSource is set to YES
 
 */

/*
- (NSInteger)numberOfItemsInComboBox:(NSComboBox *)aComboBox {
    return [self.controlChannels count];
}

- (id)comboBox:(NSComboBox *)aComboBox objectValueForItemAtIndex:(NSInteger)index {
    
    int controlChannel = [(NSNumber*)[self.controlChannels objectAtIndex:index] intValue];
    
    if (controlChannel < 0) {
        return @"Other";
    }
    
    std::string s = [self.midiTest getMidi]->controlNumberString((VSCMIDI::ControlNumber)controlChannel);
    
    return [NSString stringWithStdString:s];
}
 
 */


#pragma mark - NSTableView Delegate/Datasource

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView {
    
    if (aTableView == self.midiTestView.midiInputsTable) {
        return [self.midiTest getMidi]->getInputPorts().size();
    }
    
    else if (aTableView == self.midiTestView.midiOutputsTable) {
        return [self.midiTest getMidi]->getOutputPorts().size();
    }
    
    return 0;
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex {
    
    NSString* columnIdentifier = [aTableColumn identifier];
    
    if (aTableView == self.midiTestView.midiInputsTable) {
        
        const std::list<VSCMIDIInputPort> portList = [self.midiTest getMidi]->getInputPorts();
        std::list<VSCMIDIInputPort>::const_iterator portIt = [self.midiTest getMidi]->getInputPorts().begin();
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
    
    if (aTableView == self.midiTestView.midiOutputsTable) {
        
        const std::list<VSCMIDIOutputPort> portList = [self.midiTest getMidi]->getOutputPorts();
        std::list<VSCMIDIOutputPort>::const_iterator portIt = [self.midiTest getMidi]->getOutputPorts().begin();
        std::advance(portIt, rowIndex);
        
        if ([columnIdentifier isEqualToString:VSCMIDIPortNameColumnIdentifier]) {
            return [NSString stringWithStdString:portIt->name];
        }
        else if ([columnIdentifier isEqualToString:VSCMIDIPortNumberColumnIdentifier]) {
            return [NSNumber numberWithUnsignedInt:portIt->number];
        }
        else if ([columnIdentifier isEqualToString:VSCMIDIPortVirtualColumnIdentifier]) {
            //return portIt->isVirtual ? @"Yes" : @"No";
        }
        else if ([columnIdentifier isEqualToString:VSCMIDIPortSelectedColumnIdentifier]) {
            //return @"No";
            if ([self.midiTest getMidiOutput]) {
                if ([self.midiTest getMidiOutput]->getOutputPort() == (*portIt)) return [NSNumber numberWithBool:YES];
                else return [NSNumber numberWithBool:NO];
            }
        }
        
    }
    
    return @"";
}



@end
