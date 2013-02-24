
#ifndef _VSC_IM_EVENT_H_
#define _VSC_IM_EVENT_H_

#include "VSC.h"
#include "VSCTask.h"
#include "VSCTaskQueue.h"
#include "VSCIM.h"


#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <vector>

namespace VSC
{
    
    namespace IM
    {
        
        class Event
        {
            
        public:
            
            typedef boost::shared_ptr<Event>    SPtr;
            typedef boost::weak_ptr<Event>      WPtr;
            
            typedef std::map<Target, Mapping_SPtr>          TargetMappingMap;
            typedef std::map<Trigger, TargetMappingMap>     TriggerTargetMappingMap;
            typedef std::map<Target, Float>                 TargetValueMap;
            
            Event() {}
            virtual ~Event() {}
            
            const Targets& getRequiredMappingTargets(void) {return mRequiredMappingTargets;}
            
            Mapping_SPtr getMapping(const Trigger trigger, const Target target);
            
        protected:
            
            void addRequiredMappingTarget(Target target);
            void clearRequiredMappingTargets();
            
            void clearMappings(void);
            void generateMappings(void);
            bool checkRequiredMappings(void);
            
        private:
            
            Targets                     mRequiredMappingTargets;
            
            TriggerTargetMappingMap     mMappingMap;
            
        };

        typedef std::vector<Event::SPtr> Events;

    }
    
}

#endif // _VSC_IM_EVENT_H_
