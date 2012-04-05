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

#include <boost/shared_ptr.hpp>
#include <boost/chrono.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "VSCSound.h"

class VSCController;

typedef boost::shared_ptr<VSCController> VSCControllerPtr;

class VSCController {
    
public:
    
    enum State {
        StateNone,
        StateStopped,
        StateRunning,
        StateEnded
    };
    
    typedef std::pair<VSCSFloat, VSCSFloat> ValueRange;
    
    VSCController(void);
    
    virtual VSCSFloat getCurrentControlValue(void) const;
    
    State getState(void) const;
    
    ValueRange getValueRange(void) const;
    void setValueRange(ValueRange valueRange);
    
    bool valueIsValid(const VSCSFloat val) const;
    
protected:
    
    void setState(const State state); 
    
private:
    
    State _state;
    ValueRange _valueRange;
    
};


#endif

