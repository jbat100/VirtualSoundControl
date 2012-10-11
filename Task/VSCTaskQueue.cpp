/*
 *  Created by Jonathan Thorpe on 22/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCTaskQueue.h"

#include <boost/foreach.hpp>

VSC::TaskQueue::TaskQueue() :
mState(StateStopped),
mStopRequested(false),
mLastStepTime(Time(boost::posix_time::neg_infin)),
mMinimumStepDuration(boost::posix_time::milliseconds(10))
{
    
}

void VSC::TaskQueue::enqueueTask(Task::SPtr task)
{
    boost::lock_guard<boost::mutex> lock(mMutex);
    Tasks::iterator it = std::find(mQueuedTasks.begin(), mQueuedTasks.end(), task);
    if (it == mQueuedTasks.end())
    {
        /*
         *  So that we can always keep the mQueuedTasks list 
         */
        for (it = mQueuedTasks.begin(); it != mQueuedTasks.end(); it++)
        {
            Task::SPtr t = *it;
            if (task->getExecutionStartTime() < t->getExecutionStartTime()) break;
        }
        mQueuedTasks.insert(it, task);
    }
}

bool VSC::TaskQueue::cancelTask(Task::SPtr task)
{

    boost::lock_guard<boost::mutex> lock(mMutex);
    Tasks::iterator it = std::find(mQueuedTasks.begin(), mQueuedTasks.end(), task);
    if (it != mQueuedTasks.end())
    {
        task->setState(Task::StateCancelled);
        mQueuedTasks.erase(it);
        return true;
    }
    it = std::find(mQueuedTasks.begin(), mQueuedTasks.end(), task);
    if (it != mQueuedTasks.end())
    {
        task->setState(Task::StateCancelled);
        mQueuedTasks.erase(it);
        return true;
    }

    return false;
}

bool VSC::TaskQueue::cancelAllTask(void)
{
    boost::lock_guard<boost::mutex> lock(mMutex);
    
    bool cancelledAtLeastOneTask = false;
    
    BOOST_FOREACH(Task::SPtr task, mQueuedTasks)
    {
        cancelledAtLeastOneTask = true;
        task->setState(Task::StateCancelled);
    }
    mQueuedTasks.clear();
    
    
    BOOST_FOREACH(Task::SPtr task, mRunningTasks)
    {
        cancelledAtLeastOneTask = true;
        task->setState(Task::StateCancelled);
    }
    mQueuedTasks.clear();
    
    return cancelledAtLeastOneTask;
}

void VSC::TaskQueue::start()
{
    if (this->getState() != StateRunning)
    {
        mLastStepTime = Time(boost::posix_time::neg_infin);
        this->requestStop(false);
        mInternalThread = boost::thread(&TaskQueue::threadedExecutionFunction, this);
        this->setState(StateRunning);
    }
}

void VSC::TaskQueue::stop()
{
    if (this->getState() == StateRunning)
    {
        this->requestStop(true);
        mInternalThread.join();
        this->setState(StateStopped);
    }
    else if (this->getState() != StateStopped)
    {
        this->setState(StateStopped);
    }
    this->requestStop(false);
    return;
}

VSC::TaskQueue::State VSC::TaskQueue::getState(void)
{
    boost::lock_guard<boost::mutex> lock(mMutex);
    State s = mState;
    return s;
}

void VSC::TaskQueue::setState(State state)
{
    boost::lock_guard<boost::mutex> lock(mMutex);
    mState = state;
}

VSC::TimeDuration VSC::TaskQueue::getMinimumExecutionStepDuration(void)
{
    boost::lock_guard<boost::mutex> lock(mMutex);
    TimeDuration d = mMinimumStepDuration;
    return d;
}

void VSC::TaskQueue::setMinimumExecutionStepDuration(TimeDuration duration)
{
    boost::lock_guard<boost::mutex> lock(mMutex);
    mMinimumStepDuration = duration;
}

void VSC::TaskQueue::threadedExecutionFunction()
{
    while (this->stopRequested() == false)
    {
        TimeDuration durationSinceLastStep = CurrentTime() - mLastStepTime;
        TimeDuration minimumStepDuration = this->getMinimumExecutionStepDuration();
        
        if (durationSinceLastStep < minimumStepDuration)
        {
            TimeDuration sleepTime = minimumStepDuration - durationSinceLastStep;
            boost::this_thread::sleep(sleepTime);
        }
        
        this->stepExecution();
    }
}

void VSC::TaskQueue::stepExecution()
{
    mLastStepTime = CurrentTime();
    
    /*
     *  Get the tasks that need to be dequeued
     */
    
    Tasks dequeuedTasks;
    Tasks localTasks;
    
    {
        /*
         *  Locked scope to protect mQueuedTasks which is accessible from external threads
         */
        boost::lock_guard<boost::mutex> lock(mMutex);
        Tasks::iterator it = mQueuedTasks.begin();
        while (it != mQueuedTasks.end())
        {
            Task::SPtr task = *it;
            if (task->getExecutionStartTime() < mLastStepTime)
            {
                dequeuedTasks.push_back(task);
                it = mQueuedTasks.erase(it);
            }
            else
            {
                ++it;
            }
        }
        
        BOOST_FOREACH(Task::SPtr task, dequeuedTasks)
        {
            Tasks::iterator checkIt = std::find(mRunningTasks.begin(), mRunningTasks.end(), task);
            if (checkIt == mRunningTasks.end())
            {
                mRunningTasks.push_back(task);
            }
        }
        
        localTasks = mRunningTasks;
    }
    
    Tasks::iterator it = localTasks.begin();
    while (it != localTasks.end())
    {
        Task::SPtr task = *it;
        bool ended = task->stepExecution();
        if (ended)
        {
            it = mRunningTasks.erase(it);
        }
        else
        {
            ++it;
        }
    }
    
}

void VSC::TaskQueue::requestStop(bool stop)
{
    boost::lock_guard<boost::mutex> lock(mMutex);
    mStopRequested = stop;
}

bool VSC::TaskQueue::stopRequested()
{
    boost::lock_guard<boost::mutex> lock(mMutex);
    bool s = mStopRequested;
    return s;
}

