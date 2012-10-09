/*
 *  VSCController.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 29/07/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#ifndef _VSC_CONTROLLER_H_
#define _VSC_CONTROLLER_H_

#include "VSC.h"

#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/chrono.hpp>
#include <boost/serialization/shared_ptr.hpp>

namespace VSC {
    
    class Controller {
        
    public:
        
        typedef boost::shared_ptr<Controller> SPtr;
        
        enum State {
            StateNone,
            StateStopped,
            StateRunning,
            StateEnded
        };
        
        typedef std::pair<Float, Float> ValueRange;
        
        Controller(void);
        
        virtual Float getCurrentControlValue(void) const;
        
        State getState(void) const;
        
        ValueRange getValueRange(void) const;
        void setValueRange(ValueRange valueRange);
        
        bool valueIsValid(const Float val) const;
        
    protected:
        
        void setState(const State state);
        
    private:
        
        State       mState;
        ValueRange  mValueRange;
        
    };
    
}

#endif

