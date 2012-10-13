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

@end
