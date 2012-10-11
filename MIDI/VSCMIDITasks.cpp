/*
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCMIDITasks.h"

// MARK: Note On

VSC::MIDI::MIDINoteOnTask::MIDINoteOnTask(Output::SPtr output,
                                          unsigned int channel,
                                          unsigned int pitch,
                                          unsigned int velocity) :
MIDITask(output),
mChannel(channel),
mPitch(pitch),
mVelocity(velocity) {}

bool VSC::MIDI::MIDINoteOnTask::stepExecution(void)
{
    output->sendNoteOn(mChannel, mPitch, mVelocity);
    this->setState(StateEnded);
    return true;
}

// MARK: Note Off

VSC::MIDI::MIDINoteOffTask::MIDINoteOffTask(Output::SPtr output,
                                            unsigned int channel,
                                            unsigned int pitch,
                                            unsigned int velocity) :
MIDITask(output),
mChannel(channel),
mPitch(pitch),
mVelocity(velocity) {}

bool VSC::MIDI::MIDINoteOffTask::stepExecution(void)
{
    output->sendNoteOff(mChannel, mPitch, mVelocity);
    this->setState(StateEnded);
    return true;
}

// MARK: Note On And Off

VSC::MIDI::MIDINoteOnAndOffTask::MIDINoteOnAndOffTask(Output::SPtr output,
                                                      unsigned int channel,
                                                      unsigned int pitch,
                                                      unsigned int onVelocity,
                                                      unsigned int offVelocity,
                                                      TimeDuration duration) :
MIDITask(output),
mChannel(channel),
mPitch(pitch),
mOnVelocity(onVelocity),
mOffVelocity(offVelocity),
mDuration(duration)
mSentNoteOn(false) {}

bool VSC::MIDI::MIDINoteOnAndOffTask::stepExecution(void)
{
    if (!sentNoteOn)
    {
        output->sendNoteOn(mChannel, mPitch, mOnVelocity);
    }
    
    if (mDuration > this->getDurationSinceExecutionTime())
    {
        return false;
    }
    
    output->sendNoteOff(mChannel, mPitch, mVelocity);
    this->setState(StateEnded);
    return true;
}

// MARK: Control Change

VSC::MIDI::MIDIControlChangeTask::MIDIControlChangeTask(Output::SPtr output,
                                                        unsigned int channel,
                                                        ControlNumber controlNumber,
                                                        unsigned int value) :
MIDITask(output),
mChannel(channel),
mControlNumber(controlNumber),
mValue(value) {}

bool VSC::MIDI::MIDIControlChangeTask::stepExecution(void)
{
    
}

