//
//  VSCOSXMIDIWindowController.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 3/19/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXMIDIWindowController.h"
#import "VSCOSXMIDITestController.h"
#import "VSCOSXMIDIOutputJAView.h"
#import "VSCOSXMIDITest.h"

#import "NSString+VSCAdditions.h"

#include "VSCMIDIOutputManager.h"
#include "VSCException.h"

NSString* const VSCOSXMIDINoSelectedChannelMenuItemString = @"No Selected MIDI Output";

@interface VSCOSXMIDIWindowController ()

-(void) updateTestMIDIOutputs;

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
    
    BOOST_ASSERT(self.channelTextField.formatter);
    BOOST_ASSERT(self.pitchTextField.formatter);
    BOOST_ASSERT(self.velocityTextField.formatter);
    BOOST_ASSERT(self.controlValueTextField.formatter);
    
    NSArray* formatters = @[
    self.channelTextField.formatter,
    self.pitchTextField.formatter,
    self.velocityTextField.formatter,
    self.controlValueTextField.formatter];
    
    for (NSNumberFormatter* formatter in formatters) {
        
        [formatter setAllowsFloats:NO];
        
        if (formatter == self.channelTextField.formatter) [formatter setMinimum:[NSNumber numberWithUnsignedInt:1]];
        else [formatter setMinimum:[NSNumber numberWithUnsignedInt:0]];
        
        if (formatter == self.channelTextField.formatter) [formatter setMaximum:[NSNumber numberWithUnsignedInt:16]];
        else [formatter setMaximum:[NSNumber numberWithUnsignedInt:127]];
        
    }
    
    [self.controlValueSlider setMinValue:0.0];
    [self.controlValueSlider setMaxValue:127.0];
    
    [self updateControlNumbers];
    
}

-(void) updateControlNumbers {
    
    [self.controlNumberPopUpButton removeAllItems];
    
    //[self.controlNumberPopUpButton addItemWithTitle:VSCOSXMIDINoSelectedChannelMenuItemString];
    
    if (self.testMIDIOutput) {
        
        const VSC::MIDI::ControlNumbers& validControlNumbers = self.testMIDIOutput->getValidControlNumbers();
        
        BOOST_FOREACH(const VSC::MIDI::ControlNumber& controlNumber, validControlNumbers)
        {
            std::string name = VSC::MIDI::controlNumberToString(controlNumber);
            BOOST_ASSERT(!name.empty());
            NSString* title = [NSString stringWithFormat:@"%u - %@", (unsigned int)controlNumber, [NSString stringWithStdString:name]];
            [self.controlNumberPopUpButton addItemWithTitle:title];
        }
    }
    
}

-(IBAction) controlNumberSelected:(id)sender
{
    NSString* title = [[self.controlNumberPopUpButton selectedItem] title];
    
    if (self.testMIDIOutput)
    {
    
        NSUInteger separatorLocation = [title rangeOfString:@" - "].location;
        BOOST_ASSERT(separatorLocation != NSNotFound);
        if (separatorLocation == NSNotFound) return;
        
        NSInteger number = [[title substringToIndex:separatorLocation] integerValue];
        BOOST_ASSERT(self.testMIDIOutput->controlNumberIsValid((VSC::MIDI::ControlNumber)number));
        self.controlNumber = (VSC::MIDI::ControlNumber)number;
    }
}

-(void) updateTestMIDIOutputs
{
    
    [self.midiOutputPopUpButton removeAllItems];
    
    [self.midiOutputPopUpButton addItemWithTitle:VSCOSXMIDINoSelectedChannelMenuItemString];
    
    if (self.applicationManager.midiOutputManager) {
        
        const VSC::MIDI::Outputs& outputs = self.applicationManager.midiOutputManager->getOutputs();
        
        BOOST_FOREACH(const VSC::MIDI::Output::SPtr& output, outputs)
        {
            NSString* title = [NSString stringWithStdString:output->getDescription()];
            [self.midiOutputPopUpButton addItemWithTitle:title];
        }
        
        if (self.testMIDIOutput)
        {
            VSC::MIDI::Outputs::const_iterator it = std::find(outputs.begin(), outputs.end(), self.testMIDIOutput);
            if (it != outputs.end())
            {
                NSString* title = [NSString stringWithStdString:(*it)->getDescription()];
                [self.midiOutputPopUpButton selectItemWithTitle:title];
            }
        }
    }
    
}


-(IBAction) midiOutputSelected:(id)sender
{
    
    if (self.applicationManager.midiOutputManager) {
        
        std::string description = [[[self.midiOutputPopUpButton selectedItem] title] stdString];
        
        VSC::MIDI::Output::SPtr selectedOutput = self.applicationManager.midiOutputManager->getOutputWithDescription(description);
        
        self.testMIDIOutput = selectedOutput;
    }
    
}


-(IBAction) refreshInputs:(id)sender
{
    

}

-(IBAction) refreshOutputs:(id)sender {
    
    if (self.applicationManager.midiOutputManager)
    {
        self.applicationManager.midiOutputManager->refreshOutputs();
    }
    
    [self.midiOutputsListView reloadData];
    
    [self updateTestMIDIOutputs];
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
                VSCOSXMIDIOutputJAView* outputView = [VSCOSXMIDIOutputJAView midiOutputView];
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
    if(self.testMIDIOutput)
    {
        self.testMIDIOutput->sendControlChange(self.channel, self.controlNumber, self.controlValue);
    }
}

-(IBAction) sendMidiNoteOnMessage:(id)sender {
    if(self.testMIDIOutput)
    {
        self.testMIDIOutput->sendNoteOn(self.channel, self.pitch, self.velocity);
    }
}

-(IBAction) sendMidiNoteOffMessage:(id)sender {
    if(self.testMIDIOutput)
    {
        self.testMIDIOutput->sendNoteOff(self.channel, self.pitch, self.velocity);
    }
}

-(IBAction) controlSliderChangedValue:(id)sender {
    
    if (sender == self.controlValueSlider) {
        if (self.testMIDIOutput)
        {
            self.testMIDIOutput->sendControlChange(self.channel, self.controlNumber, self.controlValue);
        }
    }
    
}

-(IBAction) showEnveloppeEditor:(id)sender
{
    
}

-(IBAction) fireEnveloppe:(id)sender
{
    
}

 

@end
