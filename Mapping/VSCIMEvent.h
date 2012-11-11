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
            virtual ~Event() {}
            
        };

        typedef std::vector<Event::SPtr> Events;

    }
    
}

#endif // _VSC_IM_EVENT_H_
