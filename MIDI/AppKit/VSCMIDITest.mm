//
//  VSCMIDITest.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 3/20/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCMIDITest.h"

@interface VSCMIDITest () 

-(void) setMidi:(VSCMIDIPtr)midi;

@end

@implementation VSCMIDITest

@synthesize midiChannel = _midiChannel;
@synthesize controlChannel = _controlChannel;
@synthesize controlValue = _controlValue;
@synthesize pitchValue = _pitchValue;
@synthesize velocityValue = _velocityValue;
@synthesize autoControlUpdateFrequency = _autoControlUpdateFrequency;

-(id) init {
    
    if ((self = [super init])) {
        self.midiChannel = 1;
        self.controlChannel = 1;
        self.controlValue = 80;
        self.pitchValue = 80;
        self.velocityValue = 80;
        [self setMidi:VSCMIDIPtr(new VSCMIDI())];
    }
    
    return self;
    
}

-(VSCMIDIPtr) getMidi {
    return _midi;
}

-(void) setMidi:(VSCMIDIPtr)midi {
    _midi = midi;
}

-(void) setMidiOutput:(VSCMIDIOutputPtr)output {
    _midiOutput = output;
    
}

-(VSCMIDIOutputPtr) getMidiOutput {
    return _midiOutput;
}


-(void) sendMidiControlMessage {
    VSCMIDI::Message m = VSCMIDI::messageForControl(self.midiChannel, self.controlChannel, self.controlValue);
    if (_midiOutput) {
        //std::cout << "Sending " << VSCMIDI::messageDescription(m) << std::endl;
        _midiOutput->sendMessage(m);
    }
}

-(void) sendMidiNoteOnMessage {
    VSCMIDI::Message m = VSCMIDI::messageForNote(self.midiChannel, self.pitchValue, self.velocityValue, true);
    if (_midiOutput) {
        //std::cout << "Sending " << VSCMIDI::messageDescription(m) << std::endl;
        _midiOutput->sendMessage(m);
    }
}

-(void) sendMidiNoteOffMessage {
    VSCMIDI::Message m = VSCMIDI::messageForNote(self.midiChannel, self.pitchValue, self.velocityValue, false);
    if (_midiOutput) {
        //std::cout << "Sending " << VSCMIDI::messageDescription(m) << std::endl;
        _midiOutput->sendMessage(m);
    }
}

@end
