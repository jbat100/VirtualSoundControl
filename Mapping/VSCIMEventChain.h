
#ifndef _VSC_IM_EVENT_CHAIN_H_
#define _VSC_IM_EVENT_CHAIN_H_

#include "VSC.h"
#include "VSCIM.h"
#include "VSCIMEvent.h"
#include "VSCOB.h"
#include "VSCUsernamed.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace VSC
{
    
    namespace IM
    {
        
        class EventChain : public Usernamed
        {
            
        public:
            
            friend class Environment;
            
            typedef boost::shared_ptr<EventChain>   SPtr;
            typedef boost::weak_ptr<EventChain>     WPtr;
            
            virtual ~EventChain() {}
            
            unsigned int getNumberOfEvents(void);
            const Events& getEvents(void) {return mEvents;}
            Event::SPtr getEventAtIndex(unsigned int index);

            void appendEvent(Event::SPtr event);
            void prependEvent(Event::SPtr event);
            void insertEventAtIndex(Event::SPtr event, unsigned int index);
            void insertEventAfterEvent(Event::SPtr insertedEvent, Event::SPtr event);
            void insertEventBeforeEvent(Event::SPtr insertedEvent, Event::SPtr event);
            void swapEvents(Event::SPtr firstEvent, Event::SPtr secondEvent);
            void removeEvent(Event::SPtr event);
            
            virtual void perform(void);
            virtual void performForCollision(OB::Collision_SPtr collision, OB::Element_SPtr effector);
            
        protected:
            
            /*
             *  Constructor is protected so that it can only be called by friend class Environment
             */
            
            EventChain() {}
            
            /*
             *  Can be over-ridden by subclass to reject invalid events.
             */
            
            virtual bool checkEvent(Event::SPtr event);
            
        private:
            
            Events mEvents;
            
        };

    }
    
}

#endif // _VSC_IM_EVENT_CHAIN_H_
