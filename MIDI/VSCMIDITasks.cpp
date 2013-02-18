/*
 *  Created by Jonathan Thorpe on 26/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCMIDITasks.h"
#include "VSCException.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/assert.hpp>

boost::once_flag midiSingletonInitilizedFlag = BOOST_ONCE_INIT;
static VSC::TaskQueue::SPtr midiTaskQueue = VSC::TaskQueue::SPtr();

namespace VSC
{
    namespace MIDI
    {
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

VSC::MIDI::MIDISendMessageTask::Payload::Payload() :
midiOutput(Output::SPtr()),
messageDescription(MessageDescription::SPtr(new MessageDescription)),
timeOffset(boost::posix_time::seconds(0))
{
    
}

VSC::MIDI::MIDISendMessageTask::MIDISendMessageTask(Task::Payload::SPtr payload) : MIDITask(payload)
{
    MIDISendMessageTask::Payload::SPtr midiPayload = boost::dynamic_pointer_cast<MIDISendMessageTask::Payload>(payload);
    BOOST_ASSERT(midiPayload);
    if (!midiPayload)
    {
        throw VSCInvalidArgumentException("Payload for MIDISendMessageTask should be MIDISendMessageTask::Payload");
    }
}

bool VSC::MIDI::MIDISendMessageTask::stepExecution(void)
{
    if (mTraceExecution) std::cout << "VSC::MIDI::MIDISendMessageTask::stepExecution:" << std::endl;
    
    MIDISendMessageTask::Payload::SPtr payload = boost::static_pointer_cast<MIDISendMessageTask::Payload>(this->getPayload());
    BOOST_ASSERT(payload);
    if (!payload)
    {
        throw VSCInvalidArgumentException("Payload for MIDISendMessageTask should be MIDISendMessageTask::Payload");
    }
    
    Time targetTime = this->getExecutionStartTime() + payload->timeOffset;
    Time currentTime = CurrentTime();
    
    if (mTraceExecution)
    {
        std::cout << "    Execution time: " << this->getExecutionStartTime() << std::endl;
        std::cout << "    Time offset: " << payload->timeOffset << std::endl;
        std::cout << "    Target time: " << targetTime << std::endl;
        std::cout << "    Current time: " << currentTime << std::endl;
    }
    
    if (targetTime <= CurrentTime())
    {
        if (mTraceExecution) std::cout << "    Executing!" << std::endl;
        //BOOST_ASSERT(payload->midiOutput);
        if (payload->midiOutput)
        {
            payload->midiOutput->sendMessage(payload->messageDescription);
        }
        else
        {
            if (mTraceExecution) std::cout << "    NO MIDI OUT!!!" << std::endl;
        }
        this->setState(StateEnded);
        return true;
    }
    
    if (mTraceExecution) std::cout << "    Not yet!" << std::endl;
    
    return false;
}

