//
//  VSCOSXMIDIOutputView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 12/3/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXMIDIOutputView.h"
#import "NSString+VSCAdditions.h"

#include <boost/assert.hpp>

@interface VSCOSXMIDIOutputView ()

@end

@implementation VSCOSXMIDIOutputView

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
}

-(void) setMidiOutput:(VSC::MIDI::Output::WPtr)midiOutput {
    _midiOutput = midiOutput;
    [self refresh];
}

-(void) refresh {
    
    VSC::MIDI::Output::SPtr output = self.midiOutput.lock();
    
    if (output) {
        [self.nameTextField setStringValue:[NSString stringWithStdString:output->getOutputPort().name]];
        [self.numberTextField setStringValue:[@(output->getOutputPort().number) stringValue]];
        [self.virtualTextField setStringValue:output->getOutputPort().isVirtual ? @"Virtual" : @"Non-virtual"];
    }
    else {
        [self.nameTextField setStringValue:@"No Output"];
        [self.numberTextField setStringValue:[@(output->getOutputPort().number) stringValue]];
        [self.virtualTextField setStringValue:output->getOutputPort().isVirtual ? @"Virtual" : @"Non-virtual"];
    }
    
    if (output->getState() == VSC::MIDI::Output::StateOpened) {
        [self.mainActionButton setTitle:@"Close"];
        [self.statusTextField setStringValue:@"Opened"];
    }
    else if (output->getState() == VSC::MIDI::Output::StateClosed) {
        [self.mainActionButton setTitle:@"Open"];
        [self.statusTextField setStringValue:@"Closed"];
    }
    else {
        [self.mainActionButton setTitle:@"Unknown Status"];
        [self.statusTextField setStringValue:@"Attempt Open"];
    }
    
}

#pragma mark - UI Callback

-(IBAction) mainAction:(id)sender
{
    VSC::MIDI::Output::SPtr output = self.midiOutput.lock();
    
    if (output->getState() == VSC::MIDI::Output::StateOpened) {
        output->close();
        [self refresh];
    }
    else if (output->getState() == VSC::MIDI::Output::StateClosed) {
        output->open();
        [self refresh];
    }
    else {
        output->open();
        [self refresh];
    }
}

@end
