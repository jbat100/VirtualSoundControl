//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_IM_ACTION_H_
#define _VSC_IM_ACTION_H_

#include "VSC.h"
#include "VSCTask.h"
#include "VSCTaskQueue.h"
#include "VSCIMEvent.h"

#include <boost/shared_ptr.hpp>

#include <vector>

namespace VSC {
    
    namespace IM {
        
        /*
         *  A very abstract class to provide an interface for performable actions.
         *  Subclasses should provide a way to generate tasks which will be queued
         *  on the action's task queue.
         */
        
        class Action : Event {
            
        public:
            
            typedef boost::shared_ptr<Action> SPtr;
            
            Action() {}
            
            TaskQueue::SPtr getTaskQueue(void) {return mTaskQueue;}
            void setTaskQueue(TaskQueue::SPtr queue) {mTaskQueue = queue;}
            
        private:
            
            TaskQueue::SPtr     mTaskQueue;
            
        };

        typedef std::vector<Action::SPtr> Actions;

    }
    
}

#endif // _VSC_IM_ACTION_H_