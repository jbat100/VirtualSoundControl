/*
 *  VSCController.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 29/07/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCController.h"
#include "VSCException.h"

VSCSFloat VSCController::getCurrentControlValue(void) const {
    throw VSCSCalledPureVirtualFunctionException();
}

VSCController::State VSCController::getState(void) const {
    return _state;
}

void VSCController::setState(State state) {
    _state = state;
}

VSCController::ValueRange VSCController::getValueRange(void) {
    return _valueRange;
}

void VSCController::setValueRange(ValueRange valueRange) {
    _valueRange = valueRange;
}