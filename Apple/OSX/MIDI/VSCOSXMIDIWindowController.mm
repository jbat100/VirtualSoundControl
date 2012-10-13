//
//  VSCOSXMIDIWindowController.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 3/19/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXMIDIWindowController.h"
#import "VSCOSXMIDITestController.h"
#import "VSCOSXMIDIOutputView.h"

#import "NSString+VSCAdditions.h"

#include "VSCMIDIOutputManager.h"
#include "VSCException.h"

@interface VSCOSXMIDIWindowController ()

@end

@implementation VSCOSXMIDIWindowController


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
    
    self.midiOutputsListView.canCallDataSourceInParallel = YES;
    [self.midiOutputsListView reloadData];
    
    NSArray* formatters = @[self.midiChannelTextField.formatter,
    //self.midiTestView.controlNumberTextField.formatter,
    self.notePitchTextField.formatter,
    self.noteVelocityTextField.formatter,
    self.controlValueTextField.formatter];
    
    for (NSNumberFormatter* formatter in formatters) {
        
        [formatter setAllowsFloats:NO];
        
        if (formatter == self.midiChannelTextField.formatter) [formatter setMinimum:[NSNumber numberWithUnsignedInt:1]];
        else [formatter setMinimum:[NSNumber numberWithUnsignedInt:0]];
        
        if (formatter == self.midiChannelTextField.formatter) [formatter setMaximum:[NSNumber numberWithUnsignedInt:16]];
        else [formatter setMaximum:[NSNumber numberWithUnsignedInt:127]];
        
    }
    
    
    for (NSNumber* number in self.controlChannels) {
        int controlNumber = [number intValue];
        try {
            std::string controlString = VSC::MIDI::controlNumberToString((VSC::MIDI::ControlNumber)controlNumber);
            [self.rtControlChannelComboBox addItemWithObjectValue:[NSString stringWithStdString:controlString]];
        } catch (VSCMIDIException& exception) {
            [self.midiTestView.rtControlChannelComboBox addItemWithObjectValue:VSCMIDIOtherControlChannelDescriptorString];
        }
    }
    
    
    //[self.midiTestView.rtControlChannelComboBox setUsesDataSource:YES];
    [self.rtControlChannelComboBox setHasVerticalScroller:YES];
    [self.rtControlChannelComboBox setNumberOfVisibleItems:10];
    //[self.midiTestView.rtControlChannelComboBox reloadData];
    
}

-(IBAction) refreshInputs:(id)sender {
    

}

-(IBAction) refreshOutputs:(id)sender {
    
    if (self.applicationManager.midiOutputManager)
    {
        self.applicationManager.midiOutputManager->refreshOutputs();
    }
    
    [self.midiOutputsListView reloadData];
    
}


#pragma mark - JAListViewDataSource

- (NSUInteger)numberOfItemsInListView:(JAListView *)listView
{
    if (self.applicationManager.midiOutputManager)
    {
        return (NSUInteger)self.applicationManager.midiOutputManager->getOutputs().size();
    }
    
    return 0;
}

- (JAListViewItem *)listView:(JAListView *)listView viewAtIndex:(NSUInteger)index
{
    if (listView == self.midiOutputsListView) {
        
        if (self.applicationManager.midiOutputManager)
        {
            const VSC::MIDI::Outputs& outputs = self.applicationManager.midiOutputManager->getOutputs();
            
            if ((NSUInteger)outputs.size() > index)
            {
                VSCOSXMIDIOutputView* outputView = [VSCOSXMIDIOutputView midiOutputView];
                outputView.midiOutput = outputs.at(index);
                return outputView;
            }
        }
        
    }
    
    return nil;
}

#pragma mark - JAListViewDelegate

#pragma mark - UI Callbacks


-(IBAction) sendMidiControlMessage:(id)sender {
    if(self.midiOutput)
    {
        self.midiOutput->sendControlChange(self.midiChannel, self.controlNumber, self.controlValue);
    }
}

-(IBAction) sendMidiNoteOnMessage:(id)sender {
    if(self.midiOutput)
    {
        self.midiOutput->sendNoteOn(self.midiChannel, self.pitchValue, self.velocityValue);
    }
}

-(IBAction) sendMidiNoteOffMessage:(id)sender {
    if(self.midiOutput)
    {
        self.midiOutput->sendNoteOff(self.midiChannel, self.pitchValue, self.velocityValue);
    }
}

-(IBAction) controlSliderChangedValue:(id)sender {
    
    if (sender == self.midiTestView.rtControlSlider) {
        int val = [(NSSlider*)self.rtControlSlider intValue];
        if (val >= 0 && val <= 127) {
            //unsigned int uval = (unsigned int)val;
            if (self.midiOutput)
            {
                self.midiOutput->sendControlChange(self.midiTest.midiChannel, self.midiTest.controlNumber, self.midiTest.controlValue);
            }
        }
    }
    
}



 

@end
