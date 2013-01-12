/*
 *  VSCController.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 29/07/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCManualController.h"
#include "VSCException.h"

VSCManualController::VSCManualController(void) {
    this->setCurrentControlValue(0);
}

Float VSCManualController::getCurrentControlValue(void) const {
    return _currentControlValue;
}

void VSCManualController::setCurrentControlValue(const Float val) {
    
    if (this->valueIsValid(val)) {
        _currentControlValue = val;
    }
    
    else {
        throw VSCInvalidArgumentException("Tried to set value beyond allowed control range");
    }
    
}