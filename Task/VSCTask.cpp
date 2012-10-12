/*
 *  VSCSound.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 22/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCTask.h"

#include <boost/assert.hpp>
#include <boost/thread.hpp>

VSC::Task::Task(const Time& executionStartTime, Payload::SPtr payload) :
mExecutionStartTime(executionStartTime),
mPayload(payload),
mState(StateWaiting)
{
    
}

const VSC::TimeDuration VSC::Task::getDurationSinceExecutionTime(void) const
{
    return CurrentTime() - mExecutionStartTime;
}

VSC::Task::State VSC::Task::getState(void) 
{
    boost::lock_guard<boost::mutex> l(mMutex);
    State s = mState;
    return s;
}


void VSC::Task::setState(State state)
{
    boost::lock_guard<boost::mutex> lock(mMutex);
    mState = state;
}