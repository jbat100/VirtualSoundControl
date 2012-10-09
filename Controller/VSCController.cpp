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

VSC::Controller::Controller(void) {
    this->setValueRange(ValueRange(0.0, 1.0));
}

VSC::Float VSC::Controller::getCurrentControlValue(void) const {
    throw VSCSCalledPureVirtualFunctionException();
}

VSC::Controller::State VSC::Controller::getState(void) const {
    return mState;
}

void VSC::Controller::setState(State state) {
    mState = state;
}

bool VSC::Controller::valueIsValid(const VSC::Float val) const {
    return (val > mValueRange.first && val < mValueRange.first + mValueRange.second) ? true : false;
}

VSC::Controller::ValueRange VSC::Controller::getValueRange(void) const {
    return mValueRange;
}

void VSC::Controller::setValueRange(ValueRange valueRange) {
    mValueRange = valueRange;
}