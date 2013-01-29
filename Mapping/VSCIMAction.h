
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
         *  A very abstract class to provide an interface for performable actions.
         *  Subclasses should provide a way to generate tasks which will be queued
         *  on the action's task queue.
         */
        
        class Action : public Event
        {
            
        public:
            
            typedef boost::shared_ptr<Action> SPtr;
            
            typedef std::map<Target, Mapping::SPtr>             MappingMap;
            typedef std::map<Target, CollisionMapping::SPtr>    CollisionMappingMap;
            typedef std::map<Target, Float>                     ValueMap;
            
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
            
            /*
             *  Mappings
             */
            
            virtual void createDefaultMappings() {}
            
            const Targets& getRequiredMappingTargets(void) {return mRequiredMappingTargets;}
            
            Mapping::SPtr           getMappingForTarget(const Target target);
            CollisionMapping::SPtr  getCollisionMappingForTarget(const Target target);
            
            void                    setMappingForTarget(Mapping::SPtr mapping, const Target target);
            void                    setCollisionMappingForTarget(CollisionMapping::SPtr mapping, const Target target);
            
        protected:
            
            void addRequiredMappingTarget(Target target);
            
            bool checkRequiredMappings(void);
            
            // this is the only generate function which should be subclassed
            virtual const Tasks generateTasksWithValueMap(Action::ValueMap& valueMap) = 0;
            
        private:
            
            TaskQueue::SPtr         mTaskQueue;
            
            bool                    mMuted;
            
            Targets                 mRequiredMappingTargets;
            
            CollisionMappingMap     mCollisionMappingMap;
            
            MappingMap              mMappingMap;
            
        };

        typedef std::vector<Action::SPtr> Actions;

    }
    
}

#endif // _VSC_IM_ACTION_H_
