/*
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCMIDITasks.h"
#include "VSCException.h"

#include <boost/shared_ptr.hpp>
#include <boost/assert.hpp>

boost::once_flag midiSingletonInitilizedFlag = BOOST_ONCE_INIT;
static VSC::TaskQueue::SPtr midiTaskQueue = VSC::TaskQueue::SPtr();

namespace VSC {
    namespace MIDI {
        void InitialiseSingletonMIDITaskQueue();
    }
}

void VSC::MIDI::InitialiseSingletonMIDITaskQueue()
{
    midiTaskQueue = TaskQueue::SPtr(new TaskQueue);
}

VSC::TaskQueue::SPtr VSC::MIDI::SingletonMIDITaskQueue()
{
    boost::call_once(&InitialiseSingletonMIDITaskQueue, midiSingletonInitilizedFlag);
    BOOST_ASSERT(midiTaskQueue);
    return midiTaskQueue;
}

// MARK: Note On

VSC::MIDI::MIDINoteOnTask::MIDINoteOnTask(Task::Payload::SPtr payload) : MIDITask(payload)
{
    MIDINoteOnTask::Payload::SPtr midiPayload = boost::dynamic_pointer_cast<MIDINoteOnTask::Payload>(payload);
    
    if (!midiPayload)
    {
        throw VSCInvalidArgumentException("Payload for MIDINoteOnTask should be MIDINoteOnTask::Payload");
    }
}

bool VSC::MIDI::MIDINoteOnTask::stepExecution(void)
{
    VSC::Task::stepExecution();
    
    MIDINoteOnTask::Payload::SPtr midiPayload = boost::static_pointer_cast<MIDINoteOnTask::Payload>(this->getPayload());
    
    midiPayload->midiOutput->sendNoteOn(midiPayload->channel, midiPayload->pitch, midiPayload->velocity);
    this->setState(StateEnded);
    
    return true;
}

// MARK: Note Off

VSC::MIDI::MIDINoteOffTask::MIDINoteOffTask(Task::Payload::SPtr payload) : MIDITask(payload)
{
    MIDINoteOffTask::Payload::SPtr midiPayload = boost::dynamic_pointer_cast<MIDINoteOffTask::Payload>(payload);
    
    if (!midiPayload)
    {
        throw VSCInvalidArgumentException("Payload for MIDINoteOffTask should be MIDINoteOffTask::Payload");
    }
}

bool VSC::MIDI::MIDINoteOffTask::stepExecution(void)
{
    VSC::Task::stepExecution();
    
    MIDINoteOffTask::Payload::SPtr midiPayload = boost::static_pointer_cast<MIDINoteOffTask::Payload>(this->getPayload());
    
    midiPayload->midiOutput->sendNoteOff(midiPayload->channel, midiPayload->pitch, midiPayload->velocity);
    this->setState(StateEnded);
    
    return true;
}

// MARK: Note On And Off

VSC::MIDI::MIDINoteOnAndOffTask::MIDINoteOnAndOffTask(Task::Payload::SPtr payload) : MIDITask(payload),
mSentNoteOn(false)
{
    MIDINoteOnAndOffTask::Payload::SPtr midiPayload = boost::dynamic_pointer_cast<MIDINoteOnAndOffTask::Payload>(payload);
    
    if (!midiPayload)
    {
        throw VSCInvalidArgumentException("Payload for MIDINoteOnAndOffTask should be MIDINoteOnAndOffTask::Payload");
    }
}


bool VSC::MIDI::MIDINoteOnAndOffTask::stepExecution(void)
{
    
    MIDINoteOnAndOffTask::Payload::SPtr midiPayload = boost::static_pointer_cast<MIDINoteOnAndOffTask::Payload>(this->getPayload());
    
    if (!mSentNoteOn)
    {
        midiPayload->midiOutput->sendNoteOn(midiPayload->channel, midiPayload->pitch, midiPayload->onVelocity);
        mSentNoteOn = true;
    }
    
    if (midiPayload->duration < this->getDurationSinceExecutionTime())
    {
        midiPayload->midiOutput->sendNoteOff(midiPayload->channel, midiPayload->pitch, midiPayload->offVelocity);
        this->setState(StateEnded);
        
        return true;
    }

    return false;
}

// MARK: Control Change

VSC::MIDI::MIDIControlChangeTask::MIDIControlChangeTask(Task::Payload::SPtr payload) : MIDITask(payload)
{
    MIDIControlChangeTask::Payload::SPtr midiPayload = boost::dynamic_pointer_cast<MIDIControlChangeTask::Payload>(payload);
    
    if (!midiPayload)
    {
        throw VSCInvalidArgumentException("Payload for MIDIControlChangeTask should be MIDIControlChangeTask::Payload");
    }
}

bool VSC::MIDI::MIDIControlChangeTask::stepExecution(void)
{
    MIDIControlChangeTask::Payload::SPtr midiPayload = boost::static_pointer_cast<MIDIControlChangeTask::Payload>(this->getPayload());
    
    midiPayload->midiOutput->sendNoteOff(midiPayload->channel, midiPayload->controlNumber, midiPayload->value);
    this->setState(StateEnded);
    
    return true;
}

