/*
 *  VSCController.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 29/07/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#ifndef _VSC_MANUAL_CONTROLLER_H_
#define _VSC_MANUAL_CONTROLLER_H_

#include <boost/shared_ptr.hpp>

#include "VSCController.h"
#include "VSCSound.h"

class VSCManualController : public VSCController {
    
public:
    
    VSCManualController(void);
    
    virtual Float getCurrentControlValue(void) const;
    
    void setCurrentControlValue(const Float val);
    
private:
    
    Float _currentControlValue;
    
};

typedef boost::shared_ptr<VSCManualController> VSCManualControllerPtr;

#endif

