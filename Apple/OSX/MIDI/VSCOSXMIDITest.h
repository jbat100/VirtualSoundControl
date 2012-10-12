//
//  VSCOSXMIDITest.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 3/20/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "VSCMIDI.h"
#include "VSCMIDIOutput.h"

@interface VSCOSXMIDITest : NSObject  {  
    

}

@property (nonatomic, assign) unsigned int midiChannel;
@property (nonatomic, assign) unsigned int controlValue;
@property (nonatomic, assign) unsigned int pitchValue;
@property (nonatomic, assign) unsigned int velocityValue;

@property (nonatomic, assign) VSC::MIDI::Output::SPtr midiOut;

-(void) sendMidiControlMessage;
-(void) sendMidiNoteOnMessage;
-(void) sendMidiNoteOffMessage;

@end
