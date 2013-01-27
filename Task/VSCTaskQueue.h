
#ifndef _VSC_TASK_QUEUE_H_
#define _VSC_TASK_QUEUE_H_

#include "VSC.h"
#include "VSCTask.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

namespace VSC {
    
    class TaskQueue {
        
    public:
        
        typedef boost::shared_ptr<TaskQueue> SPtr;
        
        enum State {
            StateNone = 0,
            StateStopped,
            StateRunning
        };
        
        TaskQueue();
        ~TaskQueue() { stop(); }
        
        /*
         *  Can be called before or after start,
         *  stop will remove current tasks
         */
        
        void enqueueTask(Task::SPtr task);
        
        bool cancelTask(Task::SPtr task);
        bool cancelAllTask(void);
        
        void start();
        void stop(); // synchronous, waits for the internal thread to exit
        
        State getState(void);
        
        TimeDuration getMinimumExecutionStepDuration(void);
        void setMinimumExecutionStepDuration(TimeDuration duration);
        
    protected:
        
        void threadedExecutionFunction();
        void stepExecution();
        void setState(State state);
        
        void requestStop(bool stop);
        bool stopRequested();
        
        bool tasksAreRunning();
        bool tasksAreQueued();
        TimeDuration durationUntilNextQueuedTaskExecutionTime();
        
        
    private:
        
        Tasks                       mQueuedTasks;
        Tasks                       mRunningTasks;
        
        State                       mState;
        bool                        mStopRequested;
        
        TimeDuration                mMinimumStepDuration;
        Time                        mLastStepTime;
        
        boost::thread               mInternalThread;
        boost::recursive_mutex      mMutex;
        boost::mutex                mTaskConditionMutex;
        boost::mutex                mRequestStopMutex;
        boost::condition_variable   mTaskCondition;
        
        const static bool           mTraceQueue = true;
        const static bool           mTraceExecution = true;
        const static bool           mTraceTasks = true;
    };
    
}

#endif // _VSC_TASK_QUEUE_H_


