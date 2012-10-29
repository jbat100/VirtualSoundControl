//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_IM_EVENT_H_
#define _VSC_IM_EVENT_H_

#include "VSC.h"
#include "VSCTask.h"
#include "VSCTaskQueue.h"

#include <boost/shared_ptr.hpp>

#include <vector>

namespace VSC {
    
    namespace IM {
        
        /*
         *  A very very abstract class to provide a container for event chains
         *  currently planned subclasses include delay events and action events
         */
        
        class Event {
            
        public:
            
            typedef boost::shared_ptr<Event> SPtr;
            
            Event() {}
            
        };

        typedef std::vector<Event::SPtr> Events;
        
        /*
         *  Event chain for actions and delays
         */
        
        class EventChain {
            
        public:
            
            typedef boost::shared_ptr<EventChain> SPtr;
            
            virtual ~EventChain();
            
            unsigned int numberOfEvents(void);
            
            void appendEvent(Event::SPtr event);
            void prependEvent(Event::SPtr event);
            
            void insertEventAtIndex(Event::SPtr event, unsigned int index);
            
            void swapEvents(Event::SPtr firstEvent, Event::SPtr secondEvent);
            
        protected:
            
            /*
             *  Can be over-ridden by subclass to reject invalid events.
             */
            
            virtual bool checkEvent(Event::SPtr event) {return true;}
            
        private:
            
            Events mEvents;
            
        };
        
        typedef std::vector<EventChain::SPtr> EventChains;

    }
    
}

#endif // _VSC_IM_EVENT_H_
