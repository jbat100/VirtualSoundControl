
#ifndef _VSC_IM_ACTION_H_
#define _VSC_IM_ACTION_H_

#include "VSC.h"
#include "VSCOB.h"
#include "VSCTask.h"
#include "VSCTaskQueue.h"
#include "VSCIMEvent.h"
#include "VSCIMTarget.h"
#include "VSCIMMapping.h"
#include "VSCIMCollisionMapping.h"

#include <boost/shared_ptr.hpp>

#include <vector>

namespace VSC {
    
    namespace IM {
        
        /*
         *  A class to provide an interface for performable actions, with mappings
         *  depending on the trigger (Collision, Gesture, VocalCommand...).
         *  Subclasses should provide a way to generate tasks which will be queued
         *  on the action's task queue.
         */
        
        class Action : public Event
        {
            
        public:
            
            typedef boost::shared_ptr<Action> SPtr;
            
            Action() {}
            virtual ~Action() {}
            
            TaskQueue::SPtr getTaskQueue(void) {return mTaskQueue;}
            void setTaskQueue(TaskQueue::SPtr queue) {mTaskQueue = queue;}
            
            bool isMuted(void) {return mMuted;}
            void setMuted(bool mute) {mMuted = mute;}
            
            /*
             *  Task generation, note these functions are non virtual, they will be used
             *  to fill in a ValueMap, to then call the virtual protected function 
             *  virtual Tasks generateTasksWithValueMap(Action::ValueMap& valueMap) = 0;
             */
            
            const Tasks generateTasks(void);
            const Tasks generateTasksForCollision(OB::Collision_SPtr collision, OB::Element_SPtr effector);
            
        protected:
            
            // this is the only generate function which should be subclassed
            virtual const Tasks generateTasksWithValueMap(Action::ValueMap& valueMap) = 0;
            
        private:
            
            TaskQueue::SPtr         mTaskQueue;
            
            bool                    mMuted;
            
        };

        typedef std::vector<Action::SPtr> Actions;

    }
    
}

#endif // _VSC_IM_ACTION_H_
