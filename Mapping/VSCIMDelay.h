//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_IM_DELAY_H_
#define _VSC_IM_DELAY_H_

#include "VSC.h"
#include "VSCIM.h"
#include "VSCIMEvent.h"
#include "VSCOB.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <vector>

namespace VSC {
    
    namespace IM {
        
        /*
         *  Simple delay class subclassing Event, to be used in event chains
         */
        
        class Delay : public Event
        {
            
        public:
            
            typedef boost::shared_ptr<Delay>    SPtr;
            typedef boost::weak_ptr<Delay>      WPtr;
            
            Delay(void);
            
            TimeDuration getDuration(Trigger trigger, TriggerPayload::SPtr payload);
            
        };

    }
}

#endif // _VSC_IM_DELAY_H_
