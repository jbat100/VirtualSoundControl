
#ifndef _VSC_IM_EVENT_CHAIN_H_
#define _VSC_IM_EVENT_CHAIN_H_

#include "VSC.h"
#include "VSCIM.h"
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
            Event_SPtr getEventAtIndex(unsigned int index);

            void appendEvent(Event_SPtr event);
            void prependEvent(Event_SPtr event);
            void insertEventAtIndex(Event_SPtr event, unsigned int index);
            void insertEventAfterEvent(Event_SPtr insertedEvent, Event_SPtr event);
            void insertEventBeforeEvent(Event_SPtr insertedEvent, Event_SPtr event);
            void swapEvents(Event_SPtr firstEvent, Event_SPtr secondEvent);
            void removeEvent(Event_SPtr event);
            
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
            
            virtual bool checkEvent(Event_SPtr event);
            
        private:
            
            Events mEvents;
            
        };

    }
    
}

#endif // _VSC_IM_EVENT_CHAIN_H_
