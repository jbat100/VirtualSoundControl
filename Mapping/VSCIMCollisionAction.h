//
//  VSCIMAction.h
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_IM_COLLISION_ACTION_H_
#define _VSC_IM_COLLISION_ACTION_H_

#include "VSC.h"
#include "VSCTask.h"
#include "VSCTaskQueue.h"
#include "VSCOBScene.h"

#include <boost/shared_ptr.hpp>

#include <vector>

namespace VSC {
    
    namespace IM {
        
        /*
         *  A very abstract class to provide an interface for performable actions
         */
        
        class CollisionAction {
            
        public:
            
            typedef boost::shared_ptr<CollisionAction> SPtr;
            
            CollisionAction() {}
            
            /*
             *  perform() will call the protected internalPerform after the specified delay, on the same
             *  thread (using boost asio ?)
             */
            
            virtual Task::SPtr createTaskForCollision(OB::Scene::Element::SPtr element,
                                                      OB::Scene::Collision::SPtr collision) = 0;
            
            void setDelay(TimeDuration delay) {mDelay = delay;}
            TimeDuration getDelay(void) {return mDelay;}
            
            TaskQueue::SPtr getTaskQueue(void) {return mTaskQueue;}
            void setTaskQueue(TaskQueue::SPtr queue) {mTaskQueue = queue;}
            
        private:
            
            TimeDuration        mDelay;
            TaskQueue::SPtr     mTaskQueue;
            
        };

        typedef std::vector<CollisionAction::SPtr> CollisionActions;

    }
    
}

#endif // _VSC_IM_COLLISION_ACTION_H_
