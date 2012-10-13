//
//  VSCOSXMIDITest.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 3/20/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXMIDITest.h"

#include "VSCMIDI.h"
#include "VSCMIDIOutput.h"
#include "VSCMIDIOutputManager.h"

@implementation VSCOSXMIDITest

-(id) init {
    
    if ((self = [super init])) {
        self.midiChannel = 1;
        self.controlValue = 80;
        self.pitchValue = 80;
        self.velocityValue = 80;
        self.controlNumber = VSC::MIDI::ControlBreath;
    }
    
    return self;
    
}

-(void) sendMidiControlMessage {
    if(self.midiOutput)
    {
        self.midiOutput->sendControlChange(self.midiChannel, self.controlNumber, self.controlValue);
    }
}

-(void) sendMidiNoteOnMessage {
    if(self.midiOutput)
    {
        self.midiOutput->sendNoteOn(self.midiChannel, self.pitchValue, self.velocityValue);
    }
}

-(void) sendMidiNoteOffMessage {
    if(self.midiOutput)
    {
        self.midiOutput->sendNoteOff(self.midiChannel, self.pitchValue, self.velocityValue);
    }
}

@end
