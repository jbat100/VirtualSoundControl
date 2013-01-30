
#ifndef _VSC_IM_EVENT_H_
#define _VSC_IM_EVENT_H_

#include "VSC.h"
#include "VSCTask.h"
#include "VSCTaskQueue.h"
#include "VSCIMTarget.h"
#include "VSCIMMapping.h"
#include "VSCIMCollisionMapping.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <vector>

namespace VSC
{
    
    namespace IM
    {
        
        /*
         *  A very very abstract class to provide a container for event chains
         *  currently planned subclasses include delay events and action events.
         */
        
        class Event
        {
            
        public:
            
            typedef boost::shared_ptr<Event>    SPtr;
            typedef boost::weak_ptr<Event>      WPtr;
            
            typedef std::map<Target, Mapping::SPtr>             MappingMap;
            typedef std::map<Target, Float>                     ValueMap;
            
            Event() {}
            virtual ~Event() {}
            
            /*
             *  Mappings
             */
            
            virtual void createDefaultMappings(void) {}
            
            const Targets& getRequiredMappingTargets(void) {return mRequiredMappingTargets;}
            
            Mapping::SPtr getMappingForTarget(const Target target);
            Mapping::SPtr getCollisionMappingForTarget(const Target target);
            
            void setMappingForTarget(Mapping::SPtr mapping, const Target target);
            void setCollisionMappingForTarget(Mapping::SPtr mapping, const Target target);
            
        protected:
            
            void addRequiredMappingTarget(Target target);
            
            bool checkRequiredMappings(void);
            
        private:
            
            Targets                 mRequiredMappingTargets;
            
            MappingMap              mCollisionMappingMap;
            
            MappingMap              mMappingMap;
            
        };

        typedef std::vector<Event::SPtr> Events;

    }
    
}

#endif // _VSC_IM_EVENT_H_
