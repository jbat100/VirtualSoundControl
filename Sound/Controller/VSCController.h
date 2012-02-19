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

#include <map>
#include <boost/chrono.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "VSCSound.h"

class VSCController {
    
public:
    
    enum State {
        StateNone,
        StateStopped,
        StateRunning,
        StateEnded
    };
    
    typedef std::pair<VSCSFloat, VSCSFloat> ValueRange;
    
    virtual VSCSFloat getCurrentControlValue(void) const;
    
    State getState(void) const;
    void setState(const State state); 
    
    ValueRange getValueRange(void);
    void setValueRange(ValueRange valueRange);
    
private:
    
    State _state;
    ValueRange _valueRange;
    
};

typedef boost::shared_ptr<VSCController> VSCControllerPtr;

#endif

