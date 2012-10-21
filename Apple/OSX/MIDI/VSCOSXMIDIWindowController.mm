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

#include "VSCMIDI.h"
#include "VSCMIDIOutputManager.h"
#include "VSCException.h"

#include <boost/foreach.hpp>

NSString* const VSCOSXMIDINoSelectedChannelMenuItemString = @"No selected MIDI output";
NSString* const VSCOSXMIDINoValidControlNumberItemString = @"No valid control number (set output)";

@interface VSCOSXMIDIWindowController ()

-(void) initTest;

@end

@implementation VSCOSXMIDIWindowController


#pragma mark - NSWindowController

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self) {
        // Initialization code here.
        [self initTest];
    }
    
    return self;
}

-(void) initTest {
    
    self.channel = 1;
    self.controlValue = 64;
    self.pitch = 64;
    self.velocity = 64;
    self.controlNumber = VSC::MIDI::ControlInvalid;
    
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
    
    [self updateMIDIOutputInterface];
    [self updateControlNumbers];
    
}

#pragma mark - Setters

-(void) setTestMIDIOutput:(VSC::MIDI::Output::SPtr)testMIDIOutput {
    
    _testMIDIOutput = testMIDIOutput;
    
    [self updateControlNumbers];
    
}

#pragma mark - Interface

-(void) updateControlNumbers {
    
    [self.controlNumberPopUpButton removeAllItems];
    
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
    
    else {
        
        [self.controlNumberPopUpButton addItemWithTitle:VSCOSXMIDINoValidControlNumberItemString];
        
    }
    
}


-(void) updateMIDIOutputInterface
{
    /*
     *  Main output tab
     */
    
    [self.midiOutputsListView reloadData];
    
    /*
     *  Test output
     */
    
    [self.midiOutputPopUpButton removeAllItems];
    
    [self.midiOutputPopUpButton addItemWithTitle:VSCOSXMIDINoSelectedChannelMenuItemString];
    
    if (self.midiOutputManager)
    {
        
        const VSC::MIDI::Outputs& outputs = self.midiOutputManager->getOutputs();
        
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

#pragma mark - UI Callbacks

-(IBAction) controlNumberSelected:(id)sender
{
    NSString* title = [[self.controlNumberPopUpButton selectedItem] title];
    
    if ([title isEqualToString:VSCOSXMIDINoValidControlNumberItemString]) {
        return;
    }
    
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



-(IBAction) midiOutputSelected:(id)sender
{
    
    if (self.midiOutputManager) {
        
        std::string description = [[[self.midiOutputPopUpButton selectedItem] title] stdString];
        
        VSC::MIDI::Output::SPtr selectedOutput = self.midiOutputManager->getOutputWithDescription(description);
        
        self.testMIDIOutput = selectedOutput;
    }
    
}

-(IBAction) refreshInputs:(id)sender
{
    
    
}

-(IBAction) refreshOutputs:(id)sender {
    
    NSLog(@"Refreshing outputs");
    
    if (self.midiOutputManager)
    {
        self.midiOutputManager->refreshOutputs();
        NSLog(@"Refreshed outputs");
    }
    else
    {
        NSLog(@"No MIDI output manager");
    }
    
    [self updateMIDIOutputInterface];
}


-(IBAction) sendMidiControlMessage:(id)sender {
    if(self.testMIDIOutput)
    {
        try {
            self.testMIDIOutput->sendControlChange(self.channel, self.controlNumber, self.controlValue);
        } catch (VSCInvalidArgumentException& e) {
            std::cerr << e.what() << "Additional Info: " << e.getValueForKey(VSCBaseExceptionAdditionalInfoKey);
        }
    }
}

-(IBAction) sendMidiNoteOnMessage:(id)sender {
    if(self.testMIDIOutput)
    {
        try {
            self.testMIDIOutput->sendNoteOn(self.channel, self.pitch, self.velocity);
        } catch (VSCInvalidArgumentException& e) {
            std::cerr << e.what() << "Additional Info: " << e.getValueForKey(VSCBaseExceptionAdditionalInfoKey);
        }
    }
}

-(IBAction) sendMidiNoteOffMessage:(id)sender {
    if(self.testMIDIOutput)
    {
        try {
            self.testMIDIOutput->sendNoteOff(self.channel, self.pitch, self.velocity);
        } catch (VSCInvalidArgumentException& e) {
            std::cerr << e.what() << "Additional Info: " << e.getValueForKey(VSCBaseExceptionAdditionalInfoKey);
        }
    }
}

-(IBAction) controlSliderChangedValue:(id)sender {
    
    if (sender == self.controlValueSlider) {
        if (self.testMIDIOutput)
        {
            try {
                self.testMIDIOutput->sendControlChange(self.channel, self.controlNumber, self.controlValue);
            } catch (VSCInvalidArgumentException& e) {
                std::cerr << e.what() << "Additional Info: " << e.getValueForKey(VSCBaseExceptionAdditionalInfoKey);
            }
        }
    }
    
}

-(IBAction) showEnveloppeEditor:(id)sender
{
    
}

-(IBAction) fireEnveloppe:(id)sender
{
    
}

#pragma mark - JAListViewDataSource

- (NSUInteger)numberOfItemsInListView:(JAListView *)listView
{
    if (self.midiOutputManager)
    {
        return (NSUInteger)self.midiOutputManager->getOutputs().size();
    }
    
    return 0;
}

- (JAListViewItem *)listView:(JAListView *)listView viewAtIndex:(NSUInteger)index
{
    if (listView == self.midiOutputsListView) {
        
        if (self.midiOutputManager)
        {
            const VSC::MIDI::Outputs& outputs = self.midiOutputManager->getOutputs();
            
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
 

@end
