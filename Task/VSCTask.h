/*
 *  Created by Jonathan Thorpe on 22/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 *	General definitions for everything that has to do with sound stuff
 *
 */


#ifndef _VSC_TASK_H_
#define _VSC_TASK_H_

#include "VSC.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <list>

namespace VSC {
    
    class TaskQueue;
    
    class Task {
        
    public:
        
        friend class TaskQueue;
        
        typedef boost::shared_ptr<Task> SPtr;
        
        enum State {
            StateNone = 0,
            StateWaiting,
            StateCancelled,
            StateRunning,
            StateEnded
        };
        
        class Payload  {
        public:
            virtual ~Payload() {} // makes class polymorphic
            typedef boost::shared_ptr<Payload> SPtr;
        };
        
        Task(const Time& executionStartTime, Payload::SPtr payload);
        virtual ~Task() {}
        
        const Time& getExecutionStartTime(void) const {return mExecutionStartTime;} // constant so no need for mutex
        
        State getState(void); // cannot be const because of mutex lock
        
        const TimeDuration getDurationSinceExecutionTime(void) const;
        
    protected:
        
        void setState(State state);
        
        Payload::SPtr getPayload(void) {return mPayload;}
        
        /*
         *  Once the stepExecution function has been called, the Task should transition into 
         *  either StateRunning or StateEnded. Returns true if ended
         */
        
        virtual bool stepExecution(void) = 0;
        
    private:
        
        State           mState; // should be mutex protected
        
        boost::mutex    mMutex;
        
        Time            mExecutionStartTime;
        
        Payload::SPtr   mPayload;
        
    };
    
    typedef std::list<Task::SPtr> Tasks;
    
}

#endif


