//
//  VSCOSXMIDITestController.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 3/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXMIDITestController.h"

//#import "VSCEnveloppeEditorWindowController.h"
#import "VSCOSXMIDITest.h"
#import "VSCOSXMIDITestView.h"
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

@implementation VSCOSXMIDITestController


-(id) init {
    
    if ((self = [super init])) {
        
        self.controlChannels = @[
        @((int)VSC::MIDI::ControlBankSelect),
        @((int)VSC::MIDI::ControlModulationWheel),
        @((int)VSC::MIDI::ControlBreath),
        @((int)VSC::MIDI::ControlChannelVolume),
        @((int)VSC::MIDI::ControlBalance),
        @((int)VSC::MIDI::ControlPan),
        @((int)VSC::MIDI::ControlExpression),
        ]; 
        
    }
    
    return self;
    
}

#pragma mark - KVO

-(void) setMidiTest:(VSCOSXMIDITest*)midiTest {
    
    [_midiTest removeObserver:self forKeyPath:@"controlChannel"];
    _midiTest = midiTest;
    [_midiTest addObserver:self forKeyPath:@"controlChannel" options:NSKeyValueObservingOptionNew  context:NULL];
    
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
    /*
    if ([keyPath isEqual:@"controlNumber"] && object == self.midiTest) {
        
        BOOL isOtherControlChannel = YES;
        
        NSNumber* number = nil;
        
        for (number in self.controlChannels) {
            int controlChannel = [number intValue];
            if (self.midiTest.controlNumber == controlNumber) {
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
     */

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
    
    [self.midiTest ]->refreshInputPorts();
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

-(IBAction) showEnveloppeEditor:(id)sender {
    
    if (self.enveloppeEditorWindowController == nil) {
        self.enveloppeEditorWindowController = [[VSCEnveloppeEditorWindowController alloc] 
                                                initWithWindowNibName:@"VSCEnveloppeEditorWindowController"];
    }
    
    if ([self.enveloppeEditorWindowController.enveloppeController getCurrentEnveloppe]) {
         [self.enveloppeEditorWindowController.enveloppeController setCurrentEnveloppe:VSCEnveloppe::createADSREnveloppe(0.5, 0.5, 1.0, 1.0)];
    }
    
    [self.enveloppeEditorWindowController showWindow:self];
    
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


@end
