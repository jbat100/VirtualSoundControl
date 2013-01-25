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
#import "VSCOSXMIDITest.h"
#import "NSString+VSCAdditions.h"

#include "VSCTask.h"
#include "VSCTaskQueue.h"
#include "VSCMIDI.h"
#include "VSCMIDITasks.h"
#include "VSCMIDIOutputManager.h"
#include "VSCException.h"

#include <boost/foreach.hpp>

#define VSCOSX_USE_MIDI_TASK_QUEUE

NSString* const VSCOSXMIDINoSelectedChannelMenuItemString = @"No selected MIDI output";
NSString* const VSCOSXMIDINoValidControlNumberItemString = @"No valid control number (set output)";

@interface VSCOSXMIDIWindowController ()

-(void) initTest;

-(void) sendMIDIMessage:(VSC::MIDI::MessageDescription::SPtr)messageDescription;

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
    
    [self.midiOutputsTableView reloadData];
    
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
    VSC::MIDI::OutputManager::SPtr outputManager = VSC::MIDI::OutputManager::singletonManager();
    BOOST_ASSERT(outputManager);
    if (!outputManager) return;
    
    /*
     *  Main output tab
     */
    
    [self.midiOutputsTableView reloadData];
    
    /*
     *  Test output
     */
    
    [self.midiOutputPopUpButton removeAllItems];
    
    [self.midiOutputPopUpButton addItemWithTitle:VSCOSXMIDINoSelectedChannelMenuItemString];
    
    
    const VSC::MIDI::Outputs& outputs = outputManager->getOutputs();
    
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

#pragma mark - MIDI Helpers

-(void) sendMIDIMessage:(VSC::MIDI::MessageDescription::SPtr)messageDescription
{
    if(self.testMIDIOutput)
    {
#ifdef VSCOSX_USE_MIDI_TASK_QUEUE
        VSC::MIDI::MIDISendMessageTask::Payload::SPtr payload(new VSC::MIDI::MIDISendMessageTask::Payload);
        payload->midiOutput = self.testMIDIOutput;
        payload->messageDescription = messageDescription;
        VSC::MIDI::MIDISendMessageTask::SPtr task(new VSC::MIDI::MIDISendMessageTask(boost::dynamic_pointer_cast<VSC::Task::Payload>(payload)));
        BOOST_ASSERT(task);
        VSC::TaskQueue::SPtr taskQueue = VSC::MIDI::SingletonMIDITaskQueue();
        BOOST_ASSERT(taskQueue);
        if (task && taskQueue) taskQueue->enqueueTask(task);
#else
        try
        {
            self.testMIDIOutput->sendMessage(messageDescription);
        }
        catch (VSCInvalidArgumentException& e)
        {
            std::cerr << e.what() << "Additional Info: " << e.getValueForKey(VSCBaseExceptionAdditionalInfoKey);
        }
#endif
    }
}

#pragma mark - UI Callbacks

-(IBAction) controlNumberSelected:(id)sender
{
    NSString* title = [[self.controlNumberPopUpButton selectedItem] title];
    
    if ([title isEqualToString:VSCOSXMIDINoValidControlNumberItemString])
    {
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
    VSC::MIDI::OutputManager::SPtr outputManager = VSC::MIDI::OutputManager::singletonManager();
    BOOST_ASSERT(outputManager);
    if (!outputManager) return;    

    std::string description = [[[self.midiOutputPopUpButton selectedItem] title] stdString];
    VSC::MIDI::Output::SPtr selectedOutput = outputManager->getOutputWithDescription(description);
    self.testMIDIOutput = selectedOutput;

}

-(IBAction) refreshInputs:(id)sender
{
    
    
}

-(IBAction) refreshOutputs:(id)sender {
    
    VSC::MIDI::OutputManager::SPtr outputManager = VSC::MIDI::OutputManager::singletonManager();
    BOOST_ASSERT(outputManager);
    if (!outputManager) return;
    
    outputManager->refreshOutputs();
    NSLog(@"Refreshed outputs");

    [self updateMIDIOutputInterface];
}


-(IBAction) sendMidiControlMessage:(id)sender
{
    VSC::MIDI::MessageDescription::SPtr messageDescription(new VSC::MIDI::MessageDescription);
    messageDescription->type = VSC::MIDI::MessageTypeControlChange;
    messageDescription->parameterMap[VSC::MIDI::MessageParameterKeyChannel] = (unsigned char)self.channel;
    messageDescription->parameterMap[VSC::MIDI::MessageParameterKeyControlNumber] = (unsigned char)self.controlNumber;
    messageDescription->parameterMap[VSC::MIDI::MessageParameterKeyValue] = (unsigned char)self.controlValue;
    [self sendMIDIMessage:messageDescription];
}

-(IBAction) sendMidiNoteOnMessage:(id)sender
{
    VSC::MIDI::MessageDescription::SPtr messageDescription(new VSC::MIDI::MessageDescription);
    messageDescription->type = VSC::MIDI::MessageTypeNoteOn;
    messageDescription->parameterMap[VSC::MIDI::MessageParameterKeyChannel] = (unsigned char)self.channel;
    messageDescription->parameterMap[VSC::MIDI::MessageParameterKeyPitch] = (unsigned char)self.pitch;
    messageDescription->parameterMap[VSC::MIDI::MessageParameterKeyVelocity] = (unsigned char)self.velocity;
    [self sendMIDIMessage:messageDescription];
}

-(IBAction) sendMidiNoteOffMessage:(id)sender
{
    VSC::MIDI::MessageDescription::SPtr messageDescription(new VSC::MIDI::MessageDescription);
    messageDescription->type = VSC::MIDI::MessageTypeNoteOff;
    messageDescription->parameterMap[VSC::MIDI::MessageParameterKeyChannel] = (unsigned char)self.channel;
    messageDescription->parameterMap[VSC::MIDI::MessageParameterKeyPitch] = (unsigned char)self.pitch;
    messageDescription->parameterMap[VSC::MIDI::MessageParameterKeyVelocity] = (unsigned char)self.velocity;
    [self sendMIDIMessage:messageDescription];
}

-(IBAction) controlSliderChangedValue:(id)sender
{
    if (sender == self.controlValueSlider)
    {
        VSC::MIDI::MessageDescription::SPtr messageDescription(new VSC::MIDI::MessageDescription);
        messageDescription->type = VSC::MIDI::MessageTypeControlChange;
        messageDescription->parameterMap[VSC::MIDI::MessageParameterKeyChannel] = (unsigned char)self.channel;
        messageDescription->parameterMap[VSC::MIDI::MessageParameterKeyControlNumber] = (unsigned char)self.controlNumber;
        messageDescription->parameterMap[VSC::MIDI::MessageParameterKeyValue] = (unsigned char)self.controlValue;
        [self sendMIDIMessage:messageDescription];
    }
}

-(IBAction) showEnveloppeEditor:(id)sender
{
    
}

-(IBAction) fireEnveloppe:(id)sender
{
    
}

#pragma mark - NSTableViewDelegate and NSTableViewDataSource

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    BOOST_ASSERT(aTableView == self.midiOutputsTableView);
    VSC::MIDI::OutputManager::SPtr outputManager = VSC::MIDI::OutputManager::singletonManager();
    BOOST_ASSERT(outputManager);
    if (aTableView == self.midiOutputsTableView && outputManager)
    {
        return (NSUInteger)(outputManager->getOutputs().size());
    }
    
    return 0;
}

- (NSView *)tableView:(NSTableView *)tableView viewForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row
{
    BOOST_ASSERT(tableView == self.midiOutputsTableView);
    VSC::MIDI::OutputManager::SPtr outputManager = VSC::MIDI::OutputManager::singletonManager();
    BOOST_ASSERT(outputManager);
    if (tableView == self.midiOutputsTableView && outputManager)
    {
        const VSC::MIDI::Outputs& outputs = outputManager->getOutputs();
        BOOST_ASSERT((NSUInteger)outputs.size() > row);
        if ((NSUInteger)outputs.size() > row)
        {
            VSCOSXMIDIOutputView* outputView = [tableView makeViewWithIdentifier:[[VSCOSXMIDIOutputView class] description] owner:self];
            BOOST_ASSERT(outputView);
            outputView.midiOutput = outputs.at(row);
            return outputView;
        }
    }
    
	return nil;
}

- (CGFloat)tableView:(NSTableView *)tableView heightOfRow:(NSInteger)row
{
    BOOST_ASSERT(tableView == self.midiOutputsTableView);
    if (tableView == self.midiOutputsTableView)
    {
        return [VSCOSXMIDIOutputView defaultViewHeight];
    }
    
    return 0;
}
 

@end
