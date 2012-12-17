/*
 *  Created by Jonathan Thorpe on 22/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCTaskQueue.h"
#include "VSCException.h"

#include <boost/chrono/chrono.hpp>
#include <boost/foreach.hpp>
#include <boost/assert.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

VSC::TaskQueue::TaskQueue() :
mState(StateStopped),
mStopRequested(false),
mLastStepTime(Time(boost::posix_time::neg_infin)),
mMinimumStepDuration(boost::posix_time::milliseconds(10))
{
    
}

void VSC::TaskQueue::enqueueTask(Task::SPtr task)
{
    boost::lock_guard<boost::recursive_mutex> lock(mMutex);
    
    if (mTraceQueue) std::cout << "VSC::TaskQueue::enqueueTask " << *task << std::endl;
    
    BOOST_ASSERT(this->getState() == StateRunning);
    if (this->getState() != StateRunning)
    {
        std::cerr << "VSC::TaskQueue::enqueueTask queue is not running!!" << std::endl;
    }
    
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
        mTaskCondition.notify_one();
    }
}

bool VSC::TaskQueue::cancelTask(Task::SPtr task)
{
    boost::lock_guard<boost::recursive_mutex> lock(mMutex);
    
    bool erased = false;
    
    if (mTraceQueue) std::cout << "VSC::TaskQueue::cancelTask " << *task << std::endl;
    Tasks::iterator it = std::find(mQueuedTasks.begin(), mQueuedTasks.end(), task);
    if (it != mQueuedTasks.end())
    {
        task->setState(Task::StateCancelled);
        mQueuedTasks.erase(it);
        erased = true;
    }
    it = std::find(mRunningTasks.begin(), mRunningTasks.end(), task);
    if (it != mRunningTasks.end())
    {
        task->setState(Task::StateCancelled);
        mRunningTasks.erase(it);
        erased = true;
    }

    return false;
}

bool VSC::TaskQueue::cancelAllTask(void)
{
    boost::lock_guard<boost::recursive_mutex> lock(mMutex);
    
    if (mTraceQueue) std::cout << "VSC::TaskQueue::cancelAllTask" << std::endl;
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
    boost::lock_guard<boost::recursive_mutex> lock(mMutex);
    
    if (this->getState() != StateRunning)
    {
        if (mTraceExecution) std::cout << "VSC::TaskQueue::start" << std::endl;
        
        mLastStepTime = Time(boost::posix_time::neg_infin);
        this->requestStop(false);
        mInternalThread = boost::thread(&TaskQueue::threadedExecutionFunction, this);
        this->setState(StateRunning);
        
        mTaskCondition.notify_one();
    }
}

void VSC::TaskQueue::stop()
{
    boost::lock_guard<boost::recursive_mutex> lock(mMutex);
    
    if (this->getState() == StateRunning)
    {
        if (mTraceExecution) std::cout << "VSC::TaskQueue::stopping..." << std::endl;
        this->requestStop(true);
        
        mInternalThread.join(); // cannot lock mutex for join... 
        
        this->setState(StateStopped);
        if (mTraceExecution) std::cout << "VSC::TaskQueue::stopped" << std::endl;
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
    boost::lock_guard<boost::recursive_mutex> lock(mMutex);
    
    State s = mState;
    return s;
}

void VSC::TaskQueue::setState(State state)
{
    boost::lock_guard<boost::recursive_mutex> lock(mMutex);
    
    mState = state;
}

VSC::TimeDuration VSC::TaskQueue::getMinimumExecutionStepDuration(void)
{
    boost::lock_guard<boost::recursive_mutex> lock(mMutex);
    
    TimeDuration d = mMinimumStepDuration;
    return d;
}

void VSC::TaskQueue::setMinimumExecutionStepDuration(TimeDuration duration)
{
    boost::lock_guard<boost::recursive_mutex> lock(mMutex);
    
    mMinimumStepDuration = duration;
}

bool VSC::TaskQueue::tasksAreRunning()
{
    boost::lock_guard<boost::recursive_mutex> lock(mMutex);
    
    return mRunningTasks.size() > 0;
}

bool VSC::TaskQueue::tasksAreQueued()
{
    boost::lock_guard<boost::recursive_mutex> lock(mMutex);
    
    return mQueuedTasks.size() > 0;
}

VSC::TimeDuration VSC::TaskQueue::durationUntilNextQueuedTaskExecutionTime()
{
    boost::lock_guard<boost::recursive_mutex> lock(mMutex);
    
    if (mQueuedTasks.size() == 0)
    {
        return VSC::TimeDuration(boost::posix_time::pos_infin);
    }
    else
    {
        Tasks::iterator it = mQueuedTasks.begin();
        BOOST_ASSERT(it != mQueuedTasks.end());
        if (it == mQueuedTasks.end()) return VSC::TimeDuration(boost::posix_time::pos_infin);
        Task::SPtr task = *it;
        return task->getExecutionStartTime() - CurrentTime();
    }
}

void VSC::TaskQueue::threadedExecutionFunction()
{
    if (mTraceExecution) std::cout << "VSC::TaskQueue::threadedExecutionFunction START" << std::endl;
    
    while (this->stopRequested() == false)
    {
        if (mTraceExecution) std::cout << "VSC::TaskQueue::threadedExecutionFunction BEGIN LOOP" << std::endl;
        
        this->stepExecution();
        
        if (this->tasksAreRunning() || this->tasksAreQueued())
        {
            if (mTraceExecution) std::cout << "VSC::TaskQueue::threadedExecutionFunction tasks are queued or running" << std::endl;
            
            TimeDuration waitDuration = mMinimumStepDuration;
            
            if (this->tasksAreQueued())
            {
                TimeDuration durationUntilNextQueuedTask = this->durationUntilNextQueuedTaskExecutionTime();
                if (this->tasksAreRunning())
                {
                    if (durationUntilNextQueuedTask < mMinimumStepDuration)
                    {
                        if (mTraceExecution) std::cout << "VSC::TaskQueue::threadedExecutionFunction next task is sooner than step duration";
                        waitDuration = durationUntilNextQueuedTask;
                    }
                }
                else
                {
                    waitDuration = durationUntilNextQueuedTask;
                }

            }
            
            if (mTraceExecution)
            {
                std::cout << "VSC::TaskQueue::threadedExecutionFunction waiting for " << waitDuration.total_milliseconds() << "ms" << std::endl;
            }
            
            boost::chrono::milliseconds chronoMs(waitDuration.total_milliseconds());
            boost::unique_lock<boost::mutex> lock(mTaskConditionMutex);
            mTaskCondition.wait_for(lock, chronoMs);
        }
        
        else
        {
            std::cout << "VSC::TaskQueue::threadedExecutionFunction no task queued or running, waiting indefinately..." << std::endl;
            // if no tasks are queued or running then wait for signal indefinately...
            boost::unique_lock<boost::mutex> lock(mTaskConditionMutex);
            mTaskCondition.wait(lock);
        }
        
        if (mTraceExecution) std::cout << "VSC::TaskQueue::threadedExecutionFunction END LOOP" << std::endl;
        
    }
    if (mTraceExecution) std::cout << "VSC::TaskQueue::threadedExecutionFunction END" << std::endl;
}

void VSC::TaskQueue::stepExecution()
{
    
    Tasks localRunningTasks;
    
    {
        boost::lock_guard<boost::recursive_mutex> lock(mMutex);
        
        try
        {
            if (mTraceTasks) std::cout << "VSC::TaskQueue::stepExecution START" << std::endl;
            mLastStepTime = CurrentTime();
            if (mTraceExecution) std::cout << "VSC::TaskQueue::stepExecution current time: " << mLastStepTime << std::endl;
            if (mTraceExecution) std::cout << "VSC::TaskQueue::stepExecution getting tasks to execute..." << std::endl;
            
            {
                /*
                 *  Purge cancelled tasks from mQueuedTasks and promote to running the tasks which
                 *  have reached their execution start time
                 */
                Tasks::iterator it = mQueuedTasks.begin();
                while (it != mQueuedTasks.end())
                {
                    Task::SPtr task = *it;
                    
                    Task::State taskState = task->getState();
                    
                    if (taskState == Task::StateCancelled)
                    {
                        it = mQueuedTasks.erase(it);
                        continue;
                    }
                    
                    BOOST_ASSERT(taskState == Task::StateWaiting);
                    
                    if (task->getExecutionStartTime() < mLastStepTime)
                    {
                        task->setState(Task::StateRunning);
                        Tasks::iterator checkIt = std::find(mRunningTasks.begin(), mRunningTasks.end(), task);
                        BOOST_ASSERT(checkIt == mRunningTasks.end());
                        if (checkIt == mRunningTasks.end())
                        {
                            mRunningTasks.push_back(task);
                            if (mTraceTasks) std::cout << "VSC::TaskQueue::stepExecution dequeued task " << task << std::endl;
                        }
                        it = mQueuedTasks.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
                
                /*
                 *  Remove cancelled and ended running tasks (likely cancelled or ended)
                 */
                
                it = mRunningTasks.begin();
                while (it != mRunningTasks.end())
                {
                    Task::SPtr task = *it;
                    if (task->getState() != Task::StateRunning)
                    {
                        it = mRunningTasks.erase(it);
                        if (mTraceTasks) std::cout << "VSC::TaskQueue::stepExecution finished running task " << task << std::endl;
                    }
                    else ++it;
                }
                
                localRunningTasks = mRunningTasks;
            }
            if (mTraceExecution) std::cout << "VSC::TaskQueue::stepExecution got " << localRunningTasks.size() << " task(s) to run" << std::endl;
        }
        catch (std::exception& e)
        {
            std::cout << "VSC::TaskQueue::stepExecution EXCEPTION!!! : " << e.what() << std::endl;
        }
        

    }

    /*
     *  After having unlocked the mutex we iterate over the copy of the 
     *  running task list and execute them.
     */
    
    BOOST_FOREACH(Task::SPtr task, localRunningTasks)
    {
        if (mTraceExecution) std::cout << "VSC::TaskQueue::stepExecution will stepExecution for task : " << task << std::endl;
        try 
        {
            bool ended = task->stepExecution();
            if (ended) {/* */}
        }
        catch (VSCInvalidArgumentException& e)
        {
            std::cout << "VSC::TaskQueue::stepExecution INVALID ARGUMENT EXCEPTION!!! : " << e.what();
            std::cout << "; Additional info: " << e.getValueForKey(VSCBaseExceptionAdditionalInfoKey) << std::endl;
            this->cancelTask(task);
        }
        catch (std::exception& e)
        {
            std::cout << "VSC::TaskQueue::stepExecution EXCEPTION!!! : " << e.what() << std::endl;
            this->cancelTask(task);
        }
    }
    
    if (mTraceExecution) std::cout  << "VSC::TaskQueue::stepExecution END" << std::endl;
    
}

void VSC::TaskQueue::requestStop(bool stop)
{
    boost::lock_guard<boost::mutex> lock(mRequestStopMutex);
    if (mTraceExecution) std::cout << "VSC::TaskQueue::requestStop" << stop << std::endl;
    mStopRequested = stop;
    mTaskCondition.notify_one();
}

bool VSC::TaskQueue::stopRequested()
{
    boost::lock_guard<boost::mutex> lock(mRequestStopMutex);
    //if (mTraceExecution) std::cout << "VSC::TaskQueue::stopRequested" << std::endl;
    bool s = mStopRequested;
    return s;
}

