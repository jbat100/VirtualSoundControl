//
//  VSCMIDITest.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 3/20/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "VSCMIDI.h"
#include "VSCMIDIOutput.h"

@interface VSCMIDITest : NSObject  {  
    
    @private

    VSCMIDIPtr _midi;
    VSCMIDIOutputPtr _midiOutput;

}

@property (nonatomic, assign) unsigned int midiChannel;
@property (nonatomic, assign) unsigned int controlChannel;
@property (nonatomic, assign) unsigned int controlValue;
@property (nonatomic, assign) unsigned int pitchValue;
@property (nonatomic, assign) unsigned int velocityValue;

-(VSCMIDIPtr) getMidi;

-(VSCMIDIOutputPtr) getMidiOutput;
-(void) setMidiOutput:(VSCMIDIOutputPtr)midiOutput;

-(void) sendMidiControlMessage;
-(void) sendMidiNoteOnMessage;
-(void) sendMidiNoteOffMessage;

@end
