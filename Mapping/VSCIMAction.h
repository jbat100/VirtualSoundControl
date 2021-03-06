
#ifndef _VSC_IM_ACTION_H_
#define _VSC_IM_ACTION_H_

#include "VSC.h"
#include "VSCIM.h"
#include "VSCOB.h"
#include "VSCTask.h"
#include "VSCTaskQueue.h"
#include "VSCIMEvent.h"

#include "VSCIMMapping.h"

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <vector>

namespace VSC {
    
    namespace IM {
        
        /*
         *  A class to provide an interface for performable actions, with mappings
         *  depending on the trigger (Collision, Gesture, VocalCommand...).
         *  Subclasses should provide a way to generate tasks which will be queued
         *  on the action's task queue.
         */
        
        class Action : public Event, public boost::enable_shared_from_this<Action>
        {
            
        public:
            
            typedef boost::shared_ptr<Action>   SPtr;
            typedef boost::weak_ptr<Action>     WPtr;
            
            Action() {}
            virtual ~Action() {}
            
            TaskQueue::SPtr getTaskQueue(void) {return mTaskQueue;}
            void setTaskQueue(TaskQueue::SPtr queue) {mTaskQueue = queue;}
            
            bool isMuted(void) {return mMuted;}
            void setMuted(bool mute) {mMuted = mute;}
            
            void setActionType(ActionType actionType);
            ActionType getActionType(void) {return mActionType;}
            
            /*
             *  Task generation, note these functions are non virtual, they will be used
             *  to fill in a TargetValueMap, to then call the virtual protected function 
             *  virtual Tasks generateTasksWithTargetValueMap(Action::TargetValueMap& valueMap) = 0;
             */
            
            const Tasks generateTasks(Trigger trigger, TriggerPayload::SPtr payload);
            
            const Tasks generateTasksWithTargetValueMap(Event::TargetValueMap& valueMap);
            
            /*
             *  Implementation
             */
            
            class Implementation
            {
            public:
                typedef boost::shared_ptr<Implementation>   SPtr;
                typedef boost::weak_ptr<Implementation>     WPtr;
                virtual const Tasks generateTasksWithTargetValueMap(Event::TargetValueMap& valueMap) = 0;
                virtual void setupMappings(Action::SPtr action) = 0;
            };
            class ImplementationVoid;
            class ImplementationMIDINoteOn;
            class ImplementationMIDINoteOff;
            class ImplementationMIDINoteOnAndOff;
            class ImplementationMIDIControlChange;
            
            Implementation::SPtr getImplementation(void) {return mImplementation;}
            
        private:
            
            
            TaskQueue::SPtr         mTaskQueue;
            
            bool                    mMuted;
            
            ActionType              mActionType;
            
            Implementation::SPtr    mImplementation;
            
        };

        typedef std::vector<Action::SPtr> Actions;
        
    }
    
}

#endif // _VSC_IM_ACTION_H_
