
#ifndef _VSC_IM_ACTION_H_
#define _VSC_IM_ACTION_H_

#include "VSC.h"
#include "VSCIM.h"
#include "VSCOB.h"
#include "VSCTask.h"
#include "VSCTaskQueue.h"
#include "VSCIMEvent.h"

#include "VSCIMMapping.h"
#include "VSCIMCollisionMapping.h"

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
        
        class Action : public Event
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
             *  to fill in a ValueMap, to then call the virtual protected function 
             *  virtual Tasks generateTasksWithValueMap(Action::ValueMap& valueMap) = 0;
             */
            
            const Tasks generateTasks(void);
            const Tasks generateTasksForCollision(OB::Collision_SPtr collision, OB::Element_SPtr effector);
            const Tasks generateTasksWithValueMap(Event::ValueMap& valueMap);
            
        private:
            
            class Implementation
            {
            public:
                typedef boost::shared_ptr<Implementation>   SPtr;
                typedef boost::weak_ptr<Implementation>     WPtr;
                virtual const Tasks generateTasksWithValueMap(Event::ValueMap& valueMap) = 0;
                virtual void createDefaultMappings() = 0;
            };
            class ImplementationMIDINoteOn;
            class ImplementationMIDINoteOff;
            class ImplementationMIDINoteOnAndOff;
            class ImplementationMIDIControlChange;
            
            
            TaskQueue::SPtr         mTaskQueue;
            
            bool                    mMuted;
            
            ActionType              mActionType;
            
            Implementation::SPtr    mImplementation;
            
        };

        typedef std::vector<Action::SPtr> Actions;
        
        /*
         *  Implementations
         */
        
        class Action::ImplementationMIDINoteOn : public Action::Implementation
        {
            virtual const Tasks generateTasksWithValueMap(Event::ValueMap& valueMap);
            virtual void createDefaultMappings();
        };
        
        class Action::ImplementationMIDINoteOff : public Action::Implementation
        {
            virtual const Tasks generateTasksWithValueMap(Event::ValueMap& valueMap);
            virtual void createDefaultMappings();
        };
        
        class Action::ImplementationMIDINoteOnAndOff : public Action::Implementation
        {
            virtual const Tasks generateTasksWithValueMap(Event::ValueMap& valueMap);
            virtual void createDefaultMappings();
        };
        
        class Action::ImplementationMIDIControlChange : public Action::Implementation
        {
            virtual const Tasks generateTasksWithValueMap(Event::ValueMap& valueMap);
            virtual void createDefaultMappings();
        };

    }
    
}

#endif // _VSC_IM_ACTION_H_
