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

VSCController::VSCController(void) {
    this->setValueRange(ValueRange(0.0, 1.0));
}

VSCSFloat VSCController::getCurrentControlValue(void) const {
    throw VSCSCalledPureVirtualFunctionException();
}

VSCController::State VSCController::getState(void) const {
    return _state;
}

void VSCController::setState(State state) {
    _state = state;
}

bool VSCController::valueIsValid(const VSCSFloat val) const {
    return (val > _valueRange.first && val < _valueRange.first + _valueRange.second) ? true : false;
}

VSCController::ValueRange VSCController::getValueRange(void) const {
    return _valueRange;
}

void VSCController::setValueRange(ValueRange valueRange) {
    _valueRange = valueRange;
}