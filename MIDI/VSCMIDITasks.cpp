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

// MARK: Control Change

VSC::MIDI::MIDISendMessageTask::MIDISendMessageTask(Task::Payload::SPtr payload) : MIDITask(payload)
{
    MIDISendMessageTask::Payload::SPtr midiPayload = boost::dynamic_pointer_cast<MIDISendMessageTask::Payload>(payload);
    BOOST_ASSERT(midiPayload);
    if (!midiPayload)
    {
        throw VSCInvalidArgumentException("Payload for MIDIControlChangeTask should be MIDIControlChangeTask::Payload");
    }
}

bool VSC::MIDI::MIDISendMessageTask::stepExecution(void)
{
    MIDISendMessageTask::Payload::SPtr payload = boost::static_pointer_cast<MIDISendMessageTask::Payload>(this->getPayload());
    BOOST_ASSERT(payload);
    
    if (this->getExecutionStartTime() + payload->delay >= CurrentTime())
    {
        payload->midiOutput->sendMessage(payload->messageDescription);
        this->setState(StateEnded);
        return true;
    }
    
    return false;
}

