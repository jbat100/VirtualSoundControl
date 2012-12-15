/*
 *  VSCController.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 29/07/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#ifndef _VSC_TRIGGERED_CONTROLLER_H_
#define _VSC_TRIGGERED_CONTROLLER_H_

#include <boost/chrono.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "VSCSound.h"
#include "VSCController.h"

class VSCTriggeredController : public VSCController {
    
public:
    
    Float getDurationSinceTriggerTime();
    
    void trigger();
    
private:

    boost::chrono::system_clock::time_point triggerTimePoint;
    
};

typedef boost::shared_ptr<VSCTriggeredController> VSCTriggeredController;

#endif

